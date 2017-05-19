#ifndef _WJ_AutomateManager_H_
#define _WJ_AutomateManager_H_



#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>

#include "WJAutomation.h"
//#include "common/Common.h"

#ifdef _DEBUG
#pragma comment(lib, "WJAutomation_d.lib")
#else
#pragma comment(lib, "WJAutomation.lib")
#endif


#define AUTOMATE_START_PROGRAM		0x0001	/* ������Ϸ */
#define AUTOMATE_CREATE_ROOM		0x0002	/* �������� */
#define AUTOMATE_JOIN_ROOM			0x0003	/* ���뷿�� */


typedef	std::map<int, std::string>	MAP_IntStr;

class AutomateManager
{
public:
	static bool init(const char* _luaFile);


	/**
	* @date:	2016/08/17
	* @brief:	ִ���Զ����ű�����½
	* @param:	[_loginInfo]:	��½��Ϣ
	* @param:	[_pFunc]:	�ص�����
	* @return:	[int]	0: �ű�ִ�гɹ�; >0: �ű�ִ��ʧ��;
	* @usage:	
	*/
	static int executeLogin(WJAM::LOGININFO& _loginInfo, PFuncCallBack _pFunc);

	/**
	* @date:	2016/08/17
	* @brief:	ִ���Զ����ű�����������
	* @param:	[_roomInfo]:	������Ϣ
	* @param:	[_pFunc]:	�ص�����
	* @return:	[int]	0: �ű�ִ�гɹ�; >0: �ű�ִ��ʧ��;
	* @usage:
	*/
	static int executeCreateRoom(WJAM::ROOMINFO& _roomInfo, PFuncCallBack _pFunc);

	/**
	* @date:	2016/08/17
	* @brief:	ִ���Զ����ű������뷿��
	* @param:	[_gameId]:	������Ϣ
	* @param:	[_pFunc]:	�ص�����
	* @return:	[int]	0: �ű�ִ�гɹ�; >0: �ű�ִ��ʧ��;
	* @usage:
	*/
	static int executeJoinRoom(WJAM::ROOMINFO& _roomInfo, PFuncCallBack _pFunc);



	static int executeStartGame(WJAM::GAME_ID _gameId, bool _isRoomOwner);

	//��ʼ��¯ʯʶ��Ӣ��
	static int InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile);

	//¯ʯʶ��Ӣ��
	static void SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback);
private:
	/**
	* @date:	2016/08/16
	* @brief:	ִ���Զ����ű�
	* @param:	[_gameId]:	��ϷID
	* @param:	[_cmd]:		����ָ��
	* @param:	[_pFunc]:	�ص�����
	* @return:	[bool]	true: ���óɹ�;	false: ����ʧ��
	* @usage:
	*/
	static bool execute(int _gameId, int _cmd, PFuncCallBack _pFunc);


private:
	
};





#endif // !_WJ_AutomateManager_H_
