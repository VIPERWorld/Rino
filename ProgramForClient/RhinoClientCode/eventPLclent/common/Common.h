#ifndef _COMMON_H_
#define _COMMON_H_

#include <QtWebEngineWidgets\qwebengineview.h>
#include <QtWidgets\QMainWindow>
#include <QtWebEngineWidgets\qwebengineview.h>
#include <QtWebChannel\qwebchannel.h>
#include <QMessageBox>
#include <QWidget>
#include <QtGui>
#include <qpropertyanimation.h>
#include <qdir.h>
#include <QLatin1String> 
#include <QFile>
#include <QtCore>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QtWebChannel/QtWebChannel>

#include <unordered_map>
#include <functional>
#include "webpopup.h"
#include "webpopuppag.h"
#include "common/Model.h"
#include "QMbox.h"
#include "bridge.h"
#include "ClientLink.h"
#include "common/clientconfig.h"
#include "ClientLink.h"
#include "message/WJNetPlatformMessage.h"
//#include "WJGameInfo.h"


using namespace std;

QString GBK2UTF8(const QString &inStr);
QString UTF82GBK(const QString &inStr);
std::string UnicodeToUTF8(const wchar_t* str);

DWORD FindProcess(char *strProcessName);
VOID KillProcess(char *strProcessName);
bool ProcessIsLOL(DWORD pid);
DWORD GetProcessIdByName(LPCTSTR pName, bool bCheck = false);
BOOL GetProcessModule(DWORD dwPID, wchar_t* dll);

class IBaseMyDataDrocessing;
class IBaseMyHallLogic;
class IBaseMyGameRoom;

enum E_GameID;
enum class MatchType;

class CCommon
{
public:
	//ȷ�Ͽ��ñ���
	E_GameID m_NowGame;		//��ǰѡ�е���Ϸid
	UINT m_NowContest;		//��ǰѡ�еı���id
	UINT m_NowRoom;			//��ǰѡ�еķ���id
	UINT m_NowErea;			//��ǰѡ�е�����id

	std::vector<WJ::ComRoomInfo> m_GameRoomList;	//��ǰѡ�б����ڷ����б�
	HALLDATA m_NowContestInfo;						//��ǰ�����ڵ����ݣ�solo�������Ŷ�����
	QString lolroute;			//lol·��
	QString HearthStoneroute;	//¯ʯ��˵·��
	USERDATA userinfo;			//�û���Ϣ
	TVBETINFO m_BetInfo;		//LOLѺע����
	TVBETINFO m_BetHsInfo;		//¯ʯѺע����

	std::vector<WJ::ComGameinfo> m_GameNameList;		//��Ϸ���б�
	std::vector<WJ::ComContestInfo> m_GameContestList;	//�����б�
	std::vector<WJ::ComEreainfo> m_AreaList;			//��Ϸ�����б�

	//��������
	
	GAMEINFORMATION GameInformation;	//��������
	QMAHALLDATA HallData;				//�������ݴ洢�б�
	
	//GAMECONT Eventdetails;//������ϸ��Ϣ
	
	MatchType match;
	QString Resultspagecontent;		//���ͽ��ҳ����
	bool bExecutionAutomationDoing;	//�Ƿ���ִ���Զ���
	QString GameContname;			//�жϸ����������Ŷ���
	QString webstring;				//��ҳ��ַ
	QString version;				//�汾��
	QString GameName;
	QString strweb;					//�жϴ򿪵���ʲôҳ��
	QString currentpath;			//��ǰ����·��
	QString qquinNUME;				//qquinNUME
	QString qquinID;				//��java������ȡ������ U5912975290486760802
	QList<int> presongamelist;		//������list
	QList<int> teamgamelist;		//�Ŷ���list
	QString userAccount;			//�����û��˻���Ϣ

	QString Logpath;				//log��־·��
	//������ݻ������ڵ������½�Ѻע���
	QJsonObject m_BetCompleter;

	//��������
public:
	//������
	//title:�������ݣ�okButton��ȷ�ϰ�ť���ݣ�noButton��ȡ����ť���ݣ�butNub�Ƿ���ȡ����ť2���У�1��û�У�
	//���أ�true:ȷ��   falseȡ��
	bool BoxMess(QWidget *widget, QString title, QString okButton = QStringLiteral("ȷ��"), QString noButton = QStringLiteral("ȡ��"), int butNub = 1);

	///���������Ϣ
	void SetHallData(QString str);
	void GetAllGameRoute();	//��ȡ������Ϸ�ļ�·��
	bool GetLolRoute();		//�ж�lol·���Ƿ��Ѿ�����
	void SetGameRoute();	//д����Ϸ�ļ���ַ

	////���汾�Ƿ���Ը���
	//bool VersionUpdate();
	////����update.exe
	///*
	//std::string version ���汾
	//std::string smallversion �Ӱ汾
	//std::string _filename ���ص��ļ�
	//std::string strDownLoadUpdatepADDR ���صĸ���ַ
	//std::string startgamename �����ĳ�������
	//*/
	//bool updateOnline(std::string version, std::string smallversion, std::string _filename, std::string strDownLoadUpdatepADDR, std::string startgamename);
	//��õ�ǰ��������
	void GetNowContestInfo();
public:
	static CCommon* getInstance();

	//ע����Ϸ
	void RegisterGameList();

	IBaseMyDataDrocessing* GetMyDataDrocessing(E_GameID igameid);
	IBaseMyHallLogic* GetMyHallLogic(E_GameID igameid);
	IBaseMyGameRoom* GetMyGameRoom(E_GameID igameid);
	//���
private:
	// ���캯����˽�е�
	CCommon(void);

	// ��������
	virtual ~CCommon(void);

private:
	//�࿽����˽�е�
	CCommon(const CCommon &);
	CCommon & operator = (const CCommon &);
};


#define  CommonData()	CCommon::getInstance()

//������ȫ������װ
#include <qwidget.h>
#define WJ_SAFE_QBoxMess(_p, _title) \
if (nullptr != _p)\
	CommonData()->BoxMess((QWidget*)_p, _title, QStringLiteral("ȷ��"), "", 1);

#endif