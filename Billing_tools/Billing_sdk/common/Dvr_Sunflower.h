/** $Id$ $DateTime$
 *  @file  Dvr_Sunflower.h
 *  @brief Sunflowerϵ��Dvr��ʵ������
 *  @version 0.0.1
 *  @since 0.0.1
 *  @warning m_pBscp������������䣬���ڱ�������������١����ڱ���Ŀ���Ǹ�����
 */


#ifndef __DVR_SUNFLOWER_H__
#define __DVR_SUNFLOWER_H__

#include "DvrBase.h"
#include "../common/BscpSocket.h"
#include <vector>
#include "ConfigBscp.h"

//class ConfigBscp;

/** @defgroup grDvr_Sunflower SunflowerDvr
 *  @ingroup grDvr
 *  @{
 */

/// ��¼�����Դ���
#define LOGIN_TRY_TIMES			3
#define DVX7X_MSG_REOPEN  20005
/** @class DvrSunflower
 *  @brief Sunflowerϵ��Dvr��ʵ������, ��DvrBase�д���������
 */
class DvrSunflower : public DvrBase
{
public:
	DvrSunflower( BscpSocket* pBscp, unsigned int nProductId );
	~DvrSunflower();

public:
	virtual int login( const char* szUserName, const char* szPasswd, int tmout = 10000);
	virtual int logout();
  virtual bool isLogin();

public:
  virtual void setReconnect( bool bReconnect );

public:	
  //Զ������
  virtual ConfigBscp* getConfig(){ return m_pConfigBscp;  };

public:
  virtual void setDisconnect( bool bDisconnect );

protected:
	BscpSocket*	m_pBscp;
  
protected:
	bool m_bIsLogin;
  bool m_bReconnect;
  bool m_bDvxDisconnect;  // ֻ���ڶ�������

protected:	
  ///Զ���������
  ConfigBscp* m_pConfigBscp;

protected:
	char		                  m_szUserName[32]; /**< ��¼Dvr���û��� */
	char		                  m_szPasswd[32];		/**< �û���Ӧ������ */
  unsigned int              m_nProductId;
	int			                  m_sessionId;

  // �̲߳���
protected:
  HANDLE		m_hDvxThread;	
  HANDLE    m_hDvxEvent;  
  HWND      m_hNotifyWnd;
  bool      m_bExit;

public:
  void threadCallback();
};

/** @}*/	// end of grDvr_Sunflower

#endif	// end of __DVR_SUNFLOWER_H__

