#include "RoomLogic.h"
#include <thread>
#include "clientsocket/WJSocketMessage.h"
#include "web/WebChannelExt.h"
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QtWebChannel/QtWebChannel>
#include "common/Common.h"
#include <QTextCodec>
#include <qobjectdefs.h>
#include "clientsocket/commonfunction_c.h"
#include "web/WJMainWindow.h"
#include "common/WJGameInfo.h"
#include "WebSocketMessInfo.h"
#include "GameMonitorHS.h"
#include "common/LoadWindowsMess.h"
#include "websocket\WebSocketServer.h"
#include "web/WJMainWindow.h"
#include "websocket/DataDrocessing.h"
#include "common/LoadWindowsMess.h"
#include "websocket/WebSocketMessInfo.h"
#include "GameMonitorHS.h"
#include "message/WJNetRoomMessage.h"
#include "WJPlatformLogic.h"
void DEBUG_WriteLog(const wchar_t* filename, const void* p, int l)
{
#ifdef _DEBUG
	HANDLE	hFile;
	DWORD	NumberOfBytesWritten;
	hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	WriteFile(hFile, p, l, &NumberOfBytesWritten, NULL);
	CloseHandle(hFile);
#endif
}

CRoomLogic* g_pCRoomLogic = nullptr;

CRoomLogic::CRoomLogic()
{
	bInGame = false;
	m_WJClientSocketLogic = new WJ::WJClientSocketLogic(this, "Room");

	Sys_ReSetData();
	char szAppPath[MAX_PATH];
	GetModuleFileNameA(NULL, szAppPath, MAX_PATH);
	std::string str;
	str = CommonData()->currentpath.toLocal8Bit();
	str += "/luaScript/main.out";
	AutomateManager::init(str.c_str());
}


CRoomLogic *CRoomLogic::getInstance()
{
	if (nullptr == g_pCRoomLogic)
		g_pCRoomLogic = new CRoomLogic();
	return g_pCRoomLogic;
}

bool CRoomLogic::Sys_OnStart()
{
	int itage = CommonData()->m_NowContest - WJ::g_eventid_first;
	bool bEvent = false;//�Ƿ�Ϊ����
	if (itage > 0)
	{//����
		bEvent = true;
	}

	m_struct_StartContest.bTrue = false;
	//������ť��ʱ��ס
	DataDrocessing::inobgect()->OnContestButLock();
	//////////////////////////////////////////////////////
	if (!bEvent)
	{//����solo�����ǵ�����
		E_GameID igameid = e_gameid_10000001;
		QString award = "0";
		QString centestname;
		QString matchpoint;
		QJsonObject data = m_struct_StartContest._QJsonObject_StartContest;
		int i = 0;
		int money = 0; // ���ֻ����
		if (data.contains("CENTEST_PT"))
		{
			std::string pt = data.value("CENTEST_PT").toString().toStdString();
			i = atoi(pt.c_str());
		}
		if (data.contains("CENTEST_PS"))
		{
			std::string ps = data.value("CENTEST_PS").toString().toStdString();
			money = atoi(ps.c_str());
		}

		if (data.contains("GAMENAME"))
		{//�����ɹ�
			std::string _str = data.value("GAMENAME").toString().toStdString();
			igameid = (E_GameID)atoi(_str.c_str());
		}

		if (data.contains("I64AWARD"))
		{//����
			award = data.value("I64AWARD").toString();
		}

		if (data.contains("CENTEST_NAME"))
		{//��������
			centestname = data.value("CENTEST_NAME").toString();
		}

		if (data.contains("CENTEST_MATCHPOINT"))
		{//��������
			matchpoint = data.value("CENTEST_MATCHPOINT").toString();
		}
		//i = 1 ����  || i = 2 ���

		if (i == 1)
		{
			//���ֹ�����
			if (CommonData()->userinfo.dwPoint < money)
				return false;
		}
		else if (i == 2)
		{
			if (CommonData()->userinfo.i64Money < money)
				return false;

		}
	}

	//WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("����������Ϸ������������Ϸ�ڲ���ƥ�䣮"));
	bool bCanStart = false;//�Ƿ���Կ�ʼ
	do
	{
		if (!CommonData()->bExecutionAutomationDoing)
		{
			WJAM::closeGame(WJAM::GAME_ID_LOL);
			CommonData()->bExecutionAutomationDoing = true;
			if (CommonData()->m_GameRoomList.size() <= 0)
			{
				WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("���޷�����Ϣ�޷�����,���Ժ�����"));
				CommonData()->bExecutionAutomationDoing = false;
				break;
			}

			//����
			bCanStart = true;
		}
	} while (0);


	if (!bCanStart)
	{//���ܿ�ʼ
		DataDrocessing::inobgect()->OnContestButUnlock();
		return false;
	}

	CommonData()->bExecutionAutomationDoing = false;
	Sys_ReSetData();
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		return pgr->OnStart();
	}

	return true;
}

void CRoomLogic::CreateMonitorThread()
{
	WJ_gameid_10000001::CMyGameRoom* p = (WJ_gameid_10000001::CMyGameRoom*)CommonData()->GetMyGameRoom(e_gameid_10000001);
	if (p != nullptr)
	{
		p->CreateMonitorThread();
	}
}

void CRoomLogic::setipport()
{
	if (m_game_connect_iter_tag < CommonData()->m_GameRoomList.size())
	{
		connect(CommonData()->m_GameRoomList.at(m_game_connect_iter_tag).strIP, CommonData()->m_GameRoomList.at(m_game_connect_iter_tag).uport);
	}
	else if (m_game_connect_iter_tag == CommonData()->m_GameRoomList.size())
	{
		WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("��Ϸ��������½ʧ��,���Ժ���ս!"));
		Sys_OnDisposeErr(CommonData()->m_NowGame);
	}
	else
	{
		//ʣ�µĲ�����
	}
}
void CRoomLogic::Sys_OnJoinRoom(bool bFirst)
{
	SystemLog()->LogToFile("sdp_test c ������Ϸ������---");
	if (bFirst)
	{
		m_game_connect_iter_tag = 0;
	}
	else
	{
		++m_game_connect_iter_tag;
	}

	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->OnJoinRoom(bFirst);
	}

	if (isConnect())
	{//�Ѿ�����������
		WJ::RoomMessDate_user_action_login login;
		(UINT)login.uuserid = CommonData()->userinfo.dwUserID;
		(UINT)login.contestid = CommonData()->m_NowContest;
		sendData((UINT)WJ::e_gs_mess_user_action, (UINT)WJ::e_gs_ass_user_action_login, &login, sizeof(login));
		SystemLog()->LogToFile("sdp_test c ��Ϸ������---���͵�½��Ϣ");
	}
	else
	{
		setipport();
	}
}

//��ʼ������
void CRoomLogic::Sys_ReSetData()
{
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->ReSetData();
	}

	m_udeskid = 0;//����id
	m_udeskstation = 0;//��λ
	m_bOwerUserid = false;//�Ƿ�Ϊ��������
	m_UserDatalist.clear();
	WJ::Room_ManageInfoStruct cmd;
	m_RoomInfo = cmd;
	m_struct_StartContest.bTrue = false;
}

//��Ϸ�������쳣������Ҫ������ȷ�����ݣ���֤�����ٴε��һ������
void CRoomLogic::Sys_OnDisposeErr(E_GameID igameid)
{
	SystemLog()->LogToFile("sdp_test ��Ϸ�������쳣������Ҫ������ȷ�����ݣ���֤�����ٴε��һ������(%d)", igameid);
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->OnDisposeErr();
	}
	RoomLogic()->Sys_ReSetData();
	//ȡ����¼��Ϸ������
	WJ::RoomMessDate_user_action_login_cancel login_cancel;
	login_cancel.uuserid = CommonData()->userinfo.dwUserID;
	if (RoomLogic()->isConnect())
	{
		RoomLogic()->sendData(WJ::e_gs_mess_user_action, WJ::e_gs_ass_user_action_login_cancel, &login_cancel, sizeof(login_cancel));
	}
	else
	{
		SystemLog()->LogToFile("sdp_test c ���ڷ��䣬��ʼ�����˷�");
		//�˷�
		WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_apply, (UINT)WJ::e_hs_ass_apply_restitution, NULL, 0);
		DataDrocessing::inobgect()->OnContestButUnlock();
	}
}

//�뿪��ǰ��������ҳ
void CRoomLogic::Sys_OnLeave()
{
	//WJMainWindow()->DoMsgBoxSignal();
	bInGame = false;
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->OnLeave();
	}
	SocketClose();
	WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_apply, (UINT)WJ::e_hs_ass_apply_restitution, NULL, 0);
	Sys_ReSetData();
	SystemLog()->LogToFile("sdp_test c OnLeave()---CommonData()->m_NowContest=%d", CommonData()->m_NowContest);
	DataDrocessing::inobgect()->OnContestButUnlock();
	CommonData()->m_NowRoom = 0;
	CommonData()->m_NowContest = 0;
	CommonData()->m_NowErea = 0;
	WJMainWindow()->m_LolSoloId = 0;
	WJMainWindow()->onShowGameFinishwind(false);
}

//��Ϸ����
void CRoomLogic::Sys_OnEnd()
{
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->OnEnd();
	}

	WJ::PlatformLogic().m_bApplySuc = false;
	m_udeskid = 0;//����id
	m_udeskstation = 0;//��λ
	m_bOwerUserid = false;//�Ƿ�Ϊ��������
	m_UserDatalist.clear();
	std::memset(&m_RoomInfo, 0, sizeof(m_RoomInfo));
	m_struct_StartContest.bTrue = false;
	SystemLog()->LogToFile("sdp_test c OnEnd()---CommonData()->m_NowContest=%d", CommonData()->m_NowContest);
}

CRoomLogic::~CRoomLogic()
{
	Sys_ReSetData();
}

//�յ����Լ���صı�����Ϣ
bool CRoomLogic::Sys_OnMess(const QByteArray &eventType, void *message, long *result)
{
	if (eventType != "windows_generic_MSG")
	{
		return false;
	}

	try
	{
		IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
		if (nullptr != pgr)
		{
			pgr->OnMess(eventType, message, result);
		}
		PMSG msg = (PMSG)message;
		switch (msg->message)
		{
		case WM_GAME_MESS_2022:
		{
			char str[MAX_PATH] = { 0 };
			sprintf_s(str, "����ʧ�ܣ����Ժ����ԣ�(%d)", (UINT)msg->lParam);
			QString s = QString::fromLocal8Bit(str);
			WJ_SAFE_QBoxMess(WJMainWindow(), s);
		}break;
		case WM_GAME_MESS_2009://��Ϸ��������½�ɹ�
		{
			qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("��Ϸ��������½�ɹ�");
		}break;
		case WM_GAME_MESS_2007://��Ϸ������---���ӳ�ʱ
		{
			SystemLog()->LogToFile("sdp_test c ��Ϸ������---���ӳ�ʱ,�������ҷ��䲢����---");
			Sys_OnJoinRoom(false);
		}break;
		case WM_GAME_MESS_2008://��Ϸ������---����ʧ��
		{
			SystemLog()->LogToFile("sdp_test c ��Ϸ������---����ʧ��,�������ҷ��䲢����---");
		}break;
		case WM_GAME_MESS_2021:
		{
			bool OnStart = false;
			int itage = CommonData()->m_NowContest - WJ::g_eventid_first;
			if (itage > 0)
			{//˵��������
				OnStart = true;
			}
			else
			{
				if (m_struct_StartContest.bTrue)
				{
					OnStart = true;
				}
			}

			if (OnStart)
			{
				Sys_OnStart();
			}

		}break;
		default:
			break;
		}
	}
	catch (...)
	{
		///
	}

	return true;
}

bool CRoomLogic::connect(QString cip, int iport)
{
	if (nullptr == cip.toLatin1().data())
	{
		_serverAddress = clientconfig().GetData()->hall_server_address;
	}
	else
	{
		_serverAddress = cip.toLatin1().data();
	}

	if (0 >= iport)
	{
		_serverPort = 0;
	}
	else
	{
		_serverPort = iport;
	}
	m_WJClientSocketLogic->close();
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("��Ϸ������----��������")
		<< _serverAddress.c_str() << _serverPort;
	return m_WJClientSocketLogic->openWithIp(_serverAddress.c_str(), _serverPort);
}
bool CRoomLogic::SocketClose()
{	
	_connected = false;
	return m_WJClientSocketLogic->close();

}

bool CRoomLogic::isConnect() const
{
	return _connected;
}

bool CRoomLogic::isLogin() const
{

	return _logined;
}

void CRoomLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
{
	if (MainID == 104 && AssistantID == 3)
	{
		OutputDebugStringA("sdp_test client send mISocketMessageDelegate::sendData");
	}

	m_WJClientSocketLogic->send(MainID, AssistantID, object, objectSize);
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("��Ϸ������---������Ϣ��") << MainID << AssistantID << objectSize;
}

//void mISocketMessageDelegate::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, WJ::SEL_SocketMessage selector)
//{
//	m_WJClientSocketLogic->send(MainID, AssistantID, object, objectSize);
//	m_WJClientSocketLogic->addEventSelector(MainID, AssistantID, selector);
//}

void CRoomLogic::addEventSelector(UINT MainID, UINT AssistantID, WJ::SEL_SocketMessage selector)
{
	m_WJClientSocketLogic->addEventSelector(MainID, AssistantID, selector);
}
void CRoomLogic::removeEventSelector(UINT MainID, UINT AssistantID)
{
	m_WJClientSocketLogic->removeEventSelector(MainID, AssistantID);
}

void CRoomLogic::onConnected(bool connect, WJ::emSocketStatus status)
{
	if (WJ::SocketStatus_OUTTIME == status)
	{
		//���ӳ�ʱ����Ϣ
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("��Ϸ������---���ӳ�ʱ");
		_logined = false;
		_connected = false;

		WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2007, 0, 0);
	}
	else if (WJ::SocketStatus_SUCCESS == status)
	{
		//���ӳɹ�����Ϣ
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("��Ϸ������---���ӳɹ�");
	}

	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->onConnected(connect, status);
	}
}
void CRoomLogic::onDisConnect()
{
	//����ʧ�ܵ���Ϣ
	bInGame = false;
	//SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "��Ϸ������---����ʧ��");
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("��Ϸ������---����ʧ��");
	_logined = false;
	_connected = false;
	WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2008, 0, 0);
	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->onDisConnect();
	}
}
void CRoomLogic::onSocketMessage(WJ::WJSocketMessage* socketMessage)
{
	SystemLog()->LogToFile("sdp_test c onSocketMessage��Ϸ���������أ�%d,%d,%d"
		, socketMessage->messageHead.bMainID, socketMessage->messageHead.bAssistantID, socketMessage->messageHead.bHandleCode);
	//�����յ�����Ϣ������������д
	switch (socketMessage->messageHead.bMainID)
	{
	case NET_M_CONNECT:
	{
		if (ASS_NET_CONNECT_2 == socketMessage->messageHead.bAssistantID)
		{
			_connected = true;

			WJ::RoomMessDate_user_action_login login;
			(UINT)login.uuserid = CommonData()->userinfo.dwUserID;
			(UINT)login.contestid = CommonData()->m_NowContest;
			sendData((UINT)WJ::e_gs_mess_user_action, (UINT)WJ::e_gs_ass_user_action_login, &login, sizeof(login));
			SystemLog()->LogToFile("sdp_test c ��Ϸ������---���͵�½��Ϣ");
		}
	}break;
	case WJ::e_gs_mess_user_action:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_user_action_login)
		{
			if (socketMessage->messageHead.bHandleCode == WJ::e_gs_hcid_login_suc_)
			{
				_logined = true;
				SystemLog()->LogToFile("sdp_test c ��Ϸ��������½�ɹ�");
				WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2009, 0, 0);
			}
			else if (socketMessage->messageHead.bHandleCode == WJ::e_gs_hcid_login_suc_recome)
			{
				SystemLog()->LogToFile("sdp_test c ��Ϸ��������½�ɹ�,�����ػ�---");
				WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2010, 0, 0);
			}
			else if (socketMessage->messageHead.bHandleCode == WJ::e_gs_hcid_login_err_)
			{
				SystemLog()->LogToFile("sdp_test c ��Ϸ��������½ʧ�ܣ���������������Ϸ������---");
				SocketClose();
				Sys_OnJoinRoom(false);
			}
		}
		else if (WJ::e_gs_ass_user_action_login_cancel == socketMessage->messageHead.bAssistantID)
		{
			if (WJ::e_gs_hcid_login_cancel_suc_ == socketMessage->messageHead.bHandleCode)
			{
				SystemLog()->LogToFile("sdp_test c ��Ϸ������ȡ����½�ɹ�����ʼ�����˷�");
				//�˷�
				WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_apply, (UINT)WJ::e_hs_ass_apply_restitution, NULL, 0);
				DataDrocessing::inobgect()->OnContestButUnlock();
			} 
			else if (WJ::e_gs_hcid_login_cancel_err_ == socketMessage->messageHead.bHandleCode)
			{
				//�ݲ�����
			}
		}
	}break;
	case WJ::e_gs_mess_queue:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_queue_allot)
		{
			WJ::RoomMessDate_queue_allot* p = (WJ::RoomMessDate_queue_allot*)socketMessage->object;
			m_udeskid = p->udeskid;//����id
			m_udeskstation = p->udeskstation;//��λ
			m_bOwerUserid = p->bOwerUserid;//�Ƿ�Ϊ��������
			SystemLog()->LogToFile("sdp_test c ��Ϸ������---ƥ��ɹ�,����������������");
			sendData((UINT)WJ::e_gs_mess_game_frame, (UINT)WJ::e_gs_ass_game_frame_info, NULL, 0);

		}
	}break;
	case WJ::e_gs_mess_user_list:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_user_list_get_desk_userinfo)
		{//����Ϸ��ʼ֮ǰ���ڻ�ȡ���ӻ������� �� ״̬���ݵ�ͬʱ������˽�����Ҳ�����������������û����ݣ��������������Ϊ
			//��Ϸ��ʼǰ�����з����׼�������Ѿ���ɣ��������ͻ��˿�����������׼����ʼ��Ϸǰ�����ж�����,���ǵ÷������ͬ�⿪ʼ��Ϸ����Ϣ
			SystemLog()->LogToFile("sdp_test c ��Ϸ������---��ȡ��������������ҵ�����");
			int size = sizeof(WJ::UserInfoInRoomStruct);
			for (int i = 0; i < socketMessage->objectSize / size; i++)
			{
				WJ::UserInfoInRoomStruct* pCmd = (WJ::UserInfoInRoomStruct*)(socketMessage->object + i*(sizeof(WJ::UserInfoInRoomStruct)));
				WJ::UserInfoInRoomStruct cmd;
				std::memcpy(&cmd, pCmd, sizeof(WJ::UserInfoInRoomStruct));
				SystemLog()->LogToFile("sdp_test c �û�����(uDeskStation=%d,dwUserID=%d,nickName=%s)", cmd.uDeskStation, cmd.basedata.dwUserID, cmd.basedata.nickName);
				m_UserDatalist.push_back(cmd);
				
				///lol/////////////////////////////////
				if (e_gameid_10000001 == CommonData()->m_NowGame)
				{
					//����solo��ƥ�䵽�Ķ�����ϸ��Ϣ
					QEventLoop loop;
					QNetworkAccessManager Manager;
					//����һ������
					QNetworkRequest requestq;
					QString userId = QString::number(cmd.basedata.dwUserID, 10);
					QString str;
					str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/userInfo?id=") + userId;
					requestq.setUrl(QUrl(str));
					//����GET����
					QNetworkReply *p = Manager.get(requestq);
					QObject::connect(p, SIGNAL(finished()), &loop, SLOT(quit()));//�ؼ�֮��1
					loop.exec();//�ؼ�֮��2
					QByteArray data = p->readAll();

					//֪ͨH5ƥ�䵽�Ķ�����Ϣ
					QJsonObject sendjsonpart;
					sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_3007);
					sendjsonpart.insert(QStringLiteral("TYPE"), QString::fromUtf8(data).toStdString().c_str());
					QJsonDocument documentpack;
					documentpack.setObject(sendjsonpart);
					QString json_json(documentpack.toJson(QJsonDocument::Compact));
					WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
				}
				//////////////////////////////////////
			}
			WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2011, 0, 0);
		}
	}break;
	case WJ::e_gs_mess_room:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_room_info)
		{//��ȡ��������
			SystemLog()->LogToFile("sdp_test c ��Ϸ������---��������");
			WJ::Room_ManageInfoStruct* pCmd = (WJ::Room_ManageInfoStruct*)socketMessage->object;
			std::memcpy(&m_RoomInfo, pCmd, sizeof(WJ::Room_ManageInfoStruct));

			SystemLog()->LogToFile("sdp_test c ��Ϸ������---��������(award=%d, %d)(_i64cost=%I64d, %I64d)"
				, m_RoomInfo.award, pCmd->award
				, m_RoomInfo._i64cost, pCmd->_i64cost);
		}
	}break;
	case WJ::e_gs_mess_game_frame:
	{
		if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_info)
		{//��ȡ����Ϸ����������Ϣ
			SystemLog()->LogToFile("sdp_test c ��Ϸ������---��ȡ��Ϸ����������Ϣ");
			WJ::RoomMessDate_deskinfo* pCmd = (WJ::RoomMessDate_deskinfo*)socketMessage->object;
			CommonData()->GameInformation.GameState = pCmd->bGameStation;
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_station)
		{//��ȡ����Ϸ��ǰ״̬������Ϣ

		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_agree)
		{//���˷�����ͬ�⿪ʼ��Ϸ
			SystemLog()->LogToFile("sdp_test c ���˷�����ͬ�⿪ʼ��Ϸ");
			WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2013, 0, 0);
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_begin)
		{//��Ϸ��ʽ��ʼ
			SystemLog()->LogToFile("sdp_test c ��Ϸ��ʽ��ʼ");
			WJ_SAFE_QSendMessage(WJMainWindow(), WM_GAME_MESS_2012, 0, 0);
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_end_c)
		{//ĳ���������֪�Լ�������
			SystemLog()->LogToFile("sdp_test c ��Ϸ������---����Ҵﵽ�����Ͽ��ر���Ϸ");	
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_end_s)
		{//��Ϸ��ʽ����
		}
		else if (socketMessage->messageHead.bAssistantID == WJ::e_gs_ass_game_frame_user_leave)
		{//����ڷ���Ϸ�������뿪����
			m_IsLOLPVPExist = false;
			Sys_OnDisposeErr(e_gameid_10000001);

			QJsonObject sendjsonpart;
			sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_3013);
			QJsonDocument documentpack;
			documentpack.setObject(sendjsonpart);
			QString json_json(documentpack.toJson(QJsonDocument::Compact));
			WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

			char s[MAX_PATH] = { 0 };
			sprintf_s(s, "���ֵ��ߣ������¿�ʼƥ�䡣");
			WJ_SAFE_QBoxMess(WJMainWindow(), QString(QString::fromLocal8Bit(s)));
		}

	}break;
	case WJ::e_gs_mess_game_notify:
	{

	}break;
	default:
		break;
	}

	IBaseMyGameRoom* pgr = CommonData()->GetMyGameRoom(CommonData()->m_NowGame);
	if (nullptr != pgr)
	{
		pgr->onSocketMessage(socketMessage);
	}
}