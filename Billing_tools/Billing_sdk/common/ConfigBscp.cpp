/** 
 *  @file  ConfigBscp.cpp
 *  @brief ����BSCP����ʵ��
 *  @version 1.0.0.1
 *  @since 1.0.0.1
 */


#include "StdAfx.h"
#include "ConfigBscp.h"


ConfigBscp::ConfigBscp(BscpSocket *pBscp)
: m_pBscpSocket( pBscp )
{
}

/**
 * @fn ConfigBscp::~ConfigBscp()
 * @brief      ��������
 */
ConfigBscp::~ConfigBscp()
{	
}

/**
 * @fn ConfigBscp::ConfigParaSet
 * @brief ��DVR���ò���    
 * @param[in]  hdvx DVR���
 * @param[in]  cmd  �Զ�������������
 * @param[in]  lpSend ����ָ��
 * @param[in]  nSend ����
 * @return int
 * @retval  0 �ɹ�
 * @retval  ����ֵ ������
 */
int
ConfigBscp::ConfigParaSet(enumCMD cmd, LPVOID lpSend, int nSend)
{	
 if ( NULL == m_pBscpSocket)
 {
		return DVX_ERR_DVX_NO_LOGIN;
 }
 int nRet = DVX_OK;
 switch( cmd )
 {
	 ///ϵͳ����
 case BSCP_SUNFLOWER_CMD_HELLO:
	 nRet = m_pBscpSocket->Execute( BSCP_SUNFLOWER_CMD_HELLO,lpSend, nSend, NULL, 0);
	 break;
 default:
	 break;
 }
 return nRet;
}

/**
* @fn ConfigBscp::ConfigParaGet     
* @brief ��DVR��ȡ���� 
* @param[in]  hdvx DVR���
* @param[in]  cmd  �Զ�������������
* @param[in]  lpSend ����ָ��
* @param[in]  nSend ����
* @param[in]  lpRecv ���ָ��
* @param[in]  nRecv ����
* @return int
* @retval  0 �ɹ�
* @retval  ����ֵ ������
*/  
int
ConfigBscp::ConfigParaGet(enumCMD cmd, LPVOID lpSend, int nSend, LPVOID lpRecv, int nRecv)
{	
	if ( NULL == m_pBscpSocket)
	{
		return DVX_ERR_DVX_NO_LOGIN;
	}
	int nRet = DVX_OK;
	switch( cmd )
	{
		///ϵͳ����
	case BSCP_SUNFLOWER_CMD_HELLO:
	case BSCP_SUNFLOWER_CMD_HEART:
	case BSCP_SUNFLOWER_CMD_LOGIN:
	case BSCP_SUNFLOWER_CMD_ONLINE:
	case BSCP_SUNFLOWER_CMD_LOGOUT:
  case BSCP_SUNFLOWER_CMD_LOGIN_BY_TWO:
  case BSCP_SUNFLOWER_CMD_TEST_CURL:
	{
		nRet = m_pBscpSocket->Execute(cmd, lpSend, nSend, lpRecv, nRecv);
	}
	break;	
	default:
		nRet = DVX_ERR_NO_CMD;
		break;
	}
	return nRet;
}

