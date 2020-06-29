/** 
 *  @file  Lock.cpp
 *  @brief ��װCRITICAL_SECTION��ʹ��������ʹ��
 *  @version 0.0.1
 *  @since 0.0.1
 */


#include "stdafx.h"
#include "Lock.h"


/**
 * @fn Lock::Lock()
 * @brief      Lock�Ĺ��캯���������ʼ����
 */
Lock::Lock()
{
	InitializeCriticalSection( &m_cs );
}


/**
 * @fn Lock::~Lock()
 * @brief      Lock�����������������ͷ���
 */
Lock::~Lock()
{
	DeleteCriticalSection( &m_cs );
}

/**
 * @fn void Lock::lock()
 * @brief      ��CRITICAL_SECTION�������
 * @return ��
 */
void Lock::lock()
{
	EnterCriticalSection( &m_cs );
}

/**
 * @fn void Lock::unlock()
 * @brief      ��CRITICAL_SECTION�������
 * @return ��
 */
void Lock::unlock()
{
	LeaveCriticalSection( &m_cs );
}


/**
 * @fn LockPtr::LockPtr(Lock* pLock )
 * @brief      LockPtr�Ĺ��캯��
 * @param[in]  pLock Loc���ָ�����LockPtr����Ҫ������
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
 * @brief      LockPtr����������
 */
LockPtr::~LockPtr()
{
	if ( m_pLock )
  {
		m_pLock->unlock();
  }
}
