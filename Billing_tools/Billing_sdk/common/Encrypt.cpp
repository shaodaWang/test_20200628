/** 
 *  @file  Encrypt.cpp
 *  @brief �ӽ���ģ���ʵ�֣������ָ�������ݽ��м��ܡ����ܲ�����
 *  @version 0.0.1
 *  @since 0.0.1
 */


#include "StdAfx.h"
#include "Encrypt.h"
#include <stdio.h>
#include "RC5.hpp"

/**
 * @fn Encrypt::Encrypt()
 * @brief      Encrypt��Ĺ��캯��
 */
Encrypt::Encrypt()
: m_encryption(EENCRYPTION_UNSET)
, m_keyLen(0)
{
	memset( m_encryptKey, 0, MAX_ENCRYPTKEY );
}

/**
 * @fn Encrypt::~Encrypt()
 * @brief      Encrypt��������
 */
Encrypt::~Encrypt()
{
}

/**
 * @fn int Encrypt::setEncryptKey( char* pKey, int len )
 * @brief  ������Կ    
 * @param[in]  pKey ��Կ
 * @param[in]  len  ��Կ�ĳ���
 * @return
 * @retval  0 �ɹ�
 * @retval  1 ��������ȷ
 */
int 
Encrypt::setEncryptKey( char* pKey, int len )
{
	if ( len > MAX_ENCRYPTKEY || pKey == NULL )
		return 1;			// ��������ȷ

	memcpy( m_encryptKey, pKey, len );
	m_keyLen = len;
	return 0;
}

/**
 * @fn int Encrypt::getEncryptKey( char* pKey, int len )
 * @brief      �õ���ǰ���õ���Կ
 * @param[in]  pKey ��Կ������
 * @param[in]  len  �������ĳ���
 * @return
 * @retval  0 �ɹ�
 * @retval  1 ��������ȷ
 */
int 
Encrypt::getEncryptKey( char* pKey, int len )
{
	if ( pKey == NULL || m_keyLen > len )
		return 1;			// ��������ȷ

	memcpy( pKey, m_encryptKey, m_keyLen );
	return m_keyLen;
}

/**
 * @fn int Encrypt::setEncryption( TEncryption encryption )
 * @brief      ���õ�ǰʹ�õļ����㷨
 * @param[in]  encryption �����㷨���� ::Encrypt::TEncryption
 */
void 
Encrypt::setEncryption( TEncryption encryption )
{
	m_encryption = encryption;
}

/**
 * @fn TEncryption Encrypt::getEncryption()
 * @brief      �õ���ǰʹ�õļ����㷨
 * @return
 * @retval  0 û�����ü����㷨
 * @retval  >0 ��ǰ���õļ����㷨���� ::Encrypt::TEncryption
 */
Encrypt::TEncryption 
Encrypt::getEncryption()
{
	return m_encryption;
}


/**
 * @fn int Encrypt::encrypt(void *pIn, int size, void *pOut)
 * @brief      ���ܲ���
 * @param[in]  pIn ����������
 * @param[in]  size ���������ݳ���
 * @param[out] pOut ��ż��ܺ�����
 * @return
 * @retval  >=0  ���ܺ�����ݳ���
 */
int 
Encrypt::encrypt(void *pIn, int size, void *pOut)
{
	int ret = 0;
	switch( m_encryption )
	{
	case EENCRYPTION_UNSET:	//���ģ�������
		memcpy( pOut, pIn, size );
		ret = size;
		break;
	case EENCRYPTION_RC5:		//����rc5�㷨����
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
 * @brief      ���ܲ���
 * @param[in]  pIn ����������
 * @param[in]  size ���������ݳ���
 * @param[out] pOut ��Ž��ܺ�����
 * @return
 * @retval >=0: ���ܺ�����ݳ���
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
