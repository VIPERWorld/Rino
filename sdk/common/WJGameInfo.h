#pragma once
#include "common/WJBaseType.h"


enum E_GameID
{
	e_gameid_unknow = 0,	//δ֪��Ϸ
	e_gameid_10000001 = 1,	//Ӣ������
	e_gameid_10000002 = 2,	//¯ʯ��˵
};

enum class MatchType
{
	LOLMission,
	LOLSolo,
	LOLTeam,
	HSMission,
	HSSolo,
};

static char g_Exe_Name_BattleNet[MAX_PATH] = { "Battle.net.exe" };//ս����������
static char g_Exe_Name_Hearthstone[MAX_PATH] = { "Hearthstone.exe" };//¯ʯ��������

static char g_Wind_Name_BattleNet[MAX_PATH] = { "��ѩ��Ϸƽ̨" };//ս����������
static char g_Wind_Name_Hearthstone[MAX_PATH] = { "¯ʯ��˵" };//¯ʯ��������




#ifdef WJ_CLIENT
///////////////////////////////////////////////////////////////
#include "clientsocket/WJCInterface.h"
//�����Ϸ

/*Ӣ������*/
#include "10000001_lol/MyGameDeskData_10000001.h"
#include "10000001_lol/client/MyDataDrocessing_10000001.h"
#include "10000001_lol/client/MyGameHall_10000001.h"
#include "10000001_lol/client/MyGameRoom_10000001.h"

/*¯ʯ��˵*/
#include "10000002_hearth_stone/MyGameDeskData_10000002.h"
#include "10000002_hearth_stone/client/MyDataDrocessing_10000002.h"
#include "10000002_hearth_stone/client/MyGameHall_10000002.h"
#include "10000002_hearth_stone/client/MyGameRoom_10000002.h"
//////////////////////////////////////////////////////////////
#endif // WJ_CLIENT