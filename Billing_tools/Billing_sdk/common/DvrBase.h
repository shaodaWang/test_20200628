#pragma once
#include "../BillSdkType.h"
#include "Lock.h"
#include "ConfigBscp.h"
class DvrBase
{
public:
	DvrBase(void);
	virtual ~DvrBase(void);
public:
	static int create( DvrBase** ppDvr, int ip, unsigned short nCmdPort, char nGameType=0, bool bHeart=true, unsigned int timeOut=10000);
	static void release(  DvrBase** ppDvr );

	//�����¼
	static int createNotLogin(const int ip, const unsigned short nCmdPort, unsigned int nCmd, const void* parmin, const unsigned int nLenthIn,
		void* parmout, unsigned int nLenout, unsigned int timeOut=3000);

	virtual bool check( int ip ){ return false; };
	virtual void setDvxAddr( unsigned int nIp ){ m_ipAddr = nIp; };
	virtual void setDvxPort( unsigned short nCmdPort, unsigned short nDataPort ){ m_nCmdPort = nCmdPort; m_nDataPort = nDataPort; };
	virtual unsigned int getDvxAddr(){ return m_ipAddr; };
	virtual unsigned short getDvxCmdPort(){ return m_nCmdPort; };
	virtual unsigned short getDvxDataPort(){ return m_nDataPort; };
	virtual void setDataPort(int  nDataPort ) { m_nDataPort = nDataPort; };

	virtual int login( const char* szUserName, const char* szPasswd, int tmout = 10000 ){ return DVX_ERR_NOIMPL; };
	virtual int logout(){ return DVX_ERR_NOIMPL; };
	virtual bool isLogin(){ return false; };

	virtual ConfigBscp* getConfig(){ return NULL;  };
protected:
	int			                  m_ipAddr;					/**< Dvr��Ip��ַ */
	unsigned short	          m_nCmdPort;	      /**< Dvr������˿ں� */
	unsigned short            m_nDataPort;      ///< Dvx�����ݶ˺�
	unsigned char               m_nGameType;  //��ͬ��type��A0��A1Э��᲻ͬ
};
