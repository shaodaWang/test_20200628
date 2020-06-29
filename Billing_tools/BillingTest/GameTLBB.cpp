#include "stdafx.h"
#include "GameTLBB.h"

using namespace BillingSdk;

GameTLBB::GameTLBB(int ntype)
{
	m_nGametype = ntype;
	memset(m_configPath, 0, sizeof(m_configPath));
	CYGetPrivateProfileString(GAMENAME[m_nGametype], BILLING_CONFIG, "", m_configPath, sizeof(m_configPath), MAIN_INI);

}

GameTLBB::~GameTLBB()
{
}

int GameTLBB::sendA2(CString &strInfo)
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
		m_req_A2.logintype = (unsigned char)CYGetPrivateProfileInt("A2", "logintype",0, m_configPath);
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
		CYGetPrivateProfileString("A2", "macaddr", "", m_req_A2.macaddr, sizeof(m_req_A2.macaddr), m_configPath);
		CYGetPrivateProfileString("A2", "userMacMd5", "", m_req_A2.userMacMd5, sizeof(m_req_A2.userMacMd5), m_configPath);
		CYGetPrivateProfileString("A2", "userPcValue", "", m_req_A2.userPcValue, sizeof(m_req_A2.userPcValue), m_configPath);
		m_req_A2.game_style = (unsigned char)CYGetPrivateProfileInt("A2", "game_style", 0, m_configPath);
		//计算长度
		m_req_A2.loginNameLenth = strlen(m_req_A2.loginName);
		m_req_A2.passwdLenth = strlen(m_req_A2.passwd);
		m_req_A2.useripLenth = strlen(m_req_A2.userip);
		m_req_A2.macaddrlen = strlen(m_req_A2.macaddr);
		m_req_A2.userPcValueLenth = strlen(m_req_A2.userPcValue);
		//组合数据
		OutputStream outPut(inputParam);
		int nParamLenth = 0;
		m_req_A2.logintype = 0;
		outPut.Write(m_req_A2.logintype);
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
		nParamLenth = outPut.GetBufferSize();
		outPut.Write(m_req_A2.macaddrlen);
		outPut.Write(m_req_A2.macaddr, m_req_A2.macaddrlen);
		outPut.Write(m_req_A2.userMacMd5, MAC_MD5);//固定长度
		outPut.Write(m_req_A2.userPcValueLenth);
		outPut.Write(m_req_A2.userPcValue, m_req_A2.userPcValueLenth);
		outPut.Write(m_req_A2.game_style);
		nParamLenth = outPut.GetBufferSize();
		//执行
		nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_LOGIN, inputParam, nParamLenth,
			outputParam, min(PROTOCOL_BUFFER_MAX, sizeof(Res_A2_login)));
		if (nRet)		{
			break;
		}
		//解析回复数据
		InputStream inPut(outputParam);
		inPut.Read(m_res_A2.logintype);
		inPut.Read(m_res_A2.loginNameLenth);
		inPut.Read(m_res_A2.loginName,m_res_A2.loginNameLenth);
		inPut.Read(m_res_A2.cnMasterLenth);
		inPut.Read(m_res_A2.cnMaster, m_res_A2.cnMasterLenth);
		inPut.Read(m_res_A2.ret);
		//
		switch (m_res_A2.ret)
		{
		case LOGIN_NEED_CONFIRMPROTOCOL: //尚未确认畅游协议
		case LOGIN_SUCCESS:	//成功
		case LOGIN_PASSPOD_NOTICE:
		case LOGIN_CN_LOCK:
		case LOGIN_UNSAFE:
		{
			inPut.Read(m_res_A2.res_A2.res_1.leftPoint);
			inPut.Read(m_res_A2.res_A2.res_1.leftDay);
			inPut.Read(m_res_A2.res_A2.res_1.is_forbidden);
			inPut.Read(m_res_A2.res_A2.res_1.online_score);
			inPut.Read(m_res_A2.res_A2.res_1.outline_score);
			inPut.Read(m_res_A2.res_A2.res_1.mobile_bind);
			inPut.Read(m_res_A2.res_A2.res_1.ip_bind);
			inPut.Read(m_res_A2.res_A2.res_1.card_bind);
			inPut.Read(m_res_A2.res_A2.res_1.mac_bind);
			inPut.Read(m_res_A2.res_A2.res_1.is_auth);
			inPut.Read(m_res_A2.res_A2.res_1.realname_bind);
			inPut.Read(m_res_A2.res_A2.res_1.fcmstatus);
			inPut.Read(m_res_A2.res_A2.res_1.phone_locked);
			inPut.Read(m_res_A2.res_A2.res_1.phoneserviceon);
			inPut.Read(m_res_A2.res_A2.res_1.passpod_bind);
			inPut.Read(m_res_A2.res_A2.res_1.expiredays);
			inPut.Read(m_res_A2.res_A2.res_1.sec_code);
			inPut.Read(m_res_A2.res_A2.res_1.vip_level);
			inPut.Read(m_res_A2.res_A2.res_1.consum_YuanBao_num);
			inPut.Read(m_res_A2.res_A2.res_1.need_notice);
			//飞信基本上没了
			inPut.Read(m_res_A2.res_A2.res_1.fetion_len); //应该是0
			inPut.Read(m_res_A2.res_A2.res_1.fetion_uid, m_res_A2.res_A2.res_1.fetion_len);
			//
			inPut.Read(m_res_A2.res_A2.res_1.lastLoginIPLenth); 
			inPut.Read(m_res_A2.res_A2.res_1.lastLoginIP, m_res_A2.res_A2.res_1.lastLoginIPLenth);
			inPut.Read(m_res_A2.res_A2.res_1.need_register);
			inPut.Read(m_res_A2.res_A2.res_1.zzb_bind);
			inPut.Read(m_res_A2.res_A2.res_1.verify_mobile_svr_on);
			inPut.Read(m_res_A2.res_A2.res_1.verify_mobile_bind);
			inPut.Read(m_res_A2.res_A2.res_1.cnlock_cd);
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
		case LOGIN_ONSAME: //用户已在本服务器组在线
		{
			inPut.Read(m_res_A2.res_A2.res_3.card_bind);
			inPut.Read(m_res_A2.res_A2.res_3.phone_locked);
			inPut.Read(m_res_A2.res_A2.res_3.passpod_bind);
			inPut.Read(m_res_A2.res_A2.res_3.zzb_bind);
		}
		break;
		case LOGIN_FORBIDDEN_ENABLETIME://用户中心三天停权禁止登陆
		{
			inPut.Read(m_res_A2.res_A2.res_4.enableTime);
		}
		break;
		default:
			break;
		}
	} while (0);
	USES_CONVERSION;
	strInfo.Format(_T("ret:%d ,%s,%s"), m_res_A2.ret, A2W(m_res_A2.loginName), A2W(m_res_A2.cnMaster));
	return nRet;
}

int GameTLBB::sendA3(CString & strInfo)
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
		memcpy(m_req_A3.cnMaster,m_res_A2.cnMaster, m_res_A2.cnMasterLenth);
		m_req_A3.cnMasterLenth = m_res_A2.cnMasterLenth;
		CYGetPrivateProfileString("A3", "rolename", "", m_req_A3.role_name, sizeof(m_req_A3.role_name), m_configPath);
		m_req_A3.rolenameLenth = strlen(m_req_A3.role_name);
		m_req_A3.guid = (unsigned long long)CYGetPrivateProfileInt("A3", "guid", 0, m_configPath);
		m_req_A3.user_class = m_req_A2.user_class;
		m_req_A3.start_time = time(NULL);
		m_req_A3.game_style - m_req_A2.game_style;
		//
		OutputStream outPut(inputParam);
		outPut.Write(m_req_A3.cnMasterLenth);
		outPut.Write(m_req_A3.cnMaster, m_req_A3.cnMasterLenth);
		outPut.Write(m_req_A3.rolenameLenth);
		outPut.Write(m_req_A3.role_name, m_req_A3.rolenameLenth);
		outPut.Write(m_req_A3.guid);
		outPut.Write(m_req_A3.user_class);
		outPut.Write(m_req_A3.start_time);
		outPut.Write(m_req_A3.game_style);
		int nParamLenth = outPut.GetBufferSize();
		//执行
		nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_ONLINE, inputParam, nParamLenth,
			outputParam, min(PROTOCOL_BUFFER_MAX, sizeof(Res_A3_online)));
		if (nRet) {
			break;
		}
		//解析回复数据
		InputStream inPut(outputParam);
		inPut.Read(m_res_A3.cnMasterLenth);
		inPut.Read(m_res_A3.cnMaster, m_res_A3.cnMasterLenth);
		inPut.Read(m_res_A3.ret);
		switch (m_res_A3.ret) {
		case BILLSTART_SUCCESS:
			inPut.Read(m_res_A3.res_A3.res_1.pay_type);
			inPut.Read(m_res_A3.res_A3.res_1.left_time);
			inPut.Read(m_res_A3.res_A3.res_1.can_use_point);
			inPut.Read(m_res_A3.res_A3.res_1.user_point);
			inPut.Read(m_res_A3.res_A3.res_1.questionnaire_id);
			inPut.Read(m_res_A3.res_A3.res_1.award_notice);
			inPut.Read(m_res_A3.res_A3.res_1.award_cn_group);
			inPut.Read(m_res_A3.res_A3.res_1.award_cn_guid);
			inPut.Read(m_res_A3.res_A3.res_1.award_cn_guid_group);
			inPut.Read(m_res_A3.res_A3.res_1.unMsgCreateTime);
			inPut.Read(m_res_A3.res_A3.res_1.szMsgType, CY_STOREMSG_LEN);
			break;
		case BILLSTART_FAIL:
			inPut.Read(m_res_A3.res_A3.res_2.errorcode);
			break;
		default:
			break;
		}
	} while (0);
	USES_CONVERSION;
	strInfo.Format(_T("ret:%d ,%s"), m_res_A3.ret, A2W(m_res_A3.cnMaster));

	return nRet;
}

int GameTLBB::sendA4(CString & strInfo)
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
		memcpy(m_req_A4.cnMaster, m_res_A2.cnMaster, m_res_A2.cnMasterLenth);
		m_req_A4.cnMasterLenth = m_res_A2.cnMasterLenth;
		m_req_A4.billType = 1;
		m_req_A4.user_class = m_req_A2.user_class;
		m_req_A4.start_time = m_req_A3.start_time;
		m_req_A4.end_time = time(NULL);
		m_req_A4.game_style = m_req_A2.game_style;
		//
		OutputStream outPut(inputParam);
		outPut.Write(m_req_A4.cnMasterLenth);
		outPut.Write(m_req_A4.cnMaster, m_req_A4.cnMasterLenth);
		outPut.Write(m_req_A4.user_class);
		outPut.Write(m_req_A4.billType);
		outPut.Write(m_req_A4.start_time);
		outPut.Write(m_req_A4.end_time);
		outPut.Write(m_req_A4.game_style);
		int nParamLenth = outPut.GetBufferSize();
		//执行
		nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_LOGOUT, inputParam, nParamLenth,
			outputParam, min(PROTOCOL_BUFFER_MAX, sizeof(Req_A4_logout)));
		if (nRet) {
			break;
		}
		//解析回复数据
		InputStream inPut(outputParam);
		inPut.Read(m_res_A4.cnMasterLenth);
		inPut.Read(m_res_A4.cnMaster, m_res_A4.cnMasterLenth);
		inPut.Read(m_res_A4.ret);
	} while (0);
	USES_CONVERSION;
	strInfo.Format(_T("ret:%d ,%s"), m_res_A4.ret, A2W(m_res_A4.cnMaster));
	return nRet;
}

int GameTLBB::sendA8(CString & strInfo)
{
  int nRet = -1;
  char inputParam[PROTOCOL_BUFFER_MAX] = { 0, };
  char outputParam[PROTOCOL_BUFFER_MAX] = { 0, };

  //	
  memset(&m_req_A8, 0, sizeof(m_req_A8));
  memset(&m_res_A8, 0, sizeof(m_res_A8));
  do
  {
    //读取配置文件中的数据
    m_req_A8.logintype = (unsigned char)CYGetPrivateProfileInt("A8", "logintype", 0, m_configPath);
    CYGetPrivateProfileString("A8", "twoDimenCode", "", m_req_A8.twoDimenCode, sizeof(m_req_A8.twoDimenCode), m_configPath);
    CYGetPrivateProfileString("A8", "userip", "", m_req_A8.userip, sizeof(m_req_A8.userip), m_configPath);
    CYGetPrivateProfileString("A8", "macaddr", "", m_req_A8.macaddr, sizeof(m_req_A8.macaddr), m_configPath);
    CYGetPrivateProfileString("A8", "userMacMd5", "", m_req_A8.userMacMd5, sizeof(m_req_A8.userMacMd5), m_configPath);
    CYGetPrivateProfileString("A8", "userPcValue", "", m_req_A8.userPcValue, sizeof(m_req_A8.userPcValue), m_configPath);
    m_req_A8.game_style = (unsigned char)CYGetPrivateProfileInt("A8", "game_style", 0, m_configPath);
    //计算长度
    m_req_A8.Codelength = strlen(m_req_A8.twoDimenCode);
    m_req_A8.useripLenth = strlen(m_req_A8.userip);
    m_req_A8.macaddrlen = strlen(m_req_A8.macaddr);
    m_req_A8.userPcValueLenth = strlen(m_req_A8.userPcValue);
    //组合数据
    OutputStream outPut(inputParam);
    int nParamLenth = 0;
    m_req_A8.logintype = 0;
    outPut.Write(m_req_A8.logintype);
    outPut.Write(m_req_A8.Codelength);
    outPut.Write(m_req_A8.twoDimenCode, m_req_A8.Codelength);
    outPut.Write(m_req_A8.useripLenth);
    outPut.Write(m_req_A8.userip, m_req_A8.useripLenth);
    outPut.Write(m_req_A8.macaddrlen);
    outPut.Write(m_req_A8.macaddr, m_req_A8.macaddrlen);
    outPut.Write(m_req_A8.userMacMd5, MAC_MD5);//固定长度
    outPut.Write(m_req_A8.userPcValueLenth);
    outPut.Write(m_req_A8.userPcValue, m_req_A8.userPcValueLenth);
    outPut.Write(m_req_A8.game_style);
    nParamLenth = outPut.GetBufferSize();
    //执行
    nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_LOGIN_BY_TWO, inputParam, nParamLenth,
      outputParam, min(PROTOCOL_BUFFER_MAX, sizeof(m_res_A8)));
    if (nRet) {
      break;
    }
    //解析回复数据
    InputStream inPut(outputParam);
    inPut.Read(m_res_A8.logintype);
    inPut.Read(m_res_A8.loginNameLenth);
    inPut.Read(m_res_A8.loginName, m_res_A8.loginNameLenth);
    inPut.Read(m_res_A8.cnMasterLenth);
    inPut.Read(m_res_A8.cnMaster, m_res_A8.cnMasterLenth);
    inPut.Read(m_res_A8.ret);
    //
    switch (m_res_A8.ret)
    {
    case LOGIN_NEED_CONFIRMPROTOCOL: //尚未确认畅游协议
    case LOGIN_SUCCESS:	//成功
    case LOGIN_PASSPOD_NOTICE:
    //case LOGIN_CN_LOCK:
    //case LOGIN_UNSAFE:
    {
      inPut.Read(m_res_A8.res_A8.res_1.leftPoint);
      inPut.Read(m_res_A8.res_A8.res_1.leftDay);
      inPut.Read(m_res_A8.res_A8.res_1.is_forbidden);
      inPut.Read(m_res_A8.res_A8.res_1.online_score);
      inPut.Read(m_res_A8.res_A8.res_1.outline_score);
      inPut.Read(m_res_A8.res_A8.res_1.mobile_bind);
      inPut.Read(m_res_A8.res_A8.res_1.ip_bind);
      inPut.Read(m_res_A8.res_A8.res_1.card_bind);
      inPut.Read(m_res_A8.res_A8.res_1.mac_bind);
      inPut.Read(m_res_A8.res_A8.res_1.is_auth);
      inPut.Read(m_res_A8.res_A8.res_1.realname_bind);
      inPut.Read(m_res_A8.res_A8.res_1.fcmstatus);
      inPut.Read(m_res_A8.res_A8.res_1.phone_locked);
      inPut.Read(m_res_A8.res_A8.res_1.phoneserviceon);
      inPut.Read(m_res_A8.res_A8.res_1.passpod_bind);
      inPut.Read(m_res_A8.res_A8.res_1.expiredays);
      inPut.Read(m_res_A8.res_A8.res_1.sec_code);
      inPut.Read(m_res_A8.res_A8.res_1.vip_level);
      inPut.Read(m_res_A8.res_A8.res_1.consum_YuanBao_num);
      inPut.Read(m_res_A8.res_A8.res_1.need_notice);
      //飞信基本上没了
      inPut.Read(m_res_A8.res_A8.res_1.fetion_len); //应该是0
      inPut.Read(m_res_A8.res_A8.res_1.fetion_uid, m_res_A8.res_A8.res_1.fetion_len);
      //
      inPut.Read(m_res_A8.res_A8.res_1.lastLoginIPLenth);
      inPut.Read(m_res_A8.res_A8.res_1.lastLoginIP, m_res_A8.res_A8.res_1.lastLoginIPLenth);
      inPut.Read(m_res_A8.res_A8.res_1.need_register);
      inPut.Read(m_res_A8.res_A8.res_1.zzb_bind);
      inPut.Read(m_res_A8.res_A8.res_1.verify_mobile_svr_on);
      inPut.Read(m_res_A8.res_A8.res_1.verify_mobile_bind);
      //inPut.Read(m_res_A8.res_A8.res_1.cnlock_cd);
    }
    break;
    case LOGIN_ONANOTHER:   //用户已在其它服务器组在线
    {
      inPut.Read(m_res_A8.res_A8.res_2.group);
      inPut.Read(m_res_A8.res_A8.res_2.ipAddrLenth);
      inPut.Read(m_res_A8.res_A8.res_2.ipAddr, m_res_A8.res_A8.res_2.ipAddrLenth);
      inPut.Read(m_res_A8.res_A8.res_2.serverNameLenth);
      inPut.Read(m_res_A8.res_A8.res_2.serverName, m_res_A8.res_A8.res_2.serverNameLenth);
    }
    break;
    //case LOGIN_ONSAME: //用户已在本服务器组在线
    //{
    //  inPut.Read(m_res_A8.res_A8.res_3.card_bind);
    //  inPut.Read(m_res_A8.res_A8.res_3.phone_locked);
    //  inPut.Read(m_res_A8.res_A8.res_3.passpod_bind);
    //  inPut.Read(m_res_A8.res_A8.res_3.zzb_bind);
    //}
    //break;
    case LOGIN_FORBIDDEN_ENABLETIME://用户中心三天停权禁止登陆
    {
      inPut.Read(m_res_A8.res_A8.res_4.enableTime);
    }
    break;
    default:
      break;
    }
  } while (0);
  USES_CONVERSION;
  strInfo.Format(_T("ret:%d ,%s,%s"), m_res_A8.ret, A2W(m_res_A8.loginName), A2W(m_res_A8.cnMaster));
  return nRet;
}

int GameTLBB::send01(CString & strInfo)
{
  int nRet = -1;
  char inputParam[PROTOCOL_BUFFER_MAX] = { 0, };
  char outputParam[PROTOCOL_BUFFER_MAX] = { 0, };

  //	
  memset(&m_req_01, 0, sizeof(m_req_01));
  memset(&m_res_01, 0, sizeof(m_res_01));
  do
  {
    if (!m_pDvr) {
      break;
    }
    //读取配置文件中的数据
    CYGetPrivateProfileString("01", "cnMaster", "", m_req_01.cnMaster, sizeof(m_req_01.cnMaster), m_configPath);
    m_req_01.cnMasterLenth = strlen(m_req_01.cnMaster);
    //
    OutputStream outPut(inputParam);
    outPut.Write(m_req_01.cnMasterLenth);
    outPut.Write(m_req_01.cnMaster, m_req_01.cnMasterLenth);
    int nParamLenth = outPut.GetBufferSize();
    //执行
    nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_TEST_CURL, inputParam, nParamLenth,
      outputParam, min(PROTOCOL_BUFFER_MAX, sizeof(m_res_01)));
    if (nRet) {
      break;
    }
    //解析回复数据
    InputStream inPut(outputParam);
    inPut.Read(m_res_01.cnMasterLenth);
    inPut.Read(m_res_01.cnMaster, m_res_01.cnMasterLenth);
    inPut.Read(m_res_01.ret);
  } while (0);
  USES_CONVERSION;
  strInfo.Format(_T("ret:%d ,%s"), m_res_01.ret, A2W(m_res_01.cnMaster));
  return nRet;
}
