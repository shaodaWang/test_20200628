/** $Id$ $DateTime$
 *  @file  BsrSdk.h
 *  @brief Dvr SDK中唯一的全局变量，管理着所有的子模块
 *  @version 0.0.1
 *  @since 0.0.1
 */


// #include "./Alarm/Alarm.h"			// 报警模块
#include "DvrBase.h"						// Dvr子模块
// #include "../LocalPlay/LocalPlay.h"   // 本地文件回放
#include <vector>
// #include "RegServer.h"

/** @defgroup grSdk SdkModule
 *  @{
 */

/** @class BsrSdk
 *  @brief SDK类，负责管理整个SDK中所有的资源
 */
class BsrSdk
{
public:
	BsrSdk();
	~BsrSdk();

public:
	// SDK的初始化与反初始化
	int init();
	int deInit();

	// Dvr的创建与销毁
 	int dvrCreate( int ip, unsigned short nCmdPort, unsigned short nDataPort, const char* szUserName, 
		const char* szPasswd, DvrBase** ppDvr, unsigned int timeOut=10000);
	int dvrCreateWithOutLogin( int ip, unsigned short nCmdPort, char nGameType, bool bHeart, DvrBase** ppDvr, unsigned int timeOut);
	int dvrDestory( DvrBase* pDvr, bool bLogout=true);
	bool dvrCheck( DvrBase* pDvr );

  //无需登录模块
  int dvrCreateNotLogin( const int ip, const unsigned short nCmdPort, unsigned int nCmd, const void* parmin, const unsigned int nlenthIn, 
    void* parmout, const unsigned int nLenthout, unsigned int timeOut=3000);
 
protected:
	std::vector<DvrBase*>	m_vecDvr;	/**< Dvr子模块 */

protected:	// 主动注册
	Lock        m_lock;

};	// end of BsrSdk

/** @}*/	// end of grSdk
