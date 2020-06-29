/**  $Id$ $DateTime$
 *  @file  BscpSocket.h
 *  @brief BSCPģ�飬���ü�������������������ģ�顣�����ϲ��������ݣ���Dvr���ͣ�������Dvr��Ӧ���ݡ�
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

// ��־
#define BSCPLOG(x) BSRLOG(x)

/** @defgroup grBSCP	BSCPģ��(BSCP Mod)
 *  @ingroup grSpeech
 *  @{
 */




/** @}*/	// end of BSCPģ������ṹ����

/** @class BscpSocket
 *  @brief BSCPģ�������
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
  //�����¼
  int initNoHeartBeat(unsigned int ipAddr, unsigned short port,unsigned int timeOut=10000);
  int GetChInfoNotLogin(unsigned int nCmd, const void* parmIn, unsigned int nLenthIn, 
                          void* parmOut, unsigned int nLenthOut, unsigned int timeOut);

protected:
	int ConnectDvr( unsigned int msTimeOut = 10000 );

public:	// �������߳�ʹ�õĻص�����
	void heartBeat();
	void SetDvrHandle( DvxHandle pDvr);
protected:
	bool    m_bExit;
	HANDLE  m_hEvent;
	HANDLE  m_hBeatHeart;		  /**< �����߳̾�� */
  HWND    m_hNotifyWnd;
  int     m_nMsgId;
	DvxHandle m_hDvx;
protected:
	char*         m_sendBuf;	/**< �������ݻ����� */
	char*         m_recvBuf;	/**< �������ݻ����� */

	unsigned int  m_sequence;
	unsigned char m_version;

protected:
	unsigned int			      m_ipAddr;		/**< Dvr��Ip��ַ */
	int			      m_nPort;		/**< Dvr�Ķ˿ں� */
	bool		      m_bReg;			/**< true: ����ע�ᣬfalse������ע�� */
	
protected:
	Encrypt		    m_objEncrypt;	  /**< ������ģ�� */
	StreamSocket	m_streamSocket;	/**< ����������ģ�� */
	Lock				  m_lock;				  /**< ����������ΪBSCPģ��Ҫͬʱ���ܶ�ģ��ʹ�ã�����һ��Ҫ���� */
  bool          m_bHasFailed;
  bool          m_bIsOk;
  unsigned char m_nGameType;

protected:	// ֻ��������ע��ʱ�Ż��õ�
// 	Lock		      m_lckFifo;
// 	AudioFifo*	  m_fifo;
  //���Ӷ����ص�
  int (*m_pfNotifyRoutine)( HANDLE hDvr, unsigned int notifyId, LPVOID lpUserData );
  LPVOID m_pNotifyUserData;
protected:  // Log
  BSCPLOG( LogHandle	m_hBscpLog; );

};
/** @}*/	// end of BSCP



