#include "stdafx.h"
#include "GameDaoJian.h"

using namespace BillingSdk;

GameDaoJian::GameDaoJian(int ntype)
{
	m_nGametype = ntype;
	memset(m_configPath, 0, sizeof(m_configPath));
	CYGetPrivateProfileString(GAMENAME[m_nGametype], BILLING_CONFIG, "", m_configPath, sizeof(m_configPath), MAIN_INI);

}

GameDaoJian::~GameDaoJian()
{
}

int GameDaoJian::sendA2(CString &strInfo)
{
	int nRet = -1;
	char inputParam[PROTOCOL_BUFFER_MAX] = { 0, };
	char outputParam[PROTOCOL_BUFFER_MAX] = { 0, };
	
	//	
	memset(&m_req_A2, 0,sizeof(m_req_A2));
	memset(&m_res_A2, 0, sizeof(m_res_A2));
	do
	{
		//读取配置文件中的数据
		m_req_A2.userGUID = (unsigned char)CYGetPrivateProfileInt("A2", "userGUID", 0, m_configPath);
		CYGetPrivateProfileString("A2", "loginName", "", m_req_A2.loginName, sizeof(m_req_A2.loginName), m_configPath);
		CYGetPrivateProfileString("A2", "passwd", "", m_req_A2.passwd, sizeof(m_req_A2.passwd), m_configPath);
		CYGetPrivateProfileString("A2", "userip", "", m_req_A2.userip, sizeof(m_req_A2.userip), m_configPath);
		m_req_A2.user_class = (unsigned char)CYGetPrivateProfileInt("A2", "user_class", 0, m_configPath);
		CYGetPrivateProfileString("A2", "pos1", "", m_req_A2.pos1, sizeof(m_req_A2.pos1), m_configPath);
		CYGetPrivateProfileString("A2", "pos2", "", m_req_A2.pos2, sizeof(m_req_A2.pos2), m_configPath);
		CYGetPrivateProfileString("A2", "pos3", "", m_req_A2.pos3, sizeof(m_req_A2.pos3), m_configPath);
		CYGetPrivateProfileString("A2", "val1", "", m_req_A2.val1, sizeof(m_req_A2.val1), m_configPath);
		CYGetPrivateProfileString("A2", "val2", "", m_req_A2.val2, sizeof(m_req_A2.val2), m_configPath);
		CYGetPrivateProfileString("A2", "val3", "", m_req_A2.val3, sizeof(m_req_A2.val3), m_configPath);
		CYGetPrivateProfileString("A2", "passpod", "", m_req_A2.passpod, sizeof(m_req_A2.passpod), m_configPath);
		//CYGetPrivateProfileString("A2", "macaddr", "", m_req_A2.macaddr, sizeof(m_req_A2.macaddr), m_configPath);
		CYGetPrivateProfileString("A2", "userMacMd5", "", m_req_A2.userMacMd5, sizeof(m_req_A2.userMacMd5), m_configPath);
		CYGetPrivateProfileString("A2", "userPcValue", "", m_req_A2.userPcValue, sizeof(m_req_A2.userPcValue), m_configPath);
		//计算长度
		m_req_A2.loginNameLenth = strlen(m_req_A2.loginName);
		m_req_A2.passwdLenth = strlen(m_req_A2.passwd);
		m_req_A2.useripLenth = strlen(m_req_A2.userip);
		//m_req_A2.macaddrlen = strlen(m_req_A2.macaddr);
		m_req_A2.userPcValueLenth = strlen(m_req_A2.userPcValue);
		//组合数据
		OutputStream outPut(inputParam);
		int nParamLenth = 0;
		m_req_A2.userGUID = 0;
		outPut.Write(m_req_A2.userGUID);
		nParamLenth = outPut.GetBufferSize();
		outPut.Write(m_req_A2.loginNameLenth);
		nParamLenth = outPut.GetBufferSize();
		outPut.Write(m_req_A2.loginName, m_req_A2.loginNameLenth);
		nParamLenth = outPut.GetBufferSize();
		outPut.Write(m_req_A2.passwdLenth);
		outPut.Write(m_req_A2.passwd, m_req_A2.passwdLenth);
		outPut.Write(m_req_A2.useripLenth);
		outPut.Write(m_req_A2.userip, m_req_A2.useripLenth);
		outPut.Write(m_req_A2.user_class);
		nParamLenth = outPut.GetBufferSize();
		outPut.Write(m_req_A2.pos1, CY_MIBAO_POS_LEN); //固定长度
		outPut.Write(m_req_A2.pos2, CY_MIBAO_POS_LEN);//固定长度
		outPut.Write(m_req_A2.pos3, CY_MIBAO_POS_LEN);//固定长度
		outPut.Write(m_req_A2.val1, CY_MIBAO_POS_LEN);//固定长度
		outPut.Write(m_req_A2.val2, CY_MIBAO_POS_LEN);//固定长度
		outPut.Write(m_req_A2.val3, CY_MIBAO_POS_LEN);//固定长度
		outPut.Write(m_req_A2.passpod, CY_PASSPOD_PASS_LEN);//固定长度
		outPut.Write(m_req_A2.passZzb, CY_ZZB_PASS_LEN);//固定长度
		nParamLenth = outPut.GetBufferSize();
		//outPut.Write(m_req_A2.macaddrlen);
		//outPut.Write(m_req_A2.macaddr, m_req_A2.macaddrlen);
		outPut.Write(m_req_A2.userMacMd5, MAC_MD5);//固定长度
		outPut.Write(m_req_A2.userPcValueLenth);
		outPut.Write(m_req_A2.userPcValue, m_req_A2.userPcValueLenth);
		outPut.Write(m_req_A2.passPhone, CY_PHONE_PASS_LEN);//固定长度
		//outPut.Write(m_req_A2.game_style);
		nParamLenth = outPut.GetBufferSize();
		//执行
		nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_LOGIN, inputParam, nParamLenth,
			outputParam, min(PROTOCOL_BUFFER_MAX, sizeof(Res_A2_login)));
		if (nRet)		{
			break;
		}
		//解析回复数据
		InputStream inPut(outputParam);
		inPut.Read(m_res_A2.userGUID);
		inPut.Read(m_res_A2.loginNameLenth);
		inPut.Read(m_res_A2.loginName,m_res_A2.loginNameLenth);
		//inPut.Read(m_res_A2.cnMasterLenth);
		//inPut.Read(m_res_A2.cnMaster, m_res_A2.cnMasterLenth);
		inPut.Read(m_res_A2.last_login_state);
		inPut.Read(m_res_A2.ret);
		//
		switch (m_res_A2.ret)
		{
		case LOGIN_NEED_CONFIRMPROTOCOL: //尚未确认畅游协议
		case LOGIN_SUCCESS:	//成功
		case LOGIN_PASSPOD_NOTICE:
		{
			inPut.Read(m_res_A2.res_A2.res_1.leftPoint);
			inPut.Read(m_res_A2.res_A2.res_1.leftDay);
			inPut.Read(m_res_A2.res_A2.res_1.is_forbidden);
			inPut.Read(m_res_A2.res_A2.res_1.online_score);
			inPut.Read(m_res_A2.res_A2.res_1.mobile_bind);
			inPut.Read(m_res_A2.res_A2.res_1.ip_bind);
			inPut.Read(m_res_A2.res_A2.res_1.card_bind);
			inPut.Read(m_res_A2.res_A2.res_1.mac_bind);
			inPut.Read(m_res_A2.res_A2.res_1.is_auth);
			inPut.Read(m_res_A2.res_A2.res_1.realname_bind);
			inPut.Read(m_res_A2.res_A2.res_1.fcmstatus);
			inPut.Read(m_res_A2.res_A2.res_1.phone_locked);
			inPut.Read(m_res_A2.res_A2.res_1.phoneserviceon);
			inPut.Read(m_res_A2.res_A2.res_1.is_Zzb_Bind);
			inPut.Read(m_res_A2.res_A2.res_1.passpod_bind);
			inPut.Read(m_res_A2.res_A2.res_1.expiredays);
			inPut.Read(m_res_A2.res_A2.res_1.mobile_passpod_bind);
			inPut.Read(m_res_A2.res_A2.res_1.ps_status);
			inPut.Read(m_res_A2.res_A2.res_1.is_bvip);
			inPut.Read(m_res_A2.res_A2.res_1.cn_danger);
			inPut.Read(m_res_A2.res_A2.res_1.uVipLevel); //应该是0
			inPut.Read(m_res_A2.res_A2.res_1.nRegDate);
			inPut.Read(m_res_A2.res_A2.res_1.nLasttLgOutDate);
			inPut.Read(m_res_A2.res_A2.res_1.user_kf_vip);
			inPut.Read(m_res_A2.res_A2.res_1.user_kf_msg_status);
		}
		break;
		case LOGIN_ONANOTHER:   //用户已在其它服务器组在线
		{
			inPut.Read(m_res_A2.res_A2.res_2.group);
			inPut.Read(m_res_A2.res_A2.res_2.ipAddrLenth);
			inPut.Read(m_res_A2.res_A2.res_2.ipAddr, m_res_A2.res_A2.res_2.ipAddrLenth);
			inPut.Read(m_res_A2.res_A2.res_2.serverNameLenth);
			inPut.Read(m_res_A2.res_A2.res_2.serverName, m_res_A2.res_A2.res_2.serverNameLenth);
		}
		break;
		default:
			break;
		}
	} while (0);
	USES_CONVERSION;
	strInfo.Format(_T("ret:%d ,%s,%s"), m_res_A2.ret, A2W(m_res_A2.loginName));
	return nRet;
}

int GameDaoJian::sendA3(CString & strInfo)
{
	int nRet = -1;
	char inputParam[PROTOCOL_BUFFER_MAX] = { 0, };
	char outputParam[PROTOCOL_BUFFER_MAX] = { 0, };

	//	
	memset(&m_req_A3, 0, sizeof(m_req_A3));
	memset(&m_res_A3, 0, sizeof(m_res_A3));
	do
	{
		//读取配置文件中的数据
		memcpy(m_req_A3.loginName,m_res_A2.loginName, m_res_A2.loginNameLenth);
		m_req_A3.loginNameLenth = m_res_A2.loginNameLenth;
		CYGetPrivateProfileString("A3", "rolename", "", m_req_A3.role_name, sizeof(m_req_A3.role_name), m_configPath);
		m_req_A3.rolenameLenth = strlen(m_req_A3.role_name);
		m_req_A3.role_guid = CYGetPrivateProfileInt("A3", "role_guid", 0, m_configPath);
		m_req_A3.user_class = CYGetPrivateProfileInt("A3", "user_class", 0, m_configPath);
		m_req_A3.user_ml_level = CYGetPrivateProfileInt("A3", "user_ml_level", 0, m_configPath);
		m_req_A3.user_ml_trlevel = CYGetPrivateProfileInt("A3", "user_ml_trlevel", 0, m_configPath);
		CYGetPrivateProfileString("A3", "user_ml_rolename", "", m_req_A3.user_ml_rolename, sizeof(m_req_A3.user_ml_rolename), m_configPath);
		m_req_A3.user_ml_rolename_lenth = strlen(m_req_A3.user_ml_rolename);
		m_req_A3.start_time = time(NULL);
		m_req_A3.login_state =1;
		//
		OutputStream outPut(inputParam);
		outPut.Write(m_req_A3.loginNameLenth);
		outPut.Write(m_req_A3.loginName, m_req_A3.loginNameLenth);
		outPut.Write(m_req_A3.rolenameLenth);
		outPut.Write(m_req_A3.role_name, m_req_A3.rolenameLenth);
		outPut.Write(m_req_A3.role_guid);
		outPut.Write(m_req_A3.user_class);
		outPut.Write(m_req_A3.user_ml_level);
		outPut.Write(m_req_A3.user_ml_trlevel);
		outPut.Write(m_req_A3.user_ml_rolename_lenth);
		outPut.Write(m_req_A3.user_ml_rolename, m_req_A3.user_ml_rolename_lenth);
		outPut.Write(m_req_A3.start_time);
		outPut.Write(m_req_A3.login_state);
		int nParamLenth = outPut.GetBufferSize();
		//执行
		nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_ONLINE, inputParam, nParamLenth,
			outputParam, min(PROTOCOL_BUFFER_MAX, sizeof(Res_A3_online)));
		if (nRet) {
			break;
		}
		//解析回复数据
		InputStream inPut(outputParam);
		inPut.Read(m_res_A3.loginNameLenth);
		inPut.Read(m_res_A3.loginName, m_res_A3.loginNameLenth);
		inPut.Read(m_res_A3.ret);
		switch (m_res_A3.ret) {
		case BILLSTART_SUCCESS:
			inPut.Read(m_res_A3.res_A3.res_1.billing_type);
			inPut.Read(m_res_A3.res_A3.res_1.validtime);
			inPut.Read(m_res_A3.res_A3.res_1.store_point);
			inPut.Read(m_res_A3.res_A3.res_1.sde_point);
			inPut.Read(m_res_A3.res_A3.res_1.questionnaire_id);
			inPut.Read(m_res_A3.res_A3.res_1.act_count);
			for (int i = 0; i < m_res_A3.res_A3.res_1.act_count; i++)
			{
				inPut.Read(m_res_A3.res_A3.res_1.activity_list[i]);
			}
			break;
		case BILLSTART_FAIL:
			inPut.Read(m_res_A3.res_A3.res_2.errorcode);
			break;
		default:
			break;
		}
	} while (0);
	USES_CONVERSION;
	strInfo.Format(_T("ret:%d ,%s"), m_res_A3.ret, A2W(m_res_A3.loginName));

	return nRet;
}

int GameDaoJian::sendA4(CString & strInfo)
{
	int nRet = -1;
	char inputParam[PROTOCOL_BUFFER_MAX] = { 0, };
	char outputParam[PROTOCOL_BUFFER_MAX] = { 0, };

	//	
	memset(&m_req_A4, 0, sizeof(m_req_A4));
	memset(&m_res_A4, 0, sizeof(m_res_A4));
	do
	{
		//读取配置文件中的数据
		memcpy(m_req_A4.loginName, m_res_A2.loginName, m_res_A2.loginNameLenth);
		m_req_A4.loginNameLenth = m_res_A2.loginNameLenth;
		m_req_A4.role_guid = m_req_A3.role_guid;
		m_req_A4.user_class = m_req_A3.user_class;
		m_req_A4.user_level = m_req_A3.user_ml_level;
		m_req_A4.user_trlevel = m_req_A3.user_ml_trlevel;
		m_req_A4.billType = 1;
		m_req_A4.start_time = m_req_A3.start_time;
		m_req_A4.end_time = time(NULL);
		
		//
		OutputStream outPut(inputParam);
		outPut.Write(m_req_A4.loginNameLenth);
		outPut.Write(m_req_A4.loginName, m_req_A4.loginNameLenth);
		outPut.Write(m_req_A4.role_guid);
		outPut.Write(m_req_A4.user_class);
		outPut.Write(m_req_A4.user_level);
		outPut.Write(m_req_A4.user_trlevel);
		outPut.Write(m_req_A4.billType);
		outPut.Write(m_req_A4.start_time);
		outPut.Write(m_req_A4.end_time);
		
		int nParamLenth = outPut.GetBufferSize();
		//执行
		nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_LOGOUT, inputParam, nParamLenth,
			outputParam, min(PROTOCOL_BUFFER_MAX, sizeof(Req_A4_logout)));
		if (nRet) {
			break;
		}
		//解析回复数据
		InputStream inPut(outputParam);
		inPut.Read(m_res_A4.loginNameLenth);
		inPut.Read(m_res_A4.loginName, m_res_A4.loginNameLenth);
		inPut.Read(m_res_A4.ret);
	} while (0);
	USES_CONVERSION;
	strInfo.Format(_T("ret:%d ,%s"), m_res_A4.ret, A2W(m_res_A4.loginName));
	return nRet;
}
