#include "clientconfig.h"
#include "clientsocket/commonfunction_c.h"
#include <windows.h>
Ccconfig& Ccconfig::getInstance()
{
	static Ccconfig cconfig;
	return cconfig;
}

Ccconfig::Ccconfig()
{
	std::string strPath = GetAppPath() + "\\aiset.ini";
	//��ȡ����ֵ
	char str[200] = {0};
	//��½������
	GetPrivateProfileStringA("config", "hall_server_address", "127.0.0.1", str, MAX_PATH, strPath.c_str());
	m_data.hall_server_address = str;
	//��������Ŀ
	GetPrivateProfileStringA("config", "AInum", "0", str, MAX_PATH, strPath.c_str());
	m_data.AInum = atoi(str);

	//��������ʼid
	GetPrivateProfileStringA("config", "AIFirstID", "0", str, MAX_PATH, strPath.c_str());
	m_data.AIFirstID = atoi(str);

	//����id
	std::memset(str, 0, sizeof(str));
	GetPrivateProfileStringA("config", "contestid", "0", str, MAX_PATH, strPath.c_str());
	m_data.contestid = atoi(str);

	//����id
	std::memset(str, 0, sizeof(str));
	GetPrivateProfileStringA("config", "roomid", "0", str, MAX_PATH, strPath.c_str());
	m_data.roomid = atoi(str);
}

Ccconfig::~Ccconfig()
{

}

//��ȡ����
Ccconfig::Ccconfig_data* Ccconfig::GetData()
{
	return &m_data;
}