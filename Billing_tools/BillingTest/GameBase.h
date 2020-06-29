#pragma once
#include "../Billing_sdk/Billing_sdk.h"
//#include "../Billing_sdk/BillSdkType.h"
#include "./common/CYRdWrIni.h"
#include "./common/BillingType.h"
#define PROTOCOL_BUFFER_MAX (1024)
class GameBase
{
public:
	GameBase();
	virtual ~GameBase();

	virtual int sendToBilling(enumCMD cmd);
	virtual int sendA2(CString &strInfo) { return -1; };
	virtual int sendA3(CString &strInfo) { return -1; };
	virtual int sendA4(CString &strInfo) { return -1; };
  virtual int sendA8(CString &strInfo) { return -1; };
  virtual int send01(CString &strInfo) { return -1; };

	DvxHandle m_pDvr;
	void SetDvxHandle(DvxHandle pdvr) { m_pDvr = pdvr; };
	void SetGameType(int ntype) { m_nGametype = ntype; };
	int m_nGametype;
	char m_configPath[MAX_PATH];
};

