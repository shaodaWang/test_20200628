#include "StdAfx.h"
#include "DvrBase.h"
#include "BscpSocket.h"
#include "./Dvr_Sunflower.h"

DvrBase::DvrBase(void)
: m_ipAddr(0)
, m_nCmdPort(3721)
, m_nDataPort(3720)
{
	m_nGameType=0;
}

DvrBase::~DvrBase(void)
{
}

/**
* @fn int DvrBase::create(DvrBase** ppDvr )
* @brief      根据Dvr型号生成相应的Dvr子类
* @param[out] ppDvr 返回Dvr对象的值
* @return int
* @retval  0 成功
* @retval  其它值 错误码
*/
int 
DvrBase::create( DvrBase** ppDvr, int ip, unsigned short nCmdPort, char nGameType, bool bHeart, unsigned int timeOut )
{
	int nRet = DVX_OK;

	/// 1.通过hello 命令，得到Dvr的productId
	BscpSocket* pBscp = new BscpSocket();

	do 
	{
		if ( NULL == pBscp )
		{
			nRet = DVX_ERR_MALLOCMEMORY;
			break;
		}

		nRet = pBscp->init( ip, nCmdPort, nGameType,bHeart,timeOut );
		if ( DVX_OK != nRet )
		{
			break;
		}
		//握手协议包, 这里是A0协议
		nRet = pBscp->HelloORheart(BSCP_SUNFLOWER_CMD_HELLO);
		if ( DVX_OK != nRet )
		{
			break;
		}

	} while (0);

	if ( DVX_OK != nRet )
	{
		pBscp->destory();
		delete pBscp;
		pBscp = NULL;
		return nRet;
	}	
	*ppDvr = (DvrBase*)new DvrSunflower(pBscp,0);
	if( *ppDvr != NULL )
	{
		(*ppDvr)->setDvxAddr( ip );
		//(*ppDvr)->setDvxPort( nCmdPort, nDataPort );
		//by zhangqiang 断线以后对外传递dvr句柄
		pBscp->SetDvrHandle((DvxHandle)*ppDvr);
	}

	return nRet;
}

/**
* @fn void DvrBase::release(DvrBase** ppDvr )
* @brief      删除Dvr对象
* @param[out] ppDvr 成功的话，返回的是空指针
*/
void 
DvrBase::release(DvrBase** ppDvr )
{
	delete *ppDvr;
	*ppDvr = NULL;
}

int DvrBase::createNotLogin( const int ip, const unsigned short nCmdPort, unsigned int nCmd, const void* parmin, const unsigned int nLenthIn,
							void* parmout, unsigned int nLenout, unsigned int timeOut/*=3000*/ )
{
	int nRet = DVX_OK;
	/// 1.通过hello 命令，得到Dvr的productId
	BscpSocket* pBscp = NULL;
	pBscp=new BscpSocket();
	do 
	{
		if ( NULL == pBscp )
		{
			nRet = DVX_ERR_MALLOCMEMORY;
			break;
		}

		nRet = pBscp->initNoHeartBeat( ip, nCmdPort, timeOut );
		if ( DVX_OK != nRet )
		{
			break;
		}

		nRet = pBscp->GetChInfoNotLogin(nCmd, parmin, nLenthIn, parmout, nLenout, timeOut);
		if ( DVX_OK != nRet )
		{
			break;
		}

	} while (0);
	//销毁
	if (pBscp)
	{
		pBscp->destory();
		delete pBscp;
		pBscp = NULL;
	}
	return nRet;
}
