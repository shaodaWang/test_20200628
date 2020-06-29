/** $Id$ $DateTime$
 *  @file  BsrSdk.cpp
 *  @brief SDKʵ��
 *  @version 0.0.1
 *  @since 0.0.1
 */


#include "stdafx.h"
#include "BsrSdk.h"
#include "../BillSdkType.h"
// #include <WinSock2.h>
#include <algorithm>
#include <assert.h>


/**
 * @fn BsrSdk::BsrSdk()
 * @brief      SDK�Ĺ��캯��
 */
BsrSdk::BsrSdk()
{
}

/**
 * @fn BsrSdk::~BsrSdk()
 * @brief      ��������
 */
BsrSdk::~BsrSdk()
{
	deInit();  
}

/**
 * @fn int BsrSdk::init()
 * @brief      SDK��ʼ������
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
int 
BsrSdk::init()
{
	/// ��ʼ������
	WSADATA wsaData;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
	{
		return DVX_ERR_NET_INIT;
	}

	return DVX_OK;
}

/**
 * @fn int BsrSdk::deInit()
 * @brief      SDK����ʼ������
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
int 
BsrSdk::deInit()
{
	LockPtr lock( &m_lock );
	/// 1.�ͷ�SDK�е�Dvr���
	if ( m_vecDvr.size() > 0 )
	{
		for ( int i=m_vecDvr.size()-1; i>=0; i-- )
		{
      if( NULL != m_vecDvr[i] )
			  this->dvrDestory( m_vecDvr[i] );
		}
		m_vecDvr.clear();
	}

	/// 3.�������
	WSACleanup();

	return DVX_OK;
}

/**
 * @fn int BsrSdk::dvrCreate(int ip, unsigned short port, const char *szUserName, const char *szPasswd, DvrBase **pDvr)
 * @brief      ����Dvr������¼
 * @param[in]  ip Dvr��IP��ַ
 * @param[in]  port Dvr�Ķ˿ںţ������ֽ���
 * @param[in]  szUserName �û���
 * @param[in]  szPasswd ����
 * @param[in]  bReceiveAlerts 3720�������տ���
 * @param[out] ppDvr ����Dvr����ĵ�ַ�����������DvxHandle
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
int 
BsrSdk::dvrCreate( int ip, unsigned short nCmdPort, unsigned short nDataPort, const char *szUserName, 
				  const char *szPasswd, DvrBase** ppDvr, unsigned int timeOut)
{
	/// 1.��鵱ǰDvr�Ƿ��Ѵ򿪡�
	///   �����ƶ�ͬһ�豸����һ������
	m_lock.lock();
	std::vector<DvrBase*>::iterator	it = m_vecDvr.begin();
	for ( ; it != m_vecDvr.end(); it++ )
	{
		// ��ǰ��check()û�����������أ�ʼ�շ���false.
		if ( (*it) != NULL && (*it)->check( ip ) )
		{
			*ppDvr = *it;
			m_lock.unlock();
			return DVX_OK;
		}
	}
	m_lock.unlock();

	/// 2.����Dvr����
	DvrBase* pDvr = NULL;
	int nRet = DvrBase::create( &pDvr, ip, nCmdPort, timeOut );
	if ( nRet != 0 )
		return nRet;

	m_lock.lock();
	m_vecDvr.push_back( pDvr );
	*ppDvr = pDvr;
	m_lock.unlock();

	/// 3.��¼Dvr
	nRet = pDvr->login( szUserName, szPasswd, timeOut);
  return nRet;
}


int 
BsrSdk::dvrCreateWithOutLogin( int ip, unsigned short nCmdPort, char nGameType, bool bHeart,
	DvrBase** ppDvr, unsigned int timeOut)
{
	/// 1.��鵱ǰDvr�Ƿ��Ѵ򿪡�
	///   �����ƶ�ͬһ�豸����һ������
	m_lock.lock();
	std::vector<DvrBase*>::iterator	it = m_vecDvr.begin();
	for ( ; it != m_vecDvr.end(); it++ )
	{
		// ��ǰ��check()û�����������أ�ʼ�շ���false.
		if ( (*it) != NULL && (*it)->check( ip ) )
		{
			*ppDvr = *it;
			m_lock.unlock();
			return DVX_OK;
		}
	}
	m_lock.unlock();

	/// 2.����Dvr����
	DvrBase* pDvr = NULL;
	int nRet = DvrBase::create( &pDvr, ip, nCmdPort, nGameType, bHeart, timeOut );
	if ( nRet != 0 )
		return nRet;

	m_lock.lock();
	m_vecDvr.push_back( pDvr );
	*ppDvr = pDvr;
	m_lock.unlock();

	return nRet;
}

/**
 * @fn int BsrSdk::dvrDestory(DvrBase *pDvr)
 * @brief      ����Dvr
 * @param[in]  pDvr Dvr����(��DvxHandleֵͬ)
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
int 
BsrSdk::dvrDestory(DvrBase *pDvr, bool bLogout)
{
	
	if ( false == dvrCheck( pDvr ) )
		return DVX_ERR_DVX_INVALID_HANDLE;
	
	/// 1.�ǳ�
	if (bLogout)
	{
		pDvr->logout();
	}
	
	LockPtr lock( &m_lock );
	/// 2.ɾ����Dvr
	std::vector<DvrBase*>::iterator	it = std::find( m_vecDvr.begin(), m_vecDvr.end(), pDvr );
	if ( it != m_vecDvr.end() )
		m_vecDvr.erase( it );

	DvrBase::release( &pDvr );
	return DVX_OK;
}

/**
 * @fn bool BsrSdk::dvrCheck(DvrBase *pDvr)
 * @brief      ��鵱ǰDvr�Ƿ����
 * @param[in]  pDvr Dvr����(��DvxHandleֵͬ)
 * @return bool
 * @retval  true ����
 * @retval  false ������
 */
bool BsrSdk::dvrCheck(DvrBase *pDvr)
{
	LockPtr lock( &m_lock );
	std::vector<DvrBase*>::iterator	it = std::find( m_vecDvr.begin(), m_vecDvr.end(), pDvr );
	if ( it == m_vecDvr.end() )
		return false;

	return true;
}


int BsrSdk::dvrCreateNotLogin( const int ip, const unsigned short nCmdPort, unsigned int nCmd, const void* parmin, const unsigned int nlenthIn, 
                              void* parmout, const unsigned int nLenthout, unsigned int timeOut/*=3000*/ )
{
  if (parmout==NULL)
  {
    return DVX_ERR_PARA;
  }
  return DvrBase::createNotLogin( ip, nCmdPort, nCmd, parmin, nlenthIn, parmout, nLenthout, timeOut );
}
