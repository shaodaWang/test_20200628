// BillingTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BillingTest.h"
#include "BillingTestDlg.h"
#include "./common/BillingType.h"
#include "GameTLBB.h"

#pragma comment(lib, "Billing_sdk.lib")

using namespace BillingSdk;
using namespace space_tlbb;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBillingTestDlg 对话框




CBillingTestDlg::CBillingTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBillingTestDlg::IDD, pParent)
	, m_nPortBilling(0)
	, m_nLastGametype(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDvr = NULL;
	memset(m_ip, 0, sizeof(m_ip));
	m_nPort = 0;
	m_pGamebase = NULL;
  m_nStress_CMD = 0;
}

void CBillingTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GAMETYPE, m_cmb_gametype);
	DDX_Control(pDX, IDC_IP_BILLING, m_ip_bill);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPortBilling);
	DDX_Control(pDX, IDC_EDIT_PORT, m_editPort);
	DDX_Control(pDX, IDC_CMB_SOCKET, m_cmbSocket);
	DDX_Control(pDX, IDC_LIST_INFO, m_listInfo);
	DDX_Control(pDX, IDC_CHECK_HEART, m_chk_heart);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_version);
}

BEGIN_MESSAGE_MAP(CBillingTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CBillingTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBillingTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_A0, &CBillingTestDlg::OnBnClickedBtnA0)
	ON_BN_CLICKED(IDC_BTN_A1, &CBillingTestDlg::OnBnClickedBtnA1)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_GAMETYPE, &CBillingTestDlg::OnCbnSelchangeComboGametype)
	//ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP_BILLING, &CBillingTestDlg::OnIpnFieldchangedIpBilling)
	//ON_EN_CHANGE(IDC_EDIT_PORT, &CBillingTestDlg::OnEnChangeEditPort)
	ON_EN_CHANGE(IDC_EDIT_PORT, &CBillingTestDlg::OnEnChangeEditPort)
	ON_CBN_SELCHANGE(IDC_CMB_SOCKET, &CBillingTestDlg::OnCbnSelchangeCmbSocket)
	ON_LBN_SELCHANGE(IDC_LIST_INFO, &CBillingTestDlg::OnLbnSelchangeListInfo)
	ON_BN_CLICKED(IDC_BTN_LOGIN_A2, &CBillingTestDlg::OnBnClickedBtnLoginA2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_LOGIN_A3, &CBillingTestDlg::OnBnClickedBtnLoginA3)
	ON_BN_CLICKED(IDC_BTN_LOGIN_A4, &CBillingTestDlg::OnBnClickedBtnLoginA4)
	ON_BN_CLICKED(IDC_CHECK_HEART, &CBillingTestDlg::OnBnClickedCheckHeart)
  ON_BN_CLICKED(IDC_BTN_0X01, &CBillingTestDlg::OnBnClickedBtn0x01)
  ON_BN_CLICKED(IDC_BTN_LOGIN_A8, &CBillingTestDlg::OnBnClickedBtnLoginA8)
  ON_BN_CLICKED(IDC_BTN_A8_STRESS, &CBillingTestDlg::OnBnClickedBtnA8Stress)
END_MESSAGE_MAP()


// CBillingTestDlg 消息处理程序

BOOL CBillingTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	dvxSdkInit();
	initCtrl();
	SelectGame();
	ShowWindow(SW_SHOW);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBillingTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBillingTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBillingTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CBillingTestDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnCancel();	
	return;
}

void CBillingTestDlg::OnBnClickedBtnA0()
{
	// TODO: 在此添加控件通知处理程序代码
	BillingCreate create;
	memcpy(create.ip_ptv4, m_ip, strlen(m_ip));
	create.nport = m_nPort;
	create.nGametype = m_cmb_gametype.GetCurSel();
	create.bNeedHeart = m_chk_heart.GetCheck();//是否需要中间加心跳
	int nRet=dvxCreateWithOutLogin(&create,&m_pDvr,1000);
	if (!nRet && m_pGamebase && m_pDvr)	{
		m_pGamebase->SetDvxHandle(m_pDvr);
	}
	CString strinfo;
	strinfo.Format(_T("%s,%s,nRet:%d"), L"0xA0",nRet?L"error":L"success",nRet);
	showinfo(strinfo);
	return;
}

void CBillingTestDlg::OnBnClickedBtnA1()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRet = -1;
	int ntype = m_cmb_gametype.GetCurSel();
	if (ntype==GAME_TLBB|| ntype==GAME_TLBB_GREEN)
	{
		Req_A1_heart_TLBB req_heart;
		Res_A1_heart res_heart;
		if (m_pDvr) {
			nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_HEART, &req_heart, sizeof(req_heart), &res_heart, sizeof(res_heart));
		}
	}
	else if (ntype==GAME_MOJIAN)
	{
		Req_A1_heart req_heart;
		Res_A1_heart res_heart;
		if (m_pDvr) {
			nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_HEART, &req_heart, sizeof(req_heart), &res_heart, sizeof(res_heart));
		}
	}
	else if (ntype==GAME_DAOJIAN)
	{
		Req_A1_heart_DAOJIAN req_heart;
		Res_A1_heart res_heart;
		if (m_pDvr) {
			nRet = dvxOperProtocol(m_pDvr, BSCP_SUNFLOWER_CMD_HEART, &req_heart, sizeof(req_heart), &res_heart, sizeof(res_heart));
		}
	}
	
	CString strinfo;
	strinfo.Format(_T("%s,%s,nRet:%d"), L"0xA1", nRet ? L"error" : L"success", nRet);
	showinfo(strinfo);
	return;
}

void CBillingTestDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pGamebase) {
		delete m_pGamebase;
		m_pGamebase = NULL;
	}
	if (m_pDvr) {
		dvxDestory(m_pDvr);
		m_pDvr = NULL;
	}
	dvxSdkDeInit();
}


void CBillingTestDlg::initCtrl()
{
	//游戏类型	
	m_cmb_gametype.AddString(_T("经典天龙"));
	m_cmb_gametype.AddString(_T("绿色天龙"));
	m_cmb_gametype.AddString(_T("刀剑"));
	m_cmb_gametype.AddString(_T("海战"));
	m_cmb_gametype.AddString(_T("魔剑"));	
	//m_cmb_gametype.AddString(_T("水浒Q传"));
	//m_cmb_gametype.AddString(_T("九鼎"));
	//m_cmb_gametype.AddString(_T("鹿鼎"));
	int nGametype = CYGetPrivateProfileInt(DEFAULT_GAMETYPE, GAMETYPE, 0, MAIN_INI);
	m_cmb_gametype.SetCurSel(nGametype);
	//ip+port
	changeGametype(nGametype);
	//tcp
	m_cmbSocket.AddString(_T("TCP"));
	m_cmbSocket.SetCurSel(0);
	//
	m_chk_heart.SetCheck(0);
	//
	CString strV(BIILING_TEST_VERSION);
	m_version.SetWindowText(strV);
	UpdateData(FALSE);
}

void CBillingTestDlg::OnCbnSelchangeComboGametype()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nGametype = m_cmb_gametype.GetCurSel();
	changeGametype(nGametype);
	SelectGame();
	UpdateData(FALSE);
}

int CBillingTestDlg::changeGametype(int nType)
{
	int nRet = -1;
	do
	{
		memset(m_ip, 0,sizeof(m_ip));
		nRet=CYGetPrivateProfileString(GAMENAME[nType], BILLING_IP, "", m_ip, 16, MAIN_INI);
		if (!nRet){
			break;
		}
		//unsigned long nIp = inet_addr(m_ip);
		CString strip(m_ip);
		m_ip_bill.SetWindowText(strip);
		m_nPort = CYGetPrivateProfileInt(GAMENAME[nType], BILLING_PORT, 0, MAIN_INI);
		m_nPortBilling = m_nPort;
		nRet = 0;
	} while (0);
	return nRet;
}


void CBillingTestDlg::OnEnChangeEditPort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CBillingTestDlg::OnCbnSelchangeCmbSocket()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CBillingTestDlg::OnLbnSelchangeListInfo()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CBillingTestDlg::showinfo(CString info, BOOL bshowtime)
{
	CString strShowInfo;
	if (bshowtime)	{
		CTime t = CTime::GetCurrentTime();
		strShowInfo = t.Format(_T("[%Y-%m-%d %H-%M-%S] "));
	}
	strShowInfo += info;
  OutputDebugStringW(strShowInfo);
  OutputDebugStringA("\n");
	//m_listInfo.AddString(strShowInfo);
	//m_listInfo.SetCurSel(m_listInfo.GetCount()-1);
}

int CBillingTestDlg::SelectGame()
{
	int nGame = m_cmb_gametype.GetCurSel();
	int nRet = 0;
	if (m_nLastGametype== nGame){
		return nRet;
	}else {
		if (m_pGamebase)	{
			delete m_pGamebase;
			m_pGamebase = NULL;
		}
	}
	switch (nGame)	{
	case GAME_TLBB:
	case GAME_TLBB_GREEN:
		//绿色天龙和经典天龙暂时没有什么区别，先共用同一个对象。但ini有区分
		m_pGamebase = new GameTLBB(nGame);
	break;
	case GAME_DAOJIAN:
		m_pGamebase = new GameMoJian(nGame);
		break;
	case GAME_MOJIAN:
		m_pGamebase = new GameMoJian(nGame);
	break;
	default:
		break;
	}
	if (!m_pGamebase) {
		nRet = -1;
	}
	return nRet;
}

void CBillingTestDlg::OnBnClickedBtnLoginA2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString info;
	int nRet = m_pGamebase->sendA2(info);
	CString strinfo;
	strinfo.Format(_T("%s,nRet:%d,info:%s"), L"0xA2", nRet, info);
	showinfo(strinfo);
	return;
}


void CBillingTestDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnOK();
	CDialog::OnClose();
}


void CBillingTestDlg::OnBnClickedBtnLoginA3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString info;
	int nRet = m_pGamebase->sendA3(info);
	CString strinfo;
	strinfo.Format(_T("%s,nRet:%d,info:%s"), L"0xA3", nRet, info);
	showinfo(strinfo);
}


void CBillingTestDlg::OnBnClickedBtnLoginA4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString info;
	int nRet = m_pGamebase->sendA4(info);
	CString strinfo;
	strinfo.Format(_T("%s,nRet:%d,info:%s"), L"0xA4", nRet, info);
	showinfo(strinfo);
}


void CBillingTestDlg::OnBnClickedCheckHeart()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BTN_A1)->EnableWindow(!m_chk_heart.GetCheck());
}


unsigned int __stdcall ThreadFun(PVOID pM) {
  CBillingTestDlg *pDlg = (CBillingTestDlg *)pM;
  unsigned int nid= GetCurrentThreadId();
  pDlg->TestCurl(nid);
  return 0;
}

void CBillingTestDlg::OnBnClickedBtn0x01()
{
  // TODO: 在此添加控件通知处理程序代码
  //CString info;
  //int nRet = m_pGamebase->send01(info);
  //CString strinfo;
  //strinfo.Format(_T("%s,nRet:%d,info:%s"), L"0xA3", nRet, info);
  //showinfo(strinfo);
  const int THREAD_NUM = 20;
  HANDLE hand[THREAD_NUM] = {0,};
  for (int i = 0; i < THREAD_NUM; i++)
  {
    hand[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, this, 0, NULL);
  }
  WaitForMultipleObjects(THREAD_NUM, hand, TRUE, INFINITE);
  //TestCurl();
}

void CBillingTestDlg::TestCurl(int nthreadid)
{

  int n=10000;
  BillingCreate create;
  memcpy(create.ip_ptv4, m_ip, strlen(m_ip));
  create.nport = m_nPort;
  create.nGametype = 1;
  create.bNeedHeart = false;//是否需要中间加心跳
  DvxHandle hDvr = NULL;
  char chCN[128] = { 0, };
  sprintf_s(chCN, "test_%d", nthreadid);
  int nRet = dvxCreateWithOutLogin(&create, &hDvr, 1000);
  if (nRet || hDvr==NULL) {
    OutputDebugStringA("error");
    OutputDebugStringA("\n");
    return ;
  }

  do
  {
    CString info;
    Req_01_Test req;
    Res_01_Test res;
    memset(&req,0,sizeof(req));
    memset(&res, 0, sizeof(res));
    
    memcpy(req.cnMaster, chCN, strlen(chCN));
    req.cnMasterLenth = strlen(req.cnMaster);
    if (hDvr) {
      nRet = dvxOperProtocol(hDvr, BSCP_SUNFLOWER_CMD_TEST_CURL, &req, req.cnMasterLenth+1, &res, sizeof(res));
    }
    CString strinfo;
    strinfo.Format(_T("%s, nRet:%d, threadid(%d), info:%s\n"), L"0x01", nRet, nthreadid, info);
    
    showinfo(strinfo);
    n--;
  } while (n);
  if (hDvr) {
    dvxDestory(hDvr);
    hDvr = NULL;
  }
}


void CBillingTestDlg::OnBnClickedBtnLoginA8()
{
  // TODO: 在此添加控件通知处理程序代码
  CString info;
  int nRet = m_pGamebase->sendA8(info);
  CString strinfo;
  strinfo.Format(_T("%s,nRet:%d,info:%s"), L"0xA8", nRet, info);
  showinfo(strinfo);
  return;
}

unsigned int __stdcall ThreadFunStress(PVOID pM) {
  CBillingTestDlg *pDlg = (CBillingTestDlg *)pM;
  unsigned int nid = GetCurrentThreadId();
  int nGame = 1;//pDlg->m_cmb_gametype.GetCurSel();
  pDlg->StressTestCurl(nid, pDlg->m_nStress_CMD, nGame);
  return 0;
}

void CBillingTestDlg::StressTestCurl(int nthreadid,int nCMD, int Game)
{

  int n = 100000;
  BillingCreate create;
  memcpy(create.ip_ptv4, m_ip, strlen(m_ip));
  create.nport = m_nPort;
  create.nGametype = 1;
  create.bNeedHeart = false;//是否需要中间加心跳
  DvxHandle hDvr = NULL;
  char chCN[128] = { 0, };
  sprintf_s(chCN, "test_%d", nthreadid);
  int nRet = dvxCreateWithOutLogin(&create, &hDvr, 1000);
  if (nRet || hDvr == NULL) {
    OutputDebugStringA("error");
    OutputDebugStringA("\n");
    return;
  }
  int nA2 = 0;
  int nA3 = 0;
  int nA4 = 0;
  int nA8 = 0;
  int nA4_2 = 0;

  GameTLBB tlbb(Game);
  tlbb.SetDvxHandle(hDvr);
  do
  {
    CString info;
    //nA2 = tlbb.sendA2(info);
    //nA3 = tlbb.sendA3(info);
    //nA4 = tlbb.sendA4(info);
    nA8 = tlbb.sendA8(info);
    //nA4_2 = tlbb.sendA4(info);
    CString strinfo;
    strinfo.Format(_T("nA2:%d,nA3:%d,nA4:%d,nA8:%d,nA4_2:%d,threadid(%d), info:%s\n"), 
      nA2, nA3, nA4, nA8, nA4_2, nthreadid, info);

    showinfo(strinfo);
    n--;
  } while (n);
  if (hDvr) {
    dvxDestory(hDvr);
    hDvr = NULL;
  }
}

void CBillingTestDlg::OnBnClickedBtnA8Stress()
{
  // TODO: 在此添加控件通知处理程序代码
  const int THREAD_NUM = 100;
  m_nStress_CMD = BSCP_SUNFLOWER_CMD_LOGIN_BY_TWO;
  HANDLE hand[THREAD_NUM] = { 0, };
  for (int i = 0; i < THREAD_NUM; i++)
  {
    hand[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunStress, this, 0, NULL);
  }
  WaitForMultipleObjects(THREAD_NUM, hand, TRUE, INFINITE);
}
