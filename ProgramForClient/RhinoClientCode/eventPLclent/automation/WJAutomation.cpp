// AutoRun.cpp : Defines the exported functions for the DLL application.
//

#include "common/LuaEngine.h"

#include "AMUtil.h"
#include "WJAutomation.h"
#include "LuaInterface.h"
#include "LuaInterfaceHS.h"
#include "GameMonitorHS.h"
GameMonitorHS g_gameMonitorHS;


#include "identification/hs_identification.h"
#include "hook/WSMessageId.h"


#include <atlimage.h>
#include <math.h>
#include "XLog.h"

const char* WINDOW_TITLE_LOL_LOGIN = "Ӣ�����˵�¼����";
const char* WINDOW_TITLE_LOL_CLIENT = "PVP.net �ͻ���";
const char* WINDOW_TITLE_LOL_GAME = "League of Legends (TM) Client";
const char* WINDOW_TITLE_HB_LOGIN = "ս����¼";
const char* WINDOW_TITLE_HB_CLIENT = "PVP.net �ͻ���";
const char* WINDOW_TITLE_HB_BATTLENET = "ս��";
const char* WINDOW_TITLE_HB_FRIENDS = "����";

HWND g_hWnd = NULL;

CLuaEngine* g_luaEngine = NULL;

int g_gameId = -1;


float windowScaleX = 1.0f;
float windowScaleY = 1.0f;

XLog g_log;

int checkLuaCallRes(int _ret, lua_State* L, const char* _funcName)
{
	if (0 != _ret)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "ִ�нű�����[%s]ʧ��", _funcName);
		OutputDebugStringA(lua_tostring(L, -1));
		return WJAM_RES_LUA_EXCE_ERROR;
	}

	int res = lua_tonumber(L, -1);
	g_log.writeLog(XLog::E_LOG_DEBUG, "ִ�нű�����[%s]��ɣ����ؽ����%d", _funcName, res);
	return res;
}

bool isLuaVaild()
{
	/* �ж�LUA�Ƿ��ʼ�� */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "[ERROR] LuaEngineδ��ʼ��");
		return false;
	}
	else
	{
		return true;
	}
}

#define AM_SHARE_MEM_HS_SERVER		TEXT("AM_SHARE_MEM_HS_SERVER")	
#define AM_SHARE_MEM_HS_CLIENT		TEXT("AM_SHARE_MEM_HS_CLIENT")	

AMCallback* g_pAmCallback = nullptr;

DLL_EXPORT bool WJAM::initLua(const char* _luaFile, AMCallback* _callback)
{
	g_pAmCallback;// = _callback;
	::CoInitialize(NULL);
	

	/* ��ʼ��LuaEngine */
	if (NULL != g_luaEngine)
	{
		delete g_luaEngine;
		g_luaEngine = NULL;
	}
	g_luaEngine = new CLuaEngine();

	lua_register(g_luaEngine->GetLuaState(), "abs", Inf_Abs);
	lua_register(g_luaEngine->GetLuaState(), "Inf_MoveWindow", Inf_MoveWindow);
	lua_register(g_luaEngine->GetLuaState(), "Inf_MouseClickLButton", Inf_MouseClickLButton);
	lua_register(g_luaEngine->GetLuaState(), "Inf_MouseClickLButton1", Inf_MouseClickLButton1);
	lua_register(g_luaEngine->GetLuaState(), "Inf_Input", Inf_Input);
	lua_register(g_luaEngine->GetLuaState(), "Inf_Backspace", Inf_Backspace);
	lua_register(g_luaEngine->GetLuaState(), "Sleep", Inf_Sleep);
	lua_register(g_luaEngine->GetLuaState(), "INF_IsWindowExist", Inf_IsWindowExist);
	lua_register(g_luaEngine->GetLuaState(), "Inf_ActiveWindow", Inf_ActiveWindow);
	lua_register(g_luaEngine->GetLuaState(), "Inf_GetWindowHwnd", Inf_GetWindowHwnd);
	lua_register(g_luaEngine->GetLuaState(), "Inf_SetWindowVisible", Inf_SetWindowVisible);
	lua_register(g_luaEngine->GetLuaState(), "Lua_GetWindowPixelRGB", Inf_GetWindowPixelRGB);
	lua_register(g_luaEngine->GetLuaState(), "Lua_SetWindowVisibleByClassName", Lua_SetWindowVisibleByClassName);
	lua_register(g_luaEngine->GetLuaState(), "Lua_SetWindowVisibleByTitle", Inf_SetWindowVisibleByTitle);
	lua_register(g_luaEngine->GetLuaState(), "Lua_InjectDll", Lua_InjectDll);
	lua_register(g_luaEngine->GetLuaState(), "Lua_SetWindowAlpha", Lua_SetWindowAlpha);
	lua_register(g_luaEngine->GetLuaState(), "Lua_ComparePixel", Lua_ComparePixel);
	lua_register(g_luaEngine->GetLuaState(), "Inf_PressKey", Inf_PressKey);
	//lua_register(g_luaEngine->GetLuaState(), "Inf_ImageRecognition", Inf_ImageRecognition);
	//lua_register(g_luaEngine->GetLuaState(), "Inf_ImageContrastRect", Inf_ImageContrastRect);
	lua_register(g_luaEngine->GetLuaState(), "Inf_GetHS_FriendWindowRect", Inf_GetHS_FriendWindowRect);
	lua_register(g_luaEngine->GetLuaState(), "Inf_GetHS_WindowRect", Inf_GetHS_WindowRect);
	lua_register(g_luaEngine->GetLuaState(), "lua_TestDebug", lua_TestDebug);
	
	if (NULL == _luaFile)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "lua�ļ�ָ��Ϊ��");
		return false;
	}
	//���л�·����lua�ű�����·��
	TCHAR path[1024] = { 0 };
	HMODULE  handle = GetModuleHandle(NULL);
	GetModuleFileName(handle, path, ARRAYSIZE(path));
	wchar_t* pwchExeName = wcsrchr(path, '\\');
	::ZeroMemory(pwchExeName, ::wcslen(pwchExeName));
	TCHAR CurPath[1024] = { 0 };
	::GetCurrentDirectory(ARRAYSIZE(CurPath), CurPath);
	::SetCurrentDirectory(path);

	//����lua
	if (!g_luaEngine->loadLuaFile(_luaFile))
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "����lua�ļ�[%s]ʧ��", _luaFile);
		return false;
	}
	else
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "����lua�ļ�[%s]�ɹ�", _luaFile);
	}
	//�л�����
	::SetCurrentDirectory(CurPath);

	/* ��ʼ�������ڴ洦���߳� */
	//g_gameMonitorHS.initEventSignal(AM_SHARE_MEM_HS_SERVER, AM_SHARE_MEM_HS_CLIENT, EVENT_EXIT_DEFAULT_NAME, &g_gameMonitorHS);
	//g_gameMonitorHS.StartThread();
	return true;
}
extern int g_gameWndOffsetX;
extern int g_gameWndOffsetY;

DLL_EXPORT int WJAM::login(LOGININFO* _loginInfo)
{
	/* �ж�LUA�Ƿ��ʼ�� */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngineδ��ʼ��");
		return WJAM_RES_LUA_NOT_INIT;
	}

	g_gameId = _loginInfo->gameid;
	std::string windowTitle;
	switch (_loginInfo->gameid)
	{
	case GAME_ID_LOL: /* Ӣ������ */
		windowTitle = WINDOW_TITLE_LOL_LOGIN;

		g_gameWndOffsetX = 0;
		g_gameWndOffsetY = 0;

		/* �ж���Ϸ�Ƿ��Ѿ����� */
		if (g_hWnd = ::FindWindowA(NULL, WINDOW_TITLE_LOL_GAME))
		{
			g_log.writeLog(XLog::E_LOG_WARNING, "LOL��Ϸ�ͻ����Ѿ�����,���������Ϸ��,��Ҫ�رտͻ��ˣ�");
			return WJAM_RES_LOL_EXIST_IN_GAME;
		}
		else if (g_hWnd = ::FindWindowA(NULL, WINDOW_TITLE_LOL_CLIENT))
		{
			g_log.writeLog(XLog::E_LOG_WARNING, "LOL��Ϸ�ͻ����Ѿ�����,����Ѿ�ѡ�������,��Ҫ�رտͻ��ˣ�");
			return WJAM_RES_LOL_EXIST_IN_SERVER;
		}
		else if (g_hWnd = ::FindWindowA(NULL, WINDOW_TITLE_LOL_LOGIN))
		{
		}
		break;

	case GAME_ID_HB: /* ¯ʯ��˵ */
		windowTitle = WINDOW_TITLE_HB_LOGIN;
		break;

	default:
		g_log.writeLog(XLog::E_LOG_ERROR, "������Ϸʧ��[id=%d]��δ�������Ϸ", _loginInfo->gameid);
		return WJAM_RES_UNKNOWN_GAME;
		break;
	}

	if (::FindWindowA(NULL, windowTitle.c_str()) == NULL)
	{
		WinExec(_loginInfo->path, SW_SHOW);
		//ShellExecute(NULL, A2T("open"), A2T(pLoginInfo->path), NULL, NULL, SW_SHOWNORMAL);
	}
	g_log.writeLog(XLog::E_LOG_DEBUG, "������Ϸ[id=%d]��%s", _loginInfo->gameid, WINDOW_TITLE_LOL_LOGIN);

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "login");
	lua_pushnumber(L, _loginInfo->gameid);
	lua_pushnumber(L, _loginInfo->serverid);
	lua_pushstring(L, _loginInfo->path);


	int ret = lua_pcall(L, 3, 1, 0);// ���ú�������������Ժ󣬻Ὣ����ֵѹ��ջ�У�3��ʾ����������1��ʾ���ؽ��������
	return checkLuaCallRes(ret, L, "login");
}


DLL_EXPORT int WJAM::createRoom(ROOMINFO* pRoomInfo)
{
	/* �ж�LUA�Ƿ��ʼ�� */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngineδ��ʼ��");
		return WJAM_RES_LUA_NOT_INIT;
	}

	if (::FindWindowA(NULL, WINDOW_TITLE_LOL_CLIENT) == NULL)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "δ�ҵ���Ϸ����: %s", WINDOW_TITLE_LOL_CLIENT);
		return WJAM_RES_WINDOW_NOT_EXIST;
	}

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "createroom");			// ��ȡ������ѹ��ջ��  
	lua_pushstring(L, pRoomInfo->roomname);          // ѹ���һ������  
	lua_pushstring(L, pRoomInfo->password);
	lua_pushstring(L, pRoomInfo->account);
	lua_pushnumber(L, pRoomInfo->mode);
	lua_pushnumber(L, pRoomInfo->map);
	lua_pushnumber(L, pRoomInfo->playernum);
	lua_pushnumber(L, pRoomInfo->observe);
	lua_pushnumber(L, pRoomInfo->gametype);

	// ���ú�����n��ʾ����������r��ʾ���ؽ��������
	return checkLuaCallRes(lua_pcall(L, 8, 1, 0), L, "createroom");
}



DLL_EXPORT int WJAM::joinRoom(ROOMINFO* pRoomInfo)
{
	/* �ж�LUA�Ƿ��ʼ�� */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngineδ��ʼ��");
		return WJAM_RES_LUA_NOT_INIT;
	}

	if (::FindWindowA(NULL, WINDOW_TITLE_LOL_CLIENT) == NULL)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "δ�ҵ���Ϸ����: %s", WINDOW_TITLE_LOL_CLIENT);
		return WJAM_RES_WINDOW_NOT_EXIST;
	}


	g_log.writeLog(XLog::E_LOG_DEBUG, "������Ϸ���� [%s]", pRoomInfo->roomname);
	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "joinroom");        // ��ȡ������ѹ��ջ��  
	lua_pushstring(L, pRoomInfo->roomname);          // ѹ���һ������  
	lua_pushstring(L, pRoomInfo->password);
	lua_pushstring(L, pRoomInfo->account);
	lua_pushnumber(L, pRoomInfo->side);

	// ���ú�����n��ʾ����������r��ʾ���ؽ��������  
	return checkLuaCallRes(lua_pcall(L, 4, 1, 0), L, "joinroom");
}


DLL_EXPORT int WJAM::startGame(WJAM::GAME_ID _gameId, bool _isRoomOwner)
{
	/* �ж�LUA�Ƿ��ʼ�� */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngineδ��ʼ��");
		return WJAM_RES_LUA_NOT_INIT;
	}

	//if (::FindWindowA(NULL, WINDOW_TITLE_LOL_CLIENT) == NULL)
	//{
	//	writeLog(LOG_FILE, "[ERROR] δ�ҵ���Ϸ����: %s", WINDOW_TITLE_LOL_CLIENT);
	//	return WJAM_RES_WINDOW_NOT_EXIST;
	//}

	g_log.writeLog(XLog::E_LOG_DEBUG, "��ʼ��Ϸ [%s]");
	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "playgame");
	lua_pushnumber(L, int(_gameId));
	lua_pushnumber(L, int(_isRoomOwner));
	
	// ����lua������n��ʾ����������r��ʾ���ؽ��������
	return checkLuaCallRes(lua_pcall(L, 2, 1, 0), L, "playgame");
}



DLL_EXPORT int injectRomateDll(WJAM::GAME_ID _gameId)
{
	/* �ж�LUA�Ƿ��ʼ�� */
	if (NULL == g_luaEngine)
	{
		g_log.writeLog(XLog::E_LOG_ERROR, "LuaEngineδ��ʼ��");
		return WJAM_RES_LUA_NOT_INIT;
	}
	g_log.writeLog(XLog::E_LOG_DEBUG, "ע��Զ��DLL����ϷID[%d]", _gameId);

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "injectDll");
	lua_pushnumber(L, int(_gameId));

	return checkLuaCallRes(lua_pcall(L, 2, 1, 0), L, "injectDll");
}


#include <psapi.h> 
#include   <tlhelp32.h>
DWORD GetProcessIdByName(LPCTSTR pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return 0;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL res;
	for (res = Process32First(hSnapshot, &pe); res; res = Process32Next(hSnapshot, &pe))
	{
		if (0 == wcscmp(pe.szExeFile, pName))
		{
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
	}
	return 0;
}


int CloseWindow(const char* _windowTitle)
{
	if (!_windowTitle)
		return 1;

	HWND hWnd = ::FindWindowA(NULL, _windowTitle);
	if (!hWnd)
		return 2;

	// ����ID, ���߳�ID 
	DWORD processId = 0;
	DWORD threadId = 0;

	/* ͬʱ�õ�����ID�����߳�ID */
	threadId = GetWindowThreadProcessId(hWnd, &processId);

	/* ͨ������ID��ȡ���ƾ�� */
	HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_ALL_ACCESS, TRUE, processId);;
	if (!TerminateProcess(handle, 0))
		return 3;

	return 0;
}


DLL_EXPORT int WJAM::closeGame(WJAM::GAME_ID _gameId)
{
	HWND hWnd = NULL;
	switch (_gameId)
	{
	case WJAM::GAME_ID_LOL:
	{
		CloseWindow(WINDOW_TITLE_LOL_GAME);
		CloseWindow(WINDOW_TITLE_LOL_CLIENT);
		CloseWindow(WINDOW_TITLE_LOL_LOGIN);
	}
		break;
	case WJAM::GAME_ID_HB:
		// WINDOW_TITLE_HB_LOGIN = "ս����¼";
		// WINDOW_TITLE_HB_CLIENT = "PVP.net �ͻ���";
		// WINDOW_TITLE_HB_BATTLENET = "ս��";
		// WINDOW_TITLE_HB_FRIENDS = "����";
		break;
	default:
		return WJAM_RES_LUA_PARAM_ERROR;	// LUA ��������
		break;
	}

	return 0;
}


HWND setWindowVisible(const char* _windowTitle, bool _visible)
{
	HWND hWnd = FindWindowA(NULL, _windowTitle);
	if (hWnd)
	{
		ShowWindow(hWnd, _visible);
		if (_visible)
		{
			BringWindowToTop(hWnd);
		}
	}
	return hWnd;
}

DLL_EXPORT void WJAM::setGameWindowVisible(WJAM::GAME_ID _gameId, bool _visible)
{
	HWND hWnd = NULL;

	switch (_gameId)
	{
	case WJAM::GAME_ID_LOL:
	{
		setWindowVisible(WINDOW_TITLE_LOL_GAME, _visible);
		setWindowVisible(WINDOW_TITLE_LOL_CLIENT, _visible);
		setWindowVisible(WINDOW_TITLE_LOL_LOGIN, _visible);
	}
		break;
	case WJAM::GAME_ID_HB:
		// WINDOW_TITLE_HB_LOGIN = "ս����¼";
		// WINDOW_TITLE_HB_CLIENT = "PVP.net �ͻ���";
		// WINDOW_TITLE_HB_BATTLENET = "ս��";
		// WINDOW_TITLE_HB_FRIENDS = "����";
		break;
	default:
		break;
	}
}





DLL_EXPORT int WJAM::startupHS(int _wsPort, const char* _url, std::string& _playerName)
{
	g_log.writeLog(XLog::E_LOG_DEBUG, "ִ���Զ�����startupHS");

	/* ����¯ʯ��������״̬ */
	g_gameMonitorHS.setGameStatus(E_STATUS_HS_HOOKED);

	// ���luaEngine��״̬�� luaEngine�쳣ʱ�޷�ִ�нű�
	if (!isLuaVaild())
	{
		return WJAM_RES_LUA_NOT_INIT;
	}
	g_gameMonitorHS.initEmbedWinHS(_wsPort, _url, "");
	g_log.writeLog(XLog::E_LOG_DEBUG, "������������ɹ�.");
	return WJAM_RES_OK;


	/**
	*	����ִ��lua�ű�����
	*/
	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "startup_hs");

	// ����lua������n��ʾ����������r��ʾ���ؽ��������
	int res = checkLuaCallRes(lua_pcall(L, 0, 1, 0), L, "startup_hs");
	if (WJAM_RES_OK != res)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "lua �ű�ִ���쳣");
		return res;
	}

	// ��ʼ�������ڴ洦���߳�
	g_gameMonitorHS.initEventSignal(AM_SHARE_MEM_HS_SERVER, AM_SHARE_MEM_HS_CLIENT, EVENT_EXIT_DEFAULT_NAME, &g_gameMonitorHS);
	g_gameMonitorHS.StartThread();


	/**
	*	��¯ʯ��Ⱦ�����д�������������ͼ��ʶ�𣬸ù�����Ҫ�ȴ�¯ʯ����������һ������ʱ��
	*	�����ڴ˴�����ѭ����ء�
	*/
	double tick = GetTickCount();
	while (true)
	{
		double t = GetTickCount() - tick;
		EnGameStatusHS& status = g_gameMonitorHS.getGameStatus();
		if (E_STATUS_HS_NOTSTART != status && E_STATUS_HS_D3D_ERROR != status)
		{
			HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
			if (!hWnd)
			{
				res = WJAM_RES_WINDOW_NOT_EXIST;
			}
			else
			{
				//g_gameMonitorHS.getPlayerName(hWnd, _playerName);
				/* ��ʼ��¯ʯ����Ƕ���� */
				g_gameMonitorHS.initEmbedWinHS(_wsPort, _url, "");
				g_log.writeLog(XLog::E_LOG_DEBUG, "������������ɹ�.");
				res = WJAM_RES_OK;
			}
			break;
		}
		//else if (E_STATUS_HS_D3D_ERROR == status)
		//{
		//	res = WJAM_RES_D3D_ERROE;
		//	break;
		//}
		else if (t > 25000)
		{
			res = WJAM_RES_HOOK_TIMEOUT;
			g_log.writeLog(XLog::E_LOG_DEBUG, "������������ʧ�ܣ���ʱ�˳���%d�룩", int(t*0.001));
			break;
		}
		Sleep(1000);
	}

	if (WJAM_RES_OK == res)
	{
	}
	return res;
}


DLL_EXPORT int WJAM::reqAddFriend(const char *_name)
{
	g_log.writeLog(XLog::E_LOG_DEBUG, "����ִ���Զ��� $BEGIN$��������Ӻ���[%s]", _name);

	HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
	if (!hWnd)
		return WJAM_RES_WINDOW_NOT_EXIST;

	if (!isLuaVaild())
		return WJAM_RES_LUA_NOT_INIT;

	

	/* ����ǰ����ʾ */
	//BringWindowToTop(hWnd, true);
	BringWindowToTop(hWnd);
	Sleep(200);

	g_gameMonitorHS.clickWindow(hWnd, 0.5, 0.2, 1000);

	/* ֪ͨ��Ƕ���� */
	g_gameMonitorHS.setEvent(GAME_EVENT_AM_ADD_FRIEND);

	//if (g_gameMonitorHS.imageContrast("game_task") >= 120)
	//{
	//	g_gameMonitorHS.clickWindow(hWnd, 0.5, 0.2);
	//	//RECT rt;
	//	//::GetWindowRect(hWnd, &rt);
	//	//AMUtil::mouseClickLButton1(hWnd, rt.left + (rt.right - rt.left) * 0.5, rt.top + (rt.bottom - rt.top) * 0.2);
	//}

	/* ����Ļ�ϵ��һ�Σ��رտ�ʼ����ҳ���Ż��������� ���ͼ��ʶ���׼ȷ�ʣ�ͨ��ͼƬʶ���ж�״̬����е�������� */
	//RECT rt;
	//::GetWindowRect(hWnd, &rt);
	//AMUtil::mouseClickLButton1(hWnd, rt.left + (rt.right - rt.left) * 0.5, rt.top + (rt.bottom - rt.top) * 0.2);

	/* ��ʾ�����б��� */
	g_gameMonitorHS.showFriendWindow(hWnd);
	std::string playerName;
	//g_gameMonitorHS.getPlayerName(hWnd, playerName);

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "reqAddFriend");
	lua_pushinteger(L, (int)hWnd);
	lua_pushstring(L, _name);

	//g_gameMonitorHS.setGameStatus(E_STATUS_HS_ADDFRIEND_REQ);

	// ����lua������n��ʾ����������r��ʾ���ؽ��������
	int res = checkLuaCallRes(lua_pcall(L, 2, 1, 0), L, "reqAddFriend");

	//g_gameMonitorHS.setGameStatus((WJAM_RES_OK == res) ? E_STATUS_HS_ADDFRIEND_REQ : E_STATUS_HS_AM_FAIL);
	return res;
	//return 0;
}

DLL_EXPORT int WJAM::acceptFriendReq(const char *_name)
{
	g_log.writeLog(XLog::E_LOG_DEBUG, "����ִ���Զ��� $BEGIN$�����ܺ���[%s]", _name);

	HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
	if (!hWnd)
		return WJAM_RES_WINDOW_NOT_EXIST;

	if (!isLuaVaild())
		return WJAM_RES_LUA_NOT_INIT;
	
	/* ����ǰ����ʾ */
	BringWindowToTop(hWnd);
	Sleep(200);

	g_gameMonitorHS.clickWindow(hWnd, 0.5, 0.2, 1000);
	/* ֪ͨ��Ƕ���� */
	g_gameMonitorHS.setEvent(GAME_EVENT_AM_ACCEPT_FRIEND);


	vector<Rect> vecRt;
	for (int i = 0; i < 3; ++i)
	{
		
		if (g_gameMonitorHS.getFriendRect(_name, vecRt))
		{
			break;
		}
		g_log.writeLog(XLog::E_LOG_DEBUG, "�Ӻ����б��л�ȡ����λ��ʧ�� [���ѣ�%s]", _name);
	}

	g_log.writeLog(XLog::E_LOG_DEBUG, "==================================");
	int size = vecRt.size();
	if (size <= 0)
		return WJAM_RES_HS_OPERATE_ERROR;
	g_log.writeLog(XLog::E_LOG_DEBUG, "==================================");

	bool flag = false;
	int res = WJAM_RES_OK;

	Rect rtFriend;
	for (auto iter = vecRt.begin(); iter != vecRt.end(); ++iter)
	{
		if (iter->x < 38)
		{
			rtFriend = *iter;

			/* ����Ļ�ϵ��һ�Σ��رտ�ʼ����ҳ���Ż��������� ���ͼ��ʶ���׼ȷ�ʣ�ͨ��ͼƬʶ���ж�״̬����е�������� */
			RECT rt;
			::GetWindowRect(hWnd, &rt);
			AMUtil::mouseClickLButton1(hWnd, rt.left + (rt.right - rt.left) * 0.5, rt.top + (rt.bottom - rt.top) * 0.2);


			/* ��ʾ�����б��� */
			g_gameMonitorHS.showFriendWindow(hWnd);
			Sleep(200);
			g_log.writeLog(XLog::E_LOG_DEBUG, "==================================");
			lua_State *L = g_luaEngine->GetLuaState();
			lua_getglobal(L, "acceptFriendReq");
			lua_pushinteger(L, (int)hWnd);
			lua_pushinteger(L, rtFriend.y + rtFriend.height);
			res = checkLuaCallRes(lua_pcall(L, 2, 1, 0), L, "acceptFriendReq");
		}
		else
		{
			g_log.writeLog(XLog::E_LOG_DEBUG, "��Ӻ���λ���쳣��x=%d", iter->x);
			flag = true;
		}
		flag = true;
	}

	if (!flag)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "����λ�û�ȡ���� [���ѣ�%s]", _name);
		res = WJAM_RES_HS_OPERATE_ERROR;
	}

	/* �����Զ���ִ��״̬ */
	//g_gameMonitorHS.setGameStatus((WJAM_RES_OK == res) ? E_STATUS_HS_ADDFRIEND_ACCEPT : E_STATUS_HS_AM_FAIL);

	g_log.writeLog(XLog::E_LOG_DEBUG, "�Զ�����END ���ܺ���, res=%d ", res);
	return res;
}

DLL_EXPORT int WJAM::reqChallenge(const char *_name)
{
	g_log.writeLog(XLog::E_LOG_DEBUG, "����ִ���Զ��� $BEGIN$��������ս����");

	HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
	if (!hWnd)
		return WJAM_RES_WINDOW_NOT_EXIST;

	if (!isLuaVaild())
		return WJAM_RES_LUA_NOT_INIT;
	
	/* ֪ͨ��Ƕ���� */
	g_gameMonitorHS.setEvent(GAME_EVENT_AM_REQ_CHALLENGE);

	/* ����ǰ����ʾ */
	BringWindowToTop(hWnd);
	Sleep(200);

	vector<Rect> vecRt;
	for (int i = 0; i < 3; ++i)
	{
		if (g_gameMonitorHS.getFriendRect(_name, vecRt))
		{
			break;
		}
		g_log.writeLog(XLog::E_LOG_DEBUG, "�Ӻ����б��л�ȡ����λ��ʧ�� [���ѣ�%s]", _name);
	}

	int size = vecRt.size();
	if (size <= 0)
		return WJAM_RES_HS_OPERATE_ERROR;

	bool flag = false;
	Rect rt;
	for (auto iter = vecRt.begin(); iter != vecRt.end(); ++iter)
	{
		rt = *iter;
		flag = true;
		break;
	}

	if (!flag)
	{
		g_log.writeLog(XLog::E_LOG_DEBUG, "����λ�û�ȡ���� [���ѣ�%s]", _name);
		return WJAM_RES_HS_OPERATE_ERROR;
	}

	lua_State *L = g_luaEngine->GetLuaState();
	lua_getglobal(L, "reqChallenge");
	lua_pushinteger(L, (int)hWnd);	// ���ھ��
	//lua_pushstring(L, _name);		// 
	lua_pushinteger(L, g_gameMonitorHS.m_rectFriendWindow.width*0.82);
	lua_pushinteger(L, rt.y + rt.height*0.5);

	int res = checkLuaCallRes(lua_pcall(L, 3, 1, 0), L, "reqChallenge");
	return res;
}


DLL_EXPORT int WJAM::acceptChallenge()
{
	HWND hWnd = g_gameMonitorHS.getGameWindowHwnd();
	if (!hWnd)
		return WJAM_RES_WINDOW_NOT_EXIST;

	if (!isLuaVaild())
		return WJAM_RES_LUA_NOT_INIT;

	g_log.writeLog(XLog::E_LOG_DEBUG, "����ִ���Զ��� $BEGIN$��������ս");

	/* ����ǰ����ʾ */
	BringWindowToTop(hWnd);
	Sleep(200);

	int w = g_gameMonitorHS.m_width;
	int h = g_gameMonitorHS.m_height;
	g_gameMonitorHS.clickWindow(hWnd, w*0.5 - h*0.54*0.168, h*0.66);
	
	//lua_State *L = g_luaEngine->GetLuaState();
	//lua_getglobal(L, "acceptChallenge");
	//lua_pushinteger(L, (int)hWnd);
	//
	//
	//int res = checkLuaCallRes(lua_pcall(L, 1, 1, 0), L, "acceptChallenge");
	//g_gameMonitorHS.setGameStatus((WJAM_RES_OK == res) ? E_STATUS_HS_CHALLENGE_ACCEPT : E_STATUS_HS_AM_FAIL);
	//if (WJAM_RES_OK == res)
	{
		/* ֪ͨ��Ƕ����, ���ý����� */
		g_gameMonitorHS.setEvent(GAME_EVENT_AM_ACCEPT_CHALLENGE);
	}
	return WJAM_RES_OK;
}


DLL_EXPORT int WJAM::getGameStatusHS()
{
	return g_gameMonitorHS.getGameStatus();
}


DLL_EXPORT int WJAM::takeEvent()
{
	return g_gameMonitorHS.takeEvent();
}
DLL_EXPORT void WJAM::setGameStatusHS(const EnGameStatusHS& _status)
{
	g_gameMonitorHS.setGameStatus(_status);
}


//��ʼ��¯ʯʶ��Ӣ��
DLL_EXPORT int WJAM::InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile)
{
	return g_gameMonitorHS.InitKnowHeroLS(modelFile, deployFile, meanFile, labelFile);
}

//¯ʯʶ��Ӣ��
DLL_EXPORT void WJAM::SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback)
{
	g_gameMonitorHS.SetKnowHeroLSCallBack(pCallback);
}