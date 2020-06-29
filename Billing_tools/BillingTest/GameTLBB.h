#pragma once
#include "GameBase.h"
namespace space_tlbb {

	// -----------A2-----UserLogin数据库接口返回值定义-------------
#define LOGIN_SUCCESS               0x01                            //成功登陆，返回用户点数信息！！！
#define LOGIN_NOUSER                0x02                            //用户不存在！！！
#define LOGIN_ERRPWD                0x03                            //密码错误！！！
#define LOGIN_ONSAME                0x04                            //此用户在本组在线!!!
#define LOGIN_ONANOTHER             0x05                            //在不同组在线,返回所在服务器的组号!!!
#define LOGIN_ERROTHERS             0x06                            //其他错误！！！
#define LOGIN_FORBIDDEN             0x07                            //被禁止登陆(包括ip,停权)！！！
#define LOGIN_LACKMONEY             0x08                            //用户帐户余额不足，让用户充值。不能让用户登陆。

#define LOGIN_NOACTIVE              0xA0                            //add by handx 2008-09-01,账号未激活！！！
#define LOGIN_NEEDACTIVE            0xA1                            //非畅游系账号不存在，需要激活!!!

#define LOGIN_LACKIP                0xB1                            //用户绑定ip!!!
#define LOGIN_LACKMOBILE            0xB2                            // 手机锁定账号  ---屏蔽了

#define LOGIN_IPOVERFLOW            0x21                            //本日登陆ip过高，禁止登录!!!
#define LOGIN_INBLACKLST            0x22                            //该登陆IP在黑名单中!!!
#define LOGIN_PASSPODCARD_LACK_PASS      0x23           //同时绑定了硬件密保和密保卡，但选择的是密保卡登录而非硬件密保!!!
#define LOGIN_PASSPODCARD_LACK_PASSCARD  0x24						//同时绑定了硬件密保和密保卡，但登录时硬件密保和密保卡都未选择！！！
#define LOGIN_PASSPODCARD_PASSPODSERVICEOFF_INPUTPASSPOD  0x25	//同时绑定了硬件密保和密保卡，硬件密保服务关闭，输入了硬件密保!!!
#define LOGIN_CHECK_PC_UNIQUECODE  0x26                         //限制登录, 不允许更换PC, 
#define LOGIN_IP_WHITELIST         0x27                         //限制登录，userip不在白名单
#define LOGIN_CN_WHITELIST         0x28                         //限制登录，cnmaster不在白名单

#define LOGIN_CN_UNDERAGE_18       (0x29)                         //限制登录，未成年人，未满18岁(经典是防沉迷一刀切，不许登陆，绿色会具体执行防沉迷操作)
#define LOGIN_CN_REMOVEACCOUNT     (0x2A)                         //限制登录，注销帐号
#define LOGIN_CN_NO_CERTNUM        (0x2B)                         //获取身份证信息为空
#define LOGIN_CN_ERROR_CERTNUM     (0x2C)                         //身份证判断格式错误
#define LOGIN_FCM_ONLINE_OVERTIME  (0x2D)                         //防沉迷用户在线允许在线时间超时(2020年：90分钟)
#define LOGIN_FCM_ONLINE_ILLEGALTIME  (0x2E)                      //防沉迷用户不可登录时间范围(2020年：22：00--次日8：00)
#define LOGIN_OB_ACTIVE            (0x2F)                         //限制登录，ob中，用户未被激活


#define LOGIN_CARDERR_OUTOFLIMIT    0x40							//当天密保卡输入错误次数过多，超过50次，冻结!!!

//前面有手机密保返回值占了8个数
#define LONG_SERVER_STOP            0x49                            //服务器处于维护状态，为了白名单功能添加!!!
#define LOGIN_FORBIDDEN_ENABLETIME  0x51                            //用户中心三天停权禁止登陆！！！
#define LOGIN_STALL_ONSAME          0x52                            //此用户在本组摆摊！！！
#define LOGIN_STALL_ONANOTHER       0x53                            //在不同组摆摊,返回所在服务器的组号！！！
//====================二维码登陆异常返回结果===================
#define LOGIN_DOI_NO_CN				      0x60                           //未查询到登陆信息
#define LOGIN_DOI_EXCEPT            0x61                           //billing调用mark异常
#define LOGIN_DOI_CLOSED            0x62						   //二维码登陆关闭
//========================end=================================
#define LOGIN_NOT_TLBB_ACTIVE		    0x70							//开启强制激活天龙功能时，用户未激活经典天龙或者从未登录过经典天龙(经典天龙的判定方式)
#define LOGIN_UNSAFE				        0x71							//不安全的登陆 !!!
#define LOGIN_TLBBGREEN_NOT_ACTIVE  (0x72)            //20180206绿色天龙判定方式-1：绿色天龙激活位or经典天龙激活位or认证手机
#define LOGIN_TLZBS_FAIL			      0x73							//没有登录天龙争霸赛资格
#define LOGIN_TLBBGREEN_NOT_ACTIVE_2   (0x74)         //20180606绿色天龙判定方式-2：绿色天龙激活位or认证手机
#define LOGIN_TLBBGREEN_NOT_ACTIVE_TG  (0x75)         //20180606绿色天龙判定方式-3：绿色天龙激活位
#define LOGIN_TLBBGREEN_NOT_ACTIVE_PH  (0x76)         //20180606绿色天龙判定方式-4：认证手机
//==================PASSPOD AUTH RET==========================
#define LOGIN_PASSPOD_LACK_BIND     0x31                            //玩家没有绑定硬件密保!!!
#define LOGIN_PASSPOD_LACK_PASS     0x32                            //玩家绑定了硬件密保但是没有输入密保密码!!!
#define LOGIN_PASSPOD_USED	        0x33	                        //这个密保密码已经使用过！！！
#define LOGIN_PASSPOD_FAILED	      0x34	                        //密保验证失败！！！
#define LOGIN_PASSPOD_EXPIRED	      0x35	                        //密保已经过期！！！
#define LOGIN_PASSPOD_DISABLED	    0x36	                        //密保已经被禁用！！！
#define LOGIN_PASSPOD_NEEDSYN	      0x37	                        //用户需要校准令牌！！！
#define LOGIN_PASSPOD_NOTICE	      0x38	                        //令牌快要过期!!!
#define LOGIN_PASSPOD_CLOSED	      0x39	                        //服务已关闭！！！
//==================END PASSPOD AUTH RET======================
#define LOGIN_NOLACKEDCARD          0xC1                            //用户未申请密宝卡功能!!!
#define LOGIN_LACKEDCARD            0xC2                            //用户申请了密宝卡功能但没有使用!!!
#define LOGIN_LOCKCARD_ERR          0xC3                            //用户输入的密宝卡信息错误!!!
#define LOGIN_SERVERBIND_ERR        0xC4                            //服务器要求用户秘宝绑定
#define LOGIN_PHONELOCKED_ERR       0xC5                            //用户申请了电话秘宝服务，但未激活帐号。---
#define LOGIN_PHONELOCKED_MUSTBE	  0xC6                            //用户必须绑定电话密保才能登陆---
#define LOGIN_FORBIDDEN_LACKNAME    0xC7                            //用户没有输入实名信息禁止登陆!!!

#define LOGIN_MAC_ERROR             0xD1                            // mac地址MD5值 匹配失败!!!
#define LOGIN_MAC_BIND              0xD2                            //mac申请绑定---

#define LOGIN_FORBINDDEN_REPEAT     0xE1                            //防沉迷帐号同时只能有一个帐号在线！！！

#define LOGIN_NEED_CONFIRMPROTOCOL  0xF1                            //用户需要确认协议（还没有接受新许可协议）!!!
#define LOGIN_COFIRM_PROTOCOL       0xF2                            // 用户确认协议---
#define LOGIN_CN_LOCK				        0XF9							              //账号被锁定，CD中（防撞库有CD,不能登录）！！！
#define LOGIN_MOBILE_BIND           0xFF                            //短信密保用户锁定账号---
/////////////////A3-online和A4-logout/////////////////////////////////
#define BILLSTART_SUCCESS           0x01                            //成功
#define BILLSTART_FAIL              0x00                            //失败

#define BILLEND_SUCCESS             0x01
#define BILLEND_FAIL                0x00
/////////////////A4logout/////////////////////////////////////
//
	struct Req_A2_login
	{
		unsigned char logintype;//登录类型:0普通登录，1飞信登录2 二维码登录
		unsigned char loginNameLenth; 
		char loginName[LOGIN_NAME_LENGTH_MAX];//登录名
		unsigned char passwdLenth;
		char passwd[LOGIN_PASSWD_LENGTH_MAX];
		unsigned char useripLenth;
		char userip[IP_LENGTH_MAX];
		unsigned short user_class;//用户级别
		char pos1[CY_MIBAO_POS_LEN+1]; //密保卡坐标1,2,3
		char pos2[CY_MIBAO_POS_LEN+1];
		char pos3[CY_MIBAO_POS_LEN+1];
		char val1[CY_MIBAO_POS_LEN + 1]; //对应密保值1，2，3
		char val2[CY_MIBAO_POS_LEN + 1];
		char val3[CY_MIBAO_POS_LEN + 1];
		char passpod[CY_PASSPOD_PASS_LEN + 1];//硬件密保值，长度为8
		unsigned char macaddrlen;
		char macaddr[CY_MAC_LEN];//MAC地址原串
		char userMacMd5[MAC_MD5 + 1]; //用户mac的md5码值，长度固定32
		unsigned char userPcValueLenth;
		char userPcValue[CY_MAX_PC_VAL_LEN];//用户机器特征码	
		unsigned char game_style; //游戏风格，0普通，1唯美
	};

	struct Res_A2_login_1 {
		unsigned int leftPoint;//用户剩余点数*1000
		unsigned short leftDay;//用户月卡剩余天数
		unsigned char is_forbidden;//是否纳入防沉迷，Y或者N
		unsigned int online_score;//用户累计在线时间
		unsigned int outline_score;//用户累计离线时间
		unsigned char mobile_bind;//是否手机绑定
		unsigned char ip_bind;//是否IP绑定
		unsigned char card_bind;//是否绑定密保卡
		unsigned char mac_bind;//是否绑定mac地址
		unsigned char is_auth;//是否通过实名认证,Y 通过,N 未通过,F 未审核
		unsigned char realname_bind;//是否绑定实名
		unsigned char fcmstatus;//防沉迷阶段 0x01 第一,0x02 第二, 0x03第三
		unsigned char phone_locked;//是否绑定电话密保
		unsigned char phoneserviceon;//电话密保是否开启
		unsigned char passpod_bind;//是否绑定硬件密保
		unsigned short expiredays;//硬件密保到期时间
		unsigned short sec_code;//用户安全等级
		unsigned short vip_level;//用户VIP等级
		unsigned int consum_YuanBao_num;//用户消耗元宝总数
		unsigned char need_notice;//是否移植通知，Y或N
		unsigned char fetion_len;//飞信长度
		char fetion_uid[LOGIN_NAME_LENGTH_MAX];//飞信UID
		unsigned char lastLoginIPLenth;//上一次登录IP地址长度
		char lastLoginIP[IP_LENGTH_MAX];//上一次登录IP
		unsigned char need_register;//是否需要补填注册账号信息,Y或者N
		unsigned char zzb_bind;//是否绑定畅游+
		unsigned char verify_mobile_svr_on;//是否开启手机信息推送服务
		unsigned char verify_mobile_bind;//是否绑定手机消息推送
		unsigned int cnlock_cd;//账号锁定CD时间
	};

	struct Res_A2_login_2 {
		unsigned short group;//用户在线服务器组号
		unsigned char ipAddrLenth; //用户在线服务器IP长度
		char ipAddr[IP_LENGTH_MAX];//用户在线服务器IP地址
		unsigned char serverNameLenth;//用户在线服务器名称长度
		char serverName[LOGIN_NAME_LENGTH_MAX];//用户在服务器名称
	};

	struct Res_A2_login_3 {
		unsigned char card_bind;//是否绑定密保卡
		unsigned char phone_locked;//是否绑定电话密保
		unsigned char passpod_bind;//是否绑定硬件密保
		unsigned char zzb_bind;//是否绑定畅游+
	};

	struct Res_A2_login_4 {
		unsigned int enableTime;
	};

	union UnionRes_A2
	{
		char res[PROTOCOL_BUFFER_MAX];
		Res_A2_login_1 res_1;
		Res_A2_login_2 res_2;
		Res_A2_login_3 res_3;
		Res_A2_login_4 res_4;
	};

	struct Res_A2_login
	{
		unsigned char logintype;//登录类型:0普通登录，1飞信登录2 二维码登录
		unsigned char loginNameLenth; //登录名长度
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned char cnMasterLenth; //主帐号长度
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;//根据不同的ret，返回不同的结构数据
		UnionRes_A2 res_A2;
	};


	struct Req_A3_online {
		unsigned char cnMasterLenth; //主帐号长度
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned char rolenameLenth; //角色名
		char role_name[LOGIN_NAME_LENGTH_MAX + 1];
		unsigned long long guid; //角色guid
		unsigned short user_class; //角色等级
		unsigned int start_time; //开始时间
		unsigned char game_style; //游戏风格，0 经典 1唯美
	};

	struct Res_A3_online_1{
		unsigned char          pay_type;   //填充0,0x01:包月0x02:扣点
		unsigned int            left_time;      //剩余时间秒数，填充1天
		int                      can_use_point;           //可供商店使用的点数，填充0
		int                      user_point;              //玩家的真正点数填充0
		unsigned short           questionnaire_id;                //调查的问卷的id。如果为0,则该服务器不需要调查
		unsigned char			award_notice;			//领奖提示标识，0x00功能未开启，0x01有奖提示，0x02无奖，0x06处理错误
		unsigned char			award_cn_group;						//账号、指定服务器领奖
		unsigned char			award_cn_guid;						//账号、角色领奖
		unsigned char			award_cn_guid_group;				//账号、角色、指定服务领奖
		unsigned int			unMsgCreateTime;				//Billing缓存的最新一条消息生成时间
		char			szMsgType[CY_STOREMSG_LEN + 1];	//Billing缓存的最新一条消息内容
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
		unsigned char cnMasterLenth; //主帐号长度
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;
		UnionRes_A3 res_A3;
	};

	struct Req_A4_logout {
		unsigned char cnMasterLenth; //主帐号长度
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned short user_class; //角色等级
		unsigned char billType; //计费结束类型，0x01 正常结束	0x02 异常结束 0x03强制结束
		unsigned int start_time; //开始时间
		unsigned int end_time; //结束时间
		unsigned char game_style; //游戏风格，0 经典 1唯美
	};

	struct Res_A4_logout {
		unsigned char cnMasterLenth; //主帐号长度
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;
	};

  struct Req_A8_login_ByTwo
  {
    unsigned char logintype;//登录类型:0普通登录，1飞信登录2 二维码登录
    unsigned char Codelength;
    char twoDimenCode[MAX_TWO_DIMEN_CODE_LEN+1];//二维码串
    unsigned char useripLenth;
    char userip[IP_LENGTH_MAX];
    unsigned char macaddrlen;
    char macaddr[CY_MAC_LEN];//MAC地址原串
    char userMacMd5[MAC_MD5 + 1]; //用户mac的md5码值，长度固定32
    unsigned char userPcValueLenth;
    char userPcValue[CY_MAX_PC_VAL_LEN];//用户机器特征码	
    unsigned char game_style; //游戏风格，0普通，1唯美
  };

  struct Res_A8_login_1 {
    unsigned int leftPoint;//用户剩余点数*1000
    unsigned short leftDay;//用户月卡剩余天数
    unsigned char is_forbidden;//是否纳入防沉迷，Y或者N
    unsigned int online_score;//用户累计在线时间
    unsigned int outline_score;//用户累计离线时间
    unsigned char mobile_bind;//是否手机绑定
    unsigned char ip_bind;//是否IP绑定
    unsigned char card_bind;//是否绑定密保卡
    unsigned char mac_bind;//是否绑定mac地址
    unsigned char is_auth;//是否通过实名认证,Y 通过,N 未通过,F 未审核
    unsigned char realname_bind;//是否绑定实名
    unsigned char fcmstatus;//防沉迷阶段 0x01 第一,0x02 第二, 0x03第三
    unsigned char phone_locked;//是否绑定电话密保
    unsigned char phoneserviceon;//电话密保是否开启
    unsigned char passpod_bind;//是否绑定硬件密保
    unsigned short expiredays;//硬件密保到期时间
    unsigned short sec_code;//用户安全等级
    unsigned short vip_level;//用户VIP等级
    unsigned int consum_YuanBao_num;//用户消耗元宝总数
    unsigned char need_notice;//是否移植通知，Y或N
    unsigned char fetion_len;//飞信长度
    char fetion_uid[LOGIN_NAME_LENGTH_MAX];//飞信UID
    unsigned char lastLoginIPLenth;//上一次登录IP地址长度
    char lastLoginIP[IP_LENGTH_MAX];//上一次登录IP
    unsigned char need_register;//是否需要补填注册账号信息,Y或者N
    unsigned char zzb_bind;//是否绑定畅游+
    unsigned char verify_mobile_svr_on;//是否开启手机信息推送服务
    unsigned char verify_mobile_bind;//是否绑定手机消息推送
  };

  union UnionRes_A8  
  {
    char res[PROTOCOL_BUFFER_MAX];
    Res_A8_login_1 res_1;  //没有cnlock_cd
    Res_A2_login_2 res_2;  //和A2一样，没有res_3结构体
    Res_A2_login_4 res_4;  //和A2一样
  };

  struct Res_A8_login_ByTwo   //A8和A2返回值主要的区别有两个: 没有res_3结构体，没有cnlock_cd
  {
    unsigned char logintype;//登录类型:0普通登录，1飞信登录2 二维码登录
    unsigned char loginNameLenth; //登录名长度
    char loginName[LOGIN_NAME_LENGTH_MAX];
    unsigned char cnMasterLenth; //主帐号长度
    char cnMaster[LOGIN_NAME_LENGTH_MAX];
    unsigned char ret;//根据不同的ret，返回不同的结构数据
    UnionRes_A8 res_A8;
  };

  struct Req_01_Test {
    unsigned char cnMasterLenth; //主帐号长度
    char cnMaster[LOGIN_NAME_LENGTH_MAX];
  };
  struct Res_01_Test {
    unsigned char cnMasterLenth; //主帐号长度
    char cnMaster[LOGIN_NAME_LENGTH_MAX];
    unsigned char ret;
  };

}

using namespace space_tlbb;
class GameTLBB :
	public GameBase
{
public:
	GameTLBB(int ntype);
	virtual ~GameTLBB();
	virtual int sendA2(CString &strInfo);
	virtual int sendA3(CString &strInfo);
	virtual int sendA4(CString &strInfo);
  virtual int sendA8(CString &strInfo);
  virtual int send01(CString &strInfo);
	int m_nGametype;
private:
	Req_A2_login m_req_A2;
	Res_A2_login m_res_A2;
	Req_A3_online m_req_A3;
	Res_A3_online m_res_A3;
	Req_A4_logout m_req_A4;
	Res_A4_logout m_res_A4;
  Req_A8_login_ByTwo m_req_A8;
  Res_A8_login_ByTwo m_res_A8;
  Req_01_Test m_req_01;
  Res_01_Test m_res_01;
	//DvxHandle m_pDvr;
};

