#include "WebSocketClient.h"

WebSocketClient::WebSocketClient(QString _name, QObject *parent)
	: QObject(parent)
	, m_msgHandler(NULL)
{
	m_webSocket.setObjectName(_name);
	
	/* �������ӺͶ������źŲ� */
	connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
	connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onCloseClient);
}

WebSocketClient::~WebSocketClient()
{

}

//#include <QEventLoop>

bool WebSocketClient::connToServer(const QUrl &_url, WSMsgHandler *_msgHandler)
{
	/* ������Ϣ�������ָ�� */
	m_msgHandler = _msgHandler;
	
	/* �������url */
	m_url = _url;


	/* ���ӵ� webSocket */
	m_webSocket.open(_url);
	

	//if (m_webSocket.isValid())
	return true;
}

#include <Windows.h>
void WebSocketClient::onConnected()
{
	OutputDebugStringA("WebSocket: connect server successful!");
	if (m_msgHandler)
	{
		connect(&m_webSocket, &QWebSocket::textMessageReceived, m_msgHandler, &WSMsgHandler::onRecvMsg);
		m_msgHandler->callbackConn(&m_webSocket, false);
	}
}


bool WebSocketClient::sendMsg(const QString& _msg)
{
	if (!m_webSocket.isValid())
	{
		qDebug() << "WebSocket �����ã� (" << m_url.toString() << ")";
		return false;
	}

	m_webSocket.sendTextMessage(_msg);
	qDebug() << "Send MSG:" << _msg;
	return true;
}


void WebSocketClient::onCloseClient()
{
	m_webSocket.close();
}