#ifndef INC_BALANCESERVER_H
#define INC_BALANCESERVER_H
#ifdef __cplusplus
extern "C"
{
#endif

	//��������Ҫ��ͷ�ļ�
#include "event2/event.h"
#include "event2/listener.h"
#include "event2/bufferevent.h"
#include "event2/thread.h"
#include "event2/buffer.h"

#ifdef __cplusplus
};
#endif
#include <deque>
#include <map>
#include "json/json.h"
#ifdef _MSC_VER
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libevent_core.lib")
#endif
#define MAX_LINE    256 
#include "BalanceSelect.h"

struct ClientData
{
	ClientData()
	{
		m_strIP = "";
		m_nLastKeepAlive = 0;
		m_nBufferLen = 0;
		m_nMaxLen = 128;
		m_nRecvLen = -1;
		m_RecvBuffer= new char[m_nMaxLen];
	}
	~ClientData()
	{
		if (m_RecvBuffer)
		{
			delete[]m_RecvBuffer;
		}
	}
	std::string m_strIP;//ip��ַ
	long m_nRecvLen;//�������ݳ���
	long m_nBufferLen;//�������ݳ���
	long m_nMaxLen;//��󳤶�
	char * m_RecvBuffer;//������������
	long long m_nLastKeepAlive;
};

void initLoggingSystem(int argc, char* argv[]);
typedef struct tagNetMessageHead
{
	UINT						uMessageSize;			// ���ݰ���С
	UINT						bMainID;				// ����������
	UINT						bAssistantID;			// ������������ ID
	UINT						bHandleCode;			// ������id
	UINT						bReserve;				// �����ֶ�

	//tagNetMessageHead()
	//{
	//	std::memset(this, 0, sizeof(tagNetMessageHead));
	//}
}  NetMessageHead;
struct CMessage
{
	long m_nFd;
	std::string m_strData;
};
class CBalanceServer
{
public:
	CBalanceServer();
	~CBalanceServer();
	void StartService();
	void StopService();
	static void ParasJsonData(CMessage &msg);
	static void ParasLoginJsonData(std::string &strJsonString, bufferevent *bev);
	//�����ص�����
	static void listener_cb(evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *arg);

	//��Socket������Ϣ�Ļص�����
	static void socket_read_cb(bufferevent *bev, void *arg);

	//��Socket�¼��Ļص�����
	static void socket_event_cb(bufferevent *bev, short events, void *arg);


	//�����ص�����
	static void loginlistener_cb(evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *arg);

	//��Socket������Ϣ�Ļص�����
	static void loginsocket_read_cb(bufferevent *bev, void *arg);

	//��Socket�¼��Ļص�����
	static void loginsocket_event_cb(bufferevent *bev, short events, void *arg);

	static DWORD WINAPI  CheckKeepLiveFunc(LPVOID arg);

	static DWORD WINAPI  ClientProcessFunc(LPVOID arg);
private:
	static long long m_nConnect;
	static CBalanceSelect m_BalanceSelect;
	static std::recursive_mutex m_MutexMapClient;//�������������̶߳Ի���������
	static std::recursive_mutex m_MutexMapLoginServer;//�������������̶߳Ի���������
	static std::map<evutil_socket_t, ClientData*>   m_MapClient;
	static std::map<evutil_socket_t, ClientData*>   m_MapLoginServer;

	static std::recursive_mutex m_MutexMessage;//�������������̶߳Ի���������
	static std::deque<CMessage> m_deqMessage;
	int  m_nLoginPort;//���ķ������˿ڣ�����ls��
	int  m_nBalancePort;//���ķ������˿ڣ�����ͻ��ˣ�
	bool m_bExit;
	HANDLE m_hCheckKeepLive;
	HANDLE m_hClientProcess;
};
#endif