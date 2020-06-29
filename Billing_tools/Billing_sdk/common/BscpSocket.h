/**  $Id$ $DateTime$
 *  @file  BscpSocket.h
 *  @brief BSCP模块，调用加密与网络连接两个子模块。接收上层任意数据，向Dvr发送，并接收Dvr回应数据。
 *  @version 0.0.1
 *  @since 0.0.1
 */


#pragma once

#include "Encrypt.h"
#include "StreamSocket.h"
#include "Lock.h"
// #include "AudioFifo.h"
#include "bsrLog.h"
#include "../BillSdkType.h"

// 日志
#define BSCPLOG(x) BSRLOG(x)

/** @defgroup grBSCP	BSCP模块(BSCP Mod)
 *  @ingroup grSpeech
 *  @{
 */




/** @}*/	// end of BSCP模块所需结构定义

/** @class BscpSocket
 *  @brief BSCP模块的主类
 */
class BscpSocket
{
public:
	BscpSocket();
	~BscpSocket();

public:
	int init(unsigned int ipAddr, unsigned short port, char gametype=0, bool bHeart=true, unsigned int timeOut=10000 );
// 	int init( SOCKET sDvr );
	void destory();
  void setNotifyWnd( HWND hNotifyWnd, int nMsgId );
  int  setNotifyCallback( int (*pfNotifyRoutine)( HANDLE hDvr, unsigned int notifyId, LPVOID lpUserData ), LPVOID lpUserData );
	void setVersion( unsigned char version );
	unsigned char getVersion();

	int HelloORheart(int cmd);
	int Execute( int cmd, void* pArg, unsigned int argSize, void* pResult, unsigned int resSize, int timeout = 10000);
  bool IsOk();
  unsigned int GetCurIpAddr(){return m_ipAddr;}
  unsigned int GetCurPort(){return m_nPort;}
  //无需登录
  int initNoHeartBeat(unsigned int ipAddr, unsigned short port,unsigned int timeOut=10000);
  int GetChInfoNotLogin(unsigned int nCmd, const void* parmIn, unsigned int nLenthIn, 
                          void* parmOut, unsigned int nLenthOut, unsigned int timeOut);

protected:
	int ConnectDvr( unsigned int msTimeOut = 10000 );

public:	// 供心跳线程使用的回调函数
	void heartBeat();
	void SetDvrHandle( DvxHandle pDvr);
protected:
	bool    m_bExit;
	HANDLE  m_hEvent;
	HANDLE  m_hBeatHeart;		  /**< 心跳线程句柄 */
  HWND    m_hNotifyWnd;
  int     m_nMsgId;
	DvxHandle m_hDvx;
protected:
	char*         m_sendBuf;	/**< 发送数据缓冲区 */
	char*         m_recvBuf;	/**< 接收数据缓冲区 */

	unsigned int  m_sequence;
	unsigned char m_version;

protected:
	unsigned int			      m_ipAddr;		/**< Dvr的Ip地址 */
	int			      m_nPort;		/**< Dvr的端口号 */
	bool		      m_bReg;			/**< true: 主动注册，false非主动注册 */
	
protected:
	Encrypt		    m_objEncrypt;	  /**< 加密子模块 */
	StreamSocket	m_streamSocket;	/**< 网络连接子模块 */
	Lock				  m_lock;				  /**< 互斥锁，因为BSCP模块要同时被很多模块使用，所以一定要加锁 */
  bool          m_bHasFailed;
  bool          m_bIsOk;
  unsigned char m_nGameType;

protected:	// 只有在主动注册时才会用到
// 	Lock		      m_lckFifo;
// 	AudioFifo*	  m_fifo;
  //增加断网回调
  int (*m_pfNotifyRoutine)( HANDLE hDvr, unsigned int notifyId, LPVOID lpUserData );
  LPVOID m_pNotifyUserData;
protected:  // Log
  BSCPLOG( LogHandle	m_hBscpLog; );

};
/** @}*/	// end of BSCP



