/**  $Id$ $DateTime$
 *  @file  BscpSocket.cpp
 *  @brief 实现BSCP模块，调用加密与网络连接两个子模块。接收上层任意数据，向Dvr发送，并接收Dvr回应数据。
 *  @version 0.0.1
 *  @since 0.0.1
 */

#include "StdAfx.h"
#include "BscpSocket.h"
#include "../BillSdkType.h"
// #include "../Dvr/CmdDef_Sunflower.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bsrLog.h"
#include <time.h>

//#include "../Interface/IATypeDef.h"

/// bscp命令最大长度  //qyf 扩大*2
#define MAX_CMD_LEN 1024 * 130 *3

/** @name BSCP模块的内部结构
 *  @{
 */



/** @}*/	// end of BSCP模块的内部结构

/// 发送心跳信号线程
DWORD WINAPI bscpHeartBeat( LPVOID lpvoid );

/**
 * @fn BscpSocket::BscpSocket
 * @brief      BscpSocket的构造函数
 */
BscpSocket::BscpSocket()
: m_ipAddr(0)
, m_nPort(0)
, m_hEvent(NULL)
, m_hBeatHeart(NULL)
, m_bExit(FALSE)
, m_sendBuf(NULL)
, m_recvBuf(NULL)
, m_bReg(false)
// , m_fifo(NULL)
, m_version(30) // 协议规定，当前必须为20
, m_sequence(0)
, m_bHasFailed(true)
, m_hNotifyWnd(NULL)
, m_nMsgId(0)
, m_hDvx(NULL)
{
  /// 日志部分
  BSCPLOG( m_hBscpLog = NULL; );
  BSCPLOG( 
    char szPath[MAX_PATH];
  sprintf( szPath, "Bscp_0x%08x", this );
  m_hBscpLog = logCreate( szPath, LOGLEVEL_1 | LOGLEVEL_2, LOGMODE_TIME ); 
  );
 
  //增加断网回调
  m_pfNotifyRoutine = NULL;
  m_pNotifyUserData = NULL;
  m_nGameType = 0;
}

/**
* @fn BscpSocket::~BscpSocket()
* @brief      BscpSocket类的析构参数
*/
BscpSocket::~BscpSocket()
{
	destory();	

  /// 释放日志句柄
  BSCPLOG( logDestory( m_hBscpLog ); );
  BSCPLOG( m_hBscpLog = NULL; );
}

/**
 * @fn init
 * @brief      初始化(设置)BscpSocket
 * @param[in]  ipAddr Dvr的Ip地址
 * @param[in]  port   Dvr的端口号
 * @return
 * @retval  0 成功
 * @retval  其它值 错误码
 */
int 
BscpSocket::init(unsigned int ipAddr, unsigned short port, char gametype, bool bHeart, unsigned int timeOut )
{
	int nRet;
	
	do 
	{
		m_ipAddr			= ipAddr;
		m_nPort				= port;
		m_nGameType = gametype;
		/// 2.申请 发送、接收 缓冲区

		__try
		{
			m_sendBuf = (char*)new char[MAX_CMD_LEN];
			m_recvBuf = (char*)new char[MAX_CMD_LEN];
		}__except(EXCEPTION_EXECUTE_HANDLER)
		{
			OutputDebugStringA("=====failed to malloc memory in bscp class====\n");
		}

		if ( NULL == m_sendBuf || NULL == m_recvBuf )
		{
			nRet = DVX_ERR_MALLOCMEMORY;
			break;
		}

		/// 3.建立网络连接
		m_streamSocket.close();
		m_streamSocket.open( m_ipAddr, m_nPort, 32*1024, 32*1024,timeOut);
		if ( m_streamSocket.isConnected() == false )
		{
			nRet = DVX_ERR_NET_DISCONNECT;
			break;
		}
    m_bHasFailed = false;

		/// 4.设置Beat线程的Event
		m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL );
		if ( NULL == m_hEvent )
		{
			nRet = DVX_ERR_CREATE_EVENT;
			break;
		}

		m_bReg = false;

		/// 5.创建心跳线程
		m_bExit = false;
		if (bHeart) {
			m_hBeatHeart = CreateThread(NULL, 0, bscpHeartBeat, this, 0, 0);
			if (NULL == m_hBeatHeart)
			{
				nRet = DVX_ERR_CREATE_THREAD;
				break;
			}
		}		

		/// 6.成功退出
		return 0;
	} while (0);
  
	/// 如果没有init成功，则销毁当前已申请的数据
	destory();
	return nRet;
}

/**
 * @fn int BscpSocket::init( SOCKET sDvr )
 * @brief      初始化(设置)BscpSocket
 * @param[in]  sDvr Dvr端的SOCKET，是通过主动注册得到的。
 * @return int
 * @retval  0 成功
 * @retval  其它值 错误码
 */
/*
int BscpSocket::init( SOCKET sDvr )
{
	int nRet;

	do 
	{
		m_ipAddr			= 0;
		m_nPort				= 0;

		/// 2.申请 发送、接收 缓冲区
		m_sendBuf = (char*)new char[MAX_CMD_LEN];
		m_recvBuf = (char*)new char[MAX_CMD_LEN];
		m_fifo = new AudioFifo( MAX_CMD_LEN * 2 );
		if ( NULL == m_sendBuf || NULL == m_recvBuf || NULL == m_fifo )
		{
			nRet = DVX_ERR_MALLOCMEMORY;
			break;
		}

		/// 3.建立网络连接
		m_streamSocket.close();
		m_streamSocket.open( sDvr, 32*1024, 32*1024 );
		if ( m_streamSocket.isConnected() == false )
		{
			nRet = DVX_ERR_NET_DISCONNECT;
			break;
		}
    m_bHasFailed = false;

		/// 4.设置Beat线程的Event
		m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL );
		if ( NULL == m_hEvent )
		{
			nRet = DVX_ERR_CREATE_EVENT;
			break;
		}

		m_bReg = true;

		/// 5.创建心跳线程
		m_bExit = false;
		m_hBeatHeart = CreateThread( NULL, 0, bscpHeartBeat, this, 0, 0 );
		if ( NULL == m_hBeatHeart )
		{
			nRet = DVX_ERR_CREATE_THREAD;
			break;
		}
		/// 6.成功退出
		return 0;
	} while (0);

	/// 如果没有init成功，则销毁当前已申请的数据
	destory();
	return nRet;
}
/*
/**
 * @fn void BscpSocket::destory()
 * @brief      销毁BSCP模块实例
 * @return 无
 */
void 
BscpSocket::destory()
{
  /// 4.关闭网络
	m_streamSocket.close();

  /// 2. 退出Beat线程
	m_bExit = true;
	if (NULL != m_hBeatHeart)
	{
		SetEvent( m_hEvent );
		// IE关不掉线程，直接强关
    if( WAIT_OBJECT_0 != WaitForSingleObject( m_hBeatHeart, 5000 ) )
    {
      ::TerminateThread( m_hBeatHeart, 0 );
    }

		CloseHandle(m_hBeatHeart);

		m_hBeatHeart = NULL;
	}
	if (NULL != m_hEvent)
	{
		CloseHandle( m_hEvent );
		m_hEvent = NULL;
	}

	/// 3.释放内存资源(接收缓冲区 和 发送缓冲区)
	if ( m_sendBuf != NULL )
	{
		delete[] m_sendBuf ;
		m_sendBuf = NULL;
	}
	if ( m_recvBuf != NULL )
	{
		delete[] m_recvBuf ;
		m_recvBuf = NULL;
	}
// 	if ( m_fifo )
// 	{
// 		delete m_fifo;
// 	}
}

/**
 * @fn setNotifyWnd
 * @brief      设置消息通知窗口
 * @param[in]  hNotifyWnd 消息通知窗口句柄
 * @param[in]  nMsgId 消息ID
 * @return int
 * @retval  0 成功
 * @retval  其它值 错误码
 */
void 
BscpSocket::setNotifyWnd( HWND hNotifyWnd, int nMsgId )
{
  m_hNotifyWnd = hNotifyWnd;
  m_nMsgId = nMsgId;
}


/**
 * @fn setNotifyCallback
 * @brief  
 *         
 * @param  
 * @param  pfNotifyRoutine
 * @param  hDvr
 * @param  notifyId
 * @param  
 * @param  lpUserData
 * @return
 * @retval  
 */
int 
BscpSocket::setNotifyCallback( int (*pfNotifyRoutine)( HANDLE hDvr, unsigned int notifyId, LPVOID lpUserData ), LPVOID lpUserData )
{
  m_pfNotifyRoutine = pfNotifyRoutine;
  m_pNotifyUserData = lpUserData;
  return 0;
}
/**
 * @fn int BscpSocket::setVersion( unsigned char version )
 * @brief      设置版本号
 * @param[in]  version BSCP版本号
 * @return 无
 */
void
BscpSocket::setVersion( unsigned char version )
{
	m_version = version;
}

/**
 * @fn unsigned char BscpSocket::getVersion()
 * @brief 获取bscp版本号     
 * @return
 * @retval >=0 bscp版本号  
 */
unsigned char 
BscpSocket::getVersion()
{
	return m_version;
}


/**
 * @fn int BscpSocket::Hello(HelloRes* pRes )
 * @brief      心跳，调用Dvr的Hello命令(不用登录)。见《对外接口表_DVR7-1.0.03.pdf》0x0000.0000
 * @param[out] pRes Hello命令的返回结果。见 ::HelloRes
 * @return
 * @retval  0: 成功
 * @retval  其它值: 错误码
 */
int 
BscpSocket::HelloORheart(int cmd)
{
	ConnectDvr();
	char paramIn[PROTOCOL_BUFFER_MAX] = { 0, };
	char paramOut[PROTOCOL_BUFFER_MAX]={ 0, };
	int nInLenth = 0;
	switch (cmd)
	{
	case BSCP_SUNFLOWER_CMD_HELLO: 
	{
		if (m_nGameType==GAME_MOJIAN){
			Req_A0_hello req;
			memcpy(paramIn, &req,sizeof(req));
			nInLenth = strlen(paramIn);
			nInLenth = sizeof(req);
		}else if (m_nGameType == GAME_TLBB || m_nGameType == GAME_TLBB_GREEN) {
			Req_A0_hello_TLBB req;
			memcpy(paramIn, &req, sizeof(req)-(16- req.ipv4_lenth));
			nInLenth = sizeof(req) - (16 - req.ipv4_lenth);
		}else if (m_nGameType==GAME_DAOJIAN)	{
			Req_A0_hello_DAOJIAN req;
			memcpy(paramIn, &req, sizeof(req) - (16 - req.ipv4_lenth));
			nInLenth = sizeof(req) - (16 - req.ipv4_lenth);
			//memcpy(paramIn, &req, sizeof(req.ipv4_lenth)+req.ipv4_lenth);
			//memcpy(paramIn+sizeof(req.ipv4_lenth)+req.ipv4_lenth, &req.serverZoneid, 
			//	sizeof(req.serverZoneid)+sizeof(req.serverGrpid));
			//nInLenth = sizeof(req) - (16 - req.ipv4_lenth);
		}
	}		
		break;
	case BSCP_SUNFLOWER_CMD_HEART:
	{
		if (m_nGameType == GAME_MOJIAN) {
			Req_A1_heart req;
			memcpy(paramIn, &req, sizeof(req));
			nInLenth = sizeof(req);
		}
		else if (m_nGameType == GAME_TLBB || m_nGameType == GAME_TLBB_GREEN) {
			Req_A1_heart_TLBB req;
			memcpy(paramIn, &req, sizeof(req));
			nInLenth = sizeof(req);
		}
		else if (m_nGameType == GAME_DAOJIAN) {
			Req_A1_heart_DAOJIAN req;
			memcpy(paramIn, &req, sizeof(req));
			nInLenth = sizeof(req);
		}
	}
		break;
	default:
		return DVX_ERR_PARA;
	}
	return Execute(cmd, paramIn, nInLenth, paramOut, sizeof(paramOut), 1000);
}



/**
 * @fn int BscpSocket::Execute( int cmd, void* pArg, unsigned int argSize, void* pResult, unsigned int resSize, int timeout )
 * @brief      执行BSCP命令，并返回Dvr的回应。如果超时，则返回超时错。
 * @brief			 每次调用，都应该先调用一下Login，确保在线
 * @param[in]  cmd	BSCP命令。见BSCP协议
 * @param[in]  pArg BSCP命令的参数。见相应命令的BSCP协议
 * @param[in]  argSize BSCP命令的参数的大小(单位：字节)
 * @param[out] pResult BSCP命令的返回结果
 * @param[in]  resSize 参数::pResult的缓冲区的大小
 * @param[in]  timeout 超时时间(单位：毫秒)
 * @return
 * @retval  0: 成功
 * @retval  其它值: 错误码
 */
int 
BscpSocket::Execute( int cmd, void* pArg, unsigned int argSize, void* pResult, unsigned int resSize, int timeout )
{
	if (BSCP_SUNFLOWER_CMD_LOGIN == cmd && m_bHasFailed)
	{
		m_bIsOk = false;
		BSCPLOG(logPrint1(m_hBscpLog, "Execute(LOGIN, %d ) closesocket", timeout));
		m_streamSocket.close();
		m_streamSocket.open(m_ipAddr, m_nPort, 32 * 1024, 32 * 1024, timeout);
		if (!this->m_streamSocket.isConnected())
		{
			m_bHasFailed = true;
			return DVX_ERR_NET_DISCONNECT;
		}
	}

	// 加锁，解锁操作会在pLock的构造、析构函数中自动实现

	LockPtr		pLock(&m_lock);

	if (BSCP_SUNFLOWER_CMD_HELLO != cmd)
		m_bHasFailed = true;

	int val = 0;
	int ret = 0;

	if (m_nGameType == GAME_MOJIAN ) {
		BscpCmdHeader header;
		BscpCmdTail tail;

		/// 2.构造bscp协议头 
		header.length = htons(sizeof(header.packetid) + sizeof(header.packetIndex) + argSize);//
		header.packetIndex = htons(m_sequence);
		header.packetid = htonl(cmd);
		/// 3.填充发送数据，并加密
		/// 3.1加密、组织命令头之外的内容
	// 	header.length = m_objEncrypt.encrypt( m_sendBuf, header.length, m_recvBuf );

		/// 3.2组织整个要发送的数据(命令头 + 命令体)
		memcpy(m_sendBuf, &header, sizeof(header));
		memcpy((m_sendBuf + sizeof(header)), pArg, argSize);
		memcpy((m_sendBuf + sizeof(header) + argSize), &tail, sizeof(tail));
		unsigned int nSendLenth = sizeof(header) + sizeof(tail) + argSize;

		char chSend[2048] = { 0, };
		char chTest[16] = { 0, };
		sprintf(chSend, "Packet %02x--Send: ", cmd);
		for (unsigned int i = 0; i < nSendLenth; i++)
		{
			memset(chTest,0,sizeof(chTest));
			sprintf(chTest, "%02x ", (unsigned char)(*(m_sendBuf+i)));
			strncat(chSend, chTest, strlen(chTest));
		}
		OutputDebugStringA(chSend);
		OutputDebugStringA("\n");
		/// 4.发送命令
		ret = m_streamSocket.write(m_sendBuf, nSendLenth, &val, timeout);
		if (ret != 0)
			return ret;		// 返回WSAGetLastError();

		/// 5.接收回应数据
		//如果是这些命令，则没有回应数据
		//if (cmd == BSCP_SUNFLOWER_CMD_LOGOUT)
		//	return 0;

		int len = 0;
		memset(&header, 0, sizeof(header));

		do
		{
			/// 5.1 接收bscp协议头
			ret = m_streamSocket.read((char*)&header, sizeof(header), &len, timeout);
			if (ret != 0)
			{
				//	return ret;		// 返回WSAGetLastError();
				return DVX_BSCP_NETERR;
			}

			/// 5.2 接收剩余的加密字段
			header.length = ntohs(header.length);
			header.packetid = ntohl(header.packetid);
			if (header.length <= 0)
				return DVX_BSCP_NETERR;
			if (header.length > MAX_CMD_LEN)
			{
				return DVX_BSCP_NETERR;
			}

			int nlastRecv = header.length + sizeof(tail) - sizeof(header.packetid) - sizeof(header.packetIndex);
			ret = m_streamSocket.read(m_recvBuf, nlastRecv, &len, timeout);
			if (ret != 0 || (len != nlastRecv))
			{
				return DVX_BSCP_NETERR;
			}

			m_sequence++;

			/// 5.3对加密字段解密
			//len = m_objEncrypt.decrypt( m_recvBuf, header.length, m_sendBuf );
			//memcpy( &body, m_sendBuf, sizeof( body ) );
		} while (header.packetid != cmd);
		// 	}
			//packetid和packetIndex我给包在头里，返回的时候就不带了
		unsigned int nRetLenth = header.length - sizeof(header.packetid) - sizeof(header.packetIndex);
		memcpy(pResult, m_recvBuf, min(nRetLenth, resSize));
	}
	///////注意：：天龙的头和ptv5 不太一样	
	else if (m_nGameType ==GAME_TLBB || m_nGameType==GAME_TLBB_GREEN || m_nGameType == GAME_DAOJIAN)	{
		BscpCmdHeader_TLBB header;
		BscpCmdTail tail;

		/// 2.构造bscp协议头 
		header.length = htons(sizeof(header.packetid) + sizeof(header.packetIndex) + argSize);//

		header.packetIndex = htons(m_sequence);
		header.packetid = cmd;

		/// 3.填充发送数据，并加密
		/// 3.1加密、组织命令头之外的内容
	// 	header.length = m_objEncrypt.encrypt( m_sendBuf, header.length, m_recvBuf );

		/// 3.2组织整个要发送的数据(命令头 + 命令体)
		memcpy(m_sendBuf, &header, sizeof(header));
		memcpy((m_sendBuf + sizeof(header)), pArg, argSize);
		memcpy((m_sendBuf + sizeof(header) + argSize), &tail, sizeof(tail));

		/// 4.发送命令
		ret = m_streamSocket.write(m_sendBuf, sizeof(header) + sizeof(tail) + argSize, &val, timeout);
		if (ret != 0)
			return ret;		// 返回WSAGetLastError();

		/// 5.接收回应数据
		//可以设置一些命令，没有回应数据
		//if (cmd == BSCP_SUNFLOWER_CMD_LOGOUT)
		//	return 0;

		int len = 0;
		memset(&header, 0, sizeof(header));

		do
		{
			/// 5.1 接收bscp协议头
			ret = m_streamSocket.read((char*)&header, sizeof(header), &len, timeout);
			if (ret != 0)
			{
				//	return ret;		// 返回WSAGetLastError();
				return DVX_BSCP_NETERR;
			}

			/// 5.2 接收剩余的加密字段
			header.length = ntohs(header.length);
			if (header.length <= 0)
				return DVX_BSCP_NETERR;
			if (header.length > MAX_CMD_LEN)
			{
				return DVX_BSCP_NETERR;
			}

			int nlastRecv = header.length + sizeof(tail) - sizeof(header.packetid) - sizeof(header.packetIndex);
			ret = m_streamSocket.read(m_recvBuf, nlastRecv, &len, timeout);
			if (ret != 0 || (len != nlastRecv))
			{
				return DVX_BSCP_NETERR;
			}

			m_sequence++;

			/// 5.3对加密字段解密
			//len = m_objEncrypt.decrypt( m_recvBuf, header.length, m_sendBuf );
			//memcpy( &body, m_sendBuf, sizeof( body ) );
		} while (header.packetid != cmd);
		// 	}
			//packetid和packetIndex我给包在头里，返回的时候就不带了
		unsigned int nRetLenth = header.length - sizeof(header.packetid) - sizeof(header.packetIndex);
		memcpy(pResult, m_recvBuf, min(nRetLenth, resSize));
	}
	

	m_bHasFailed = false;
	if (BSCP_SUNFLOWER_CMD_LOGIN == cmd)
	{
		m_bIsOk = true;
	}

	return ret;
}

/**
 * @fn int BscpSocket::ConnectDvr()
 * @brief      连接Dvr。
 * @return
 * @retval  0 当前已连接
 * @retval  其它值 连接不上Dvr(返回的是WSA的错误码)
 */
int BscpSocket::ConnectDvr( unsigned int msTimeOut )
{
  int ret = 0;
  if( 0 != m_streamSocket.getLastErr() )
  {
    BSCPLOG( logPrint1( m_hBscpLog, "ConnectDvr(%d) closesocket", msTimeOut ) );
    m_streamSocket.close();
    ret = m_streamSocket.open( m_ipAddr, m_nPort, 32*1024, 32*1024, msTimeOut );
  }
  
	return ret;
}

/**
 * @fn IsOk
 * @brief  检查网络连接是否完好    
 * @return bool
 * @retval  true 好
 * @retval  false 已断
 */
bool BscpSocket::IsOk()
{ 
  LockPtr		pLock(&m_lock);
	if( m_bIsOk && !m_bHasFailed )
	{
		BSCPLOG( logPrint1( m_hBscpLog, "BscpSocket::IsOk, m_bIsOk(%s), m_bHasFailed(%s)", m_bIsOk ? "true" : "false", m_bHasFailed ? "true" : "false" ) );
	}
  return m_bIsOk && !m_bHasFailed; 
}

void BscpSocket::heartBeat()
{
	char* buf1 = NULL;
	char* buf2 = NULL;
  int nTryTimes = 0;
//  m_bIsOk = true;
	
	do{
		{// 非主动注册：由SDK发送心跳命令
			WaitForSingleObject( this->m_hEvent, 1000 * 30 );
			if( m_bExit )
				break;
			//int nRet = this->Hello( NULL );
			/*
			Req_A1_heart paramIn;
			char paramOut[PROTOCOL_BUFFER_MAX] = { 0, };
			//Res_A1_heart paramOut;
			int nRet = Execute( BSCP_SUNFLOWER_CMD_HEART, &paramIn, sizeof(paramIn), paramOut, sizeof(paramOut), 1000 );
			*/
			int nRet = HelloORheart(BSCP_SUNFLOWER_CMD_HEART);
			if( nRet == DVX_OK )
			{
				nTryTimes = 0;           
				//        m_bIsOk = true;
			}
			else
			{
				nTryTimes++;

				//五次心跳异常，则认为网络已断，需要重新登录
				if( nTryTimes >= 5 )
				{
					OutputDebugStringA("BscpSocket::heartBeat() error\n");
					BSCPLOG( logPrint1( m_hBscpLog, "heartBeat, nTryTimes(%d)", nTryTimes ) );
					m_bHasFailed = true;
					m_bIsOk = false;

				}
			}
		}

		Sleep(0);
	}while( !this->m_bExit );

}

/**
 * @fn DWORD WINAPI bscpHeartBeat( LPVOID lpvoid )
 * @brief      发送心跳信号线程函数
 * @param[in]  lpvoid 线程参数，是BscpSocket的指针，见 ::BscpSocket
 * @return
 * @retval  0 
 */
DWORD WINAPI 
bscpHeartBeat( LPVOID lpvoid )
{
	BscpSocket* pBscp = (BscpSocket* )lpvoid;
	if ( pBscp == NULL )
	{
		return 0;
	}

   BSRLOG( LogHandle	m_hThreadLog; );
   BSRLOG( m_hThreadLog = logCreate( "ThreadLog", LOGLEVEL_1, LOGMODE_TIME | LOGMODE_TOFILE | LOGMODE_REOPEN));
   BSRLOG( logPrint1( m_hThreadLog, "bscpHeartBeat(0x%08x) in", lpvoid ) );

	pBscp->heartBeat();	

   BSRLOG( logPrint1( m_hThreadLog, "bscpHeartBeat(0x%08x) out", lpvoid ) );
   BSRLOG( logDestory( m_hThreadLog ); );
   BSRLOG( m_hThreadLog = NULL; );
	return 0;
}

void BscpSocket::SetDvrHandle(DvxHandle pDvr)
{
	if (pDvr == NULL)
	{
		return;
	}
	m_hDvx = pDvr;
}

int BscpSocket::GetChInfoNotLogin(unsigned int nCmd, const void* parmIn, unsigned int nLenthIn, void* parmOut, 
                                  unsigned int nLenthOut, unsigned int timeOut)
{
  ConnectDvr();

  return Execute( nCmd, (void*)parmIn, nLenthIn, parmOut, nLenthOut, timeOut );
}

int BscpSocket::initNoHeartBeat( unsigned int ipAddr, unsigned short port,unsigned int timeOut/*=10000*/ )
{
  int nRet=0;	
	do 
	{
		m_ipAddr			= ipAddr;
		m_nPort				= port;	
		/// 2.申请 发送、接收 缓冲区
		__try
		{
			m_sendBuf = (char*)new char[MAX_CMD_LEN];
			m_recvBuf = (char*)new char[MAX_CMD_LEN];
		}__except(EXCEPTION_EXECUTE_HANDLER)
		{
			OutputDebugStringA("=====failed to malloc memory in bscp class====\n");
		}

		if ( NULL == m_sendBuf || NULL == m_recvBuf )
		{
			nRet = DVX_ERR_MALLOCMEMORY;
			break;
		}

		/// 3.建立网络连接
		m_streamSocket.close();
		m_streamSocket.open( m_ipAddr, m_nPort, 32*1024, 32*1024,timeOut);
		if ( m_streamSocket.isConnected() == false )
		{
			nRet = DVX_ERR_NET_DISCONNECT;
			break;
		}
    m_bHasFailed = false;

		/// 6.成功退出
		return 0;
	} while (0);
  
	/// 如果没有init成功，则销毁当前已申请的数据
	destory();
	return nRet;
}
