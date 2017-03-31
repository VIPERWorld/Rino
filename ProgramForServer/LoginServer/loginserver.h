#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <list>
#include <mutex>


#include "mainmanage_login.h"
#include "balancesocket.h"
#include "Redis\WJRedis.h"

class LoginServer
{

public:
	LoginServer();
	~LoginServer();
	void StatServer();
	//�رշ�����
	void StopServer();

private:
	WJ::CMainManage_login *m_pMainManage_login;
	UINT m_uConnectCount;//��ǰ������
	UINT m_uConnectCount_gs;//��ǰgs������
	UINT m_uOnlineCount;//��ǰ��������
	UINT m_uOnlineCount_gs;//��ǰgs��Ч������
	std::string m_ApplyInfo_str;//��ǰ��������Ϣ
	std::string m_TeamInfo_str;//��ǰ�Ŷ�����Ϣ
	WJ::ManageInfoStruct m_ManageInfoStruct;//���������Ļ�������
	CBalanceSocket *m_pBalanceSocket;//�����ķ�����������
};

#endif // LOGINSERVER_H
