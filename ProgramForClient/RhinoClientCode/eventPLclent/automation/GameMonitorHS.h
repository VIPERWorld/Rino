#ifndef _ImageRecognition_H_
#define _ImageRecognition_H_



#define WINDOW_TITLE_HS		"¯ʯ��˵"

#include <process.h>
#include <Windows.h>
#include <string>

#include "hook/ICapture.h"

#include "hook/IDataRecive.h"
#include "hook/EventSignal.h"
#include "hook/HookEventParam.h"

#include "identification/hs_identification.h"

#include "Monitor_Status_HS.h"

#include <vector>
class CLuaEngine;

typedef std::map<std::string, int>	MAP_StringInt;

class Callback_KnowHeroLS
{
public:
	//��֪������ѡӢ��ʱ�����лص�
	virtual void onDoForKnowHeroLS(std::string Hero) = 0;
};

class GameMonitorHS : public IDataRecive, public CaptureCallback, public EventSignal
{
public:
	GameMonitorHS();
	~GameMonitorHS();

public:
	/*
	* \brief:	��ʼ��
	* \param:	[_wsPort]: WEB SOCKET �˿�
	* \param:	[_urlMatch]: H5 WEB url ��ַ1 : ƥ��ҳ
	* \param:	[_urlCover]: H5 WEB url ��ַ2 : ��������ҳ
	*/
	bool initEmbedWinHS(int _wsPort, const char* _urlMatch, const char* _urlCover);

	bool initResource();

	/**
	* \brief:	���ݸ���, ���ص�ǰ����Ϸ״̬
	*/
	EnGameStatusHS& update();

	void saveTempImg(const char* _name, int _val, Mat& _mat);

	bool getWindowMatHS(HWND hWnd, Mat& mat);
	//Mat* getRanderData();


	/**
	* \brief:	ͼ��ʶ���ȡ�û�����
	* \param:	[_name]: �û���
	*/
	bool getPlayerName(HWND _hWnd, std::string& _name);

	/*
	* \brief:	ͼ��Աȣ���ָ��ͼ���Ŀ��ͼ��Ŀ��ͼ��ΪHOOK��ȡ����ͼ��
	* \param:	[_image]:	Ҫʶ����жԱȵ�ͼ��
	* \return:	int: ͼƬ��ƥ���
	*/
	int imageContrast(Mat& mat, const char* _image, bool _saveImg = false);

	//bool imageContrastRect(const char* _image, float _x, float _y, float _w, float _h);
	int imageContrastRect(Mat& mat, const char* _image, Rect& _rt, bool _saveImg = false);
	//
	///*
	//* \brief:	ͼ��ʶ�𣬺�Ԥ���õ�����ͼƬ���жԱ�ʶ��
	//* \return:	ƥ���ͼ���ID���ƣ�null��ʾ������ͼ�񶼲�ƥ��
	//*/
	//char* imageRecognition();


	/**
	* \brief:	����ʶ��
	*/
	std::string textRecognition(const char* _name, Mat& _mat);

	/**
	* \brief:	��ȡ¯ʯ��˵�ĺ��ѹ���������
	*/
	Rect& getFriendWindowRect() { return m_rectFriendWindow; };
	bool getFriendRect(const char* _name, vector<Rect>& _vecRt);


	HWND getWindowRect(RECT& _rt)
	{
		HWND hWnd = ::FindWindowA(NULL, "¯ʯ��˵");
		if (!hWnd)
			return hWnd;

		::GetWindowRect(hWnd, &_rt);
		return hWnd;
	}


	bool textCompare(std::string& _text1, std::string& _text2);

	virtual void DoData(void* pData, size_t size);
	virtual void SendData(void* pData, size_t maxSize);
	virtual void* _callback(void* _data);

	//bool sendWebSocketMsg(const char* _msg);

	//virtual void onClientRecvMsg(const QString &_msg);
	//virtual void onSrvRecvMsg(const QString &_msg);
	//virtual void callbackConn(QWebSocket *_socket);


	void setGameStatus(const EnGameStatusHS& _status);

	EnGameStatusHS& getGameStatus();


	void setEvent(int _eventId) { m_eventId = _eventId; };
	int takeEvent()
	{
		int id = m_eventId;
		m_eventId = 0;
		return id;
	};



	HWND getGameWindowHwnd() { return ::FindWindowA(NULL, WINDOW_TITLE_HS); }

	void showFriendWindow(HWND _hWnd);

	void setGameWindwoSize(int _w, int _h);

	std::string getGameImage(Mat& mat, int& _size);

	void clickWindow(HWND _hWnd, float _x, float _y, int _sleep = 0);

	//��ʼ��¯ʯʶ��Ӣ��
	int InitKnowHeroLS(std::string &modelFile, std::string &deployFile, std::string &meanFile, std::string &labelFile);

	//����¯ʯʶ��Ӣ�۵Ļص���
	void SetKnowHeroLSCallBack(Callback_KnowHeroLS* pCallback);
private:
	/**
	* \brief:	��ʼ��D3D HOOK
	*/
	//bool initD3D();

	/**
	* \brief:	��ʼ����������
	*/
	bool initShareTexture(CaptureInfo& _info);


	int getImgThreshold(const char* _name)
	{
		auto iter = m_mapImgThreshold.find(_name);
		if (m_mapImgThreshold.end() == iter)
			return -1;

		return iter->second;
	}


public:
	/**
	* \brief:	ͼ��ʶ����
	*/
	CHSIdentification m_imageIdentifucation;

	
	//WebSocketServer* m_webSocketSrv;
	//QWebSocket* m_webSocketClient;
	std::string m_appPath;

	bool m_isVaild;

	/**
	* \brief:	��Ϸ��ǰ״̬
	*/
	EnGameStatusHS m_gameStatus;

	int m_width;
	int m_height;

	Rect m_rectFriendWindow;


	HANDLE m_initThreadHandle;

	CShareMemory *m_shareMem;

	int m_eventId;


	CLuaEngine m_luaEngine;

	/* ͼ��Ա���ֵ */
	MAP_StringInt m_mapImgThreshold;


	vector<std::string> m_vecImg;
	bool m_binit_KnowHeroLS;
	//Mat* m_matTemp;

	Callback_KnowHeroLS* m_pCallback_KnowHeroLS;//¯ʯʶ��Ӣ�۵Ļص���
};
//
//
#endif // !_ImageRecognition_H_
