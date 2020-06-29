/** 
 *  @file  ConfigBscp.h
 *  @brief 配置命令头文件
 *  @version 1.0.0.1
 *  @since 1.0.0.1
 */

#pragma once
#include "BscpSocket.h"
// #include "../Dvr/CmdDef_Sunflower.h"


class ConfigBscp
{
public:
	ConfigBscp( BscpSocket* pBscp );
	virtual ~ConfigBscp();

protected:
	BscpSocket*	m_pBscpSocket;		/**< BSCP模块，只负责命令的发送与接收 */

public:
	// 配置
	int ConfigParaSet(enumCMD cmd, LPVOID lpSend, int nSend);
	int ConfigParaGet(enumCMD cmd, LPVOID lpSend, int nSend, LPVOID lpRecv, int nRecv);
};