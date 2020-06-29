/**  $Id$ $DateTime$
 *  @file  Encrypt.h
 *  @brief 加解密模块，负责对指定的数据进行加密、解密操作。
 *  @version 0.0.1
 *  @since 0.0.1
 */


#pragma once

/** @defgroup grEncrypt 加解密模块(En/Decrypt Mod)
 *  @ingroup grBSCP
 *  @{
 */

/// 密钥最大长度
#define MAX_ENCRYPTKEY		20   

/** @class Encrypt
 *  @brief 负责加解密的类。
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
	TEncryption m_encryption;			/**< 加密算法 */
	char				m_encryptKey[MAX_ENCRYPTKEY];			/**< 密钥 */
	int					m_keyLen;					/**< 当前密钥的长度 */
};

/** @}*/ // end of 加解密模块(En/Decrypt Mod)

