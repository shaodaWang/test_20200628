// 这是主 DLL 文件。

#include "stdafx.h"
#include "BillSdkType.h"
#include "./common/BsrSdk.h"

#include "Billing_sdk.h"
#include "common/ConfigBscp.h"

//using namespace Billing_sdk;

/// 全局变量，管理着当前SDK所有的资源
BsrSdk*	g_sdk = NULL;

DVXSDK_API int 
BillingSdk::dvxSdkInit()
{
  int nRet = DVX_OK;

  do 
  {
    if ( g_sdk != NULL )
      break;

    /// 1.分配新的Dvr全局数据
    g_sdk = new BsrSdk();
    if ( NULL == g_sdk )
    {
      nRet = DVX_ERR_MALLOCMEMORY;
      break;
    }

    /// 2.初始化
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
 * @brief      反初始化SDK，把SDK中的资源全部释放。当不再使用SDK时调用这个函数
 * @return int
 * @retval  0 成功
 * @retval  其它值 错误码
 */
DVXSDK_API int 
BillingSdk::dvxSdkDeInit()
{
	if ( NULL == g_sdk )
		return 0;

	/// 反初始化SDK，并清楚SDK中的资源
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
		/// 1.检查参数
		if ( NULL == pParam->ip_ptv4 || NULL == pDvr )
		{
			nRet = DVX_ERR_PARA; 
			break;
		}

		/// @warning 当前不支持"255.255.255.255"
		int nIp = inet_addr(pParam->ip_ptv4);
		if ( nIp == 0 )
		{
			nRet = DVX_ERR_PARA;
			break;
		}
		pParam->nipv4 = nIp;
		*pDvr = NULL;

		/// 2.检查SDK是否初始化
		if ( NULL == g_sdk )
		{
			nRet = DVX_ERR_SDK_NO_INIT;
			break;
		}

		/// 3.创建Dvr
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
		/// 1.检查参数
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

		/// 2.销毁Dvr
		nRet = g_sdk->dvrDestory( (DvrBase*)hDvx );
	} while (0);

	return nRet;
}

DVXSDK_API int BillingSdk::dvxOperProtocol( DvxHandle hDvx,enumCMD cmd, LPVOID lpSend, int nSend, LPVOID lpRecv, int nRecv )
{
	int nRet = DVX_OK;
	do
	{
		/// 1.检查参数
		if ( NULL == hDvx)
		{
			nRet = DVX_ERR_PARA;
			break;
		}
		/// 2.检查SDK是否初始化
		if ( NULL == g_sdk )
		{
			nRet = DVX_ERR_SDK_NO_INIT;
			break;
		}

		/// 3.检查hDvx句柄是否有效
		DvrBase* pDvr = (DvrBase*)hDvx;

		if ( !g_sdk->dvrCheck( pDvr ) )
		{
			nRet = DVX_ERR_DVX_INVALID_HANDLE;
			break;
		}
		/// 4 获得系统管理对象
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
