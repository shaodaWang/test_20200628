#ifndef USE_BILL_SDK_TYPE_HEADER
#define USE_BILL_SDK_TYPE_HEADER

#pragma warning (disable: 4200)


/// DVR 句柄
typedef void*	DvxHandle;
#define PROTOCOL_BUFFER_MAX (1024)

#pragma pack( push, 1 )

//包头信息

//包尾信息

/*********************************socket返回值定义*************************************************/
// 
enum
{
	/// 1.对于没有实现的方法或属性设置返回值均为 -1
	DVX_ERR_NOIMPL					= (-1),
	/// 2.函数执行成功，返回值均为 0
	DVX_OK									= (0),
	/// 3.对于小于0x10000均为dvr错误码

	/// 5.本地错误码：从0x20000开始
	DVX_ERR_BASE						= 0x00020000,

	/// 5.1一般性错误
	DVX_ERR_ASSERT					= (DVX_ERR_BASE + 0x1),		///< 出现了绝不应该出现的情况，基本上是SDK的问题
	DVX_ERR_MALLOCMEMORY		= (DVX_ERR_BASE + 0x2),		///< 分配内存出错，系统错误
	DVX_ERR_CREATE_EVENT		= (DVX_ERR_BASE + 0x3),		///< 创建Event句柄出错，系统错误
	DVX_ERR_CREATE_THREAD		= (DVX_ERR_BASE + 0x4),		///< 创建线程出错，系统错误
	DVX_ERR_PARA						= (DVX_ERR_BASE + 0x5),		///< 参数错误
	DVX_ERR_SDK_NO_INIT			= (DVX_ERR_BASE + 0x6),		///< SDK没有初始化
	DVX_ERR_DVX_NO_LOGIN		= (DVX_ERR_BASE + 0x7),		///< 还没有登录
	DVX_ERR_DVX_INVALID_HANDLE	= (DVX_ERR_BASE + 0x8),	///< 无效的DvxHandle句柄
	DVX_ERR_COMMON					= (DVX_ERR_BASE + 0x9),		///< 通用错误

	/// 5.2网络错误
	DVX_ERR_ERR_TRANS_OPEN	= (DVX_ERR_BASE + 0x21),  ///<打开传输流错误
	DVX_ERR_NET_DISCONNECT	= (DVX_ERR_BASE + 0x22),  ///<网络断开
	DVX_ERR_HELLO           = (DVX_ERR_BASE + 0x23),  ///<网络断开

	/// 5.3报警错误
	DVX_ERR_ALARM_DISABLE		= (DVX_ERR_BASE + 0x101),	///< 报警功能没有打开
	DVX_ERR_ENABLE_ALARM		= (DVX_ERR_BASE + 0x102),	///< 启动报警功能时出错
	DVX_ERR_NOMORE_ALARM    = (DVX_ERR_BASE + 0x103),	///< 没有更多的报警信息了

	/// 5.4现场错误
	DVX_ERR_REAL_INVALID_HANDLE	= (DVX_ERR_BASE + 0x201),	///< 无效的现场句柄

	/// 5.5对讲错误
	DVX_ERR_SPEECH_INVALID_HANDLE = (DVX_ERR_BASE + 0x301),	///< 无效的对讲句柄

	/// 5.6 本地回放错误
	DVX_ERR_LOCALPLAY_INVALID_HANDLE = (DVX_ERR_BASE + 0x401),	///< 无效的本地回放句柄
	DVX_ERR_LOCALPLAY_INVALID_FNAME = (DVX_ERR_BASE + 0x402),	  ///< 无效的本地回放句柄
	DVX_ERR_LOCALPLAY_NO_OPEN = (DVX_ERR_BASE + 0x403),	        ///< 本地回放，还没有打开

	/// 5.7 
	DVX_ERR_CREATE_FILE			= (DVX_ERR_BASE + 0x501), ///<创建文件失败
	DVX_ERR_NET							= (DVX_ERR_BASE + 0x901), ///<网络错误
	DVX_BSCP_BUSY						= (DVX_ERR_BASE + 0x902), ///<命令通道忙
	DVX_BSCP_TIMEOUT				= (DVX_ERR_BASE + 0x903), ///<bscp命令执行超时
	DVX_BSCP_NETERR					= (DVX_ERR_BASE + 0x904), ///<bscp网络错误
	DVX_ERR_INIT						= (DVX_ERR_BASE + 0x905), ///<初始化错误
	DVX_ERR_NET_INIT				= (DVX_ERR_BASE + 0x906), ///<网络初始化错误
	///5.8
	DVX_ERR_NO_CMD                 = (DVX_ERR_BASE + 0x1000), ///不识别的命令
};
////////////////////////////////////////////////////////////////////////////////////////

//当前协议命令号
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
 /////////////////////////// 命令头//////////////////////////////////////////////////////////////
struct BscpCmdHeader
{
	unsigned char mark[2]; /* 前导标示，0xaa 0x55 */
	unsigned short length; /* 包体长度，整个长度去掉0xaa55和0x55aa,还有length本身的长度，也就是：整体长度-6 */
	unsigned int packetid; /*协议号 和天龙的主要区别就在协议号的类型上*/
	unsigned short packetIndex;   //包序号,这个协议里应该是数据内容，但基本上每个都有，直接放在数据头里吧 
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
	unsigned char mark[2]; /* 前导标示，0xaa 0x55 */
	unsigned short length; /* 包体长度，整个长度去掉0xaa55和0x55aa,还有length本身的长度，也就是：整体长度-6 */
	unsigned char packetid; /*协议号 和ptv5的主要区别就在协议号的类型上*/
	unsigned short packetIndex;   //包序号,这个协议里应该是数据内容，但基本上每个都有，直接放在数据头里吧 
	BscpCmdHeader_TLBB() {
		mark[0] = 0xaa;
		mark[1] = 0x55;
		length = 0;
		packetid = 0;
		packetIndex = 0;
	}
};
/////////////////////////// 命令尾//////////////////////////////////////////////////////////////
struct BscpCmdTail
{
	unsigned char mark[2]; /**< 前导标示，0x55 0xaa */
	BscpCmdTail() {
		mark[1] = 0xaa;
		mark[0] = 0x55;
	}
};

///////////////////////////////hello-A0/////////////////////////////////////////////////////////////////
struct Req_A0_hello
{
	unsigned short serverZoneid; //游戏服务器id
	unsigned short serverGrpid;  //游戏服务组id
	Req_A0_hello(){
		serverZoneid=0;
		serverGrpid=htons(1000);
	}
};

struct Req_A0_hello_TLBB
{
	unsigned short serverZoneid; //游戏服务器id
	unsigned short serverGrpid;  //游戏服务组id
	unsigned char ipv4_lenth;
	unsigned char ipv4_local[16];
	Req_A0_hello_TLBB() {
		serverZoneid = 0;
		serverGrpid = htons(1000);
		memcpy(ipv4_local, LOCAL_IP, strlen(LOCAL_IP));
		ipv4_lenth = strlen(LOCAL_IP);
	}
};

struct Req_A0_hello_DAOJIAN   //刀剑协议pdf的不对，这是按照billing实际接收为准
{
	unsigned short serverGrpid;  //游戏服务组id	
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
	unsigned short reserved;//目前未使用
	Res_A0_hello(){
		reserved=0;
	}
};

///////////////////////////////heart-A1/////////////////////////////////////////////////////////////////
struct Req_A1_heart
{
	unsigned short serverZoneid; //游戏服务器id
	unsigned short serverGrpid;  //游戏服务组id
	unsigned short serverOnlineNum;  //游戏服务器在线人数
	Req_A1_heart(){
		serverZoneid= htons(0);
		serverGrpid=htons(1000);
		serverOnlineNum=htons(5);
	}
};

struct Req_A1_heart_TLBB
{
	unsigned short serverZoneid; //游戏服务器id
	unsigned short serverGrpid;  //游戏服务组id
	unsigned short serverOnlineNum;  //游戏服务器在线人数	
	unsigned short connectcount; //过去30秒连接login服务器的请求数
	unsigned short entergamecount;  //过去30秒进入游戏玩家数
	unsigned short leavegamecount;  //过去30秒离开游戏玩家数
	unsigned short queuecount; //排队人数
	unsigned short sendQueueCount;  //二级排队人数
	unsigned short stolencount;  //疑似被盗人数
	unsigned short tmpcnt1; //普通客户端在线人数
	unsigned short tmpcnt2;  //唯美客户端在线人数
	unsigned short tmpcnt3;  //临时人数
	unsigned short tmpcnt4; //临时人数
	unsigned short tmpcnt5;  //临时人数
	unsigned short tmpcnt6;  //临时人数
	unsigned short tmpcnt7; //临时人数
	unsigned short tmpcnt8;  //临时人数
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
	unsigned short serverGrpid;  //游戏服务组id
	unsigned short serverOnlineNum;  //游戏服务器在线人数
	Req_A1_heart_DAOJIAN() {
		serverGrpid = htons(1000);
		serverOnlineNum = htons(5);
	}
};

struct Res_A1_heart
{
	unsigned short reserved;//目前未使用
	Res_A1_heart(){
		reserved=0;
	}
};


/////////////////////////////其他////////////////////////////////////////////////////
const int LOGIN_NAME_LENGTH_MAX=80;
const int LOGIN_PASSWD_LENGTH_MAX=32;
const int IP_LENGTH_MAX=16;
const int  CY_MAC_LEN = 32;
const int MAC_MD5 =32;
const int CY_MIBAO_POS_LEN = 2;
const int   CY_PASSPOD_PASS_LEN = 8; //硬件密码密码
const int   CY_ZZB_PASS_LEN = 6;   //掌中宝密码
const int   CY_PHONE_PASS_LEN = 8; //手机密保密码
const int  CY_MAX_PC_VAL_LEN = 35;
const int CY_STOREMSG_LEN = 32;
const int ROLE_NAME_LEN = 21; //角色名称
const int MAX_TWO_DIMEN_CODE_LEN = 254;

struct stcCreate
{
	unsigned int ipv4;
	unsigned short nport;
	unsigned char nGametype;
	bool bNeedHeart;
};
//下面是暂时不用的
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