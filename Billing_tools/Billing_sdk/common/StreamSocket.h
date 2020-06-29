/** $Id$ $DateTime$
 *  @file  StreamSocket.h
 *  @brief StreamSocket类，只负责TCP网络的连接、发送/接收数据。
 *  @version 0.0.1
 *  @since 0.0.1
 */


#ifndef BSTAR_STREAM_SOCKET_
#define BSTAR_STREAM_SOCKET_

#include <winsock2.h>

/** @defgroup grTcpSocket TCP网络管理模块(TCP Socket Manange Mod)
 *  @ingroup grBSCP
 *  @{
 */


/** @class StreamSocket
 *  @brief TCP网络管理
 */
class StreamSocket
{
public:
	StreamSocket(void);
	~StreamSocket(void);

public:
	int open( const char *ip, unsigned short port, int sendBufSize , int recvBufSize );
	int open( unsigned int ip, unsigned short port, int sendBufSize , int recvBufSize, unsigned int msTimeOut = 10000 );
	int open( SOCKET s, int sendBufSize , int recvBufSize );
	void close( );
	int write( char* buf, int nSize, int *pWriteSize, int timeout = 1000 );
	int read( char* buf, int maxSize, int *pReadSize, int timeout = 1000 );
	bool isConnected( ){ return m_bIsConnected; };
  DWORD getLastErr(){ return m_dwLassErr; };

private:
	int writeWithTimeout( char* buf, int nSize, int* pWriteSize, int timeout );
	int readWithTimeout( char* buf, int maxSize, int* pReadSize, int timeout );
	bool isDiscnnted( );

private:
	SOCKET  m_socket;	      ///< TCP Socket
	bool    m_bIsConnected;	///< 仅标识网络连接是否建立: true: 建立；false:未建立
  DWORD   m_dwLassErr;    ///< 记录上次调用socket的err，
};

/** @}*/ // end of TCP网络管理模块

#endif