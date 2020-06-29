/** 
 *  @file  Lock.h
 *  @brief 封装CRITICAL_SECTION，使它更方便使用
 *  @version 0.0.1
 *  @since 0.0.1
 */


#ifndef __LOCK_H__
#define __LOCK_H__

#include <windows.h>

/** @defgroup grLock 互斥锁
 *  @{
 */

/** @class Lock
 *  @brief 封装CRITICAL_SECTION
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
 *  @brief 封装CRITICAL_SECTION，但类对象本身并不管理此CRITICAL_SECTION\r\n
 *  @brief 主要用于一定的作用域内部，自动退出
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
