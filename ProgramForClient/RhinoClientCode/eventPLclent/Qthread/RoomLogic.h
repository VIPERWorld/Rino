#ifndef GAMEQTHREAD_H
#define GAMEQTHREAD_H
#include <QThread>
#include <QObject>
#include <QStringList>
#include <QMutex>
#include <string>

#include "automation/AutomateManager.h"
#include "../hook/HookManager.h"
#include "../hook/ExtendGameManager.h"
#include "clientsocket/WJSocketMessageDelegate.h"
#include "clientsocket/WJSocketMessage.h"
#include "common/WJUserInfoBaseStruct.h"
#include "server/comstruct.h"
#include "clientsocket/wjclientsocketlogic.h"
class ExtendGameManager;
class AutomateManager;
enum E_GameID;

struct struct_StartContest
{
	bool bTrue;//�Ƿ��������
	QJsonObject _QJsonObject_StartContest;

	struct_StartContest()
	{
		bTrue = false;
	}
};

class CRoomLogic : public QObject, public WJ::ISocketMessageDelegate
{
	Q_OBJECT
		//����
public:
	////////////////////////////////////
	//��Ҫ��Ƴ�ģ�幩��Ϸ�߼����õķ���
	//��ʼ
	bool Sys_OnStart();
	//��ʼ������
	void Sys_ReSetData();
	//��Ϸ�������쳣������Ҫ������ȷ�����ݣ���֤�����ٴε��һ������
	void Sys_OnDisposeErr(E_GameID igameid);
	//�뿪��ǰ��������ҳ
	void Sys_OnLeave();
	//��Ϸ����
	void Sys_OnEnd();
	//���뷿��
	void Sys_OnJoinRoom(bool bFirst);
	//�յ����Լ���صı�����Ϣ
	bool Sys_OnMess(const QByteArray &eventType, void *message, long *result);
	// ������Ϣ
	virtual void onConnected(bool connect, WJ::emSocketStatus status) override;
	// ������Ϣ
	virtual void onDisConnect() override;
	// TCP��Ϣת��
	virtual void onSocketMessage(WJ::WJSocketMessage* socketMessage) override;
	////////////////////////////////////

public:
	CRoomLogic();
	~CRoomLogic();
	static CRoomLogic* getInstance();


	void setipport();

	//�������Dllע���߳�
	void CreateMonitorThread();

protected:

public:

	// ����ƽ̨
	bool connect(QString cip, int iport);
	// ��������
	void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

	// �ر�ƽ̨
	bool SocketClose();
	// �Ƿ�����
	bool isConnect() const;
	// �Ƿ��½
	bool isLogin() const;

	// �������ݣ����ص���
	//SEL_SocketMessage ʹ�þ���
	//PlatformLogic()->sendData(1, 1, nullptr, 0,WJ_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
	//void sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, WJ::SEL_SocketMessage selector);
	// ���ָ����Ϣ����
	//SEL_SocketMessage ʹ�þ���
	//PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, WJ_SOCKET_CALLBACK(BindPhoneLayer::modifyUserInfoSelector, this));
	void addEventSelector(UINT MainID, UINT AssistantID, WJ::SEL_SocketMessage selector);
	// �Ƴ�ָ����Ϣ����
	void removeEventSelector(UINT MainID, UINT AssistantID);

public:

	//��Ա����
public:
	WJ::Room_ManageInfoStruct m_RoomInfo;//������Ϣ
	struct_StartContest m_struct_StartContest;//���һ������,һ�������������Ϣ
	UINT m_udeskid;//����id
	UINT m_udeskstation;//��λ
	bool m_bOwerUserid;//�Ƿ�Ϊ��������
	std::vector<WJ::UserInfoInRoomStruct> m_UserDatalist;//�������û��б�
	int m_game_connect_iter_tag;//��ǰ���ӵ���Ϸ�������ڵ�
private:
	WJ::WJClientSocketLogic* m_WJClientSocketLogic;
	// ƽ̨�Ƿ����ӱ�־
	bool			_connected;
	// ƽ̨�Ƿ��½��־
	bool			_logined;
	// ��������ַ��ip����������
	std::string		_serverAddress;
	// �������˿�
	INT				_serverPort;
public:
	//��Ҵ�����Ϸ״̬
	bool bInGame;	
	//��Ϸ��ʼ��رռ��LOL�Ƿ�����߳�
	bool m_IsLOLPVPExist = false;
};

#define  RoomLogic()	CRoomLogic::getInstance()
#endif // GAMEQTHREAD_H
