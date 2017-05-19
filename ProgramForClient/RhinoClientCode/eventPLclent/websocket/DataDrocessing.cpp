#include "DataDrocessing.h"
#include <QtCore>
#include "common/Common.h"
#include "common/Log.h"
#include "WebSocketMessInfo.h"
#include "common/WJGameInfo.h"
#include "clientsocket/commonfunction_c.h"
#include "websocket\WebSocketServer.h"
#include "WJPlatformLogic.h"
#include "common/Common.h"
#include "ClientLink.h"
#include "Qthread/RoomLogic.h"
#include "web/WJMainWindow.h"
#include "common/LoadWindowsMess.h"

DataDrocessing::DataDrocessing()
//: QObject(parent)
{

}

DataDrocessing::~DataDrocessing()
{

}

DataDrocessing *DataDrocessing::inobgect()
{
	static DataDrocessing s_obj;
	return &s_obj;
}

void DataDrocessing::callbackConn(QWebSocket *_socket, bool _isSrv)
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1000);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	_socket->sendTextMessage(json_json);
}

void DataDrocessing::Processing(const QString &_msg, Cweb_c_type _type)//websocket��Ϣ����
{
	QJsonParseError json_errorver;
	QJsonDocument parse_doucmentver = QJsonDocument::fromJson(_msg.toUtf8(), &json_errorver);
	if (json_errorver.error == QJsonParseError::NoError)
	{
		if (parse_doucmentver.isObject())
		{
			QJsonObject objectver = parse_doucmentver.object();
			QJsonValue value = objectver.value(QString("MSGID"));
			QJsonObject _TYPE;
			if (!objectver.contains("MSGID"))
			{
				return;
			}

			if (objectver.contains("TYPE"))
			{
				_TYPE = objectver.value(QString("TYPE")).toObject();
			}

			int iMSGID = value.toInt();
			switch (iMSGID)
			{
			case WEB_SOCKET_1027:
			{//��֪��ǰ��ʲô��Ϸid
				web_Processing_1027(_TYPE);
			}break;
			case WEB_SOCKET_1001://��½
			{
				web_Processing_1001(_TYPE);//���ܵ�web��½��Ϣ
			}break;
			case WEB_SOCKET_1004://��������������
			{
				web_Processing_1004(_TYPE);//���ܵ�web���������Ϣ
			}break;
			case WEB_SOCKET_1005://����������
			{
				web_Processing_1005(_TYPE);//���ܵ�web����������������Ϣ
			}break;
			case WEB_SOCKET_1006://��������������
			{
				web_Processing_1006(_TYPE);//���ܵ�web������������������
			}break;
			case WEB_SOCKET_1031:
			{
				web_Processing_1031(_TYPE);
			}break;
			case WEB_SOCKET_9001:
			{//΢�ŵ�½��ά��ɨ�贰�ڴ򿪽���
				web_Processing_9001();
			}break;
			case WEB_SOCKET_9002:
			{//΢�ŵ�½��ά��ɨ�贰�ڹرս���
				web_Processing_9002(_TYPE);
			}break;
			case WEB_SOCKET_9003:
			{//΢�ŵ�½��ά��ɨ���õ��û���Ϣ����ת������
				web_Processing_9003(_TYPE);
			}break;
			case WEB_SOCKET_9004:
			{//΢�ŵ�½��ά��ɨ��֪ͨ����ʹ���˻���½����
				web_Processing_9004();
			}break;
			case WEB_SOCKET_9005:
			{//΢�ŵ�½��ά��ɨ��֪ͨ����ʹ��key+userid��½ҵ�����������
				web_Processing_9005(_TYPE);
			}break;
			case WEB_SOCKET_9006:
			{//�������޸�����
				web_Processing_9006(_TYPE);
			}break;
			case WEB_SOCKET_1013:
			{//��������ڵ��ȡ�����б���
				web_Processing_1013(_TYPE);
			}break;
			case WEB_SOCKET_1014:
			{//�뿪��������ҳ�潻��
				web_Processing_1014(_TYPE);
			}break;
			case WEB_SOCKET_7000:
			{//H5��Ҫע����Ҫ���ƹ�id
				web_Processing_7000();
			}break;
			case WEB_SOCKET_1028:
			{//��֪��ǰ�ͻ��˰汾
				web_Processing_1028();
			}break;
			case WEB_SOCKET_9013:
			{//ע�ᴰ�ڴ򿪽���
				WJMainWindow()->showRegistrationPage(true);
			}break;
			case WEB_SOCKET_1029:
			{//�ر�ע�ᴰ��
				WJMainWindow()->showRegistrationPage(false);
			}break;
			case WEB_SOCKET_3012:
			{//�����ػ��Զ����뷿�����ס������ť������Ӣ������

				web_Processing_3012(_TYPE);
			}break;
			case WEB_SOCKET_6000:
			{
				OnGetWebAddr();
			}break;
			case WEB_SOCKET_9011:
			{//��ͨ��¼ת΢�ŵ�¼
				web_Processing_9011();
			}break;
			case WEB_SOCKET_9012:
			{//��ͨ��¼���ڹرս���
				web_Processing_9012();
			}break;
			case WEB_SOCKET_9010:
			{//����Ƿ�鿴����ͼ
				web_Processing_9010(_TYPE);
			}break;
			case WEB_SOCKET_9016:
			{//¯ʯ���pickӢ��
				web_Processing_9016(_TYPE);
			}break;
			case WEB_SOCKET_9021:
			{//¯ʯ���banӢ��
				web_Processing_9021(_TYPE);
			}
			case WEB_SOCKET_3014:
			{//LOL���������ҳ�ر�
				WJMainWindow()->onShowGameTaskLOL(false);
			}break;
			case WEB_SOCKET_1000:
			{//��LOL���������ҳ�ɹ���������
				if (_type == web_c_lol_task_result)
				{
					WJ_SAFE_QPostMessage(WJMainWindow(), WM_HALL_MESS_1015, 0, 0);
				}
			}break;
			case WEB_SOCKET_1037:
			{//������������ȷ�ϰ�ť�ر�¯ʯ��Ƕ����
				if (nullptr != WJMainWindow()->m_pEmbedWindowHS)
				{
					WJMainWindow()->m_pEmbedWindowHS->close();
					delete WJMainWindow()->m_pEmbedWindowHS;
					WJMainWindow()->m_pEmbedWindowHS = nullptr;
				}
			}break;
			case WEB_SOCKET_9017:
			{//������������ҷ���������Ϣ
				web_Processing_9017(_TYPE);
			}break;
			case WEB_SOCKET_9019:
			{//���¼��ش���ҳ��
				std::string str_url = clientconfig().GetData()->h5_server_address + "index.html";
				WJMainWindow()->openUrl(QUrl(QString(str_url.c_str())));
			}
			default:
			{
				if (WEB_SOCKET_1034 == iMSGID)
				{
					int a = 0;
					a = 1;
				}
				IBaseMyDataDrocessing*p = (IBaseMyDataDrocessing*)CommonData()->GetMyDataDrocessing(CommonData()->m_NowGame);
				if (nullptr != p)
				{
					p->Processing(iMSGID, _TYPE);
				}
				break;
			}
			}
		}
	}
}

//��������ڵ��ȡ�����б���
void DataDrocessing::web_Processing_1013(QJsonObject TYPE)
{
	int icontestid = TYPE.value(QString("CONTESTID")).toString().toInt();

	if (TYPE.contains("MODE"))
	{
		int inSolo = TYPE.value("MODE").toInt();
		WJMainWindow()->m_LolSoloId = inSolo;
	}

	CommonData()->m_NowContest = icontestid;
	CommonData()->GetNowContestInfo();

	SystemLog()->LogToFile("��������ڵ��ȡ�����б���,����id = %d", CommonData()->m_NowContest);
	SystemLog()->LogToFile("sdp_test c ��������ڵ��ȡ�����б���---CommonData()->m_NowContest=%d", CommonData()->m_NowContest);
}

//�뿪��������ҳ�潻��
void DataDrocessing::web_Processing_1014(QJsonObject TYPE)
{
	QJsonDocument documentpack;
	documentpack.setObject(TYPE);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));

	int modeId = (TYPE.value(QString("modeId")).toInt());
	SystemLog()->LogToFile("�뿪��������ҳ�潻��");
	SystemLog()->LogToFile(json_json);

	if (modeId == 2)
	{
		return;
	}


	RoomLogic()->Sys_OnLeave();
}

void DataDrocessing::web_Processing_1031(QJsonObject TYPE)
{
	//¯ʯ��Ϸ���ж�
	if (nullptr != WJMainWindow()->m_pEmbedWindowHS)
	{
		RoomLogic()->bInGame = true;
	}
	//LOL��Ϸ���ж�
	DWORD pid = GetProcessIdByName(TEXT("LolClient.exe"));
	if (pid != 0)
		RoomLogic()->bInGame = true;

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1031);
	sendjsonpart.insert(QStringLiteral("GAMESTATE"), RoomLogic()->bInGame);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//΢�ŵ�½��ά��ɨ�贰�ڴ򿪽���
void DataDrocessing::web_Processing_9001()
{
	SystemLog()->LogToFile("΢�ŵ�½��ά��ɨ�贰�ڴ򿪽���");
	WJMainWindow()->onShowWXwind(true);
}

//΢�ŵ�½��ά��ɨ�贰�ڹرս���
void DataDrocessing::web_Processing_9002(QJsonObject TYPE)
{
	SystemLog()->LogToFile("΢�ŵ�½��ά��ɨ�贰�ڹرս���");
	WJMainWindow()->onShowWXwind(false);
	QJsonObject sendjsonpart;
	QJsonObject json;
	json.insert(QStringLiteral("status"), TYPE.value("status").toString());
	sendjsonpart.insert(QStringLiteral("TYPE"), json);
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9002);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//΢�ŵ�½��ά��ɨ���õ��û���Ϣ����ת������
void DataDrocessing::web_Processing_9003(QJsonObject TYPE)
{
	SystemLog()->LogToFile("΢�ŵ�½��ά��ɨ���õ��û���Ϣ����ת������");
	QJsonObject sendjsonpart;
	QJsonObject json;
	json.insert(QStringLiteral("KEY"), TYPE.value("KEY").toString());
	sendjsonpart.insert(QStringLiteral("TYPE"), json);
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9003);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

	if (TYPE.value("KEY").isObject()) {

		std::memcpy(WJ::PlatformLogic().m_logininfo_WX.wx_key, TYPE.value("KEY").toString().toStdString().c_str(), sizeof(WJ::PlatformLogic().m_logininfo_WX.wx_key));
	}
}

//֪ͨ����ʹ����ͨ��½����
void DataDrocessing::web_Processing_9004()
{
	SystemLog()->LogToFile("΢�ŵ�½��ά��ɨ��֪ͨ����ʹ���˻���½����");
	WJMainWindow()->onShowWXwind(false);
	SystemLog()->LogToFile("��ͨ��¼���ڴ򿪽���");
	WJMainWindow()->onShowLogin(true);

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9004);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//΢�ŵ�½��ά��ɨ��֪ͨ����ʹ��key+userid��½ҵ�����������
void DataDrocessing::web_Processing_9005(QJsonObject TYPE)
{
	SystemLog()->LogToFile("΢�ŵ�½��ά��ɨ��֪ͨ����ʹ��key+userid��½ҵ�����������");
	WJ::PlatformLogic().m_logininfo_WX.userid = TYPE.value(QString("USERID")).toInt();
	std::memcpy(WJ::PlatformLogic().m_logininfo_WX.username, TYPE.value("USERNAME").toString().toStdString().c_str(), sizeof(WJ::PlatformLogic().m_logininfo_WX.username));

	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("�����½��ť");
	WJ::PlatformLogic().m_bWX_login = true;//΢�ŵ�½���
	WJ::PlatformLogic().close();
	//{
		ClientLink()->onStart();
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("���Ӳ��ɹ����µ�½");
	//}
	//else
	//{
	//	WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_login, (UINT)WJ::e_hs_ass_login_WX, &WJ::PlatformLogic().m_logininfo_WX, sizeof(WJ::PlatformLogic().m_logininfo_WX));
	//	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("���ӳɹ����͡�΢�š���¼");
	//}
}

void DataDrocessing::web_Processing_9006(QJsonObject TYPE)
{

	QString tUserId;
	QString tOlePassWord;
	QString tNewPassWord;

	if (TYPE.contains("USERID"))
	{
		//tUserId = QString("%1").arg(TYPE.value("USERID").toInt());
		tUserId = TYPE.value("USERID").toString();
	}

	if (TYPE.contains("oldPSW"))
	{
		tOlePassWord = TYPE.value("oldPSW").toString();
	}

	if (TYPE.contains("newPSW"))
	{
		tNewPassWord = TYPE.value("newPSW").toString();
	}

	//����һ��������
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	//������������ź�
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	//SystemLog::getInstance()->writeLog(SystemLog::E_LOG_DEBUG, "WJTestLOG.log", "1", 1, "�����޸�����");
	//����һ������
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/changePassword?userId=") + tUserId + QStringLiteral("&originalPassword=") + tOlePassWord + QStringLiteral("&newpassword=") + tNewPassWord;
	request.setUrl(QUrl(str));

	SystemLog()->LogToFile("�޸���������:");
	SystemLog()->LogToFile(str.toStdString().c_str());
	//����GET����
	QNetworkReply *reply = manager->get(request);
	//������Ӧʱ���������ź�
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//�ؼ�֮��1
	loop.exec();//�ؼ�֮��2
	/////////////////////////////////////////////
	QByteArray data = reply->readAll();//��ҳ�������

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_9006);
	QJsonParseError json_errorver;
	QJsonDocument JsionDecument = QJsonDocument::fromJson(data, &json_errorver);
	sendjsonpart.insert(QStringLiteral("TYPE"), JsionDecument.object());
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//��֪��ǰ��ʲô��Ϸid
void DataDrocessing::web_Processing_1027(QJsonObject TYPE)
{
	CommonData()->m_NowGame = (E_GameID)TYPE.value(QString("GAME")).toInt();
	SystemLog()->LogToFile("��ǰѡ�е���ϷID=%d", CommonData()->m_NowGame);
	SystemLog()->LogToFile("sdp_test ��֪��ǰ��Ϸid=%d", CommonData()->m_NowGame);
}

//H5��Ҫע����Ҫ���ƹ�id
void DataDrocessing::web_Processing_7000()
{
	std::string str_file_update = GetAppPath().c_str();
	str_file_update += "\\update.ini";
	int isource = GetPrivateProfileIntA("config", "source", 0, str_file_update.c_str());//�ƹ�id
	QString sSource = QString::number(isource, 10);
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_7001);
	sendjsonpart.insert(QStringLiteral("TYPE"), sSource);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_pt_login);
}

//��֪��ǰ�ͻ��˰汾
void DataDrocessing::web_Processing_1028()
{
	std::string str_file_update = GetAppPath().c_str();
	str_file_update += "\\update.ini";
	int iversion = GetPrivateProfileIntA("config", "version", 0, str_file_update.c_str());//���汾��
	int ismallversion = GetPrivateProfileIntA("config", "smallversion", 0, str_file_update.c_str());//�Ӱ汾��
	QString sVersion = QString::number(iversion, 10);
	sVersion += QStringLiteral("-");
	sVersion += QString::number(ismallversion, 10);

	int isource = GetPrivateProfileIntA("config", "source", 0, str_file_update.c_str());//�ƹ�id
	QString sSource = QString::number(isource, 10);

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1028);
	sendjsonpart.insert(QStringLiteral("TYPE"), sVersion);
	sendjsonpart.insert(QStringLiteral("ID"), sSource);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

void DataDrocessing::web_Processing_1006(QJsonObject TYPE)//���ܵ�web������������������
{
	QString TASKID;
	QString USERID;
	if (TYPE.contains("TASKID"))
	{
		TASKID = TYPE.value("TASKID").toString();
	}
	if (TYPE.contains("USERID"))
	{
		USERID = QString("%1").arg(TYPE.value("USERID").toInt());
	}

	QEventLoop loop;
	QNetworkAccessManager m_networkAccessManger;
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/task/loldetail?userId=") + USERID + QStringLiteral("&taskId=") + TASKID;
	request.setUrl(QUrl(str));

	SystemLog()->LogToFile("������������������:");
	SystemLog()->LogToFile(str.toStdString().c_str());
	//����GET����
	QNetworkReply *reply = m_networkAccessManger.get(request);

	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//�ؼ�֮��1
	loop.exec();//�ؼ�֮��2
	/////////////////////////////////////////////
	QByteArray data = reply->readAll();//��ҳ�������

	SystemLog()->LogToFile("������������������:");
	SystemLog()->LogToFile(data.toStdString().c_str());

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1006);
	sendjsonpart.insert(QStringLiteral("TYPE"), data.toStdString().c_str());
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

	WJ_SAFE_QPostMessage(WJMainWindow(), WM_HALL_MESS_1009, 0, 0);
	reply->deleteLater();
}
void DataDrocessing::web_Processing_1005(QJsonObject TYPE)//���ܵ�web����������������Ϣ
{
	QString TASKID;
	QString USERID;
	if (TYPE.contains("TASKID"))
	{
		TASKID = QString("%1").arg(TYPE.value("TASKID").toInt());
	}
	if (TYPE.contains("USERID"))
	{
		USERID = TYPE.value("USERID").toString();
	}
	if (TYPE.contains("SERVER"))
	{
		CommonData()->m_NowErea = TYPE.value("SERVER").toInt();
	}

	QEventLoop loop;
	QNetworkAccessManager m_networkAccessManger;
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/task/apply?userId=") + USERID + QStringLiteral("&taskId=") + TASKID;
	request.setUrl(QUrl(str));

	SystemLog()->LogToFile("������������������:");
	SystemLog()->LogToFile(str);
	//����GET����
	QNetworkReply *reply = m_networkAccessManger.get(request);
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//�ؼ�֮��1
	loop.exec();//�ؼ�֮��2
	/////////////////////////////////////////////
	QByteArray data = reply->readAll();//��ҳ�������
	SystemLog()->LogToFile("������������������:");
	SystemLog()->LogToFile(data.toStdString());

	QJsonParseError json_errorver;
	QJsonDocument parse_doucmentver = QJsonDocument::fromJson(data, &json_errorver);
	if (json_errorver.error == QJsonParseError::NoError)
	{
		if (parse_doucmentver.isObject())
		{
			QJsonObject objectver = parse_doucmentver.object();
			if (objectver.contains("code"))
			{
				if (objectver.value("code").toInt() == 1)//�����ɹ�
				{
					CommonData()->match = MatchType::LOLMission;
				}
			}

		}
	}

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1005);
	sendjsonpart.insert(QStringLiteral("TYPE"), QString::fromUtf8(data).toStdString().c_str());
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

	SystemLog()->LogToFile("����������������Ϣ�����ǰ��,����:");
	SystemLog()->LogToFile(json_json.toStdString().c_str());
	reply->deleteLater();
}
void DataDrocessing::web_Processing_1004(QJsonObject TYPE)//���ܵ�web����������������Ϣ
{
	QString SERVER;
	QString GAME;
	QString USERID;

	if (TYPE.contains("GAME"))
	{
		GAME = QString("%1").arg(TYPE.value("GAME").toInt());
	}
	if (TYPE.contains("USERID"))
	{
		USERID = QString("%1").arg(TYPE.value("USERID").toInt());
	}

	QEventLoop loop;
	QNetworkAccessManager m_networkAccessManger;
	QNetworkRequest request;
	QString str = Q_WEB_ROOT_ADDRESS + QStringLiteral("api/task/list?userId=") + USERID + QStringLiteral("&game=") + GAME;
	request.setUrl(QUrl(str));
	//qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("������������������:") << str;
	//����GET����
	QNetworkReply *reply = m_networkAccessManger.get(request);

	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));//�ؼ�֮��1
	loop.exec();//�ؼ�֮��2
	/////////////////////////////////////////////
	QByteArray data = reply->readAll();//��ҳ�������
	//qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("������������������:") << QString::fromUtf8(data).toStdString().c_str();

	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1004);
	sendjsonpart.insert(QStringLiteral("TYPE"), data.data());
	//sendjsonpart.insert(QStringLiteral("TYPE"), data.toStdString().c_str());
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	SystemLog()->LogToFile("web����������������Ϣ,��������:");
	SystemLog()->LogToFile(json_json.toStdString().c_str());
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);

	reply->deleteLater();
}

void DataDrocessing::web_Processing_1001(QJsonObject TYPE)//���ܵ�web��½��Ϣ
{
	SystemLog()->LogToFile("����web��½��Ϣ");
	QString NAME;
	QString PASSWORD;
	if (TYPE.contains("NAME"))
	{
		NAME = TYPE.value("NAME").toString();
	}
	if (TYPE.contains("PASSWORD"))
	{
		PASSWORD = TYPE.value("PASSWORD").toString();
	}

	std::memcpy(WJ::PlatformLogic().m_logininfo.username, NAME.toLatin1().data(), sizeof(WJ::PlatformLogic().m_logininfo.username));
	std::memcpy(WJ::PlatformLogic().m_logininfo.password, PASSWORD.toLatin1().data(), sizeof(WJ::PlatformLogic().m_logininfo.password));
	WJ::PlatformLogic().close();
	//if (!WJ::PlatformLogic().isConnect())
	//{
		ClientLink()->onStart();
	//}
	//else
	//{
	//	WJ::PlatformLogic().sendData((UINT)WJ::e_hs_mess_login, (UINT)WJ::e_hs_ass_login, &WJ::PlatformLogic().m_logininfo, sizeof(WJ::PlatformLogic().m_logininfo));
	//}
}

void DataDrocessing::SetRoute(QString &str_result, QString &str_name)
{
	QString str_route;//�ļ�·��
	while (1)
	{
		str_route = QFileDialog::getOpenFileName();
		if (str_route == QStringLiteral(""))
		{//û���ã��뿪
			break;
		}
		else
		{
			if (!CheckRoute(str_route, str_name))
			{//��ʾ���ô���
				WJ_SAFE_QBoxMess(WJMainWindow(), QStringLiteral("��Ϸ·�����ô���,����������"));
			}
			else
			{//�ɹ����뿪
				break;
			}
		}
	};



	str_result = str_route;
}
void DataDrocessing::OnGetWebAddr()
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_6000);
	sendjsonpart.insert(QStringLiteral("TYPE"), Q_WEB_ROOT_ADDRESS);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}
bool DataDrocessing::CheckRoute(QString &str_route, QString &str_name)
{
	QFile file(str_route);//��·����ָ���ļ�
	QString str = str_route;
	str = str.mid(str.lastIndexOf("/") + 1, str.length());
	str = str.mid(0, str.indexOf("."));
	if (str != str_name || !file.exists())
	{//��ʾ���ô���
		return false;
	}

	return true;
}

//��ס��������ҳ��ť
void DataDrocessing::OnContestButLock()
{
	//������ť����
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1015);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}
//������������ҳ��ť
void DataDrocessing::OnContestButUnlock()
{
	//������ť����
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1016);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//֪ͨh5��·�����ԣ������ٴ�������Ϸ·��
void DataDrocessing::HTTPMessageProcessing_1012()
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1012);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}

//�����Ϸ·������
void DataDrocessing::HTTPMessageProcessing_1010(QString &data)
{
	QJsonObject json;
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1010);
	{
		json.insert("GAMENAME", data.toStdString().c_str());
	}
	sendjsonpart.insert(QStringLiteral("TYPE"), json);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}
void  DataDrocessing::web_Processing_3012(QJsonObject TYPE)
{
	OnContestButLock();
	int serverId;
	if (TYPE.contains("SEVERID"))
		serverId = TYPE.value("SEVERID").toInt();

	bool bExist_route = false;//·���Ƿ����
	bExist_route = DataDrocessing::inobgect()->CheckRoute(CommonData()->lolroute, QStringLiteral("Client"));
	WJAM::LOGININFO stLogin = { 0 };
	stLogin.gameid = WJAM::GAME_ID_LOL;    /*��Ϸ*/
	strcpy_s(stLogin.account, "");
	strcpy_s(stLogin.password, "");
	stLogin.serverid = serverId;    /*������ID*/
	strcpy_s(stLogin.path, CommonData()->lolroute.toLocal8Bit());   /*��ִ���ļ�·��*/
	//  int retlol = AutomateManager::executeLogin(stLogin, nullptr);
	if (bExist_route)
	{
		AutomateManager::executeLogin(stLogin, nullptr);
	}
	else
		DataDrocessing::inobgect()->HTTPMessageProcessing_1012();

}
//��ͨ��¼ת΢�ŵ�¼
void DataDrocessing::web_Processing_9011()
{
	SystemLog()->LogToFile("��ͨ��¼ת΢�ŵ�¼");
	WJMainWindow()->onShowLogin(false);
	WJMainWindow()->onShowWXwind(true);
}
//��ͨ��¼���ڹرս���
void DataDrocessing::web_Processing_9012()
{
	SystemLog()->LogToFile("��ͨ��¼���ڹرս���");
	WJMainWindow()->onShowLogin(false);
}
//����Ƿ�鿴����ͼ
void DataDrocessing::web_Processing_9010(QJsonObject TYPE)
{
	int ViewDraw;
	int EventId;
	if (TYPE.contains("VIEWDRAW"))
	{
		ViewDraw = TYPE.value("VIEWDRAW").toInt();
	}
	if (TYPE.contains("EVENTID"))
	{
		EventId = TYPE.value("EVENTID").toInt();
	}
	if (1 == ViewDraw)
	{
		WJ::struct_data_hs_mess_event_clash _ID;
		_ID.eventId = EventId;
		WJ::PlatformLogic().sendDataL(WJ::e_hs_mess_event, WJ::e_hs_ass_event_clash, WJ::e_hs_hcid_event_clash_looking, &_ID, sizeof(_ID));
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("������ڹ۲����ͼ");
	}
	else if (0 == ViewDraw)
	{
		WJ::PlatformLogic().sendDataL(WJ::e_hs_mess_event, WJ::e_hs_ass_event_clash, WJ::e_hs_hcid_event_unlooking);
		qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("����뿪����ͼ����");
	}
}
//¯ʯ�������pickӢ��
void DataDrocessing::web_Processing_9016(QJsonObject TYPE)
{
	WJ::_struct_data_hs_mess_pick tPick;

	if (TYPE.contains("PICK_HEROS"))
	{
		QJsonArray TempArray = TYPE.value("PICK_HEROS").toArray();
		QJsonDocument document;
		document.setArray(TempArray);
		QByteArray byte_array = document.toJson(QJsonDocument::Compact);
		::memcpy(tPick.chPickData, byte_array, byte_array.length());
	} 
	if (TYPE.contains("SELF_ID"))
	{
		tPick.uFromId = TYPE.value("SELF_ID").toInt();
	}
	if (TYPE.contains("OPPONENT_ID"))
	{
		tPick.uToId= TYPE.value("OPPONENT_ID").toInt();
	}

	WJ::PlatformLogic().sendData(WJ::e_hs_mess_pick, WJ::e_hs_ass_event_pick, &tPick, sizeof(tPick));
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("��������PICKӢ��.");
}

//¯ʯ�������banӢ��
void DataDrocessing::web_Processing_9021(QJsonObject TYPE)
{
	
}

//������������ҷ���������Ϣ
void DataDrocessing::web_Processing_9017(QJsonObject TYPE)
{
	WJ::struct_data_hs_mess_chat m_data;
	if (TYPE.contains("CHAT_DATA"))
	{
		sprintf_s(m_data.chat_data, "%s", TYPE.value("CHAT_DATA").toString().toStdString().c_str());
	}
	if (TYPE.contains("FROMID"))
	{
		m_data.FromId = TYPE.value("FROMID").toInt();
	}
	if (TYPE.contains("TOID"))
	{
		m_data.ToId = TYPE.value("TOID").toInt();
	}

	WJ::PlatformLogic().sendData(WJ::e_hs_mess_chat, WJ::e_hs_ass_event_chat, &m_data, sizeof(m_data));
	qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz") << QStringLiteral("������������ҷ���������Ϣ");
}
//֪ͨ��ҿ�ʼ����С����Ϸ�Ƿ�ɹ�
//UINT resid
//0 �ɹ�
//1 ·������
//2 ��������
void DataDrocessing::tellEventGroupStartRes(int resid)
{
	QJsonObject sendjsonpart;
	sendjsonpart.insert(QStringLiteral("MSGID"), WEB_SOCKET_1034);
	QJsonObject res_js;
	res_js.insert("res", resid);
	sendjsonpart.insert(QStringLiteral("TYPE"), res_js);
	QJsonDocument documentpack;
	documentpack.setObject(sendjsonpart);
	QString json_json(documentpack.toJson(QJsonDocument::Compact));
	WebSocketServer::inobgect()->sendMessage(json_json, web_c_hall);
}