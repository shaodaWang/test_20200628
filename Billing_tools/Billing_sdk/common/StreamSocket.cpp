/**  $Id$ $DateTime$
 *  @file  StreamSocket.cpp
 *  @brief 实现TCP网络的连接、发送/接收数据
 *  @version 0.0.1
 *  @since 0.0.1
 */


#include "stdafx.h"
#include "StreamSocket.h"
#include <assert.h>
#include "bsrlog.h"


#define  MAX_SEND_SIZE_BYTE 500
/**
 * @fn StreamSocket::StreamSocket
 * @brief      StreamSocket的构造函数
 */
StreamSocket::StreamSocket(void)
: m_socket(INVALID_SOCKET)
, m_bIsConnected(false)
, m_dwLassErr(0)
{
}

/**
 * @fn StreamSocket::~StreamSocket
 * @brief      StreamSocket类的析构函数 
 */
StreamSocket::~StreamSocket(void)
{
	close( );
}

/**
 * @fn int StreamSocket::open( const char *ip, unsigned short port, int sendBufSize , int recvBufSize )
 * @brief  open的一个重载函数，负责打开网络连接    
 * @param[in]  ip   字符串形式的IP地址，例："127.0.0.1"
 * @param[in]  port 服务器的端口号
 * @param[in]  sendBufSize TCP内部发送缓冲区的大小
 * @param[in]  recvBufSize TCP内部接收缓冲区的大小
 * @return
 * @retval  0: 成功
 * @retval  其它值: 错误码，返回WSAGetLastError的错误值
 */
int 
StreamSocket::open( const char *ip, unsigned short port, int sendBufSize , int recvBufSize )
{
  return this->open( inet_addr(ip), port, sendBufSize, recvBufSize );
}

/**
 * @fn int StreamSocket::open( unsigned int ip, unsigned short port, int sendBufSize , int recvBufSize )
 * @brief      open的一个重载函数，负责打开网络连接
 * @param[in]  ip   服务器的IP地址
 * @param[in]  port 服务器的端口号
 * @param[in]  sendBufSize TCP内部发送缓冲区的大小
 * @param[in]  recvBufSize TCP内部接收缓冲区的大小
 * @return
 * @retval  0: 成功
 * @retval  其它值: 错误码，返回WSAGetLastError的错误值
 */
int 
StreamSocket::open( unsigned int ip, unsigned short port, int sendBufSize , int recvBufSize, unsigned int msTimeOut )
{
   do
   {
      /// 1.创建socket
      m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
      if ( m_socket == INVALID_SOCKET )
      {
         break;
      }

      /// 2.把socket设置为异步模式
      int iMode = 1;
      if ( ioctlsocket( m_socket, FIONBIO, (u_long FAR*) &iMode) != 0 )
      {
         break;
      }

      BOOL sopt = TRUE;
      setsockopt( m_socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&sopt, sizeof(BOOL));
      setsockopt( m_socket, SOL_SOCKET, SO_SNDBUF, (const char* )&sendBufSize, sizeof( sendBufSize ) );
      setsockopt( m_socket, SOL_SOCKET, SO_RCVBUF, (const char* )&recvBufSize, sizeof( recvBufSize ) );

			/// 3.和server建立连接
      sockaddr_in addr; 
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = ip;
      addr.sin_port = htons( port );

      connect( m_socket, (SOCKADDR*) &addr, sizeof(sockaddr_in) );
      timeval tv;
      fd_set fd;
      FD_ZERO( &fd );
      FD_SET( m_socket , &fd );

      BSRLOG( LogHandle	m_hThreadLog; );
      BSRLOG( m_hThreadLog = logCreate( "ThreadLog", LOGLEVEL_1, LOGMODE_TIME | LOGMODE_TOFILE | LOGMODE_REOPEN));
      BSRLOG( logPrint1( m_hThreadLog, "StreamSocket::socket(0x%08x) connect", this ) );
      BSRLOG( logDestory( m_hThreadLog ); );
      BSRLOG( m_hThreadLog = NULL; );

      tv.tv_sec = msTimeOut/1000;
      tv.tv_usec = msTimeOut%1000 * 1000;

      if ( select(m_socket+1, NULL, &fd, NULL, &tv ) == 1 )
      {
         m_bIsConnected = true;
         m_dwLassErr = 0;

         BOOL bKeepAlive = TRUE;
         //设置为长连接
         setsockopt( m_socket, SOL_SOCKET, SO_KEEPALIVE, (const char* )&bKeepAlive, sizeof( bKeepAlive ) );
         m_dwLassErr = 0;
         return 0;
      }
   }while( 0 );

   m_dwLassErr = WSAGetLastError( );
   return m_dwLassErr;
}

/**
 * @fn int StreamSocket::open( SOCKET s, int sendBufSize , int recvBufSize )
 * @brief      为了适应主动注册，添加的接口
 * @param[in]  s Dvr端的SOCKET
 * @param[in]  sendBufSize TCP内部发送缓冲区的大小
 * @param[in]  recvBufSize TCP内部接收缓冲区的大小
 * @return int
 * @retval  0 成功
 * @retval  其它值 错误码
 */
int StreamSocket::open( SOCKET s, int sendBufSize , int recvBufSize )
{
	do
	{
		m_socket = s;

		timeval tv;
		fd_set fd;
		FD_ZERO( &fd );
		FD_SET( m_socket , &fd );

		tv.tv_sec = 10;
		tv.tv_usec = 0;

		if ( select( 1, NULL, &fd, NULL, &tv ) == 1 )
		{
			m_bIsConnected = true;

      BOOL bKeepAlive = TRUE;
      //设置为长连接
      setsockopt( m_socket, SOL_SOCKET, SO_KEEPALIVE, (const char* )&bKeepAlive, sizeof( bKeepAlive ) );
      m_dwLassErr = 0;
			return 0;
		}
	}while( 0 );

  m_dwLassErr = WSAGetLastError( );
  return m_dwLassErr;
}

/**
 * @fn void StreamSocket::close( )
 * @brief   关闭网络连接
 * @return
 * @retval  无
 */
void 
StreamSocket::close( )
{
   if ( m_socket != INVALID_SOCKET )
   {
      closesocket( m_socket );
      m_socket = INVALID_SOCKET;

      BSRLOG( LogHandle	m_hThreadLog; );
      BSRLOG( m_hThreadLog = logCreate( "ThreadLog", LOGLEVEL_1, LOGMODE_TIME | LOGMODE_TOFILE | LOGMODE_REOPEN));
      BSRLOG( logPrint1( m_hThreadLog, "StreamSocket::socket(0x%08x) out", this ) );
      BSRLOG( logDestory( m_hThreadLog ); );
      BSRLOG( m_hThreadLog = NULL; );
   }
	 m_bIsConnected = false;
   m_dwLassErr = 0;
}

/**
 * @fn int StreamSocket::write( char* buf, int nSize, int *pWriteSize, int timeout )
 * @brief      通过socket发送指定长度的数据，直到socket出错
 * @param[in]  buf 待发送数据缓冲区
 * @param[in]  nSize 待发送数据长度
 * @param[out]  pWriteSize 整型数指针，接收实际发送的数据长度，以字节为单位
 * @param[in] timeout 超时时间(单位：毫秒)
 * @return
 * @retval  0: 成功
 * @retval  其它值: 错误码
 */
int 
StreamSocket::write( char* buf, int nSize, int *pWriteSize, int timeout )
{
   int left = nSize;
   int ret = 0;
   int dataSent = 0;

   do
   {
      dataSent = 0;
      ret = writeWithTimeout( ( buf + nSize - left ), left, &dataSent, timeout );
      if ( ret != 0 )
      {
         break;

         if ( isDiscnnted( ) )
         {
            if ( ( ret == WSANOTINITIALISED ) || ( ret == WSAEFAULT ) || (ret == WSAECONNABORTED) || (ret == WSAECONNRESET ) || (ret == WSAENOTSOCK))
            {
               break;
            }
         }
      }else
      {
        if( dataSent <= 0 )
          return 2;

         left -= dataSent;
      }

   }while( left > 0 );

   *pWriteSize = nSize - left;

   m_dwLassErr = ret;
   return ret;
}

/**
 * @fn int StreamSocket::read( char* buf, int nSize, int *pReadSize, int timeout )
 * @brief      通过socket读取指定长度的数据，直到socket出错
 * @param[in]  buf 接收数据缓冲区
 * @param[in]  nSize 待接收数据长度
 * @param[out] pReadSize 指向整型数指针，接收实际读取的数据长度，以字节为单位
 * @param[in] timeout 超时时间(单位：毫秒)
 * @return
 * @retval  0: 成功
 * @retval  其它值: 错误码
 */
int 
StreamSocket::read( char* buf, int nSize, int *pReadSize, int timeout )
{
   int left = nSize;
   int ret  = 0;
   int rcv  = 0;

   int nCount = 0;
   do{
      rcv = 0;
      ret = readWithTimeout( ( buf + nSize - left ), left, &rcv, timeout );
      if ( ret != 0 )
      {     
        break; //如果读超时，直接判socket出错
//            if ( (ret == WSANOTINITIALISED ) || ( ret == WSAEFAULT ) || (ret == WSAECONNRESET) )
//            {
////               m_bIsConnected = false;
//               break;
//            }
         
      }
      else
      {
        if( rcv<=0 )
          return 2;

        if( rcv<=0 )
          nCount++;

        left -= rcv;
      }

   }while( left > 0 && nCount<10);

   *pReadSize = nSize - left;

   m_dwLassErr = ret;
   return ret;
}


/**
 * @fn int StreamSocket::writeWithTimeout( char* buf, int nSize, int* pWriteSize, int timeout )
 * @brief     内部函数，在指定超时时间内发送数据
 * @param[in] buf 待发送数据缓冲区
 * @param[in] nSize 待发送数据长度
 * @param[in] 指向整型数指针，接收实际发送的数据长度，以字节为单位
 * @param[in] timeout 超时时间(单位：毫秒)
 * @return
 * @retval  0: 成功
 * @retval  其它值: 错误码
 */
int 
StreamSocket::writeWithTimeout( char* buf, int nSize, int* pWriteSize, int timeout )
{
   int ret = 0;
   int nTryCount = 0;
   DWORD err = 0;

   do
   {
      timeval tv;
      fd_set fd;
      FD_ZERO( &fd );
      FD_SET( m_socket , &fd );

      tv.tv_sec = 0;
      tv.tv_usec = timeout * 1000;
      ret = select( 1, NULL, &fd, NULL, &tv );
      if ( ret == 1 )
      {
        int nLeftSize = nSize;
        char *p = buf;
        int nSendLen = 0;
        int nTotalSendLen = 0;
        FD_ISSET(m_socket,&fd);

        while(nLeftSize > MAX_SEND_SIZE_BYTE)
        {
          nSendLen = send( m_socket, p, MAX_SEND_SIZE_BYTE, 0 );
          if ( nSendLen == -1 )
          {
            err = GetLastError();
            if ( 10035 == err )
            {
              if ( 20 == nTryCount )
              {
                break;
              }

              nTryCount++;

              Sleep(50);
              continue;
            }

            //若出错，判断socket是否断开
            if ( isDiscnnted( ) )
            {
              //               m_bIsConnected = false;
            }
            break;
          }

          nTryCount = 0;

          nLeftSize -= MAX_SEND_SIZE_BYTE; 
          p += MAX_SEND_SIZE_BYTE;
          nTotalSendLen += nSendLen;
        }

        if ( nSendLen == -1 )
        {
          break;
        }

        if ( nLeftSize > 0 )
        {
          nSendLen = send( m_socket, p, nLeftSize, 0 );
          if ( nSendLen == -1 )
          {
            err = GetLastError();
            if ( 10035 == err )
            {
              Sleep(200);
              nSendLen = send( m_socket, p, nLeftSize, 0 );

              if ( -1 == nSendLen )
              {
                //若出错，判断socket是否断开
                if ( isDiscnnted( ) )
                {
                  //               m_bIsConnected = false;
                }
                break;
              }
            }

          }
        }

        nTotalSendLen += nSendLen;
        *pWriteSize = nTotalSendLen;

#if 0
         *pWriteSize = send( m_socket, buf, nSize, 0 );
         if ( *pWriteSize == -1 )
         {
            //若出错，判断socket是否断开
            if ( isDiscnnted( ) )
            {
//               m_bIsConnected = false;
            }
            break;
         }else
         {
            break;
         }

#endif
      }else if ( ret == -1 )
      {
         //若出错，判断socket是否断开
         if ( isDiscnnted( ) )
         {
//            m_bIsConnected = false;
         }
         break;
      }
   }while(0);

   return WSAGetLastError( );
}

/**
 * @fn int StreamSocket::readWithTimeout( char* buf, int nSize, int* pReadSize, int timeout )
 * @brief     内部函数，在指定超时时间内接收数据
 * @param[in] buf 接收数据缓冲区
 * @param[in] nSize 要接收的数据长度
 * @param[in] pReadSize 指向整型数指针，接收实际接收到的数据长度，以字节为单位
 * @param[in] timeout 超时时间(单位：毫秒)
 * @return
 * @retval  0: 成功
 * @retval  其它值: 错误码
 */
int 
StreamSocket::readWithTimeout( char* buf, int nSize, int* pReadSize, int timeout )
{
   int ret = 0;
   int count = 0;

	 timeval tv;
	 fd_set fd;
	 FD_ZERO( &fd );
	 FD_SET( m_socket , &fd );

	 do 
	 {
		 tv.tv_sec = 0;
		 tv.tv_usec = timeout * 1000;
		 ret = select( 1, &fd, NULL, NULL, &tv );
		 if ( ret == 1 )
		 {
			 *pReadSize = recv( m_socket, buf, nSize, 0 );
			 if ( *pReadSize <= 0 )
			 {
				 //若出错，判断socket是否断开
				 if ( isDiscnnted( ) )
				 {
//					 m_bIsConnected = false;
				 }
				 count ++;
				 if (count >3)
				 {
//					 m_bIsConnected = false;
					 return 502;
				 }
			 }else
			 {
				 count = 0;
				 break;
			 }

		 }else if ( ret == -1 )
		 {
			 //若出错，判断socket是否断开
			 if ( isDiscnnted( ) )
			 {
//				 m_bIsConnected = false;
				 break;
			 }
		 }
     else
     {
       break;
//       return 9;  // time out
     }
	 } while (0);
	 
   return WSAGetLastError( );
}

/**
 * @fn bool StreamSocket::isDiscnnted( )
 * @brief      内部函数，判断socket 连接是否断开
 * @return
 * @retval  true: 连接断开
 * @retval  false: 连接正常
 */
bool
StreamSocket::isDiscnnted( )
{
#if 1
  m_dwLassErr = WSAGetLastError();
   switch( m_dwLassErr )
   {
   case WSAENETDOWN:
   case WSAENETRESET:
   case WSAENOTCONN:
   case WSAESHUTDOWN:
   case WSAECONNABORTED:
   case WSAECONNRESET:
   case WSAETIMEDOUT:
      return true;
   }
#else
   if ( m_dwLassErr != 0)
   {
      return true;
   }
#endif
   return false;
}
