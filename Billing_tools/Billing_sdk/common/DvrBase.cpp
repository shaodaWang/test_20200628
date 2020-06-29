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
* @brief      ����Dvr�ͺ�������Ӧ��Dvr����
* @param[out] ppDvr ����Dvr�����ֵ
* @return int
* @retval  0 �ɹ�
* @retval  ����ֵ ������
*/
int 
DvrBase::create( DvrBase** ppDvr, int ip, unsigned short nCmdPort, char nGameType, bool bHeart, unsigned int timeOut )
{
	int nRet = DVX_OK;

	/// 1.ͨ��hello ����õ�Dvr��productId
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
		//����Э���, ������A0Э��
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
		//by zhangqiang �����Ժ���⴫��dvr���
		pBscp->SetDvrHandle((DvxHandle)*ppDvr);
	}

	return nRet;
}

/**
* @fn void DvrBase::release(DvrBase** ppDvr )
* @brief      ɾ��Dvr����
* @param[out] ppDvr �ɹ��Ļ������ص��ǿ�ָ��
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
	/// 1.ͨ��hello ����õ�Dvr��productId
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
	//����
	if (pBscp)
	{
		pBscp->destory();
		delete pBscp;
		pBscp = NULL;
	}
	return nRet;
}
