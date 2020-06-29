// Billing_sdk.h

#pragma once

// #include <windows.h>
// #include <winsock2.h>
// using namespace System;
#include "BillSdkType.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC 
#endif

#define DVXSDK_EXPORTS
#ifdef DVXSDK_EXPORTS
#define DVXSDK_API EXTERNC __declspec(dllexport)	
#else	// else BSRDVRSDK_EXPORTS
#define DVXSDK_API EXTERNC __declspec(dllimport)
#endif	// end BSRDVRSDK_EXPORTS

namespace BillingSdk {
	struct BillingCreate
	{
		char ip_ptv4[16];
		unsigned int nipv4;
		unsigned short nport;
		unsigned char nGametype;
		bool bNeedHeart;
		BillingCreate() {
			memset(ip_ptv4,0,sizeof(ip_ptv4));
			nipv4 = 0;
			nport = 0;
			nGametype = 0;
			bNeedHeart = true;
		}
	};

	DVXSDK_API int dvxSdkInit();
	DVXSDK_API int dvxSdkDeInit();
	//创建实例，但只有hello和heart.没有默认login,0xA2命令
	DVXSDK_API int dvxCreateWithOutLogin(BillingCreate *pParam, DvxHandle* pDvr, unsigned int timeOut=2000);
	DVXSDK_API int dvxDestory( DvxHandle hDvx );
	//根据创建的实例，进行其他协议
	DVXSDK_API int dvxOperProtocol( DvxHandle hDvx,enumCMD cmd, LPVOID lpSend, int nSend, LPVOID lpRecv, int nRecv );

}
