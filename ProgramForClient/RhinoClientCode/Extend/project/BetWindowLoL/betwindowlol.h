#ifndef BETWINDOWLOL_H
#define BETWINDOWLOL_H

#include "betwindowlol_global.h"

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

class WSMsgHandleLOL;
class DlgLoading;
class DlgWebView;
class DlgTouchWebView;

class BETWINDOWLOL_EXPORT BetWindowLoL : public QDialog {

	Q_OBJECT

public:
	BetWindowLoL(HWND _hWnd, HWND _hWndTell, const QRect& _rect);
	~BetWindowLoL();

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
	WSMsgHandleLOL *m_WSMsgHandler;

	//���ӳɹ��Ŀͻ���ͨѶ webSocket
	QWebSocket *m_sockClient;

	//Ѻעҳ��
	DlgWebView *m_dlgMatch;

	//����λ��
	QRect m_rect;

	//Ƕ��ĸ����ڵĴ��ھ��	LOL���ھ��
	HWND m_hWndParent;

	//QT����
	QPropertyAnimation* m_anim;
};

#endif // BETWINDOWLOL_H
