/** 
 *  @file  Lock.h
 *  @brief ��װCRITICAL_SECTION��ʹ��������ʹ��
 *  @version 0.0.1
 *  @since 0.0.1
 */


#ifndef __LOCK_H__
#define __LOCK_H__

#include <windows.h>

/** @defgroup grLock ������
 *  @{
 */

/** @class Lock
 *  @brief ��װCRITICAL_SECTION
 */
class Lock
{
public:
	Lock();
	virtual ~Lock();

public:
	void lock();
	void unlock();

protected:
	CRITICAL_SECTION	m_cs;
};

/** @class LockPtr
 *  @brief ��װCRITICAL_SECTION������������������CRITICAL_SECTION\r\n
 *  @brief ��Ҫ����һ�����������ڲ����Զ��˳�
 */
class LockPtr
{
public:
	LockPtr( Lock* pLock );
	~LockPtr();

protected:
	Lock*		m_pLock;
};

/** @}*/

#endif	// end of __LOCK_H__
