#pragma once
#include "GameBase.h"
namespace space_daojian {

	// -----------A2-----UserLogin���ݿ�ӿڷ���ֵ����-------------
#define LOGIN_SUCCESS 0x01 //�ɹ���½�������û�������Ϣ
#define LOGIN_NOUSER  0x02 //�û�������
#define LOGIN_ERRPWD  0x03 //�������
#define LOGIN_ONSAME  0x04 //���û��ڱ�������
#define LOGIN_ONANOTHER  0x05 //�ڲ�ͬ������,�������ڷ����������
#define LOGIN_ERROTHERS  0x06 //��������
#define LOGIN_FORBIDDEN  0x07 //����ֹ��½(����ip,ͣȨ)
#define LOGIN_LACKMONEY  0x08 //�û��ʻ����㣬���û���ֵ���������û���½��
#define LOGIN_FORBIDDEN_BY_USERSELF 0x18


#define LOGIN_IPOVERFLOW  0x21 //���յ�½ip���ߣ���ֹ��¼
#define LOGIN_INBLACKLST  0x22 //�õ�½IP�ں�������

#define LOGIN_CN_UNDERAGE_18       0x29                         //���Ƶ�¼��δ�����ˣ�δ��18��
#define LOGIN_CN_REMOVEACCOUNT     0x2A                         //���Ƶ�¼��ע���ʺ�
#define LOGIN_CN_NO_CERTNUM        0x2B                         //��ȡ���֤��ϢΪ��
#define LOGIN_CN_ERROR_CERTNUM     0x2C                         //���֤�жϸ�ʽ����

#define LOGIN_LACKIP 0xB1  //�û���ip
#define LOGIN_LACKMOBILE 0xB2 // �ֻ��Լ������˺�
#define LOGIN_LACKEDCARD 0xC2  //�û��������ܱ������ܵ�û��ʹ��
#define LOGIN_NOLACKEDCARD 0xC1  //�û�δ�����ܱ�������

#define LOGIN_FORBIDDEN_LACKNAME 0xC7 //�û�û������ʵ����Ϣ��ֹ��½

#define LOGIN_LOCKCARD_ERR 0xC3  //�û�������ܱ�����Ϣ����
#define LOGIN_SERVERBIND_ERR 0xC4 //������Ҫ���û��ر���
#define LOGIN_PHONELOCKED_ERR 0xC5 //�û������˵绰�ر����񣬵�δ�����ʺš�
#define LOGIN_CARDERR_OUTOFLIMIT 0xC6 //�ܱ�������������ճ������������
#define LOGIN_MAC_ERROR 0xD1 // mac ƥ��ʧ��
#define LOGIN_MAC_BIND 0xD2  //mac�����
#define LOGIN_FORBINDDEN_REPEAT 0xE1 //�������ʺ�ͬʱֻ����һ���ʺ�����

#define LOGIN_NEED_CONFIRMPROTOCOL 0xF1 //�û���Ҫȷ��Э��
#define LOGIN_COFIRM_PROTOCOL 0xF2 // �û�ȷ��Э��

//==================PASSPOD AUTH RET==========================
#define LOGIN_PASSPOD_LACK_BIND 0x31    //���û�а�Ӳ���ܱ�
#define LOGIN_PASSPOD_LACK_PASS 0x32    //��Ұ���Ӳ���ܱ�����û�������ܱ�����
#define LOGIN_PASSPOD_USED	0x33	//����ܱ������Ѿ�ʹ�ù�
#define LOGIN_PASSPOD_FAILED	0x34	//�ܱ���֤ʧ��
#define LOGIN_PASSPOD_EXPIRED	0x35	//�ܱ��Ѿ�����
#define LOGIN_PASSPOD_DISABLED	0x36	//�ܱ��Ѿ�������
#define LOGIN_PASSPOD_NEEDSYN	0x37	//�û���ҪУ׼����
#define LOGIN_PASSPOD_NOTICE	0x38	//���ƿ�Ҫ����
//==================END PASSPOD AUTH RET======================

//==================MOBILE PASSPOD AUTH RET==========================
#define LOGIN_MOBILE_PASSPOD_LACK_BIND 0x41    //���û�а��ֻ��ܱ�
#define LOGIN_MOBILE_PASSPOD_LACK_PASS 0x42    //��Ұ����ֻ��ܱ�����û�������ܱ�����
#define LOGIN_MOBILE_PASSPOD_DISABLED  0x43    //�ܱ��Ѿ�������
#define LOGIN_MOBILE_PASSPOD_EXPIRED   0x44    //�ܱ��Ѿ�����
#define LOGIN_MOBILE_PASSPOD_USED      0x45    //����ܱ������Ѿ�ʹ�ù�
#define LOGIN_MOBILE_PASSPOD_FAILED    0x46    //�ܱ���֤ʧ��
#define LOGIN_MOBILE_PASSPOD_NOTACTIVE 0x47    //�ܱ���û�м���
#define LOGIN_MOBILE_PASSPOD_NEEDSYN   0x48    //�û���ҪУ׼����
//==================END MOBILE PASSPOD AUTH RET======================

//=====================DIFF LOCATE SECURE CODE=========================
#define LOGIN_NEEDSECURECODE			0xD3 //��Ҫ���밲ȫ�룬���ϰ�ȫ��
#define LOGIN_LACKSECURECODE			0xD6 //��Ҫ���밲ȫ�룬�����û�û�а�ȫ��
#define LOGIN_SECURECODE_ERR_MAXTIMES	0xD7 //��Ҫ���밲ȫ�룬���ǵ������������������
//=====================END DIFF LOCATE SECURE CODE====================

//=====================LOGIN PUSH=========================================
#define LOGIN_PUSH_BEING_LOCKED 0x51 //��Ϊ���Ŷ��������˺�-������һ˲��[0,0]
#define LOGIN_PUSH_LOCKED 0x52 //��Ϊ���Ŷ��������˺�-����һ��ʱ���(0,10]

/////////////////A3-online��A4-logout/////////////////////////////////
#define BILLSTART_SUCCESS           0x01                            //�ɹ�
#define BILLSTART_FAIL              0x00                            //ʧ��
	//bill start ������� 
#define BILLSTART_ERRLOGIN		0x01	//δ��ȷ��¼
#define BILLSTART_NOPOINT		0x02	//��������
#define BILLSTART_EXPIRED		0x03	//����Ч��
#define BILLSTART_ALREADYONLINED 0x04    //�Ѿ�����

#define MONTH_USER		0x01	//�����û�
#define NORMAL_USER		0x02	//�����û�
	//bill end ������� 
#define BILLEND_SUCCESS             0x01
#define BILLEND_FAIL                0x00
/////////////////A4logout/////////////////////////////////////
//
	struct Req_A2_login
	{
		unsigned int userGUID;
		unsigned char loginNameLenth;
		char loginName[LOGIN_NAME_LENGTH_MAX];//��¼��
		unsigned char passwdLenth;
		char passwd[LOGIN_PASSWD_LENGTH_MAX];
		unsigned char useripLenth;
		char userip[IP_LENGTH_MAX];
		unsigned short user_class;//�û�����
		char pos1[CY_MIBAO_POS_LEN + 1]; //�ܱ�������1,2,3
		char pos2[CY_MIBAO_POS_LEN + 1];
		char pos3[CY_MIBAO_POS_LEN + 1];
		char val1[CY_MIBAO_POS_LEN + 1]; //��Ӧ�ܱ�ֵ1��2��3
		char val2[CY_MIBAO_POS_LEN + 1];
		char val3[CY_MIBAO_POS_LEN + 1];
		char passpod[CY_PASSPOD_PASS_LEN + 1];//Ӳ���ܱ�ֵ������Ϊ8
		char passZzb[CY_ZZB_PASS_LEN+1];//���б�����
		char userMacMd5[MAC_MD5 + 1]; //�û�mac��md5��ֵ�����ȹ̶�32
		unsigned char userPcValueLenth;
		char userPcValue[CY_MAX_PC_VAL_LEN];//�û�����������
		char passPhone[CY_PHONE_PASS_LEN + 1];//�ֻ��ܱ�����
	};

	struct Res_A2_login_1 {		
		unsigned int leftPoint;//�û�ʣ�����*1000
		unsigned short leftDay;//�û��¿�ʣ������
		unsigned char is_forbidden;//�Ƿ���������ԣ�Y����N
		unsigned int online_score;//�û��ۼ�����ʱ��
		//unsigned int outline_score;//�û��ۼ�����ʱ��
		char mobile_bind;//�Ƿ��ֻ���
		char ip_bind;//�Ƿ�IP��
		char card_bind;//�Ƿ���ܱ���
		char mac_bind;//�Ƿ��mac��ַ
		char is_auth;//�Ƿ�ͨ��ʵ����֤,Y ͨ��,N δͨ��,F δ���
		char realname_bind;//�Ƿ��ʵ��
		char fcmstatus;//�����Խ׶� 0x01 ��һ,0x02 �ڶ�, 0x03����
		char phone_locked;//�Ƿ�󶨵绰�ܱ�
		char phoneserviceon;//�绰�ܱ��Ƿ���
		char is_Zzb_Bind;
		char passpod_bind;//�Ƿ��Ӳ���ܱ�
		unsigned short expiredays;//Ӳ���ܱ�����ʱ��
		char mobile_passpod_bind;//�ֻ��ܱ�
		char ps_status;//����״̬��ֵΪPUSH_STATUSö��
		char is_bvip; //�û��Ƿ�B-VIP
		char cn_danger;		//���Ʊ���״̬
		unsigned char uVipLevel;
		unsigned int nRegDate;//�˺�ע��ʱ��
		unsigned int nLasttLgOutDate;//������һ�εǳ�ʱ��
		unsigned int user_kf_vip;
		unsigned int user_kf_msg_status;
	};

	struct Res_A2_login_2 {
		unsigned short group;//�û����߷��������
		unsigned char ipAddrLenth; //�û����߷�����IP����
		char ipAddr[IP_LENGTH_MAX];//�û����߷�����IP��ַ
		unsigned char serverNameLenth;//�û����߷��������Ƴ���
		char serverName[LOGIN_NAME_LENGTH_MAX];//�û��ڷ���������
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
		char loginName[LOGIN_NAME_LENGTH_MAX];//��¼��
		char last_login_state; //�ϴε�¼�Ƿ�ɹ�
		unsigned char ret; //���ݲ�ͬ��ret�����ز�ͬ�Ľṹ����
		UnionRes_A2 res_A2;
	};


	struct Req_A3_online {
		unsigned char loginNameLenth; //���ʺų���
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned char rolenameLenth; //��ɫ��
		char role_name[LOGIN_NAME_LENGTH_MAX + 1];
		unsigned int role_guid; //��ɫguid
		unsigned short user_class; //��ɫ�ȼ�
		unsigned short user_ml_level; //�����߽�ɫ����
		unsigned short user_ml_trlevel; //ת���ȼ�
		unsigned char user_ml_rolename_lenth; //����
		char user_ml_rolename[ROLE_NAME_LEN];	//�����߼����ɫ����
		unsigned int start_time; //��ʼʱ���
		char login_state;	//�Ƿ���Ϊ���ε�¼״̬
	};

	struct Res_A3_online_1{
		unsigned char          billing_type;   //���0,0x01:����0x02:�۵�
		unsigned int            validtime;      //ʣ��ʱ�����������1��
		int                      store_point;           //�ɹ�������Ʒ�̵�ʹ�õĵ��������0
		int                      sde_point;              //��ҵ������ʻ�ʣ����������0
		unsigned short           questionnaire_id;       //������ʾ��id�����Ϊ0,��÷���������Ҫ����
		unsigned short           act_count; //��Ҳμӻ�б���
		unsigned short           activity_list[LOGIN_NAME_LENGTH_MAX];// activity_list[act_count]
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
		unsigned char loginNameLenth; //���ʺų���
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned char ret;
		UnionRes_A3 res_A3;
	};

	struct Req_A4_logout {
		unsigned char loginNameLenth; //���ʺų���
		char loginName[LOGIN_NAME_LENGTH_MAX];
		unsigned int role_guid; //��ɫguid
		unsigned short user_class; //��ɫ�ȼ�, ����ת���ȼ�
		unsigned short user_level; //��ɫ�ȼ�, ������ת���ȼ�
		unsigned short user_trlevel;		//�����߼����ɫ��ת������
		unsigned char billType; //�Ʒѽ������ͣ�0x01 ��������	0x02 �쳣���� 0x03ǿ�ƽ���
		unsigned int start_time; //��ʼʱ��
		unsigned int end_time; //����ʱ��
	};

	struct Res_A4_logout {
		unsigned char loginNameLenth; //���ʺų���
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

