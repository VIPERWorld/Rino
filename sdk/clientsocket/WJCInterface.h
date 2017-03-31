
#ifndef WJ_CINTERFACE_FILE
#define WJ_CINTERFACE_FILE
#include "common/LoadWindowsMess.h"
#include <QtCore>
#include <QObject> 
#include <QtNetwork>
#include <QWebSocket>
#include <qbytearray.h>
#include <QString>
#include "common/Common.h"
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
#include "websocket/WebSocketMessInfo.h"
#include "clientsocket/WJSocketMessage.h"

#include "websocket/WebSocketServer.h"
#include "websocket/DataDrocessing.h"
#include "Qthread/RoomLogic.h"
#include "WJPlatformLogic.h"

/********************************************************************************************/

///����ӿڶ���

/********************************************************************************************/

class IBaseMyGameRoom;
class IBaseMyHallLogic;

///Ϊ��Ϸ��װ�Ĵ���H5ͨ�Žӿ� 
class IBaseMyDataDrocessing : public QObject
{
	Q_OBJECT
	///�ӿں���
public:
	IBaseMyDataDrocessing();
	~IBaseMyDataDrocessing();

	virtual void Processing(int iMSGID, QJsonObject &_TYPE) = 0;//���ݴ���
	virtual void InitData(IBaseMyHallLogic* phall, IBaseMyGameRoom* proom) = 0;//����������Ϸ��ص��ౣ��
};

///Ϊ��Ϸ��װ�Ĵ�������߼��ӿ�
class IBaseMyHallLogic : public QObject
{
	Q_OBJECT

	///�ӿں���
public:
	IBaseMyHallLogic();
	~IBaseMyHallLogic();
	//����������Ϸ��ص��ౣ��
	virtual void InitData(IBaseMyDataDrocessing* pDataDrocessing, IBaseMyGameRoom* proom) = 0;
	//�յ����Լ���صı�����Ϣ
	virtual bool OnMess(const QByteArray &eventType, void *message, long *result) = 0;
	// TCP��Ϣת��
	virtual void onSocketMessage(WJ::WJSocketMessage* socketMessage) = 0;
};

///Ϊ��Ϸ��װ�Ĵ������߼��ӿ� 
class IBaseMyGameRoom : public QObject
{
	Q_OBJECT
	///�ӿں���
public:
	IBaseMyGameRoom();
	~IBaseMyGameRoom();

	virtual void InitData(IBaseMyDataDrocessing* pDataDrocessing, IBaseMyHallLogic* phall) = 0;//����������Ϸ��ص��ౣ��
	//���뷿��
	virtual void OnJoinRoom(bool bFirst) = 0;
	//��ʼ
	virtual bool OnStart() = 0;
	//��ʼ������
	virtual void ReSetData() = 0;
	//��Ϸ�������쳣������Ҫ������ȷ�����ݣ���֤�����ٴε��һ������
	virtual void OnDisposeErr() = 0;
	//�뿪��ǰ��������ҳ
	virtual void OnLeave() = 0;
	//��Ϸ����
	virtual void OnEnd() = 0;
	//�յ����Լ���صı�����Ϣ
	virtual bool OnMess(const QByteArray &eventType, void *message, long *result) = 0;
	// ������Ϣ
	virtual void onConnected(bool connect, WJ::emSocketStatus status) = 0;
	// ������Ϣ
	virtual void onDisConnect() = 0;
	// TCP��Ϣת��
	virtual void onSocketMessage(WJ::WJSocketMessage* socketMessage) = 0;
};
#endif//WJ_CINTERFACE_FILE