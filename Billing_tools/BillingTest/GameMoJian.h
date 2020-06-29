#pragma once
#include "GameBase.h"

namespace space_mojian {
	struct Req_A2_login
	{
		unsigned char loginNameLenth; //登录名长度
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned char passwdLenth;
		char passwd[LOGIN_PASSWD_LENGTH_MAX+1];
		unsigned char useripLenth;
		char userip[IP_LENGTH_MAX];
		char userMacMd5[MAC_MD5 + 1];
		unsigned char userPcValueLenth;
		char userPcValue[CY_MAX_PC_VAL_LEN];
		Req_A2_login() {
			loginNameLenth = 0;
			passwdLenth = 0;
			useripLenth = 0;
			userPcValueLenth = 0;
			memset(loginName, 0, sizeof(loginName));
			memset(passwd, 0, sizeof(passwd));
			memset(userip, 0, sizeof(userip));
			memset(userMacMd5, 0, sizeof(userMacMd5));
			memset(userPcValue, 0, sizeof(userPcValue));
		}
	};

	struct Res_A2_login
	{
		unsigned char loginNameLenth; //登录名长度
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned char uinLenth; //登录名长度
		char uin[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;//
		unsigned int serverGrpid;
		unsigned char serverIPLenth;
		char serverIP[IP_LENGTH_MAX];
		unsigned char serverNameLenth;
		char serverName[LOGIN_NAME_LENGTH_MAX];
		Res_A2_login() {
			loginNameLenth = 0;
			uinLenth = 0;
			ret = -1;
			serverGrpid = 0;
			serverNameLenth = 0;
			memset(loginName, 0, sizeof(loginName));
			memset(uin, 0, sizeof(uin));
			memset(serverIP, 0, sizeof(serverIP));
			memset(serverName, 0, sizeof(serverName));
		}
	};
}

class GameMoJian :
	public GameBase
{
public:	
	GameMoJian(int ntype);
	virtual ~GameMoJian();
	//virtual int sendToBilling(enumCMD cmd);
	virtual int sendA2(CString &strInfo);
	virtual int sendA3(CString &strInfo);
	virtual int sendA4(CString &strInfo);
	//char m_configPath[MAX_PATH];
	int m_nGametype;
	//DvxHandle m_pDvr;
};

