/** $Id$ $DateTime$
 *  @file  BsrSdk.cpp
 *  @brief SDK实现
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
 * @brief      SDK的构造函数
 */
BsrSdk::BsrSdk()
{
}

/**
 * @fn BsrSdk::~BsrSdk()
 * @brief      析构函数
 */
BsrSdk::~BsrSdk()
{
	deInit();  
}

/**
 * @fn int BsrSdk::init()
 * @brief      SDK初始化函数
 * @return int
 * @retval  0 成功
 * @retval  其它值 错误码
 */
int 
BsrSdk::init()
{
	/// 初始化网络
	WSADATA wsaData;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
	{
		return DVX_ERR_NET_INIT;
	}

	return DVX_OK;
}

/**
 * @fn int BsrSdk::deInit()
 * @brief      SDK反初始化函数
 * @return int
 * @retval  0 成功
 * @retval  其它值 错误码
 */
int 
BsrSdk::deInit()
{
	LockPtr lock( &m_lock );
	/// 1.释放SDK中的Dvr句柄
	if ( m_vecDvr.size() > 0 )
	{
		for ( int i=m_vecDvr.size()-1; i>=0; i-- )
		{
      if( NULL != m_vecDvr[i] )
			  this->dvrDestory( m_vecDvr[i] );
		}
		m_vecDvr.clear();
	}

	/// 3.清除网络
	WSACleanup();

	return DVX_OK;
}

/**
 * @fn int BsrSdk::dvrCreate(int ip, unsigned short port, const char *szUserName, const char *szPasswd, DvrBase **pDvr)
 * @brief      创建Dvr，并登录
 * @param[in]  ip Dvr的IP地址
 * @param[in]  port Dvr的端口号，主机字节序
 * @param[in]  szUserName 用户名
 * @param[in]  szPasswd 密码
 * @param[in]  bReceiveAlerts 3720报警接收开关
 * @param[out] ppDvr 返回Dvr对象的地址，外界用来做DvxHandle
 * @return int
 * @retval  0 成功
 * @retval  其它值 错误码
 */
int 
BsrSdk::dvrCreate( int ip, unsigned short nCmdPort, unsigned short nDataPort, const char *szUserName, 
				  const char *szPasswd, DvrBase** ppDvr, unsigned int timeOut)
{
	/// 1.检查当前Dvr是否已打开。
	///   可限制对同一设备仅打开一个对象。
	m_lock.lock();
	std::vector<DvrBase*>::iterator	it = m_vecDvr.begin();
	for ( ; it != m_vecDvr.end(); it++ )
	{
		// 当前的check()没有派生类重载，始终返回false.
		if ( (*it) != NULL && (*it)->check( ip ) )
		{
			*ppDvr = *it;
			m_lock.unlock();
			return DVX_OK;
		}
	}
	m_lock.unlock();

	/// 2.生成Dvr对象
	DvrBase* pDvr = NULL;
	int nRet = DvrBase::create( &pDvr, ip, nCmdPort, timeOut );
	if ( nRet != 0 )
		return nRet;

	m_lock.lock();
	m_vecDvr.push_back( pDvr );
	*ppDvr = pDvr;
	m_lock.unlock();

	/// 3.登录Dvr
	nRet = pDvr->login( szUserName, szPasswd, timeOut);
  return nRet;
}


int 
BsrSdk::dvrCreateWithOutLogin( int ip, unsigned short nCmdPort, char nGameType, bool bHeart,
	DvrBase** ppDvr, unsigned int timeOut)
{
	/// 1.检查当前Dvr是否已打开。
	///   可限制对同一设备仅打开一个对象。
	m_lock.lock();
	std::vector<DvrBase*>::iterator	it = m_vecDvr.begin();
	for ( ; it != m_vecDvr.end(); it++ )
	{
		// 当前的check()没有派生类重载，始终返回false.
		if ( (*it) != NULL && (*it)->check( ip ) )
		{
			*ppDvr = *it;
			m_lock.unlock();
			return DVX_OK;
		}
	}
	m_lock.unlock();

	/// 2.生成Dvr对象
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
 * @brief      销毁Dvr
 * @param[in]  pDvr Dvr对象(与DvxHandle同值)
 * @return int
 * @retval  0 成功
 * @retval  其它值 错误码
 */
int 
BsrSdk::dvrDestory(DvrBase *pDvr, bool bLogout)
{
	
	if ( false == dvrCheck( pDvr ) )
		return DVX_ERR_DVX_INVALID_HANDLE;
	
	/// 1.登出
	if (bLogout)
	{
		pDvr->logout();
	}
	
	LockPtr lock( &m_lock );
	/// 2.删除此Dvr
	std::vector<DvrBase*>::iterator	it = std::find( m_vecDvr.begin(), m_vecDvr.end(), pDvr );
	if ( it != m_vecDvr.end() )
		m_vecDvr.erase( it );

	DvrBase::release( &pDvr );
	return DVX_OK;
}

/**
 * @fn bool BsrSdk::dvrCheck(DvrBase *pDvr)
 * @brief      检查当前Dvr是否存在
 * @param[in]  pDvr Dvr对象(与DvxHandle同值)
 * @return bool
 * @retval  true 存在
 * @retval  false 不存在
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
