/**  $Id$ $DateTime$
 *  @file  Encrypt.h
 *  @brief �ӽ���ģ�飬�����ָ�������ݽ��м��ܡ����ܲ�����
 *  @version 0.0.1
 *  @since 0.0.1
 */


#pragma once

/** @defgroup grEncrypt �ӽ���ģ��(En/Decrypt Mod)
 *  @ingroup grBSCP
 *  @{
 */

/// ��Կ��󳤶�
#define MAX_ENCRYPTKEY		20   

/** @class Encrypt
 *  @brief ����ӽ��ܵ��ࡣ
 */
class Encrypt
{
public:
	/// @enum Encrypt::TEncryption
	enum TEncryption
	{
		EENCRYPTION_UNSET = 0,
		EENCRYPTION_RC5   = 1
	};

public:
	Encrypt();
	~Encrypt();

public:
	void setEncryption( TEncryption encryption );
	TEncryption getEncryption();

	int setEncryptKey( char* pKey, int len );
	int getEncryptKey( char* pKey, int len );

	int encrypt( void* pIn, int size, void* pOut );
	int decrypt( void* pIn, int size, void* pOut );

protected:
	TEncryption m_encryption;			/**< �����㷨 */
	char				m_encryptKey[MAX_ENCRYPTKEY];			/**< ��Կ */
	int					m_keyLen;					/**< ��ǰ��Կ�ĳ��� */
};

/** @}*/ // end of �ӽ���ģ��(En/Decrypt Mod)

