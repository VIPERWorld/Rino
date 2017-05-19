#pragma once


#include "hook/HookD3D9.h"
#include "WebSocket/WebSocketServer.h"


#include "MyWebView.h"
#include "MyDialog.h"
#include "ui_mainwindow.h"
#include "hook/HookEventParam.h"

class WSMsgHandlerHS;


class MainWindow : public QDialog, public IDataRecive, public EventSignal, public QAbstractNativeEventFilter/* : public QMainWindow*/ /**/
{
	Q_OBJECT

public:
	MainWindow(HWND _hWndParent);
	~MainWindow();

public:
	/**
	* \brief:	�ƶ�ע�봰�ڣ���
	*/
	void moveMyWindow();

	void beginAutomation();
	void endAutomation();

	/* ϵͳ�¼����ش����� */
public:
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *) Q_DECL_OVERRIDE
	{
		if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
		{
			MSG* pMsg = reinterpret_cast<MSG*>(message);
			switch (pMsg->message)
			{
			case WM_MOVE + WM_USER:
				this->moveMyWindow();
				break;

			case WM_USER + 0x0100:
				this->beginAutomation();
				break;

			case WM_USER + 0x0101:
				this->endAutomation();
				break;

			default:
				break;
			}
		}
		return false;
	}


	MyDialog* getCoverWin()
	{
		return m_dlgCover;
	}

	void setWebSockClient(QWebSocket* _ws)
	{
		m_sockClient = _ws;
	}

public:
	virtual void DoData(void* pData, size_t size);
	virtual void SendData(void* pData, size_t maxSize);

	virtual void timerEvent(QTimerEvent *_event);

	void end() { m_isExitFlag = true; };



private:
	/**
	* \brief:	��ʼ��d3d
	*/
	bool initD3D();


	public slots:
	void onLoadUrlFinished(bool);
	void onTimer();


public:
	
	/**
	* \brief:	webSocket ����
	*/
	WebSocketServer *m_webSocketSrv;

	/**
	* \brief:	webSocket ����˻ص�������
	*/
	WSMsgHandlerHS *m_WSMsgHandler;

	/**
	* \brief:	���ӳɹ��Ŀͻ���ͨѶ webSocket
	*/
	QWebSocket *m_sockClient;


	/**
	* \brief:	�����ڴ洫�ݲ���
	*/
	ShareParamHS m_shareParam;

	MyDialog *m_dlgCover;
	MyWebView *m_webView;
	//QWebEngineView* m_webView;

	/**
	* \brief:	�����壨¯ʯ���Ĵ��ھ��
	*/
	HWND m_hWndParent;


	QPoint m_ptTick;

	bool m_isHooked;

	/**
	* \brief:	�رձ�ʶ
	*/
	bool m_isExitFlag;

	int m_eventId;
};
