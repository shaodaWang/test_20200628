/** 
 *  @file  Encrypt.cpp
 *  @brief 加解密模块的实现，负责对指定的数据进行加密、解密操作。
 *  @version 0.0.1
 *  @since 0.0.1
 */


#include "StdAfx.h"
#include "Encrypt.h"
#include <stdio.h>
#include "RC5.hpp"

/**
 * @fn Encrypt::Encrypt()
 * @brief      Encrypt类的构造函数
 */
Encrypt::Encrypt()
: m_encryption(EENCRYPTION_UNSET)
, m_keyLen(0)
{
	memset( m_encryptKey, 0, MAX_ENCRYPTKEY );
}

/**
 * @fn Encrypt::~Encrypt()
 * @brief      Encrypt析构函数
 */
Encrypt::~Encrypt()
{
}

/**
 * @fn int Encrypt::setEncryptKey( char* pKey, int len )
 * @brief  设置密钥    
 * @param[in]  pKey 密钥
 * @param[in]  len  密钥的长度
 * @return
 * @retval  0 成功
 * @retval  1 参数不正确
 */
int 
Encrypt::setEncryptKey( char* pKey, int len )
{
	if ( len > MAX_ENCRYPTKEY || pKey == NULL )
		return 1;			// 参数不正确

	memcpy( m_encryptKey, pKey, len );
	m_keyLen = len;
	return 0;
}

/**
 * @fn int Encrypt::getEncryptKey( char* pKey, int len )
 * @brief      得到当前设置的密钥
 * @param[in]  pKey 密钥缓冲区
 * @param[in]  len  缓冲区的长度
 * @return
 * @retval  0 成功
 * @retval  1 参数不正确
 */
int 
Encrypt::getEncryptKey( char* pKey, int len )
{
	if ( pKey == NULL || m_keyLen > len )
		return 1;			// 参数不正确

	memcpy( pKey, m_encryptKey, m_keyLen );
	return m_keyLen;
}

/**
 * @fn int Encrypt::setEncryption( TEncryption encryption )
 * @brief      设置当前使用的加密算法
 * @param[in]  encryption 加密算法，见 ::Encrypt::TEncryption
 */
void 
Encrypt::setEncryption( TEncryption encryption )
{
	m_encryption = encryption;
}

/**
 * @fn TEncryption Encrypt::getEncryption()
 * @brief      得到当前使用的加密算法
 * @return
 * @retval  0 没有设置加密算法
 * @retval  >0 当前设置的加密算法，见 ::Encrypt::TEncryption
 */
Encrypt::TEncryption 
Encrypt::getEncryption()
{
	return m_encryption;
}


/**
 * @fn int Encrypt::encrypt(void *pIn, int size, void *pOut)
 * @brief      加密操作
 * @param[in]  pIn 待加密数据
 * @param[in]  size 待加密数据长度
 * @param[out] pOut 存放加密后数据
 * @return
 * @retval  >=0  加密后的数据长度
 */
int 
Encrypt::encrypt(void *pIn, int size, void *pOut)
{
	int ret = 0;
	switch( m_encryption )
	{
	case EENCRYPTION_UNSET:	//明文，不加密
		memcpy( pOut, pIn, size );
		ret = size;
		break;
	case EENCRYPTION_RC5:		//利用rc5算法加密
		size = ( size + 8 )/8;
		size = size*8;
		ret = SEND_RC5ENCODE( (char* )pIn, size, (char* )pOut);
		break;
	default:;

	}
	return ret;
}

/**
 * @fn int Encrypt::decrypt(void* pIn, int size, void* pOut )
 * @brief      解密操作
 * @param[in]  pIn 待解密数据
 * @param[in]  size 待解密数据长度
 * @param[out] pOut 存放解密后数据
 * @return
 * @retval >=0: 解密后的数据长度
 */
int 
Encrypt::decrypt(void* pIn, int size, void* pOut )
{
	int ret = 0;
	switch( m_encryption )
	{
	case EENCRYPTION_UNSET:
		memcpy( pOut, pIn, size );
		ret = size;
		break;

	case EENCRYPTION_RC5:
		ret = RECV_RC5DECODE( ( char* )pIn, size, ( char* )pOut );
		break;

	default:;

	}
	return ret;
}
