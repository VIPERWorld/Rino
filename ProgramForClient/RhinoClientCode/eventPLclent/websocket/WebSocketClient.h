#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include "WSMsgHandler.h"

#include <QObject>
#include <QtWebSockets/QWebSocket>


class WebSocketClient : public QObject
{
	Q_OBJECT

public:
	WebSocketClient(QString _name="", QObject *parent=NULL);
	~WebSocketClient();
	
public:
	/**
	* \brief:	���� webSocket ����
	* \param:	[_url]: ����url ��QUrl(QStringLiteral("ws://localhost:1234"))��
	* \param:	[_msgHandler]: ��Ϣ�������ָ��
	* \return:	true: ���ӳɹ�;	false: ����ʧ��;
	*/
	bool connToServer(const QUrl &_url, WSMsgHandler *_msgHandler);


	/* ������Ϣ */
	bool sendMsg(const QString& _msg);

	public slots:
	void onConnected();
	void onCloseClient();

	bool isValid() { return m_webSocket.isValid(); }

private:
	QWebSocket m_webSocket;

	WSMsgHandler *m_msgHandler;

	QUrl m_url;
};

#endif // WEBSOCKETCLIENT_H
