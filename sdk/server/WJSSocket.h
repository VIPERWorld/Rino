#ifndef _WJ_SSocket_H_
#define _WJ_SSocket_H_


#include <WinSock2.h>
#include <mutex>
#include <atomic>
#include <list>
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"

namespace WJ
{
	//���Ʊ�ʶ
#define SOCKET_NEW              0                               //�µ� SOCKET ����
#define SOCKET_SND				1								//SOCKET ��������¼�
#define SOCKET_REV				2								//SOCKET ��������¼�

#define INVALID_SOCKET_ID      -1                               // Լ�� -1 Ϊ��Чsocket id

	class CWJSSocketManage;

	struct SendBuf
	{
		SendBuf(char *buf, UINT len) : m_buf(buf), m_len(len)
		{

		}
		char  *m_buf;
		UINT   m_len;
	};
	//TCP SOCKET ��
	class WJSERVERMANAGELIB_API CWJSSocket
	{
		friend class CWJSSocketManage;
	public:
		//�ṹ����
		struct OverLappedStruct		//SOCKET �ص� IO �ṹ
		{
			//��������
			OVERLAPPED				OverLapped;							//�ص��ṹ
			WSABUF					WSABuffer;							//���ݻ���
			UINT					uOperationType;						//��������

			OverLappedStruct()
			{
				ZeroMemory(this, sizeof(OverLappedStruct));
			}
		};

	private:
		static std::atomic<UINT>    m_sockidSeed;                       //socketid ��������

        std::atomic<UINT>           m_refCount;                         //socketid ��������
		UINT						m_sockid;							//SOCKET id
		SOCKET						m_hSocket;							//SOCKET ���
		UINT                        m_type;                             //�������ͣ��������ֿͻ������ӻ���LoginServer������
		//void                       *m_pUserObj;                         //ָ���û�����
		std::list<SendBuf>          m_sendList;                         //���Ͷ���
		UINT						m_sendPendingSize;					//���Ͷ����������ܳ���
		bool                        m_bSending;                         //���ڷ��Ͷ���������(IOCP ���������ѵݽ��ȴ���ɰ�)
		char						m_szRecvBuf[MAX_SEND_SIZE];			//���ݽ��ջ�����
		DWORD						m_dwRecvBuffLen;					//���ջ���������

		CWJSSocketManage		  * m_pManage;						    //SOCKET ������ָ��
		struct sockaddr_in			m_peerAddr;						    //��ַ��Ϣ

		OverLappedStruct			m_SendOverData;						//���������ص��ṹ
		OverLappedStruct			m_RecvOverData;						//���������ص��ṹ

		UINT m_Keep;//����������������
		//��������
	public:
        inline void IncRef() { ++ m_refCount; }
        inline void DecRef()
        {
            --m_refCount;
            if (0 == m_refCount)
            {
                delete this;
            }
        }
		//��������
		virtual ~CWJSSocket(void);
		//���ܺ���
	public:

		//�������ݺ���
		void SendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//�������ݺ���
		void SendData(void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//�������ݺ���
		void SendData(const char * pData, UINT uBufLen);
		//�ж��Ƿ�����
		inline bool IsConnect() { return (m_hSocket != INVALID_SOCKET); }
		// ��ȡ��������
		inline UINT GetType() { return m_type; }
		inline void SetType(UINT uSockType) { m_type = uSockType; }
		//inline void SetUserObj(void *pUserObj) { m_pUserObj = pUserObj; }
		//inline void *GetUserObj() { return m_pUserObj;  }
		inline UINT GetSendPendingSize() { return m_sendPendingSize; }
		//��ȡ������Ϣ
		//ULONG dwAccessIP = this->GetPeerAddr()->sin_addr.S_un.S_addr �ɻ�ȡip��ַ
		inline const sockaddr_in *GetPeerAddr() { return &m_peerAddr; }
		inline UINT GetSockID() { return m_sockid; }
		inline UINT64 GetSocket() { return (UINT64)m_hSocket; }
		//���ù�����ָ��
		inline void SetSocketManagePtr(CWJSSocketManage * pManage) { m_pManage = pManage; }
		//�ر� SOCKET
		void CloseSocket();
		//������ɺ���
		bool OnSendCompleted(DWORD dwThancferred);
		//������ɺ���
		bool OnRecvCompleted();
		//���õ�ǰ�׽�������ʹ��ʱ��
		void SetKeep();
		//����׽������ʹ��ʱ��
		UINT GetKeep();
    private:
		//���캯��
		CWJSSocket(void);
		//�ϲ� SOCKET
		bool Attach(SOCKET hSocket, sockaddr_in & peerAddr);
		//Ͷ�ݽ��� IO
		bool RecvData();
		//��ⷢ�����ݺ���
		bool CheckSendData();
	};
}


#endif //_WJ_SSocket_H_