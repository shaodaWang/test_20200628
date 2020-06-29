#include "stdafx.h"
#include "GameBase.h"

GameBase::GameBase()
	:m_pDvr(NULL)
{
	m_nGametype = 0;
	memset(m_configPath, 0,sizeof(m_configPath));
}

GameBase::~GameBase()
{
}

int GameBase::sendToBilling(enumCMD cmd)
{
	int nRet = -1;
	switch (cmd)
	{
	case BSCP_SUNFLOWER_CMD_HEART:
	case BSCP_SUNFLOWER_CMD_HELLO:
	{
		nRet=1; //注意，这两个协议，暂时统一在登录里
	}
		break;
	default:
		break;
	}
	return 0;
}
