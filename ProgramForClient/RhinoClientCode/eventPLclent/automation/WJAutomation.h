// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AUTORUN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_EXPORT functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef _WJ_AUTORUN_H_
#define _WJ_AUTORUN_H_


#ifndef DLL_EXPORT
#define DLL_EXPORT __declspec(dllexport)
#endif

#define STR_LEN_64 64
#define STR_LEN_1024 1024


///*��¼����ص�����*/
//typedef void(*PFLOGINRST)(const char* account, int resultcode);
///*�����������ص�����*/
//typedef void(*PFCREATEROOMRST)(const char* roomname, const char* account, int resultcode);
///*���뷿�����ص�����*/
//typedef void(*PFJOINROOMRST)(const char* roomname, const char* account, int resultcode);



extern const char* WINDOW_TITLE_LOL_LOGIN;
extern const char* WINDOW_TITLE_LOL_CLIENT;
extern const char* WINDOW_TITLE_LOL_GAME;
extern const char* WINDOW_TITLE_HB_LOGIN;
extern const char* WINDOW_TITLE_HB_CLIENT;
extern const char* WINDOW_TITLE_HB_BATTLENET;
extern const char* WINDOW_TITLE_HB_FRIENDS;


#define WJAM_RES_OK  0							/* ִ�гɹ� */

#define WJAM_RES_D3D_ERROE		1				/* ִ�гɹ� */
#define WJAM_RES_HOOK_TIMEOUT	2


#define WJAM_RES_TIMEOUT  100					/* ������ʱ */
#define WJAM_RES_TIMEOUT_LOL_LOGIN  101

#define WJAM_RES_UNKNOWN_GAME  110				/* δ֪����Ϸ */
#define WJAM_RES_WINDOW_NOT_EXIST  111			/* ���ڲ����� */
#define WJAM_RES_CLOSE_PROCESS_FAILED  112		/* �ر���Ϸʧ�� */

#define WJAM_RES_LUA_NOT_INIT  501				/* LUA δ��ʼ�� */
#define WJAM_RES_LUA_PARAM_ERROR  502			/* LUA�������� */
#define WJAM_RES_LUA_EXCE_ERROR  2				/* LUA ִ���쳣 */

#define WJAM_RES_LOL_EXIST_IN_GAME  	1001	/* LOL ������Ϸ�� */
#define WJAM_RES_LOL_EXIST_IN_SERVER  	1002	/* LOL �Ѿ���½ѡ�����������δ������Ϸ */
#define WJAM_RES_LOL_SELECT_SERVER_FAILED 1003	/* LOL ѡ�������ʧ�� */
#define WJAM_RES_LOL_ROOM_EXIST			1004	/* �����Ѿ����� */
#define WJAM_RES_LOL_LOGIN_FROM_DISCONNECTION	1005	/* ��Ϸ���ڶ�������״̬ */

#define WJAM_RES_NEW_PLAYER				1006	/* ���û��ڵ�ǰ������������ */



#define WJAM_RES_HS_OPERATE_ERROR		2001

#include <string>

/**
* \date:	2016/08/22
* \brief:	�Զ����ص�����
* \param:	��
* \return:	[int _res] 0:�ɹ�;  _res!=0: �쳣
* \usage:
*/
typedef int(*PFuncCallBack)(int _res);
//typedef void(*PFuncCallbackStatusChanged)();

//#include "GameMonitorHS.h"
enum EnGameStatusHS;
class Callback_KnowHeroLS;

DLL_EXPORT class AMCallback
{
public:
	virtual void callbackStatusChandedHS(const EnGameStatusHS& _status) 
	{
		printf(" =====> ¯ʯ״̬�ı䣺%d", _status);
	};
};

namespace WJAM
{
	/*��Ϸ*/
	enum GAME_ID
	{
		GAME_ID_LOL = 1,    /*Ӣ������*/
		GAME_ID_HB,			/*¯ʯ��˵*/
	};

	/*����ID*/
	enum WIND_ID
	{
		WIND_ID_LOL_LOGIN = 1,		/*Ӣ�����˵�½����*/
		WIND_ID_LOL_CLIENT,			/*Ӣ�����˴�������*/
		WIND_ID_HB_LOGIN,			/*¯ʯ��˵��½����*/
		WIND_ID_HB_CLIENT,			/*¯ʯ��˵�ͻ��˴���*/
		WIND_ID_HB_BATTLENET,		/*¯ʯ��˵ս������*/
		WIND_ID_HB_FRIENDS,			/*¯ʯ��˵���Ѵ���*/
	};

	/*��Ϸģʽ*/
	enum GAME_MODE
	{
		GAME_MODE_CLASSIC = 1,		/*����ģʽ*/
		GAME_MODE_BATTLE,			/*��Ŧ����ս*/
	};

	/*��ͼ*/
	enum GAME_MAP
	{
		GAME_MAP_RIFT = 1,			/*�ٻ�ʦϿ��*/
		GAME_MAP_FOREST,			/*Ť������*/
		GAME_MAP_ABYSS,				/*������Ԩ*/
	};

	/*��սģʽ*/
	enum GAME_OBSERVE
	{
		GAME_OBSERVE_ALL = 1,		/*������*/
		GAME_OBSERVE_NONE,			/*������*/
		GAME_OBSERVE_HALL,			/*ͬ�������*/
		GAME_OBSERVE_FRIENDS,		/*����*/
	};

	/*��Ϸ����*/
	enum GAME_TYPE
	{
		GAME_TYPE_SELFCHOOSE = 1,   /*��ѡ*/
		GAME_TYPE_DRAFT,			/*����*/
		GAME_TYPE_RANDOM,			/*���*/
	};

	/*�����Ӫ*/
	enum GAME_SIDE
	{
		GAME_SIDE_RED = 1,			/*�췽*/
		GAME_SIDE_BLUE,				/*����*/
	};

	/*��¼��Ϣ*/
	typedef struct LOGININFO
	{
		int gameid;					/*��Ϸ*/
		char account[STR_LEN_64];   /*�˺�*/
		char password[STR_LEN_64];  /*����*/
		int serverid;				/*������ID*/
		char path[STR_LEN_1024];    /*��ִ���ļ�·��*/
	};

	/*������Ϣ*/
	typedef struct ROOMINFO
	{
		char roomname[STR_LEN_64];  /*��������*/
		char password[STR_LEN_64];  /*��������*/
		char account[STR_LEN_64];	/*�˺�*/
		int mode;					/*��Ϸģʽ*/
		int map;					/*��ͼ*/
		int playernum;				/*��������*/
		int observe;				/*��սģʽ*/
		int gametype;				/*��Ϸ����*/
		int side;					/*�����Ӫ*/
	};


	/* ��ʼ���ű� */
	DLL_EXPORT bool initLua(const char *_luaFile, AMCallback* _callback=nullptr);
	
	/* ��¼�ӿ� */
	DLL_EXPORT int login(LOGININFO* _loginInfo);
	
	/* ��������ӿ� */
	DLL_EXPORT int createRoom(ROOMINFO* _roomInfo);
	
	/* ���뷿��ӿ� */
	DLL_EXPORT int joinRoom(ROOMINFO* _roomInfo);
	
	/**
	* \brief:	��ʼ��Ϸ
	* \param:	[_gameId]	��Ϸ����id
	* \return:	[_isRoomOwner]	true: ����; false:�Ƿ���;
	*/
	DLL_EXPORT int startGame(WJAM::GAME_ID _gameId, bool _isRoomOwner);
	
	/**
	* \brief:	�ر���Ϸ
	* \param:	[_path]	��Ϸ·��
	* \return:	[int]	0: �رճɹ�; !=0:
	*/
	DLL_EXPORT int closeGame(GAME_ID _gameId);
	
	/**
	* \brief:	���ػ���ʾ��Ϸ�Ĵ���
	* \param:	[_gameId]	��Ϸ����id
	* \param:	[_visible]	true: ��ʾ;	false:	����;
	* \return:	void
	*/
	DLL_EXPORT void setGameWindowVisible(WJAM::GAME_ID _gameId, bool _visible);
	
	/*
	* \brief:	¯ʯ�Զ���������ʼ��
	* \param:	[_wsPort]: webSocket ����˿�
	* \param:	[_url]: ��ǶH5��URL
	* \param:	[_userName]: ���ز������������
	*/
	DLL_EXPORT int startupHS(int _wsPort, const char* _url, std::string& _playerName);
	
	
	//DLL_EXPORT int playHS(const char *_player);
	
	
	/**
	* \brief:	����Ӻ���
	* \param:	[_name]: ��������
	*/
	DLL_EXPORT int reqAddFriend(const char *_name);
	
	/**
	* \brief:	���ܺ����������
	* \param:	[_name]: ��������
	*/
	DLL_EXPORT int acceptFriendReq(const char *_name);
	
	/**
	* \brief:	������ս����
	* \param:	[_name]:	��ս�ĺ��ѵ�����
	*/
	DLL_EXPORT int reqChallenge(const char *_name);
	
	/**
	* \brief:	������ս
	*/
	DLL_EXPORT int acceptChallenge();
	
	/**
	* \brief:	��ȡ¯ʯ��˵����Ϸ״̬
	*/
	DLL_EXPORT int getGameStatusHS();
	DLL_EXPORT void setGameStatusHS(const EnGameStatusHS& _status);

	DLL_EXPORT int takeEvent();

	//��ʼ��¯ʯʶ��Ӣ��
	DLL_EXPORT int InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile);

	//¯ʯʶ��Ӣ��
	DLL_EXPORT void SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback);
}

#endif // !_WJ_AUTORUN_H_