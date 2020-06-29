/** 
 *  @file  $Id$ $DateTime$
 *  @brief Sunflowerϵ��Dvr������ʵ��
 *  @version 0.0.1
 *  @since 0.0.1
 */


#include "stdafx.h"
#include "Dvr_Sunflower.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
//#include "CmdDef_Sunflower.h"

#include "BsrSdk.h"
// #include "AlertSocket.h"

/// Dvx��������������߳�
DWORD WINAPI DvxThread_Reconnect( LPVOID lpvoid );

/**
 * @fn DvrSunflower::DvrSunflower(BscpSocket* pBscp )
 * @brief      ���캯��
 */
DvrSunflower::DvrSunflower(BscpSocket* pBscp, unsigned int nProductId )
: m_pBscp(pBscp)
, m_bIsLogin(false)
, m_nProductId(nProductId)
, m_bReconnect(false)
, m_hDvxThread(NULL)
, m_hDvxEvent(NULL)
, m_bDvxDisconnect(false)
, m_bExit(false)
,m_hNotifyWnd(NULL)
{
  //����
  m_pConfigBscp = new ConfigBscp( pBscp );
}

/**
 * @fn DvrSunflower::~DvrSunflower()
 * @brief      ��������
 */
DvrSunflower::~DvrSunflower()
{
	if ( m_pConfigBscp )
	{
		delete m_pConfigBscp;
		m_pConfigBscp = NULL;
	}
	///15.�ͷ�Bscpģ��
	if ( m_pBscp )
	{
		delete m_pBscp;
		m_pBscp = NULL;
	}
	m_bIsLogin = false;
}

/**
 * @fn int DvrSunflower::login(const char *szUserName, const char *szPasswd)
 * @brief      ��¼Dvr
 * @param[in]  szUserName �û���
 * @param[in]  szPasswd ����
 * @param[in]  tmout ��ʱʱ��
 * @param[in]  bReceiveAlerts 3720�������տ���
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
int 
DvrSunflower::login( const char *szUserName, const char *szPasswd, int tmout)
{
  if ( NULL == m_pBscp )
  {
    assert(false);	// ����Ӧ�ó����������
    return DVX_ERR_ASSERT;
  }

	sprintf_s( m_szUserName, sizeof(m_szUserName), "%s", szUserName );
	sprintf_s( m_szPasswd, sizeof(m_szPasswd), "%s", szPasswd );

	/// 1.��֯����
	LoginOptsIn_Sunflower	arg;
	LoginOptsOut_Sunflower res;
  
  memset(&arg, 0, sizeof(arg));
  memcpy( arg.user, m_szUserName, 16 );
  memcpy( arg.password, m_szPasswd, 16 );
	arg.actor = 4;	// Client
	memset( &res, 0, sizeof(res) );

	/// ��¼
	int nRet = DVX_OK;
	int nTimes = LOGIN_TRY_TIMES;
//	for ( int i=0; i<nTimes; i++ )
  if( !this->isLogin() )
	{
		nRet = m_pBscp->Execute( BSCP_SUNFLOWER_CMD_LOGIN, (void*)&arg, sizeof(arg), (void*)&res, sizeof(res), tmout );
		if ( nRet == DVX_OK )
		{
			m_sessionId = res.sessionID;
			m_bIsLogin = true;
			UINT nip =this->getDvxAddr();
//			break;
		}
	}

  /// ���������������������߳�
  /*if( DVX_OK == nRet && NULL == m_hDvxEvent && NULL == m_hDvxThread )
  {
    m_hDvxEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
    m_hDvxThread = CreateThread( NULL, 64*1024, DvxThread_Reconnect, this, 0, 0 );
    if( NULL == m_hDvxEvent || NULL == m_hDvxThread )
    {
      nRet = DVX_ERR_CREATE_THREAD;      
    }
  }*/

	return nRet;
}

/**
 * @fn int DvrSunflower::logout()
 * @brief      �ǳ�
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
int 
DvrSunflower::logout()
{
  m_bExit = true;

  if ( m_hDvxThread )
  {
    if( m_hDvxEvent )
    {
      SetEvent( m_hDvxEvent );
    }

	// IE�ز����̣߳�ֱ��ǿ��
    if( WAIT_TIMEOUT == WaitForSingleObject( m_hDvxThread, 5*1000 ) ) //�ȴ���¼����
	{
      TerminateThread( m_hDvxThread, 0 );
	}
	CloseHandle( m_hDvxThread );
	 m_hDvxThread = NULL;
  }
  if ( m_hDvxEvent )
  {
    CloseHandle( m_hDvxEvent );
    m_hDvxEvent = NULL;
  }

  if ( m_bIsLogin == false || NULL == m_pBscp )
    return 0;

	
	int nRet = m_pBscp->Execute( BSCP_SUNFLOWER_CMD_LOGOUT, &m_sessionId, sizeof(m_sessionId), NULL, 0, 100 );
  m_bIsLogin = false;

  /* ��Щ����ΪBP4�����ӣ���ʵ�ʻ����з���������ӵ��ԣ����ܻᵼ��logout()����Ȼ����isLogin()���ߡ�*/
	char szMsg[256];
  sprintf( szMsg, "Logout for %X:%d nRet:%d, m_bIsLogin:%d ", this->m_ipAddr, this->m_nCmdPort, nRet, m_bIsLogin);
	OutputDebugStringA( szMsg );
  return nRet;
}

/**
 * @fn isLogin
 * @brief     ���Dvx�Ƿ��¼
 * @return bool 
 * @retval  true �ѵ�¼
 * @retval  false δ��¼����������ԭ�����
 */
bool 
DvrSunflower::isLogin()
{
  /* ��Щ����ΪBP4�����ӣ���ʵ�ʻ����з���������ӵ��ԣ����ܻᵼ��logout()����Ȼ����isLogin()���ߡ�*/
  /*char szMsg[256];
  sprintf( szMsg, "%X:%d login status is :%d ", this->m_ipAddr, this->m_nCmdPort, m_bIsLogin);
  OutputDebugString( szMsg );*/

  if( NULL == m_pBscp )
    return false;

  if( m_bIsLogin == false || NULL == m_pBscp )
    return false;

  return m_pBscp->IsOk();
}


/**
 * @fn setReconnect
 * @brief      ���ö�������
 * @param[in]  bReconnect �Ƿ��������
 * @return void
 */
void DvrSunflower::setReconnect( bool bReconnect )
{
  if( m_bReconnect != bReconnect )
  {
    m_bReconnect = bReconnect;
  }  
}


/**
 * @fn setDisconnect
 * @brief      ���ö��ߣ���Real����
 * @param[in]  bDisconnect �Ƿ����
 * @return void
 */
void 
DvrSunflower::setDisconnect( bool bDisconnect )
{ 
  if( bDisconnect != m_bDvxDisconnect )
  {
    m_bDvxDisconnect = bDisconnect; 
    if( NULL != m_hDvxEvent && INVALID_HANDLE_VALUE != m_hDvxEvent )
      SetEvent( m_hDvxEvent );
  }  
};

/**
 * @fn threadCallback
 * @brief      ���ڶ����������̻߳ص�
 * @return void
 */
void 
DvrSunflower::threadCallback()
{
  int ret = 0;
  do 
  {
   WaitForSingleObject( this->m_hDvxEvent, m_bDvxDisconnect ? 1000 : INFINITE );
    if( m_bExit )
      break;

    if( m_bDvxDisconnect && m_bReconnect )
    {
      int nBscpRet = this->login( m_szUserName, m_szPasswd, 10000 );

      if( m_bExit )
        break;

      /// 2.��¼�Ϻ󣬰Ѷϵ����ֳ���������
      if( DVX_OK != nBscpRet )
        continue;

      m_bDvxDisconnect = false;      
    }

  } while( true );

  return;		// ���߳�Ҫ�˳�
}

/**
 * @fn DvxThread_Reconnect
 * @brief      Dvx�и��ֶ����������߳�
 * @param[in]  
 * @param[out] 
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
DWORD WINAPI DvxThread_Reconnect( LPVOID lpvoid )
{
  DvrSunflower* pDvx = (DvrSunflower*)lpvoid;
  if( NULL == pDvx )
    return DVX_ERR_COMMON;

  BSRLOG( LogHandle	m_hThreadLog; );
  BSRLOG( m_hThreadLog = logCreate( "ThreadLog", LOGLEVEL_1, LOGMODE_TIME | LOGMODE_TOFILE | LOGMODE_REOPEN));
  BSRLOG( logPrint1( m_hThreadLog, "DvxThread_Reconnect(0x%08x) in", lpvoid ) );

  pDvx->threadCallback();

  BSRLOG( logPrint1( m_hThreadLog, "DvxThread_Reconnect(0x%08x) out", lpvoid ) );
  BSRLOG( logDestory( m_hThreadLog ); );
  BSRLOG( m_hThreadLog = NULL; );

  return 0;
}


