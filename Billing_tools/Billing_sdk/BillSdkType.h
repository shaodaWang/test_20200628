#ifndef USE_BILL_SDK_TYPE_HEADER
#define USE_BILL_SDK_TYPE_HEADER

#pragma warning (disable: 4200)


/// DVR ���
typedef void*	DvxHandle;
#define PROTOCOL_BUFFER_MAX (1024)

#pragma pack( push, 1 )

//��ͷ��Ϣ

//��β��Ϣ

/*********************************socket����ֵ����*************************************************/
// 
enum
{
	/// 1.����û��ʵ�ֵķ������������÷���ֵ��Ϊ -1
	DVX_ERR_NOIMPL					= (-1),
	/// 2.����ִ�гɹ�������ֵ��Ϊ 0
	DVX_OK									= (0),
	/// 3.����С��0x10000��Ϊdvr������

	/// 5.���ش����룺��0x20000��ʼ
	DVX_ERR_BASE						= 0x00020000,

	/// 5.1һ���Դ���
	DVX_ERR_ASSERT					= (DVX_ERR_BASE + 0x1),		///< �����˾���Ӧ�ó��ֵ��������������SDK������
	DVX_ERR_MALLOCMEMORY		= (DVX_ERR_BASE + 0x2),		///< �����ڴ����ϵͳ����
	DVX_ERR_CREATE_EVENT		= (DVX_ERR_BASE + 0x3),		///< ����Event�������ϵͳ����
	DVX_ERR_CREATE_THREAD		= (DVX_ERR_BASE + 0x4),		///< �����̳߳���ϵͳ����
	DVX_ERR_PARA						= (DVX_ERR_BASE + 0x5),		///< ��������
	DVX_ERR_SDK_NO_INIT			= (DVX_ERR_BASE + 0x6),		///< SDKû�г�ʼ��
	DVX_ERR_DVX_NO_LOGIN		= (DVX_ERR_BASE + 0x7),		///< ��û�е�¼
	DVX_ERR_DVX_INVALID_HANDLE	= (DVX_ERR_BASE + 0x8),	///< ��Ч��DvxHandle���
	DVX_ERR_COMMON					= (DVX_ERR_BASE + 0x9),		///< ͨ�ô���

	/// 5.2�������
	DVX_ERR_ERR_TRANS_OPEN	= (DVX_ERR_BASE + 0x21),  ///<�򿪴���������
	DVX_ERR_NET_DISCONNECT	= (DVX_ERR_BASE + 0x22),  ///<����Ͽ�
	DVX_ERR_HELLO           = (DVX_ERR_BASE + 0x23),  ///<����Ͽ�

	/// 5.3��������
	DVX_ERR_ALARM_DISABLE		= (DVX_ERR_BASE + 0x101),	///< ��������û�д�
	DVX_ERR_ENABLE_ALARM		= (DVX_ERR_BASE + 0x102),	///< ������������ʱ����
	DVX_ERR_NOMORE_ALARM    = (DVX_ERR_BASE + 0x103),	///< û�и���ı�����Ϣ��

	/// 5.4�ֳ�����
	DVX_ERR_REAL_INVALID_HANDLE	= (DVX_ERR_BASE + 0x201),	///< ��Ч���ֳ����

	/// 5.5�Խ�����
	DVX_ERR_SPEECH_INVALID_HANDLE = (DVX_ERR_BASE + 0x301),	///< ��Ч�ĶԽ����

	/// 5.6 ���ػطŴ���
	DVX_ERR_LOCALPLAY_INVALID_HANDLE = (DVX_ERR_BASE + 0x401),	///< ��Ч�ı��ػطž��
	DVX_ERR_LOCALPLAY_INVALID_FNAME = (DVX_ERR_BASE + 0x402),	  ///< ��Ч�ı��ػطž��
	DVX_ERR_LOCALPLAY_NO_OPEN = (DVX_ERR_BASE + 0x403),	        ///< ���ػطţ���û�д�

	/// 5.7 
	DVX_ERR_CREATE_FILE			= (DVX_ERR_BASE + 0x501), ///<�����ļ�ʧ��
	DVX_ERR_NET							= (DVX_ERR_BASE + 0x901), ///<�������
	DVX_BSCP_BUSY						= (DVX_ERR_BASE + 0x902), ///<����ͨ��æ
	DVX_BSCP_TIMEOUT				= (DVX_ERR_BASE + 0x903), ///<bscp����ִ�г�ʱ
	DVX_BSCP_NETERR					= (DVX_ERR_BASE + 0x904), ///<bscp�������
	DVX_ERR_INIT						= (DVX_ERR_BASE + 0x905), ///<��ʼ������
	DVX_ERR_NET_INIT				= (DVX_ERR_BASE + 0x906), ///<�����ʼ������
	///5.8
	DVX_ERR_NO_CMD                 = (DVX_ERR_BASE + 0x1000), ///��ʶ�������
};
////////////////////////////////////////////////////////////////////////////////////////

//��ǰЭ�������
typedef enum{
	BSCP_SUNFLOWER_CMD_HELLO=0xA0,
	BSCP_SUNFLOWER_CMD_HEART=0xA1,
	BSCP_SUNFLOWER_CMD_LOGIN=0xA2,
	BSCP_SUNFLOWER_CMD_ONLINE=0xA3,
	BSCP_SUNFLOWER_CMD_LOGOUT=0xA4,
  BSCP_SUNFLOWER_CMD_LOGIN_BY_TWO = 0xA8,
  BSCP_SUNFLOWER_CMD_TEST_CURL = 0x01
} enumCMD;

typedef enum {
	GAME_TLBB=0,
	GAME_TLBB_GREEN,
	GAME_DAOJIAN,
	GAME_HZSJ,
	GAME_MOJIAN
} enumGAME;

#define LOCAL_IP ("10.13.25.12")
 /////////////////////////// ����ͷ//////////////////////////////////////////////////////////////
struct BscpCmdHeader
{
	unsigned char mark[2]; /* ǰ����ʾ��0xaa 0x55 */
	unsigned short length; /* ���峤�ȣ���������ȥ��0xaa55��0x55aa,����length����ĳ��ȣ�Ҳ���ǣ����峤��-6 */
	unsigned int packetid; /*Э��� ����������Ҫ�������Э��ŵ�������*/
	unsigned short packetIndex;   //�����,���Э����Ӧ�����������ݣ���������ÿ�����У�ֱ�ӷ�������ͷ��� 
	BscpCmdHeader() {
		mark[0] = 0xaa;
		mark[1] = 0x55;
		length = 0;
		packetid = 0;
		packetIndex = 0;
	}
};

struct BscpCmdHeader_TLBB
{
	unsigned char mark[2]; /* ǰ����ʾ��0xaa 0x55 */
	unsigned short length; /* ���峤�ȣ���������ȥ��0xaa55��0x55aa,����length����ĳ��ȣ�Ҳ���ǣ����峤��-6 */
	unsigned char packetid; /*Э��� ��ptv5����Ҫ�������Э��ŵ�������*/
	unsigned short packetIndex;   //�����,���Э����Ӧ�����������ݣ���������ÿ�����У�ֱ�ӷ�������ͷ��� 
	BscpCmdHeader_TLBB() {
		mark[0] = 0xaa;
		mark[1] = 0x55;
		length = 0;
		packetid = 0;
		packetIndex = 0;
	}
};
/////////////////////////// ����β//////////////////////////////////////////////////////////////
struct BscpCmdTail
{
	unsigned char mark[2]; /**< ǰ����ʾ��0x55 0xaa */
	BscpCmdTail() {
		mark[1] = 0xaa;
		mark[0] = 0x55;
	}
};

///////////////////////////////hello-A0/////////////////////////////////////////////////////////////////
struct Req_A0_hello
{
	unsigned short serverZoneid; //��Ϸ������id
	unsigned short serverGrpid;  //��Ϸ������id
	Req_A0_hello(){
		serverZoneid=0;
		serverGrpid=htons(1000);
	}
};

struct Req_A0_hello_TLBB
{
	unsigned short serverZoneid; //��Ϸ������id
	unsigned short serverGrpid;  //��Ϸ������id
	unsigned char ipv4_lenth;
	unsigned char ipv4_local[16];
	Req_A0_hello_TLBB() {
		serverZoneid = 0;
		serverGrpid = htons(1000);
		memcpy(ipv4_local, LOCAL_IP, strlen(LOCAL_IP));
		ipv4_lenth = strlen(LOCAL_IP);
	}
};

struct Req_A0_hello_DAOJIAN   //����Э��pdf�Ĳ��ԣ����ǰ���billingʵ�ʽ���Ϊ׼
{
	unsigned short serverGrpid;  //��Ϸ������id	
	unsigned char ipv4_lenth;
	unsigned char ipv4_local[16];
	Req_A0_hello_DAOJIAN() {
		serverGrpid = htons(1000);
		memcpy(ipv4_local, LOCAL_IP, strlen(LOCAL_IP));
		ipv4_lenth = strlen(LOCAL_IP);
	}
};

struct Res_A0_hello
{
	unsigned short reserved;//Ŀǰδʹ��
	Res_A0_hello(){
		reserved=0;
	}
};

///////////////////////////////heart-A1/////////////////////////////////////////////////////////////////
struct Req_A1_heart
{
	unsigned short serverZoneid; //��Ϸ������id
	unsigned short serverGrpid;  //��Ϸ������id
	unsigned short serverOnlineNum;  //��Ϸ��������������
	Req_A1_heart(){
		serverZoneid= htons(0);
		serverGrpid=htons(1000);
		serverOnlineNum=htons(5);
	}
};

struct Req_A1_heart_TLBB
{
	unsigned short serverZoneid; //��Ϸ������id
	unsigned short serverGrpid;  //��Ϸ������id
	unsigned short serverOnlineNum;  //��Ϸ��������������	
	unsigned short connectcount; //��ȥ30������login��������������
	unsigned short entergamecount;  //��ȥ30�������Ϸ�����
	unsigned short leavegamecount;  //��ȥ30���뿪��Ϸ�����
	unsigned short queuecount; //�Ŷ�����
	unsigned short sendQueueCount;  //�����Ŷ�����
	unsigned short stolencount;  //���Ʊ�������
	unsigned short tmpcnt1; //��ͨ�ͻ�����������
	unsigned short tmpcnt2;  //Ψ���ͻ�����������
	unsigned short tmpcnt3;  //��ʱ����
	unsigned short tmpcnt4; //��ʱ����
	unsigned short tmpcnt5;  //��ʱ����
	unsigned short tmpcnt6;  //��ʱ����
	unsigned short tmpcnt7; //��ʱ����
	unsigned short tmpcnt8;  //��ʱ����
	Req_A1_heart_TLBB() {
		serverZoneid = htons(0);
		serverGrpid = htons(1000);
		serverOnlineNum = htons(3000);
		connectcount = htons(4);
		entergamecount = htons(5);
		leavegamecount = htons(6);
		queuecount = htons(7);
		sendQueueCount = htons(8);
		stolencount = htons(9);
		tmpcnt1 = htons(100);
		tmpcnt2 = htons(200);
		tmpcnt3 = htons(300);
		tmpcnt4 = htons(400);
		tmpcnt5 = htons(500);
		tmpcnt6 = htons(600);
		tmpcnt7 = htons(700);
		tmpcnt8 = htons(800);
	}
};

struct Req_A1_heart_DAOJIAN
{
	unsigned short serverGrpid;  //��Ϸ������id
	unsigned short serverOnlineNum;  //��Ϸ��������������
	Req_A1_heart_DAOJIAN() {
		serverGrpid = htons(1000);
		serverOnlineNum = htons(5);
	}
};

struct Res_A1_heart
{
	unsigned short reserved;//Ŀǰδʹ��
	Res_A1_heart(){
		reserved=0;
	}
};


/////////////////////////////����////////////////////////////////////////////////////
const int LOGIN_NAME_LENGTH_MAX=80;
const int LOGIN_PASSWD_LENGTH_MAX=32;
const int IP_LENGTH_MAX=16;
const int  CY_MAC_LEN = 32;
const int MAC_MD5 =32;
const int CY_MIBAO_POS_LEN = 2;
const int   CY_PASSPOD_PASS_LEN = 8; //Ӳ����������
const int   CY_ZZB_PASS_LEN = 6;   //���б�����
const int   CY_PHONE_PASS_LEN = 8; //�ֻ��ܱ�����
const int  CY_MAX_PC_VAL_LEN = 35;
const int CY_STOREMSG_LEN = 32;
const int ROLE_NAME_LEN = 21; //��ɫ����
const int MAX_TWO_DIMEN_CODE_LEN = 254;

struct stcCreate
{
	unsigned int ipv4;
	unsigned short nport;
	unsigned char nGametype;
	bool bNeedHeart;
};
//��������ʱ���õ�
struct LoginOptsIn_Sunflower
{
	char	user[16];
	char	password[32];
	unsigned int actor;
};
struct LoginOptsOut_Sunflower
{
	unsigned int sessionID;
	char role[16];
};

#pragma pack( pop )
#endif