#include "stdafx.h"
#include "GameMoJian.h"
using namespace BillingSdk;
using namespace std;
using namespace space_mojian;
//int GameMoJian::sendToBilling(enumCMD cmd)
//{
//	
//}

GameMoJian::GameMoJian(int ntype)
{
	m_nGametype = ntype;
	memset(m_configPath,0,sizeof(m_configPath));
	CYGetPrivateProfileString(GAMENAME[m_nGametype], BILLING_CONFIG, "", m_configPath, sizeof(m_configPath), MAIN_INI);
}

GameMoJian::~GameMoJian()
{
	
}

int GameMoJian::sendA2(CString &strInfo)
{
	int nRet = 0;
	char inputParam[PROTOCOL_BUFFER_MAX] = { 0, };
	char outputParam[PROTOCOL_BUFFER_MAX] = { 0, };


	OutputStream outPut(inputParam);

	Req_A2_login pParam;
	Res_A2_login pParamOut;
	//
	char aaa[1024] = { 0, };
	GetCurrentDirectoryA(sizeof(aaa), aaa);
	CYGetPrivateProfileString("A2", "loginName", "", pParam.loginName, sizeof(pParam.loginName), m_configPath);
	CYGetPrivateProfileString("A2", "passwd", "", pParam.passwd, sizeof(pParam.passwd), m_configPath);
	CYGetPrivateProfileString("A2", "userip", "", pParam.userip, sizeof(pParam.userip), m_configPath);
	CYGetPrivateProfileString("A2", "userMacMd5", "", pParam.userMacMd5, sizeof(pParam.userMacMd5), m_configPath);
	CYGetPrivateProfileString("A2", "userPcValue", "", pParam.userPcValue, sizeof(pParam.userPcValue), m_configPath);


	pParam.loginNameLenth = strlen(pParam.loginName);
	pParam.passwdLenth = strlen(pParam.passwd);
	pParam.useripLenth = strlen(pParam.userip);
	pParam.userPcValueLenth = strlen(pParam.userPcValue);

	outPut.Write(pParam.loginNameLenth);
	outPut.Write(pParam.loginName, pParam.loginNameLenth);
	outPut.Write(pParam.passwdLenth);
	outPut.Write(pParam.passwd, pParam.passwdLenth);
	outPut.Write(pParam.useripLenth);
	outPut.Write(pParam.userip, pParam.useripLenth);
	outPut.Write(pParam.userMacMd5, MAC_MD5);//注意userMacMd5长度不能是正好的，最好+1，存'\0'.否则后面长度不变，会报错
	outPut.Write(pParam.userPcValueLenth);
	outPut.Write(pParam.userPcValue, pParam.userPcValueLenth);
	int nParamLenth = strlen(inputParam);

	nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_LOGIN, inputParam, nParamLenth,
		outputParam, min(PROTOCOL_BUFFER_MAX, sizeof(Res_A2_login)));
	if (nRet==0)	{
		InputStream inPut(outputParam);
		inPut.Read(pParamOut.loginNameLenth);
		inPut.Read((char *)pParamOut.loginName, pParamOut.loginNameLenth);
		inPut.Read(pParamOut.uinLenth);
		inPut.Read((char *)pParamOut.uin, pParamOut.uinLenth);
		inPut.Read(pParamOut.ret);
		inPut.Read(pParamOut.serverGrpid);
		inPut.Read(pParamOut.serverIPLenth);
		inPut.Read((char *)pParamOut.serverIP, pParamOut.serverIPLenth);
		inPut.Read(pParamOut.serverNameLenth);
		inPut.Read((char *)pParamOut.serverName, pParamOut.serverNameLenth);
	}
	USES_CONVERSION;
	strInfo.Format(_T("ret(%d),loginname(%s),uin(%s),group(%d),serverip(%s),servername(%s)"), pParamOut.ret,A2W(pParamOut.loginName), A2W(pParamOut.uin), 
		pParamOut.serverGrpid, A2W(pParamOut.serverIP), A2W(pParamOut.serverName));
	return nRet;
}

int GameMoJian::sendA3(CString & strInfo)
{
	return -3;
}

int GameMoJian::sendA4(CString & strInfo)
{
	return -4;
}
