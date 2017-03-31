
#ifndef _WJ_SSocketManage_H_
#define _WJ_SSocketManage_H_

#include <mutex>
#include <thread>
#include <functional>
#include <atomic>
#include <unordered_map>
#include <deque>
#include <vector>
#include "message/WJNetMessageHead.h"
#include "server/WJListenSocket.h"
#include "server/WJSSocket.h"
#include "common/WJSpinlock.h"

namespace WJ
{
	//�׽������л�������
	void Init_WinSock();
	void UnInit_WinSock();

    struct EventItem
    {//�׽����¼��ڵ�
        EventItem(CWJSSocket *pSocket, int eventType, DWORD sentBytes = 0, void *pUserData = 0)
		: m_pSocket(pSocket), m_eventType(eventType), m_sentBytes(sentBytes), pUserData(pUserData)
        {
        }
        CWJSSocket  *m_pSocket;
        int          m_eventType;
        DWORD        m_sentBytes;
		void        *pUserData;  //�ش�����,��������ʱ�õ�
    };

	struct ConnectReq
	{
		char   ip[16];
		int    port;
		int    type;
		void  *pUserData; //�ش�
	};

	//TCP ���� SOCKET ������
	class WJSERVERMANAGELIB_API CWJSSocketManage
	{
	private:
		std::atomic<bool>			m_bRun;							//������־
		std::vector<CWJListenSocket*> m_ListenList;					//���� SOCKET
		std::unordered_map<UINT, CWJSSocket*>		m_id2socket;    // socket �������, key ��socketid, value ��CWJSSocket �ĵ�ַ
        UINT                        m_nMaxSocketCount;				//�趨�����������
		UINT						m_uRSThreadCount;				//��д�߳���Ŀ
		HANDLE						m_hCompletionPortRS;			//��д��ɶ˿�
        std::deque<EventItem>       m_eventQueue;                   // ThreadAccept  ThreadPullCompletedStatus �������߳�ͨ���˶��к�CWJSSocketManage�����߳�ͨ��
        CSpinlock                   m_eventQueueLock;
        std::vector<EventItem>      m_tmpEventResults;
		std::deque<ConnectReq>      m_connectQueue;
		CSpinlock                   m_connectQueueLock;
	public:
		//���캯��
		CWJSSocketManage(UINT nMaxSocketCount);
		//��������
		virtual ~CWJSSocketManage();

	public:
		//��ʼ����
		bool Start_WJSSocketManage(UINT uSocketThradCount);
		//ֹͣ����
		bool Stop_WJSSocketManage();

		//��Ϊ��������ɫ����˿�����
		void AsyncListen(const std::string &bindIp, int listenPort, UINT nListenType);
		//��Ϊ�ͻ��˷���ɫ��˿�����
		void AsyncConnect(const char *dstIp, int dstPort, int type, void *pUserData = NULL); // pUserData �ص��������ش�


		//��Ϣ������, ��д�ú���ʵ��ҵ���߼�
		virtual bool ProccessNetMessage(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize) = 0;
		/**
		 * �����ڵײ��Ѿ������ɹ�,���ϻص��ú���. 
		 * pUserData �� 
		 *    [�ͻ��˽�ɫ]��������ʱ�Ļش�����,��ӦAsyncConnect�еĲ���pUserData��
		 *    [����˽�ɫ]��������ʱ���ã�ʼ��ΪNULL��
		 */
		virtual void OnNewConnection(CWJSSocket * pSocket, void *pUserData) = 0; 
		//�����ڵײ�ص��˺������ڹر�ǰ�ص�
		virtual void OnEndConnection(CWJSSocket * pSocket) = 0;
		/**
		 * [�ͻ��˽�ɫ]��������ʧ��ʱ�ص�, ʧ���˾Ͳ���ص�OnNewConnection�� ���ǻص��ú���. 
		 * type ��ӦAsyncConnect�еĲ���type, pUserData ��ӦAsyncConnect�еĲ���pUserData
		 */
		virtual void OnConnectFailed(int type, void *pUserData) = 0;  

		//���ܺ���
	public:
		//�������ݺ���
		void SendData(UINT sockId, const char * pData, UINT uBufLen);
		//�������ݺ���
		void SendData(UINT sockId, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//�������ݺ���
		void SendData(UINT sockId, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//�������ݺ���
		void SendData(CWJSSocket * pSocket, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);

		//�ر�����
		void CloseSocket(UINT socketId);
		//�ж��Ƿ�����

		//��ýڵ�
		CWJSSocket* getCWJSSocket(UINT uSockedId);

	public:
        //�ú���ִ�����ϲ�ҵ�����߳���
        int ProcessNetEvents();
	private:
		//���������ӵ��߳�
		int ThreadAccept(SOCKET listenSock, UINT nListenType);
		//IO��ɽ����ȡ�߳�
		int ThreadPullCompletedStatus();
		//���������߳�
		int ThreadConnect();


	};
}
#endif //_WJ_SSocketManage_H_