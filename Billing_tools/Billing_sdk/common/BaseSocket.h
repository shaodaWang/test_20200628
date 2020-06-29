/** $Id$ $DateTime$
*  @file  BaseSocket.h
*  @brief socket客户端tcp网络通信类
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
 *  @brief socket客户端tcp网络通信类\r\n
 */
class BaseSocket
{
public:
  BaseSocket(void);
  ~BaseSocket(void);

  //主要接口函数
  int open( const char *ip, unsigned short port); //port为本机字节
  int open( unsigned int ip, unsigned short port);//ip网络字节，port为本机字节
  int close(void);
  bool isConnected();
  int send(const void *pBuf, int iBufLen, int *pSendSize,int timeout); //timeout 为毫秒单位
  int recv(void *pBuff, int iBuffLen, int *pRecvSize, int timeout);  //timeout为毫秒单位，必须接受指定数据，否则错误返回
  int recvStream(void *pBuff, int iBuffLen, int timeout);  //timeout为毫秒单位，实际接收数据为准
  int readEx( char*pBuff, int iBuffLen, int timeout );
  int recvTunnel(char*pBuff, int iBuffLen, int *pReadSize, int timeout);
  int SendTunnel(const void *pBuf, int iBufLen,int *pWriteSize, int timeout);

private:	
  static bool init( int majorVersion, int minorVersion);
  static bool unInit();
  int write(const void *pBuf, int iBufLen,int *pWriteSize, int timeout); //timeout 为毫秒单位
  int read(void *pBuff, int iBuffLen, int *pReadSize, int timeout);  //timeout为毫秒单位
  int connect( const char *strIntf, unsigned int iPort);
  bool isDiscnnted( );
 
	
private:
  bool    m_isConnected;
  SOCKET  m_socket;
};
#endif
/** @}*/ // end of BaseSocket
// end of __BaseSocket_H_