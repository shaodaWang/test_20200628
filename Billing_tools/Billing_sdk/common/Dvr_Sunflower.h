/** $Id$ $DateTime$
 *  @file  Dvr_Sunflower.h
 *  @brief Sunflower系列Dvr的实现子类
 *  @version 0.0.1
 *  @since 0.0.1
 *  @warning m_pBscp是由类的外界分配，而在本类的析构中销毁。这在本项目中是个特例
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

/// 登录的重试次数
#define LOGIN_TRY_TIMES			3
#define DVX7X_MSG_REOPEN  20005
/** @class DvrSunflower
 *  @brief Sunflower系列Dvr的实现子类, 在DvrBase中创建、销毁
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
  //远程配置
  virtual ConfigBscp* getConfig(){ return m_pConfigBscp;  };

public:
  virtual void setDisconnect( bool bDisconnect );

protected:
	BscpSocket*	m_pBscp;
  
protected:
	bool m_bIsLogin;
  bool m_bReconnect;
  bool m_bDvxDisconnect;  // 只用于断线重连

protected:	
  ///远程配置相关
  ConfigBscp* m_pConfigBscp;

protected:
	char		                  m_szUserName[32]; /**< 登录Dvr的用户名 */
	char		                  m_szPasswd[32];		/**< 用户相应的密码 */
  unsigned int              m_nProductId;
	int			                  m_sessionId;

  // 线程部分
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

