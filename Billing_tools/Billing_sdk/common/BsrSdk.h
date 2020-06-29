/** $Id$ $DateTime$
 *  @file  BsrSdk.h
 *  @brief Dvr SDK��Ψһ��ȫ�ֱ��������������е���ģ��
 *  @version 0.0.1
 *  @since 0.0.1
 */


// #include "./Alarm/Alarm.h"			// ����ģ��
#include "DvrBase.h"						// Dvr��ģ��
// #include "../LocalPlay/LocalPlay.h"   // �����ļ��ط�
#include <vector>
// #include "RegServer.h"

/** @defgroup grSdk SdkModule
 *  @{
 */

/** @class BsrSdk
 *  @brief SDK�࣬�����������SDK�����е���Դ
 */
class BsrSdk
{
public:
	BsrSdk();
	~BsrSdk();

public:
	// SDK�ĳ�ʼ���뷴��ʼ��
	int init();
	int deInit();

	// Dvr�Ĵ���������
 	int dvrCreate( int ip, unsigned short nCmdPort, unsigned short nDataPort, const char* szUserName, 
		const char* szPasswd, DvrBase** ppDvr, unsigned int timeOut=10000);
	int dvrCreateWithOutLogin( int ip, unsigned short nCmdPort, char nGameType, bool bHeart, DvrBase** ppDvr, unsigned int timeOut);
	int dvrDestory( DvrBase* pDvr, bool bLogout=true);
	bool dvrCheck( DvrBase* pDvr );

  //�����¼ģ��
  int dvrCreateNotLogin( const int ip, const unsigned short nCmdPort, unsigned int nCmd, const void* parmin, const unsigned int nlenthIn, 
    void* parmout, const unsigned int nLenthout, unsigned int timeOut=3000);
 
protected:
	std::vector<DvrBase*>	m_vecDvr;	/**< Dvr��ģ�� */

protected:	// ����ע��
	Lock        m_lock;

};	// end of BsrSdk

/** @}*/	// end of grSdk
