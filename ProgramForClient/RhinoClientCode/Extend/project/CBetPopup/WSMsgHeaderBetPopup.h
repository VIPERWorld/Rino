#pragma once

#ifndef WSMSGHEADERBETPOPUP_H
#define WSMSGHEADERBETPOPUP_H

#include "websocket/WSMsgHandler.h"
#include "cbetpopup.h"
#include "hook/WSMessageId.h"

class WSMsgHandleBetPopup : public WSMsgHandler
{
public:
	WSMsgHandleBetPopup(CBetPopup* _mainWin)
		: m_mainWin(_mainWin)
		, m_sockClient(NULL)
	{
	}

	virtual void onRecvMsg(const QString &_msg, Cweb_c_type _type)
	{
		QWebSocket *pSocket = qobject_cast<QWebSocket *>(sender());

		printf("���������յ� Web Socket ��Ϣ: %s", _msg.toStdString().c_str());

		QJsonParseError jsonError;
		QJsonDocument jsonDoc = QJsonDocument::fromJson(_msg.toLocal8Bit(), &jsonError);
		if (jsonError.error != QJsonParseError::NoError)
		{
			printf("Web Socket message JSON parser error.");
			return;
		}

		if (!jsonDoc.isObject())
		{
			printf("Web Socket message JSON data error: is not json object.");
			return;
		}

		QJsonObject obj = jsonDoc.object();
		if (!obj.contains("MSGID"))
		{
			printf("WSMsgHandleBetPopup [WSMsgHandlerHS::onRecvMsg]: JSON data error: [msgid] is not exist.");
			return;
		}

		int msgId = obj["MSGID"].toInt();

		switch (msgId)
		{
			//case GAME_EVENT_AM_BEGIN:
			//	m_mainWin->beginLoading();
			//	break;
			//case GAME_EVENT_GAME_BEGIN:
			//	m_mainWin->startGame();
			//	break;
			//case WS_MSGID_2007:
			//	break;

		default:
			break;
		}
	}

	virtual void callbackConn(QWebSocket *_socket, bool _isSrv)
	{
		//m_mainWin->setWebSockClient(_socket);
		m_sockClient = _socket;
		printf("WSMsgHandleBetPopup�� H5 Websoekcet���ӳɹ�\n");
	}

	virtual void callbackDisConn(QWebSocket *_socket, bool _isSrv)
	{
		m_sockClient = nullptr;
	}

	/**
	*	��Ϣ��ʽ��{"MSGID":"2000"}
	*/
	void sendMsg(QString& _msg, QWebSocket* _websocket = nullptr)
	{
		QJsonObject jsonRoot;
		jsonRoot.insert("MSGID", _msg);

		QJsonDocument jsonDoc;
		jsonDoc.setObject(jsonRoot);

		QString msg = jsonDoc.toJson(QJsonDocument::Compact);

		if (_websocket)
		{
			_websocket->sendTextMessage(msg);
		}
		else if (m_sockClient)
		{
			m_sockClient->sendTextMessage(msg);
		}

		printf("WebSocket : Server send msg: %s\n", msg.toStdString().c_str());
	}

public:
	CBetPopup *m_mainWin;
	QWebSocket *m_sockClient;
};

#endif