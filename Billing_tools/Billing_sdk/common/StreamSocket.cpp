/**  $Id$ $DateTime$
 *  @file  StreamSocket.cpp
 *  @brief ʵ��TCP��������ӡ�����/��������
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
 * @brief      StreamSocket�Ĺ��캯��
 */
StreamSocket::StreamSocket(void)
: m_socket(INVALID_SOCKET)
, m_bIsConnected(false)
, m_dwLassErr(0)
{
}

/**
 * @fn StreamSocket::~StreamSocket
 * @brief      StreamSocket����������� 
 */
StreamSocket::~StreamSocket(void)
{
	close( );
}

/**
 * @fn int StreamSocket::open( const char *ip, unsigned short port, int sendBufSize , int recvBufSize )
 * @brief  open��һ�����غ������������������    
 * @param[in]  ip   �ַ�����ʽ��IP��ַ������"127.0.0.1"
 * @param[in]  port �������Ķ˿ں�
 * @param[in]  sendBufSize TCP�ڲ����ͻ������Ĵ�С
 * @param[in]  recvBufSize TCP�ڲ����ջ������Ĵ�С
 * @return
 * @retval  0: �ɹ�
 * @retval  ����ֵ: �����룬����WSAGetLastError�Ĵ���ֵ
 */
int 
StreamSocket::open( const char *ip, unsigned short port, int sendBufSize , int recvBufSize )
{
  return this->open( inet_addr(ip), port, sendBufSize, recvBufSize );
}

/**
 * @fn int StreamSocket::open( unsigned int ip, unsigned short port, int sendBufSize , int recvBufSize )
 * @brief      open��һ�����غ������������������
 * @param[in]  ip   ��������IP��ַ
 * @param[in]  port �������Ķ˿ں�
 * @param[in]  sendBufSize TCP�ڲ����ͻ������Ĵ�С
 * @param[in]  recvBufSize TCP�ڲ����ջ������Ĵ�С
 * @return
 * @retval  0: �ɹ�
 * @retval  ����ֵ: �����룬����WSAGetLastError�Ĵ���ֵ
 */
int 
StreamSocket::open( unsigned int ip, unsigned short port, int sendBufSize , int recvBufSize, unsigned int msTimeOut )
{
   do
   {
      /// 1.����socket
      m_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
      if ( m_socket == INVALID_SOCKET )
      {
         break;
      }

      /// 2.��socket����Ϊ�첽ģʽ
      int iMode = 1;
      if ( ioctlsocket( m_socket, FIONBIO, (u_long FAR*) &iMode) != 0 )
      {
         break;
      }

      BOOL sopt = TRUE;
      setsockopt( m_socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&sopt, sizeof(BOOL));
      setsockopt( m_socket, SOL_SOCKET, SO_SNDBUF, (const char* )&sendBufSize, sizeof( sendBufSize ) );
      setsockopt( m_socket, SOL_SOCKET, SO_RCVBUF, (const char* )&recvBufSize, sizeof( recvBufSize ) );

			/// 3.��server��������
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
         //����Ϊ������
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
 * @brief      Ϊ����Ӧ����ע�ᣬ��ӵĽӿ�
 * @param[in]  s Dvr�˵�SOCKET
 * @param[in]  sendBufSize TCP�ڲ����ͻ������Ĵ�С
 * @param[in]  recvBufSize TCP�ڲ����ջ������Ĵ�С
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
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
      //����Ϊ������
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
 * @brief   �ر���������
 * @return
 * @retval  ��
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
 * @brief      ͨ��socket����ָ�����ȵ����ݣ�ֱ��socket����
 * @param[in]  buf ���������ݻ�����
 * @param[in]  nSize ���������ݳ���
 * @param[out]  pWriteSize ������ָ�룬����ʵ�ʷ��͵����ݳ��ȣ����ֽ�Ϊ��λ
 * @param[in] timeout ��ʱʱ��(��λ������)
 * @return
 * @retval  0: �ɹ�
 * @retval  ����ֵ: ������
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
 * @brief      ͨ��socket��ȡָ�����ȵ����ݣ�ֱ��socket����
 * @param[in]  buf �������ݻ�����
 * @param[in]  nSize ���������ݳ���
 * @param[out] pReadSize ָ��������ָ�룬����ʵ�ʶ�ȡ�����ݳ��ȣ����ֽ�Ϊ��λ
 * @param[in] timeout ��ʱʱ��(��λ������)
 * @return
 * @retval  0: �ɹ�
 * @retval  ����ֵ: ������
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
        break; //�������ʱ��ֱ����socket����
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
 * @brief     �ڲ���������ָ����ʱʱ���ڷ�������
 * @param[in] buf ���������ݻ�����
 * @param[in] nSize ���������ݳ���
 * @param[in] ָ��������ָ�룬����ʵ�ʷ��͵����ݳ��ȣ����ֽ�Ϊ��λ
 * @param[in] timeout ��ʱʱ��(��λ������)
 * @return
 * @retval  0: �ɹ�
 * @retval  ����ֵ: ������
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

            //�������ж�socket�Ƿ�Ͽ�
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
                //�������ж�socket�Ƿ�Ͽ�
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
            //�������ж�socket�Ƿ�Ͽ�
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
         //�������ж�socket�Ƿ�Ͽ�
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
 * @brief     �ڲ���������ָ����ʱʱ���ڽ�������
 * @param[in] buf �������ݻ�����
 * @param[in] nSize Ҫ���յ����ݳ���
 * @param[in] pReadSize ָ��������ָ�룬����ʵ�ʽ��յ������ݳ��ȣ����ֽ�Ϊ��λ
 * @param[in] timeout ��ʱʱ��(��λ������)
 * @return
 * @retval  0: �ɹ�
 * @retval  ����ֵ: ������
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
				 //�������ж�socket�Ƿ�Ͽ�
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
			 //�������ж�socket�Ƿ�Ͽ�
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
 * @brief      �ڲ��������ж�socket �����Ƿ�Ͽ�
 * @return
 * @retval  true: ���ӶϿ�
 * @retval  false: ��������
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
