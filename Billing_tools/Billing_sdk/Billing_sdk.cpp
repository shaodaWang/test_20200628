// ������ DLL �ļ���

#include "stdafx.h"
#include "BillSdkType.h"
#include "./common/BsrSdk.h"

#include "Billing_sdk.h"
#include "common/ConfigBscp.h"

//using namespace Billing_sdk;

/// ȫ�ֱ����������ŵ�ǰSDK���е���Դ
BsrSdk*	g_sdk = NULL;

DVXSDK_API int 
BillingSdk::dvxSdkInit()
{
  int nRet = DVX_OK;

  do 
  {
    if ( g_sdk != NULL )
      break;

    /// 1.�����µ�Dvrȫ������
    g_sdk = new BsrSdk();
    if ( NULL == g_sdk )
    {
      nRet = DVX_ERR_MALLOCMEMORY;
      break;
    }

    /// 2.��ʼ��
    nRet = g_sdk->init();
    if ( nRet != DVX_OK )
    {
      delete g_sdk;
      g_sdk = NULL;
    }    
  } while (0); 

	return nRet;
}

/**
 * @fn DVXSDK_API int dvxSdkDeInit()
 * @brief      ����ʼ��SDK����SDK�е���Դȫ���ͷš�������ʹ��SDKʱ�����������
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
DVXSDK_API int 
BillingSdk::dvxSdkDeInit()
{
	if ( NULL == g_sdk )
		return 0;

	/// ����ʼ��SDK�������SDK�е���Դ
	g_sdk->deInit();
	delete g_sdk;
	g_sdk = NULL;
	return DVX_OK;
}

DVXSDK_API int 
BillingSdk::dvxCreateWithOutLogin(BillingCreate *pParam, DvxHandle* pDvr, unsigned int timeOut )
{
	int nRet = DVX_OK;
	do
	{
		/// 1.������
		if ( NULL == pParam->ip_ptv4 || NULL == pDvr )
		{
			nRet = DVX_ERR_PARA; 
			break;
		}

		/// @warning ��ǰ��֧��"255.255.255.255"
		int nIp = inet_addr(pParam->ip_ptv4);
		if ( nIp == 0 )
		{
			nRet = DVX_ERR_PARA;
			break;
		}
		pParam->nipv4 = nIp;
		*pDvr = NULL;

		/// 2.���SDK�Ƿ��ʼ��
		if ( NULL == g_sdk )
		{
			nRet = DVX_ERR_SDK_NO_INIT;
			break;
		}

		/// 3.����Dvr
		nRet = g_sdk->dvrCreateWithOutLogin( nIp, pParam->nport, pParam->nGametype, pParam->bNeedHeart, (DvrBase**)pDvr, timeOut);
		if( DVX_OK != nRet )
		{
			g_sdk->dvrDestory( (DvrBase*)*pDvr,false );
			*pDvr = NULL;
		}
	}while( 0 );

	return nRet;
}

DVXSDK_API int BillingSdk::dvxDestory( DvxHandle hDvx )
{
	int nRet = DVX_OK;

	do 
	{
		/// 1.������
		if ( NULL == g_sdk )
		{
			nRet = DVX_ERR_SDK_NO_INIT;
			break;
		}

		if ( NULL == hDvx )
		{
			nRet = DVX_ERR_PARA;
			break;
		}

		/// 2.����Dvr
		nRet = g_sdk->dvrDestory( (DvrBase*)hDvx );
	} while (0);

	return nRet;
}

DVXSDK_API int BillingSdk::dvxOperProtocol( DvxHandle hDvx,enumCMD cmd, LPVOID lpSend, int nSend, LPVOID lpRecv, int nRecv )
{
	int nRet = DVX_OK;
	do
	{
		/// 1.������
		if ( NULL == hDvx)
		{
			nRet = DVX_ERR_PARA;
			break;
		}
		/// 2.���SDK�Ƿ��ʼ��
		if ( NULL == g_sdk )
		{
			nRet = DVX_ERR_SDK_NO_INIT;
			break;
		}

		/// 3.���hDvx����Ƿ���Ч
		DvrBase* pDvr = (DvrBase*)hDvx;

		if ( !g_sdk->dvrCheck( pDvr ) )
		{
			nRet = DVX_ERR_DVX_INVALID_HANDLE;
			break;
		}
		/// 4 ���ϵͳ�������
		ConfigBscp* pConfig = pDvr->getConfig();
		if ( NULL == pConfig )
		{
			nRet = DVX_ERR_SDK_NO_INIT;
			break;
		}

		nRet = pConfig->ConfigParaGet( cmd, lpSend, nSend, lpRecv,nRecv);
	}while( 0 );

	return nRet;
}
