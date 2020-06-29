#pragma once
#include "GameBase.h"
namespace space_daojian {

	// -----------A2-----UserLogin数据库接口返回值定义-------------
#define LOGIN_SUCCESS 0x01 //成功登陆，返回用户点数信息
#define LOGIN_NOUSER  0x02 //用户不存在
#define LOGIN_ERRPWD  0x03 //密码错误
#define LOGIN_ONSAME  0x04 //此用户在本组在线
#define LOGIN_ONANOTHER  0x05 //在不同组在线,返回所在服务器的组号
#define LOGIN_ERROTHERS  0x06 //其他错误
#define LOGIN_FORBIDDEN  0x07 //被禁止登陆(包括ip,停权)
#define LOGIN_LACKMONEY  0x08 //用户帐户余额不足，让用户充值。不能让用户登陆。
#define LOGIN_FORBIDDEN_BY_USERSELF 0x18


#define LOGIN_IPOVERFLOW  0x21 //本日登陆ip过高，禁止登录
#define LOGIN_INBLACKLST  0x22 //该登陆IP在黑名单中

#define LOGIN_CN_UNDERAGE_18       0x29                         //限制登录，未成年人，未满18岁
#define LOGIN_CN_REMOVEACCOUNT     0x2A                         //限制登录，注销帐号
#define LOGIN_CN_NO_CERTNUM        0x2B                         //获取身份证信息为空
#define LOGIN_CN_ERROR_CERTNUM     0x2C                         //身份证判断格式错误

#define LOGIN_LACKIP 0xB1  //用户绑定ip
#define LOGIN_LACKMOBILE 0xB2 // 手机以及锁定账号
#define LOGIN_LACKEDCARD 0xC2  //用户申请了密宝卡功能但没有使用
#define LOGIN_NOLACKEDCARD 0xC1  //用户未申请密宝卡功能

#define LOGIN_FORBIDDEN_LACKNAME 0xC7 //用户没有输入实名信息禁止登陆

#define LOGIN_LOCKCARD_ERR 0xC3  //用户输入的密宝卡信息错误
#define LOGIN_SERVERBIND_ERR 0xC4 //服务器要求用户秘宝绑定
#define LOGIN_PHONELOCKED_ERR 0xC5 //用户申请了电话秘宝服务，但未激活帐号。
#define LOGIN_CARDERR_OUTOFLIMIT 0xC6 //密保卡错误次数今日超过了最大限制
#define LOGIN_MAC_ERROR 0xD1 // mac 匹配失败
#define LOGIN_MAC_BIND 0xD2  //mac申请绑定
#define LOGIN_FORBINDDEN_REPEAT 0xE1 //防沉迷帐号同时只能有一个帐号在线

#define LOGIN_NEED_CONFIRMPROTOCOL 0xF1 //用户需要确认协议
#define LOGIN_COFIRM_PROTOCOL 0xF2 // 用户确认协议

//==================PASSPOD AUTH RET==========================
#define LOGIN_PASSPOD_LACK_BIND 0x31    //玩家没有绑定硬件密保
#define LOGIN_PASSPOD_LACK_PASS 0x32    //玩家绑定了硬件密保但是没有输入密保密码
#define LOGIN_PASSPOD_USED	0x33	//这个密保密码已经使用过
#define LOGIN_PASSPOD_FAILED	0x34	//密保验证失败
#define LOGIN_PASSPOD_EXPIRED	0x35	//密保已经过期
#define LOGIN_PASSPOD_DISABLED	0x36	//密保已经被禁用
#define LOGIN_PASSPOD_NEEDSYN	0x37	//用户需要校准令牌
#define LOGIN_PASSPOD_NOTICE	0x38	//令牌快要过期
//==================END PASSPOD AUTH RET======================

//==================MOBILE PASSPOD AUTH RET==========================
#define LOGIN_MOBILE_PASSPOD_LACK_BIND 0x41    //玩家没有绑定手机密保
#define LOGIN_MOBILE_PASSPOD_LACK_PASS 0x42    //玩家绑定了手机密保但是没有输入密保密码
#define LOGIN_MOBILE_PASSPOD_DISABLED  0x43    //密保已经被禁用
#define LOGIN_MOBILE_PASSPOD_EXPIRED   0x44    //密保已经过期
#define LOGIN_MOBILE_PASSPOD_USED      0x45    //这个密保密码已经使用过
#define LOGIN_MOBILE_PASSPOD_FAILED    0x46    //密保验证失败
#define LOGIN_MOBILE_PASSPOD_NOTACTIVE 0x47    //密保绑定没有激活
#define LOGIN_MOBILE_PASSPOD_NEEDSYN   0x48    //用户需要校准令牌
//==================END MOBILE PASSPOD AUTH RET======================

//=====================DIFF LOCATE SECURE CODE=========================
#define LOGIN_NEEDSECURECODE			0xD3 //需要输入安全码，带上安全码
#define LOGIN_LACKSECURECODE			0xD6 //需要输入安全码，但是用户没有安全码
#define LOGIN_SECURECODE_ERR_MAXTIMES	0xD7 //需要输入安全码，但是当天错误次数超过最大数
//=====================END DIFF LOCATE SECURE CODE====================

//=====================LOGIN PUSH=========================================
#define LOGIN_PUSH_BEING_LOCKED 0x51 //因为顶号而被锁定账号-代表这一瞬间[0,0]
#define LOGIN_PUSH_LOCKED 0x52 //因为顶号而被锁定账号-代表一个时间段(0,10]

/////////////////A3-online和A4-logout/////////////////////////////////
#define BILLSTART_SUCCESS           0x01                            //成功
#define BILLSTART_FAIL              0x00                            //失败
	//bill start 错误代码 
#define BILLSTART_ERRLOGIN		0x01	//未正确登录
#define BILLSTART_NOPOINT		0x02	//点数不足
#define BILLSTART_EXPIRED		0x03	//过有效期
#define BILLSTART_ALREADYONLINED 0x04    //已经在线

#define MONTH_USER		0x01	//包月用户
#define NORMAL_USER		0x02	//正常用户
	//bill end 错误代码 
#define BILLEND_SUCCESS             0x01
#define BILLEND_FAIL                0x00
/////////////////A4logout/////////////////////////////////////
//
	struct Req_A2_login
	{
		unsigned int userGUID;
		unsigned char loginNameLenth;
		char loginName[LOGIN_NAME_LENGTH_MAX];//登录名
		unsigned char passwdLenth;
		char passwd[LOGIN_PASSWD_LENGTH_MAX];
		unsigned char useripLenth;
		char userip[IP_LENGTH_MAX];
		unsigned short user_class;//用户级别
		char pos1[CY_MIBAO_POS_LEN + 1]; //密保卡坐标1,2,3
		char pos2[CY_MIBAO_POS_LEN + 1];
		char pos3[CY_MIBAO_POS_LEN + 1];
		char val1[CY_MIBAO_POS_LEN + 1]; //对应密保值1，2，3
		char val2[CY_MIBAO_POS_LEN + 1];
		char val3[CY_MIBAO_POS_LEN + 1];
		char passpod[CY_PASSPOD_PASS_LEN + 1];//硬件密保值，长度为8
		char passZzb[CY_ZZB_PASS_LEN+1];//掌中宝密码
		char userMacMd5[MAC_MD5 + 1]; //用户mac的md5码值，长度固定32
		unsigned char userPcValueLenth;
		char userPcValue[CY_MAX_PC_VAL_LEN];//用户机器特征码
		char passPhone[CY_PHONE_PASS_LEN + 1];//手机密保密码
	};

	struct Res_A2_login_1 {		
		unsigned int leftPoint;//用户剩余点数*1000
		unsigned short leftDay;//用户月卡剩余天数
		unsigned char is_forbidden;//是否纳入防沉迷，Y或者N
		unsigned int online_score;//用户累计在线时间
		//unsigned int outline_score;//用户累计离线时间
		char mobile_bind;//是否手机绑定
		char ip_bind;//是否IP绑定
		char card_bind;//是否绑定密保卡
		char mac_bind;//是否绑定mac地址
		char is_auth;//是否通过实名认证,Y 通过,N 未通过,F 未审核
		char realname_bind;//是否绑定实名
		char fcmstatus;//防沉迷阶段 0x01 第一,0x02 第二, 0x03第三
		char phone_locked;//是否绑定电话密保
		char phoneserviceon;//电话密保是否开启
		char is_Zzb_Bind;
		char passpod_bind;//是否绑定硬件密保
		unsigned short expiredays;//硬件密保到期时间
		char mobile_passpod_bind;//手机密保
		char ps_status;//顶号状态，值为PUSH_STATUS枚举
		char is_bvip; //用户是否B-VIP
		char cn_danger;		//疑似被盗状态
		unsigned char uVipLevel;
		unsigned int nRegDate;//账号注册时间
		unsigned int nLasttLgOutDate;//玩家最后一次登出时间
		unsigned int user_kf_vip;
		unsigned int user_kf_msg_status;
	};

	struct Res_A2_login_2 {
		unsigned short group;//用户在线服务器组号
		unsigned char ipAddrLenth; //用户在线服务器IP长度
		char ipAddr[IP_LENGTH_MAX];//用户在线服务器IP地址
		unsigned char serverNameLenth;//用户在线服务器名称长度
		char serverName[LOGIN_NAME_LENGTH_MAX];//用户在服务器名称
	};

	union UnionRes_A2
	{
		char res[PROTOCOL_BUFFER_MAX];
		Res_A2_login_1 res_1;  //f1,01,38
		Res_A2_login_2 res_2;  //05,
	};

	struct Res_A2_login
	{
		unsigned int userGUID;
		unsigned char loginNameLenth;
		char loginName[LOGIN_NAME_LENGTH_MAX];//登录名
		char last_login_state; //上次登录是否成功
		unsigned char ret; //根据不同的ret，返回不同的结构数据
		UnionRes_A2 res_A2;
	};


	struct Req_A3_online {
		unsigned char loginNameLenth; //主帐号长度
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned char rolenameLenth; //角色名
		char role_name[LOGIN_NAME_LENGTH_MAX + 1];
		unsigned int role_guid; //角色guid
		unsigned short user_class; //角色等级
		unsigned short user_ml_level; //玩家最高角色级别
		unsigned short user_ml_trlevel; //转生等级
		unsigned char user_ml_rolename_lenth; //长度
		char user_ml_rolename[ROLE_NAME_LEN];	//玩家最高级别角色名称
		unsigned int start_time; //开始时间戳
		char login_state;	//是否置为本次登录状态
	};

	struct Res_A3_online_1{
		unsigned char          billing_type;   //填充0,0x01:包月0x02:扣点
		unsigned int            validtime;      //剩余时间秒数，填充1天
		int                      store_point;           //可供虚拟物品商店使用的点数，填充0
		int                      sde_point;              //玩家的真正帐户剩余点数，填充0
		unsigned short           questionnaire_id;       //调查的问卷的id。如果为0,则该服务器不需要调查
		unsigned short           act_count; //玩家参加活动列表数
		unsigned short           activity_list[LOGIN_NAME_LENGTH_MAX];// activity_list[act_count]
	};

	struct Res_A3_online_2 {
		unsigned char errorcode;//出错原因:0x01:用户未正确登录,0x02:账户余额不足,0x03:已过有效期,0x04:用户已经在线.
	};

	union UnionRes_A3 {
		char res[PROTOCOL_BUFFER_MAX];
		Res_A3_online_1 res_1;
		Res_A3_online_2 res_2;
	};

	struct Res_A3_online {
		unsigned char loginNameLenth; //主帐号长度
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;
		UnionRes_A3 res_A3;
	};

	struct Req_A4_logout {
		unsigned char loginNameLenth; //主帐号长度
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned int role_guid; //角色guid
		unsigned short user_class; //角色等级, 包含转生等级
		unsigned short user_level; //角色等级, 不包含转生等级
		unsigned short user_trlevel;		//玩家最高级别角色，转生级别
		unsigned char billType; //计费结束类型，0x01 正常结束	0x02 异常结束 0x03强制结束
		unsigned int start_time; //开始时间
		unsigned int end_time; //结束时间
	};

	struct Res_A4_logout {
		unsigned char loginNameLenth; //主帐号长度
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;
	};
}

using namespace space_daojian;
class GameDaoJian :
	public GameBase
{
public:
	GameDaoJian(int ntype);
	virtual ~GameDaoJian();
	virtual int sendA2(CString &strInfo);
	virtual int sendA3(CString &strInfo);
	virtual int sendA4(CString &strInfo);
	virtual int sendA8(CString &strInfo) { return -8; };
	int m_nGametype;
private:
	Req_A2_login m_req_A2;
	Res_A2_login m_res_A2;
	Req_A3_online m_req_A3;
	Res_A3_online m_res_A3;
	Req_A4_logout m_req_A4;
	Res_A4_logout m_res_A4;
	//DvxHandle m_pDvr;
};

