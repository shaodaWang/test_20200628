/** 
 *  @file  ConfigBscp.h
 *  @brief ��������ͷ�ļ�
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
	BscpSocket*	m_pBscpSocket;		/**< BSCPģ�飬ֻ��������ķ�������� */

public:
	// ����
	int ConfigParaSet(enumCMD cmd, LPVOID lpSend, int nSend);
	int ConfigParaGet(enumCMD cmd, LPVOID lpSend, int nSend, LPVOID lpRecv, int nRecv);
};