// BillingTestDlg.h : ͷ�ļ�
//

#pragma once
#include "../Billing_sdk/Billing_sdk.h"
#include "./common/CYRdWrIni.h"
#include "./GameBase.h"
#include "./GameTLBB.h"
#include "./GameMoJian.h"

// CBillingTestDlg �Ի���
class CBillingTestDlg : public CDialog
{
// ����
public:
	CBillingTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BILLINGTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnA0();
	afx_msg void OnBnClickedBtnA1();
	afx_msg void OnDestroy();

	DvxHandle m_pDvr;
	CComboBox m_cmb_gametype;
	void initCtrl();
public:
	afx_msg void OnCbnSelchangeComboGametype();
	CIPAddressCtrl m_ip_bill;
	// afx_msg void OnIpnFieldchangedIpBilling(NMHDR *pNMHDR, LRESULT *pResult);	// afx_msg void OnIpnFieldchangedIpBilling(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nPortBilling;
	int changeGametype(int nType);
	char m_ip[16];
	int m_nPort;
	// afx_msg void OnEnChangeEditPort();	// afx_msg void OnEnChangeEditPort();
	CEdit m_editPort;
	afx_msg void OnEnChangeEditPort();
	CComboBox m_cmbSocket;
	afx_msg void OnCbnSelchangeCmbSocket();
	CListBox m_listInfo;
	afx_msg void OnLbnSelchangeListInfo();
	void showinfo(CString info, BOOL bshowtime=true);

	GameBase* m_pGamebase;
	int SelectGame();
	int m_nLastGametype;
	afx_msg void OnBnClickedBtnLoginA2();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnLoginA3();
	afx_msg void OnBnClickedBtnLoginA4();
	CButton m_chk_heart;
	afx_msg void OnBnClickedCheckHeart();
	CStatic m_version;
  afx_msg void OnBnClickedBtn0x01();
  void TestCurl(int nthreadid);
  afx_msg void OnBnClickedBtnLoginA8();
  afx_msg
    void StressTestCurl(int nthreadid,int nCMD, int Game);
  void OnBnClickedBtnA8Stress();
  int m_nStress_CMD;

};



