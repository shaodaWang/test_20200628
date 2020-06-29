/** 
 *  @file  Lock.cpp
 *  @brief 封装CRITICAL_SECTION，使它更方便使用
 *  @version 0.0.1
 *  @since 0.0.1
 */


#include "stdafx.h"
#include "Lock.h"


/**
 * @fn Lock::Lock()
 * @brief      Lock的构造函数，负责初始化锁
 */
Lock::Lock()
{
	InitializeCriticalSection( &m_cs );
}


/**
 * @fn Lock::~Lock()
 * @brief      Lock的析构函数，负责释放锁
 */
Lock::~Lock()
{
	DeleteCriticalSection( &m_cs );
}

/**
 * @fn void Lock::lock()
 * @brief      对CRITICAL_SECTION对象加锁
 * @return 无
 */
void Lock::lock()
{
	EnterCriticalSection( &m_cs );
}

/**
 * @fn void Lock::unlock()
 * @brief      对CRITICAL_SECTION对象解锁
 * @return 无
 */
void Lock::unlock()
{
	LeaveCriticalSection( &m_cs );
}


/**
 * @fn LockPtr::LockPtr(Lock* pLock )
 * @brief      LockPtr的构造函数
 * @param[in]  pLock Loc类的指针对象，LockPtr对象要管理它
 */
LockPtr::LockPtr(Lock* pLock )
: m_pLock(pLock)
{
	if ( m_pLock )
  {
		m_pLock->lock();
  }
}

/**
 * @fn LockPtr::~LockPtr()
 * @brief      LockPtr的析构函数
 */
LockPtr::~LockPtr()
{
	if ( m_pLock )
  {
		m_pLock->unlock();
  }
}
