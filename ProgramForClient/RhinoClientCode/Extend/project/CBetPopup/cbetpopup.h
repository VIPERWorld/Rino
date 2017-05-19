#ifndef CBETPOPUP_H
#define CBETPOPUP_H

#include "cbetpopup_global.h"


#include <QtCore/QtCore>
//#include <QtCore/QCoreApplication>
#include <QPropertyAnimation>

#ifdef _DEBUG
#pragma comment(lib, "qtmaind.lib")
#pragma comment(lib, "Qt5Cored.lib")
#pragma comment(lib, "Qt5Guid.lib")
#pragma comment(lib, "Qt5WebSocketsd.lib")
#pragma comment(lib, "Qt5Widgetsd.lib")
#pragma comment(lib, "Qt5WebEngineWidgetsd.lib")
#pragma comment(lib, "Qt5Networkd.lib")
#else
#pragma comment(lib, "qtmain.lib")
#pragma comment(lib, "Qt5Core.lib")
#pragma comment(lib, "Qt5Gui.lib")
#pragma comment(lib, "Qt5WebSockets.lib")
#pragma comment(lib, "Qt5Widgets.lib")
#pragma comment(lib, "Qt5WebEngineWidgets.lib")
#pragma comment(lib, "Qt5Network.lib")
#endif // DEBUG

#include <QDialog>
#include <qscrollarea>
#include <QMainWindow>
#include <QtWebEngineWidgets/QWebEngineView>
#include "WebSocket/WebSocketServer.h"

class WSMsgHandleBetPopup;
class DlgWebView_00000002;

class CBETPOPUP_EXPORT CBetPopup : public QDialog {

	Q_OBJECT

public:
	CBetPopup(HWND _hWnd, HWND _hWndTell, const QRect& _rect);
	~CBetPopup();

public:
	void initialize(HWND _hWndHS, QUrl& _url, int _wsPort);
	void beginLoading();
	void endLoading();
	void startGame();
	void showErrorPage();
	void endGame();


public:
	virtual void timerEvent(QTimerEvent *);

	//ҳ����ؽ���ʱ�Ĳ۴�����
	public slots:
	void onLoadUrlFinished(bool _status);
	void onAnimateFinished();
	QWidget* getdlgMatch(){ return (QWidget*)m_dlgMatch; }
	//���ô�����괩͸(1 ��͸ 0 ����͸)
	void setWindMousePenetrate(bool bPenetrate);

private:
	//webSocket ����	
	WebSocketServer *m_webSocketSrv;

	//webSocket ����˻ص�������
	WSMsgHandleBetPopup *m_WSMsgHandler;

	//���ӳɹ��Ŀͻ���ͨѶ webSocket
	QWebSocket *m_sockClient;

	//Ѻעҳ��
	DlgWebView_00000002 *m_dlgMatch;

	//����λ��
	QRect m_rect;

	//Ƕ��ĸ����ڵĴ��ھ��
	HWND m_hWndParent;

	//QT����
	QPropertyAnimation* m_anim;
};

#endif // CBETPOPUP_H
