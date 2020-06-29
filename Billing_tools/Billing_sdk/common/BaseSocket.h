/** $Id$ $DateTime$
*  @file  BaseSocket.h
*  @brief socket�ͻ���tcp����ͨ����
*  @version 0.0.1
*  @since 0.0.1
*/


#pragma once

#ifndef __BaseSocket_H_
#define __BaseSocket_H_

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

/**@class BaseSocket
 *  @brief socket�ͻ���tcp����ͨ����\r\n
 */
class BaseSocket
{
public:
  BaseSocket(void);
  ~BaseSocket(void);

  //��Ҫ�ӿں���
  int open( const char *ip, unsigned short port); //portΪ�����ֽ�
  int open( unsigned int ip, unsigned short port);//ip�����ֽڣ�portΪ�����ֽ�
  int close(void);
  bool isConnected();
  int send(const void *pBuf, int iBufLen, int *pSendSize,int timeout); //timeout Ϊ���뵥λ
  int recv(void *pBuff, int iBuffLen, int *pRecvSize, int timeout);  //timeoutΪ���뵥λ���������ָ�����ݣ�������󷵻�
  int recvStream(void *pBuff, int iBuffLen, int timeout);  //timeoutΪ���뵥λ��ʵ�ʽ�������Ϊ׼
  int readEx( char*pBuff, int iBuffLen, int timeout );
  int recvTunnel(char*pBuff, int iBuffLen, int *pReadSize, int timeout);
  int SendTunnel(const void *pBuf, int iBufLen,int *pWriteSize, int timeout);

private:	
  static bool init( int majorVersion, int minorVersion);
  static bool unInit();
  int write(const void *pBuf, int iBufLen,int *pWriteSize, int timeout); //timeout Ϊ���뵥λ
  int read(void *pBuff, int iBuffLen, int *pReadSize, int timeout);  //timeoutΪ���뵥λ
  int connect( const char *strIntf, unsigned int iPort);
  bool isDiscnnted( );
 
	
private:
  bool    m_isConnected;
  SOCKET  m_socket;
};
#endif
/** @}*/ // end of BaseSocket
// end of __BaseSocket_H_