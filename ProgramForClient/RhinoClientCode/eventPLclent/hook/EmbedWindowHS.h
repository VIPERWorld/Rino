#ifndef EMBEDWINDOWHS_H
#define EMBEDWINDOWHS_H


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
#include <QMainWindow>
#include <QtWebEngineWidgets/QWebEngineView>
#include "WebSocket/WebSocketServer.h"

class WSMsgHandlerHS;
class DlgLoading;
class DlgWebView;
class DlgTouchWebView;
class CDlgBetView;

class Q_DECL_EXPORT EmbedWindowHS : public QDialog
{
	Q_OBJECT

public:
	EmbedWindowHS(HWND _hWnd, HWND _hWndTell, const QRect& _rect);
	~EmbedWindowHS();

public:
	void initialize(HWND _hWndHS, QUrl& _url, WebSocketServer *pwebSocketSrv);
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
	QWidget* getdlgLoading(){ return (QWidget*)m_dlgLoading; }
	//���ô�����괩͸(1 ��͸ 0 ����͸)
	void setWindMousePenetrate(bool bPenetrate);
private:

	//webSocket ����	
	WebSocketServer *m_webSocketSrv;

	//���ӳɹ��Ŀͻ���ͨѶ webSocket
	QWebSocket *m_sockClient;

	//��Ƕ��Ϸ����ҳ��
	DlgLoading *m_dlgLoading;

	//��Ƕƥ��ҳ��
	DlgWebView *m_dlgMatch;

	//�����Ӵ���
	DlgTouchWebView* m_pChildWind;

	//Ѻע����
	CDlgBetView* m_pBetView;

	//����λ��
	QRect m_rect;

	//Ƕ��ĸ����ڵĴ��ھ��
	HWND m_hWndParent;

	//QT����
	QPropertyAnimation* m_anim;
};

#endif // EMBEDWINDOWHS_H
