#pragma once
#include "GameBase.h"
namespace space_tlbb {

	// -----------A2-----UserLogin���ݿ�ӿڷ���ֵ����-------------
#define LOGIN_SUCCESS               0x01                            //�ɹ���½�������û�������Ϣ������
#define LOGIN_NOUSER                0x02                            //�û������ڣ�����
#define LOGIN_ERRPWD                0x03                            //������󣡣���
#define LOGIN_ONSAME                0x04                            //���û��ڱ�������!!!
#define LOGIN_ONANOTHER             0x05                            //�ڲ�ͬ������,�������ڷ����������!!!
#define LOGIN_ERROTHERS             0x06                            //�������󣡣���
#define LOGIN_FORBIDDEN             0x07                            //����ֹ��½(����ip,ͣȨ)������
#define LOGIN_LACKMONEY             0x08                            //�û��ʻ����㣬���û���ֵ���������û���½��

#define LOGIN_NOACTIVE              0xA0                            //add by handx 2008-09-01,�˺�δ�������
#define LOGIN_NEEDACTIVE            0xA1                            //�ǳ���ϵ�˺Ų����ڣ���Ҫ����!!!

#define LOGIN_LACKIP                0xB1                            //�û���ip!!!
#define LOGIN_LACKMOBILE            0xB2                            // �ֻ������˺�  ---������

#define LOGIN_IPOVERFLOW            0x21                            //���յ�½ip���ߣ���ֹ��¼!!!
#define LOGIN_INBLACKLST            0x22                            //�õ�½IP�ں�������!!!
#define LOGIN_PASSPODCARD_LACK_PASS      0x23           //ͬʱ����Ӳ���ܱ����ܱ�������ѡ������ܱ�����¼����Ӳ���ܱ�!!!
#define LOGIN_PASSPODCARD_LACK_PASSCARD  0x24						//ͬʱ����Ӳ���ܱ����ܱ���������¼ʱӲ���ܱ����ܱ�����δѡ�񣡣���
#define LOGIN_PASSPODCARD_PASSPODSERVICEOFF_INPUTPASSPOD  0x25	//ͬʱ����Ӳ���ܱ����ܱ�����Ӳ���ܱ�����رգ�������Ӳ���ܱ�!!!
#define LOGIN_CHECK_PC_UNIQUECODE  0x26                         //���Ƶ�¼, ���������PC, 
#define LOGIN_IP_WHITELIST         0x27                         //���Ƶ�¼��userip���ڰ�����
#define LOGIN_CN_WHITELIST         0x28                         //���Ƶ�¼��cnmaster���ڰ�����

#define LOGIN_CN_UNDERAGE_18       (0x29)                         //���Ƶ�¼��δ�����ˣ�δ��18��(�����Ƿ�����һ���У������½����ɫ�����ִ�з����Բ���)
#define LOGIN_CN_REMOVEACCOUNT     (0x2A)                         //���Ƶ�¼��ע���ʺ�
#define LOGIN_CN_NO_CERTNUM        (0x2B)                         //��ȡ���֤��ϢΪ��
#define LOGIN_CN_ERROR_CERTNUM     (0x2C)                         //���֤�жϸ�ʽ����
#define LOGIN_FCM_ONLINE_OVERTIME  (0x2D)                         //�������û�������������ʱ�䳬ʱ(2020�꣺90����)
#define LOGIN_FCM_ONLINE_ILLEGALTIME  (0x2E)                      //�������û����ɵ�¼ʱ�䷶Χ(2020�꣺22��00--����8��00)
#define LOGIN_OB_ACTIVE            (0x2F)                         //���Ƶ�¼��ob�У��û�δ������


#define LOGIN_CARDERR_OUTOFLIMIT    0x40							//�����ܱ����������������࣬����50�Σ�����!!!

//ǰ�����ֻ��ܱ�����ֵռ��8����
#define LONG_SERVER_STOP            0x49                            //����������ά��״̬��Ϊ�˰������������!!!
#define LOGIN_FORBIDDEN_ENABLETIME  0x51                            //�û���������ͣȨ��ֹ��½������
#define LOGIN_STALL_ONSAME          0x52                            //���û��ڱ����̯������
#define LOGIN_STALL_ONANOTHER       0x53                            //�ڲ�ͬ���̯,�������ڷ���������ţ�����
//====================��ά���½�쳣���ؽ��===================
#define LOGIN_DOI_NO_CN				      0x60                           //δ��ѯ����½��Ϣ
#define LOGIN_DOI_EXCEPT            0x61                           //billing����mark�쳣
#define LOGIN_DOI_CLOSED            0x62						   //��ά���½�ر�
//========================end=================================
#define LOGIN_NOT_TLBB_ACTIVE		    0x70							//����ǿ�Ƽ�����������ʱ���û�δ������������ߴ�δ��¼����������(�����������ж���ʽ)
#define LOGIN_UNSAFE				        0x71							//����ȫ�ĵ�½ !!!
#define LOGIN_TLBBGREEN_NOT_ACTIVE  (0x72)            //20180206��ɫ�����ж���ʽ-1����ɫ��������λor������������λor��֤�ֻ�
#define LOGIN_TLZBS_FAIL			      0x73							//û�е�¼�����������ʸ�
#define LOGIN_TLBBGREEN_NOT_ACTIVE_2   (0x74)         //20180606��ɫ�����ж���ʽ-2����ɫ��������λor��֤�ֻ�
#define LOGIN_TLBBGREEN_NOT_ACTIVE_TG  (0x75)         //20180606��ɫ�����ж���ʽ-3����ɫ��������λ
#define LOGIN_TLBBGREEN_NOT_ACTIVE_PH  (0x76)         //20180606��ɫ�����ж���ʽ-4����֤�ֻ�
//==================PASSPOD AUTH RET==========================
#define LOGIN_PASSPOD_LACK_BIND     0x31                            //���û�а�Ӳ���ܱ�!!!
#define LOGIN_PASSPOD_LACK_PASS     0x32                            //��Ұ���Ӳ���ܱ�����û�������ܱ�����!!!
#define LOGIN_PASSPOD_USED	        0x33	                        //����ܱ������Ѿ�ʹ�ù�������
#define LOGIN_PASSPOD_FAILED	      0x34	                        //�ܱ���֤ʧ�ܣ�����
#define LOGIN_PASSPOD_EXPIRED	      0x35	                        //�ܱ��Ѿ����ڣ�����
#define LOGIN_PASSPOD_DISABLED	    0x36	                        //�ܱ��Ѿ������ã�����
#define LOGIN_PASSPOD_NEEDSYN	      0x37	                        //�û���ҪУ׼���ƣ�����
#define LOGIN_PASSPOD_NOTICE	      0x38	                        //���ƿ�Ҫ����!!!
#define LOGIN_PASSPOD_CLOSED	      0x39	                        //�����ѹرգ�����
//==================END PASSPOD AUTH RET======================
#define LOGIN_NOLACKEDCARD          0xC1                            //�û�δ�����ܱ�������!!!
#define LOGIN_LACKEDCARD            0xC2                            //�û��������ܱ������ܵ�û��ʹ��!!!
#define LOGIN_LOCKCARD_ERR          0xC3                            //�û�������ܱ�����Ϣ����!!!
#define LOGIN_SERVERBIND_ERR        0xC4                            //������Ҫ���û��ر���
#define LOGIN_PHONELOCKED_ERR       0xC5                            //�û������˵绰�ر����񣬵�δ�����ʺš�---
#define LOGIN_PHONELOCKED_MUSTBE	  0xC6                            //�û�����󶨵绰�ܱ����ܵ�½---
#define LOGIN_FORBIDDEN_LACKNAME    0xC7                            //�û�û������ʵ����Ϣ��ֹ��½!!!

#define LOGIN_MAC_ERROR             0xD1                            // mac��ַMD5ֵ ƥ��ʧ��!!!
#define LOGIN_MAC_BIND              0xD2                            //mac�����---

#define LOGIN_FORBINDDEN_REPEAT     0xE1                            //�������ʺ�ͬʱֻ����һ���ʺ����ߣ�����

#define LOGIN_NEED_CONFIRMPROTOCOL  0xF1                            //�û���Ҫȷ��Э�飨��û�н��������Э�飩!!!
#define LOGIN_COFIRM_PROTOCOL       0xF2                            // �û�ȷ��Э��---
#define LOGIN_CN_LOCK				        0XF9							              //�˺ű�������CD�У���ײ����CD,���ܵ�¼��������
#define LOGIN_MOBILE_BIND           0xFF                            //�����ܱ��û������˺�---
/////////////////A3-online��A4-logout/////////////////////////////////
#define BILLSTART_SUCCESS           0x01                            //�ɹ�
#define BILLSTART_FAIL              0x00                            //ʧ��

#define BILLEND_SUCCESS             0x01
#define BILLEND_FAIL                0x00
/////////////////A4logout/////////////////////////////////////
//
	struct Req_A2_login
	{
		unsigned char logintype;//��¼����:0��ͨ��¼��1���ŵ�¼2 ��ά���¼
		unsigned char loginNameLenth; 
		char loginName[LOGIN_NAME_LENGTH_MAX];//��¼��
		unsigned char passwdLenth;
		char passwd[LOGIN_PASSWD_LENGTH_MAX];
		unsigned char useripLenth;
		char userip[IP_LENGTH_MAX];
		unsigned short user_class;//�û�����
		char pos1[CY_MIBAO_POS_LEN+1]; //�ܱ�������1,2,3
		char pos2[CY_MIBAO_POS_LEN+1];
		char pos3[CY_MIBAO_POS_LEN+1];
		char val1[CY_MIBAO_POS_LEN + 1]; //��Ӧ�ܱ�ֵ1��2��3
		char val2[CY_MIBAO_POS_LEN + 1];
		char val3[CY_MIBAO_POS_LEN + 1];
		char passpod[CY_PASSPOD_PASS_LEN + 1];//Ӳ���ܱ�ֵ������Ϊ8
		unsigned char macaddrlen;
		char macaddr[CY_MAC_LEN];//MAC��ַԭ��
		char userMacMd5[MAC_MD5 + 1]; //�û�mac��md5��ֵ�����ȹ̶�32
		unsigned char userPcValueLenth;
		char userPcValue[CY_MAX_PC_VAL_LEN];//�û�����������	
		unsigned char game_style; //��Ϸ���0��ͨ��1Ψ��
	};

	struct Res_A2_login_1 {
		unsigned int leftPoint;//�û�ʣ�����*1000
		unsigned short leftDay;//�û��¿�ʣ������
		unsigned char is_forbidden;//�Ƿ���������ԣ�Y����N
		unsigned int online_score;//�û��ۼ�����ʱ��
		unsigned int outline_score;//�û��ۼ�����ʱ��
		unsigned char mobile_bind;//�Ƿ��ֻ���
		unsigned char ip_bind;//�Ƿ�IP��
		unsigned char card_bind;//�Ƿ���ܱ���
		unsigned char mac_bind;//�Ƿ��mac��ַ
		unsigned char is_auth;//�Ƿ�ͨ��ʵ����֤,Y ͨ��,N δͨ��,F δ���
		unsigned char realname_bind;//�Ƿ��ʵ��
		unsigned char fcmstatus;//�����Խ׶� 0x01 ��һ,0x02 �ڶ�, 0x03����
		unsigned char phone_locked;//�Ƿ�󶨵绰�ܱ�
		unsigned char phoneserviceon;//�绰�ܱ��Ƿ���
		unsigned char passpod_bind;//�Ƿ��Ӳ���ܱ�
		unsigned short expiredays;//Ӳ���ܱ�����ʱ��
		unsigned short sec_code;//�û���ȫ�ȼ�
		unsigned short vip_level;//�û�VIP�ȼ�
		unsigned int consum_YuanBao_num;//�û�����Ԫ������
		unsigned char need_notice;//�Ƿ���ֲ֪ͨ��Y��N
		unsigned char fetion_len;//���ų���
		char fetion_uid[LOGIN_NAME_LENGTH_MAX];//����UID
		unsigned char lastLoginIPLenth;//��һ�ε�¼IP��ַ����
		char lastLoginIP[IP_LENGTH_MAX];//��һ�ε�¼IP
		unsigned char need_register;//�Ƿ���Ҫ����ע���˺���Ϣ,Y����N
		unsigned char zzb_bind;//�Ƿ�󶨳���+
		unsigned char verify_mobile_svr_on;//�Ƿ����ֻ���Ϣ���ͷ���
		unsigned char verify_mobile_bind;//�Ƿ���ֻ���Ϣ����
		unsigned int cnlock_cd;//�˺�����CDʱ��
	};

	struct Res_A2_login_2 {
		unsigned short group;//�û����߷��������
		unsigned char ipAddrLenth; //�û����߷�����IP����
		char ipAddr[IP_LENGTH_MAX];//�û����߷�����IP��ַ
		unsigned char serverNameLenth;//�û����߷��������Ƴ���
		char serverName[LOGIN_NAME_LENGTH_MAX];//�û��ڷ���������
	};

	struct Res_A2_login_3 {
		unsigned char card_bind;//�Ƿ���ܱ���
		unsigned char phone_locked;//�Ƿ�󶨵绰�ܱ�
		unsigned char passpod_bind;//�Ƿ��Ӳ���ܱ�
		unsigned char zzb_bind;//�Ƿ�󶨳���+
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
		unsigned char logintype;//��¼����:0��ͨ��¼��1���ŵ�¼2 ��ά���¼
		unsigned char loginNameLenth; //��¼������
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned char cnMasterLenth; //���ʺų���
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;//���ݲ�ͬ��ret�����ز�ͬ�Ľṹ����
		UnionRes_A2 res_A2;
	};


	struct Req_A3_online {
		unsigned char cnMasterLenth; //���ʺų���
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned char rolenameLenth; //��ɫ��
		char role_name[LOGIN_NAME_LENGTH_MAX + 1];
		unsigned long long guid; //��ɫguid
		unsigned short user_class; //��ɫ�ȼ�
		unsigned int start_time; //��ʼʱ��
		unsigned char game_style; //��Ϸ���0 ���� 1Ψ��
	};

	struct Res_A3_online_1{
		unsigned char          pay_type;   //���0,0x01:����0x02:�۵�
		unsigned int            left_time;      //ʣ��ʱ�����������1��
		int                      can_use_point;           //�ɹ��̵�ʹ�õĵ��������0
		int                      user_point;              //��ҵ������������0
		unsigned short           questionnaire_id;                //������ʾ��id�����Ϊ0,��÷���������Ҫ����
		unsigned char			award_notice;			//�콱��ʾ��ʶ��0x00����δ������0x01�н���ʾ��0x02�޽���0x06�������
		unsigned char			award_cn_group;						//�˺š�ָ���������콱
		unsigned char			award_cn_guid;						//�˺š���ɫ�콱
		unsigned char			award_cn_guid_group;				//�˺š���ɫ��ָ�������콱
		unsigned int			unMsgCreateTime;				//Billing���������һ����Ϣ����ʱ��
		char			szMsgType[CY_STOREMSG_LEN + 1];	//Billing���������һ����Ϣ����
	};

	struct Res_A3_online_2 {
		unsigned char errorcode;//����ԭ��:0x01:�û�δ��ȷ��¼,0x02:�˻�����,0x03:�ѹ���Ч��,0x04:�û��Ѿ�����.
	};

	union UnionRes_A3 {
		char res[PROTOCOL_BUFFER_MAX];
		Res_A3_online_1 res_1;
		Res_A3_online_2 res_2;
	};

	struct Res_A3_online {
		unsigned char cnMasterLenth; //���ʺų���
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;
		UnionRes_A3 res_A3;
	};

	struct Req_A4_logout {
		unsigned char cnMasterLenth; //���ʺų���
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned short user_class; //��ɫ�ȼ�
		unsigned char billType; //�Ʒѽ������ͣ�0x01 ��������	0x02 �쳣���� 0x03ǿ�ƽ���
		unsigned int start_time; //��ʼʱ��
		unsigned int end_time; //����ʱ��
		unsigned char game_style; //��Ϸ���0 ���� 1Ψ��
	};

	struct Res_A4_logout {
		unsigned char cnMasterLenth; //���ʺų���
		char cnMaster[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;
	};

  struct Req_A8_login_ByTwo
  {
    unsigned char logintype;//��¼����:0��ͨ��¼��1���ŵ�¼2 ��ά���¼
    unsigned char Codelength;
    char twoDimenCode[MAX_TWO_DIMEN_CODE_LEN+1];//��ά�봮
    unsigned char useripLenth;
    char userip[IP_LENGTH_MAX];
    unsigned char macaddrlen;
    char macaddr[CY_MAC_LEN];//MAC��ַԭ��
    char userMacMd5[MAC_MD5 + 1]; //�û�mac��md5��ֵ�����ȹ̶�32
    unsigned char userPcValueLenth;
    char userPcValue[CY_MAX_PC_VAL_LEN];//�û�����������	
    unsigned char game_style; //��Ϸ���0��ͨ��1Ψ��
  };

  struct Res_A8_login_1 {
    unsigned int leftPoint;//�û�ʣ�����*1000
    unsigned short leftDay;//�û��¿�ʣ������
    unsigned char is_forbidden;//�Ƿ���������ԣ�Y����N
    unsigned int online_score;//�û��ۼ�����ʱ��
    unsigned int outline_score;//�û��ۼ�����ʱ��
    unsigned char mobile_bind;//�Ƿ��ֻ���
    unsigned char ip_bind;//�Ƿ�IP��
    unsigned char card_bind;//�Ƿ���ܱ���
    unsigned char mac_bind;//�Ƿ��mac��ַ
    unsigned char is_auth;//�Ƿ�ͨ��ʵ����֤,Y ͨ��,N δͨ��,F δ���
    unsigned char realname_bind;//�Ƿ��ʵ��
    unsigned char fcmstatus;//�����Խ׶� 0x01 ��һ,0x02 �ڶ�, 0x03����
    unsigned char phone_locked;//�Ƿ�󶨵绰�ܱ�
    unsigned char phoneserviceon;//�绰�ܱ��Ƿ���
    unsigned char passpod_bind;//�Ƿ��Ӳ���ܱ�
    unsigned short expiredays;//Ӳ���ܱ�����ʱ��
    unsigned short sec_code;//�û���ȫ�ȼ�
    unsigned short vip_level;//�û�VIP�ȼ�
    unsigned int consum_YuanBao_num;//�û�����Ԫ������
    unsigned char need_notice;//�Ƿ���ֲ֪ͨ��Y��N
    unsigned char fetion_len;//���ų���
    char fetion_uid[LOGIN_NAME_LENGTH_MAX];//����UID
    unsigned char lastLoginIPLenth;//��һ�ε�¼IP��ַ����
    char lastLoginIP[IP_LENGTH_MAX];//��һ�ε�¼IP
    unsigned char need_register;//�Ƿ���Ҫ����ע���˺���Ϣ,Y����N
    unsigned char zzb_bind;//�Ƿ�󶨳���+
    unsigned char verify_mobile_svr_on;//�Ƿ����ֻ���Ϣ���ͷ���
    unsigned char verify_mobile_bind;//�Ƿ���ֻ���Ϣ����
  };

  union UnionRes_A8  
  {
    char res[PROTOCOL_BUFFER_MAX];
    Res_A8_login_1 res_1;  //û��cnlock_cd
    Res_A2_login_2 res_2;  //��A2һ����û��res_3�ṹ��
    Res_A2_login_4 res_4;  //��A2һ��
  };

  struct Res_A8_login_ByTwo   //A8��A2����ֵ��Ҫ������������: û��res_3�ṹ�壬û��cnlock_cd
  {
    unsigned char logintype;//��¼����:0��ͨ��¼��1���ŵ�¼2 ��ά���¼
    unsigned char loginNameLenth; //��¼������
    char loginName[LOGIN_NAME_LENGTH_MAX];
    unsigned char cnMasterLenth; //���ʺų���
    char cnMaster[LOGIN_NAME_LENGTH_MAX];
    unsigned char ret;//���ݲ�ͬ��ret�����ز�ͬ�Ľṹ����
    UnionRes_A8 res_A8;
  };

  struct Req_01_Test {
    unsigned char cnMasterLenth; //���ʺų���
    char cnMaster[LOGIN_NAME_LENGTH_MAX];
  };
  struct Res_01_Test {
    unsigned char cnMasterLenth; //���ʺų���
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

