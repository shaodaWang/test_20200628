/** $Id$ $DateTime$
*  @file  BaseSocket.cpp
*  @brief socket客户端tcp网络通信类
*  @version 0.0.1
*  @since 0.0.1
*/


#include "stdafx.h"
#include "BaseSocket.h"

/**
 * @fn BaseSocket::BaseSocket()
 * @brief  BaseSocket的构造函数
 */
BaseSocket::BaseSocket(void)
{
  m_socket = 0;
  m_isConnected = false;
  init(2,2);
}

/**
 * @fn BaseSocket::~BaseSocket()
 * @brief  BaseSocket的析构函数
 */
BaseSocket::~BaseSocket(void)
{
  unInit();
}

/**
 * @fn BaseSocket::init(int majorVersion,int minorVersion )
 * @brief  socket初始化
 * @param[in] majorVersion socket主版本号
 * @param[in] minorVersion socket次版本号
 * @return bool
 * @retval true 成功
 * @retval false 失败
 */
bool 
BaseSocket::init( int majorVersion,int minorVersion )
{
  WSADATA wsaData;
  WORD wVersion;
  wVersion = MAKEWORD(minorVersion, majorVersion);
  int nResult = WSAStartup(wVersion,&wsaData);

  if (nResult != 0)
  {
    return false;
  }

  return true;
}

/**
 * @fn BaseSocket::unInit(int majorVersion,int minorVersion )
 * @brief  socket注销
 * @return bool
 * @retval true 成功
 * @retval false 失败
 */
bool 
BaseSocket::unInit()
{
  int nResult = WSACleanup();
  if (nResult != 0)
  {
    return false;
  }

  return true;
}

/**
 * @fn BaseSocket::open(const char* ip, unsigned short port)
 * @brief  创建socket连接
 * @param[in] ip 服务器ip 
 * @param[in] port 服务器端口
 * @return int
 * @retval 0 成功
 * @retval 其他值 失败
 */
int
BaseSocket::open(const char *ip, unsigned short port)
{
  m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (m_socket<0)
  {
	  return -1;
  }

  int iRc = connect(ip,port);
  if (!iRc)
  {
    m_isConnected = true;
	return iRc;
  }
  else
  {
    return iRc;
  }
}

/**
 * @fn BaseSocket::open(unsigned int ip, unsigned short port)
 * @brief  创建socket连接
 * @param[in] ip 服务器ip 
 * @param[in] port 服务器端口
 * @return int
 * @retval 0 成功
 * @retval 其他值 失败
 */
int
BaseSocket::open(unsigned int ip, unsigned short port)
{
  in_addr ipA;
  ipA.s_addr = ip;
  LPCSTR lpszIp = inet_ntoa(ipA); 
  int iRc = open(lpszIp,port);
  return iRc;
}

/**
 * @fn BaseSocket::close(void)
 * @brief  关闭socket连接
 * @return int
 * @retval 0 成功
 * @retval 其他值 失败
 */
int 
BaseSocket::close(void)
{
  int iRc = -1;
  if (INVALID_SOCKET != m_socket)
  {
    iRc = closesocket(m_socket);
    if (iRc)
	{
     return iRc;
	}
    else
	{
      m_socket = INVALID_SOCKET;
	}
  }
  m_isConnected = false;
  
  return iRc;
}

/**
 * @fn BaseSocket::connect( const char *strIntf, const char *strPort)
 * @brief  socket连接
 * @param[in] ip 服务器ip 
 * @param[in] port 服务器端口
 * @return int
 * @retval 0 成功
 * @retval 其他值 失败
 */
int
BaseSocket::connect( const char *strIntf, unsigned int iPort)
{
  int iRc = -1;
  unsigned long mode = 1;
  struct sockaddr_in m_addr;
  do
  {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(iPort);
    m_addr.sin_addr.s_addr = inet_addr(strIntf);
    
	if (m_addr.sin_addr.s_addr == (unsigned int) -1 && strcmp(strIntf,"255.255.255.255")) 
	{
          return -1;
	}
    
	if ( ioctlsocket( m_socket, FIONBIO, (u_long FAR*) &mode) != 0 )
	{
		return -2;
	}

	int nBuffSize = 8 * 1024;
	do 
	{
		
    if ( 0 != setsockopt( m_socket, SOL_SOCKET, SO_SNDBUF, (const char* )&nBuffSize, sizeof( nBuffSize ) ) )
		{
			break;
		}
    
		if ( 0 != setsockopt( m_socket, SOL_SOCKET, SO_RCVBUF, (const char* )&nBuffSize, sizeof( nBuffSize ) ) )
		{
			break;
		}

		nBuffSize *= 2;
	} while ( nBuffSize < 512 * 1024 );

  iRc = ::connect(m_socket,   (struct sockaddr*) &m_addr, sizeof(sockaddr_in));
    
	if (iRc)
	{
      struct timeval time;
      time.tv_sec = 5; //10; 
      time.tv_usec =0;

      fd_set fd;
      FD_ZERO(&fd);
      FD_SET(m_socket,&fd);

      int ret = ::select((int)m_socket+1, NULL, &fd, NULL, &time);
    
	  if (ret > 0)
	  {
        if (FD_ISSET(m_socket,&fd))
		{
			FD_CLR(m_socket,&fd);
		}
        iRc = 0;
	  }

      if (iRc)
	  {
        return iRc;
	  }

	}

  } while(0);

  return iRc;
}

/**
 * @fn BaseSocket::recv(void *pBuff, int iBuffLen, int *pRecvSize, int timeout)
 * @brief  socket数据接收
 * @param[in] pBuff 接收缓冲
 * @param[in] iBuffLen接收长度
 * @param[in] iTimeout接收超时 单位毫秒
 * @param[in] pRecvSize 实际收到的数据大小
 * @return int
 * @retval 0 成功
 * @retval 其他值 失败
 */
int
BaseSocket::recv(void *pBuff, int iBuffLen, int *pRecvSize, int timeout)
{
   int left = iBuffLen;
   int ret  = 0;
   int rcv  = 0;

   do{
      rcv = 0;
      ret = read( ( (char*)pBuff + iBuffLen - left ), left, &rcv, timeout );
      if ( ret != 0 )
      {         
          m_isConnected = false;
          break;
         
      }
	  else
      {
         left -= rcv;
      }

      if ( !m_isConnected )
      {
         return 1;
      }

   }while( left > 0 );

   *pRecvSize = iBuffLen - left;

   return ret;
}

int
BaseSocket::readEx(char *pBuff, int iBuffLen, int timeout)
{
  int left = iBuffLen;
  int ret  = 0;
  int rcv  = 0;

  BOOL bFailed = FALSE;

  int count = 0;

  do
  {
    rcv = 0;
    ret = read( ( (char*)pBuff + iBuffLen - left ), left, &rcv, timeout );
    if ( ret != 0 )
    {         
      m_isConnected = false;
      bFailed = TRUE;
      break;
    }
    else
    {
      left -= rcv;
      
    }

    if ( !m_isConnected )
    {
      bFailed = TRUE;
      break;
    }

    if( rcv == 0 )
    {
      count ++;
      if( count > 3 )
      {
        break;
      }
    }else
    {
      count = 0;
    }

  }while( left > 0 );

  ret = iBuffLen - left;
  if( bFailed )
  {
    ret = 0;
  }

  return ret;
}

/**
 * @fn BaseSocket::recvStream(void *pBuff, int iBuffLen, int *pRecvSize, int timeout)
 * @brief  socket数据接收
 * @param[in] pBuff 接收缓冲
 * @param[in] iBuffLen接收长度
 * @param[in] iTimeout接收超时 单位毫秒
 * @return int
 * @retval 实际接收到的字节数
 * @retval <=0 失败
 */
int
BaseSocket::recvStream(void *pBuff, int iBuffLen,  int timeout)
{
   int ret = 0;
   timeval tv;
   fd_set fd;
   int rc;

   FD_ZERO( &fd );
   FD_SET( m_socket , &fd );
   
   tv.tv_sec = timeout/1000;
   tv.tv_usec = (timeout%1000) * 1000;
   
   ret = select( 1, &fd, NULL, NULL, &tv );
   
   if ( ret == 1 )
   {
		rc = ::recv( m_socket, (char*)pBuff, iBuffLen, 0 );
   } 
   else
	   return -1;

   return  rc;
}


/**
 * @fn BaseSocket::send(const void *pBuff, int iBuffLen, int timeout)
 * @brief  socket数据发送
 * @param[in] pBuff 发送缓冲
 * @param[in] iBuffLen发送长度
 * @param[in] pSendSize 实际发送的数据大小
 * @param[in] iTimeout发送超时 单位毫秒
 * @return int
 * @retval 0 成功
 * @retval 其他值 失败
 */
int 
BaseSocket::send(const void *pBuff, int iBuffLen, int *pSendSize, int timeout)
{
  int left = iBuffLen;
  int ret = 0;
  int dataSent = 0;
  do
   {
      dataSent = 0;
      ret = write( ( (char*)pBuff + iBuffLen - left ), left, &dataSent, timeout );
// 	  char* nbuff = "yangkuan";
// 	  int datacount =0;
// 	  ret = write(( (char*)nbuff ), 9, &dataSent, timeout );
      if ( ret != 0 )
      {
         if ( isDiscnnted( ) )
         {
            if ( ( ret == WSANOTINITIALISED ) || ( ret == WSAEFAULT ) || (ret == 10053) || (ret == 10054 ))
            {
               break;
            }
         }
      }
	  else
      {
         left -= dataSent;
      }

   }while( left > 0 );

   *pSendSize = iBuffLen - left;

   return ret;
}

/**
 * @fn BaseSocket::isConnected()
 * @brief  socket连接状态判断
 * @return bool
 * @retval true 连接状态
 * @retval false 非连接状态
 */
bool
BaseSocket::isConnected()
{
    return m_isConnected;
}

/**
 * @fn BaseSocket::read(void *pBuff, int iBuffLen, int* pReadSize, int timeout)
 * @brief  socket数据接收
 * @param[in] pBuff 接收缓冲
 * @param[in] iBuffLen接收长度
 * @param[in] pSendSize 实际接收的数据大小
 * @param[in] iTimeout发送超时 单位毫秒
 * @return int
 * @retval 0 成功
 * @retval 其他值 失败
 */
int
BaseSocket::read(void *pBuff, int iBuffLen, int* pReadSize, int timeout)
{
     int ret = 0;
     int count = 0;

	 timeval tv;
	 fd_set fd;
	 FD_ZERO( &fd );
	 FD_SET( m_socket , &fd );

	 do 
	 {
		 tv.tv_sec = timeout/1000;
		 tv.tv_usec = (timeout%1000) * 1000;
		 ret = select( 1, &fd, NULL, NULL, &tv );
		 if ( ret == 1 )
		 {
			 *pReadSize = ::recv( m_socket, (char*)pBuff, iBuffLen, 0 );
			 if ( *pReadSize <= 0 )
			 {
				 //若出错，判断socket是否断开
				 if ( isDiscnnted( ) )
				 {
					 m_isConnected = false;
				 }
				 count ++;
				 if (count >3)
				 {
					 m_isConnected = false;
					 return 502;
				 }
			 }
			 else
			 {
				 m_isConnected = true;
				 count = 0;
				 break;
			 }

		 }
		 else if ( ret == -1 )
		 {
			 //若出错，判断socket是否断开
			 if ( isDiscnnted( ) )
			 {
				 m_isConnected = false;
				 break;
			 }
		 }
	 } while (0);
	 
   return WSAGetLastError( );
}

/**
 * @fn BaseSocket::write(const void *pBuf, int iBufLen,int* pWriteSize, int timeout)
 * @brief  socket数据发送
 * @param[in] pBuf 发送缓冲
 * @param[in] iBufLen发送长度
 * @param[in] pWriteSize 实际发送的数据大小
 * @param[in] iTimeout发送超时 单位毫秒
 * @return int
 * @retval 0 成功
 * @retval 其他值 失败
 */
int
BaseSocket::write(const void *pBuf, int iBufLen,int* pWriteSize, int timeout)
{
   int ret = 0;
   do
   {
      timeval tv;
      fd_set fd;
      FD_ZERO( &fd );
      FD_SET( m_socket , &fd );

	  tv.tv_sec = timeout/1000;
	  tv.tv_usec = (timeout%1000) * 1000;
     
	  ret = select( 1, NULL, &fd, NULL, &tv );
      if ( ret == 1 )
      {
		  *pWriteSize = ::send( m_socket, (char*)pBuf, iBufLen, 0 );
         if ( *pWriteSize == -1 )
         {
            //若出错，判断socket是否断开
            if ( isDiscnnted( ) )
            {
               m_isConnected = false;
            }
            break;
         }
		 else
         {
            break;
         }
      }
	  else if ( ret == -1 )
      {
         //若出错，判断socket是否断开
         if ( isDiscnnted( ) )
         {
            m_isConnected = false;
         }
         break;
      }
   }while(0);

   return WSAGetLastError( );
}

/**
 * @fn BaseSocket::isDiscnnted()
 * @brief  socket连接状态判断
 * @return bool
 * @retval true 非连接状态
 * @retval false 连接状态
 */bool
BaseSocket::isDiscnnted()
{
  if (WSAGetLastError() != 0)
  {
	return true;
  }
	return false;
}
/**
 * @fn recvTunnel
 * @brief   socket数据接收
 *         
 * @param  pBuff 接受缓冲区
 * @param  iBuffLen 接受大小
 * @param  pReadSize 实际接收长度
 * @param  timeout 超时时间
 * @return  0成功
 * @retval  其他值失败
 */

int 
BaseSocket::recvTunnel(char*pBuff, int iBuffLen, int *pReadSize,int timeout)
{
	int left = iBuffLen;
	int ret  = 0;
	int rcv  = 0;
	bool bRcv = TRUE;
	BOOL bFailed = FALSE;
	int count = 0;

	do
	{
		rcv = 0;
		ret = read( (char*)pBuff, left, &rcv, timeout );
		if ( ret != 0 )
		{         
			m_isConnected = false;
			bFailed = TRUE;
			break;
		}
		else
		{
			if (rcv > 0)
			{
				bRcv = FALSE;
			}	
			
		}

		if ( !m_isConnected )
		{
			bFailed = TRUE;
			break;
		}

		if( rcv == 0 )
		{
			count ++;
			if( count > 3 )
			{
				break;
			}
		}else
		{
			count = 0;
		}

	}while( bRcv );
	ret = rcv;
	if( bFailed )
	{
		ret = 0;
	}

	return ret;

}
/**
 * @fn SendTunnel
 * @brief  
 *         
 * @param  pBuff  发送数据
 * @param  nSendSize 数据大小
 * @param  tiemout 超时时间
 * @return 0成功
 * @retval 其他值失败
 */

int 
BaseSocket::SendTunnel(const void *pBuf, int iBufLen,int *pWriteSize, int timeout)
{
	int left = iBufLen;
	int ret = 0;
	int dataSent = 0;
	do
	{
		dataSent = 0;
		ret = write( ( (char*)pBuf + iBufLen - left ), left, &dataSent, timeout );
		if ( ret != 0 )
		{
			if ( isDiscnnted( ) )
			{
				if ( ( ret == WSANOTINITIALISED ) || ( ret == WSAEFAULT ) || (ret == 10053) || (ret == 10054 ))
				{
					break;
				}
			}
		}
		else
		{
			left -= dataSent;
		}

	}while( left > 0 );

	*pWriteSize = iBufLen - left;

	return ret;

}

