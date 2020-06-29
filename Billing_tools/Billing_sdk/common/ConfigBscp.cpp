/** 
 *  @file  ConfigBscp.cpp
 *  @brief 配置BSCP命令实现
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
 * @brief      析构函数
 */
ConfigBscp::~ConfigBscp()
{	
}

/**
 * @fn ConfigBscp::ConfigParaSet
 * @brief 向DVR设置参数    
 * @param[in]  hdvx DVR句柄
 * @param[in]  cmd  自定义配置命令编号
 * @param[in]  lpSend 输入指针
 * @param[in]  nSend 长度
 * @return int
 * @retval  0 成功
 * @retval  其它值 错误码
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
	 ///系统部分
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
* @brief 向DVR获取参数 
* @param[in]  hdvx DVR句柄
* @param[in]  cmd  自定义配置命令编号
* @param[in]  lpSend 输入指针
* @param[in]  nSend 长度
* @param[in]  lpRecv 输出指针
* @param[in]  nRecv 长度
* @return int
* @retval  0 成功
* @retval  其它值 错误码
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
		///系统部分
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

