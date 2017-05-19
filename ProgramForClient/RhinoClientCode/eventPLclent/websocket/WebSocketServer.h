#ifndef WEBSOCKETSERVICE_H
#define WEBSOCKETSERVICE_H


#include "WSMsgHandler.h"
#include <QThread>
#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>

#include <map>
#include <vector>

class WebSocketServer : public QThread, public WSMsgHandler_back
{
	Q_OBJECT
public:
	WebSocketServer(int _port, WSMsgHandler *_msgHandler);
	~WebSocketServer();
	
	virtual void GetSocketAndType(QWebSocket * pClient, Cweb_c_type _type);

	static WebSocketServer *inobgect();
	void sendMessage(QString massage, Cweb_c_type _type);
	void sendMessage(QByteArray massage, Cweb_c_type _type);
	
	void setEstablishwebsoocket(int post, WSMsgHandler *_msgHandler);//����websocket
public slots:
	// �ͻ��˽��������¼��ص�
	void onClientConnect();

	// �ͻ��˶Ͽ������¼��ص�
	void onClientDisconnect();



private:
	bool initService(int port, WSMsgHandler *_msgHandler);


private:
	QWebSocketServer *m_webSocketSrv;

	/* ������Ϣ�Ĵ����࣬ʹ���߼̳�WebSocketMsgHandler������Ϣ������� */
	WSMsgHandler *m_msgHandler;

	std::map<Cweb_c_type, QWebSocket*> m_Client_map;
	int webpost;
};

#endif // WEBSOCKETSERVICE_H
