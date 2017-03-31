#include "stdafx.h"
#include "server/WJSSocket.h"
#include "server/WJSSocketManage.h"
#include "common/commonfunction.h"

using namespace std;

namespace WJ
{
	atomic<UINT>    CWJSSocket::m_sockidSeed;

	//���캯��
	CWJSSocket::CWJSSocket(void)
	{
        m_refCount = 0;
		m_sockid = 0;
		m_type = 0;
		//m_pUserObj = nullptr;
		m_sendPendingSize = 0;
		m_bSending = false;
		m_dwRecvBuffLen = 0;
		m_hSocket = INVALID_SOCKET;
		m_pManage = nullptr;
		memset(&m_peerAddr, 0, sizeof(m_peerAddr));
		m_SendOverData.uOperationType = SOCKET_SND;
		m_RecvOverData.uOperationType = SOCKET_REV;
	}

	//��������
	CWJSSocket::~CWJSSocket(void)
	{
		CloseSocket();
        auto it = m_sendList.begin(), end = m_sendList.end();
        for (; it != end; ++it)
        {
            if (it->m_buf)
            {
                WJ_SAFE_DELETE_ARRAY(it->m_buf);
                it->m_buf = NULL;
            }
        }
        m_sendList.clear();
	}

	//�ϲ� SOCKET
	bool CWJSSocket::Attach(SOCKET hSocket, sockaddr_in & peerAddr)
	{
		//Ч������ 
		if (hSocket == INVALID_SOCKET) return false;

		m_sockid = ++m_sockidSeed;
        if (m_sockid == INVALID_SOCKET_ID)
        {
            m_sockid = ++m_sockidSeed;
        }
		m_sendPendingSize = 0;
		m_bSending = false;
		m_dwRecvBuffLen = 0;
		m_hSocket = hSocket;
		m_peerAddr = peerAddr;
		m_SendOverData.uOperationType = SOCKET_SND;
		m_RecvOverData.uOperationType = SOCKET_REV;
		m_Keep = (UINT)time(NULL);	//��ǰʱ��
		return true;
	}

	//�ر� SOCKET
	void CWJSSocket::CloseSocket()
	{
		if (m_hSocket != INVALID_SOCKET)
		{
			::closesocket(m_hSocket);
			m_bSending = false;
			m_dwRecvBuffLen = 0;
			m_hSocket = INVALID_SOCKET;
			m_SendOverData.uOperationType = SOCKET_SND;
			m_RecvOverData.uOperationType = SOCKET_REV;
		}
	}

	//�������ݺ���
	void CWJSSocket::SendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		//Ч������
		if (m_hSocket == INVALID_SOCKET)
		{
			return;
		}
		//��������
		
		NetMessageHead * pNetHead = (NetMessageHead *)new char[sizeof(NetMessageHead)];
		pNetHead->bMainID = bMainID;
		pNetHead->bAssistantID = bAssistantID;
		pNetHead->bHandleCode = bHandleCode;
		pNetHead->uMessageSize = sizeof(NetMessageHead);
		m_sendList.push_back(SendBuf((char*)pNetHead, sizeof(NetMessageHead)));
		CheckSendData();
	}

	//�������ݺ���
	void CWJSSocket::SendData(void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		if (/*pData == NULL ||*/ uBufLen + sizeof(NetMessageHead) > MAX_SEND_SIZE || m_hSocket == INVALID_SOCKET)
		{
			return ;
		}
		unsigned long dwzlSendBuffLen = 0;


		//��������
		NetMessageHead * pNetHead = (NetMessageHead *)new char [uBufLen + sizeof(NetMessageHead)];
		pNetHead->bMainID = bMainID;
		pNetHead->bAssistantID = bAssistantID;
		pNetHead->bHandleCode = bHandleCode;
		pNetHead->uMessageSize = sizeof(NetMessageHead) + uBufLen;
		if (pData != NULL)
		{
			CopyMemory(pNetHead + 1, pData, uBufLen);
		}
		m_sendList.push_back(SendBuf((char*)pNetHead, uBufLen + sizeof(NetMessageHead)));

		CheckSendData();
	}

	//�������ݺ���
	void CWJSSocket::SendData(const char * pData, UINT uBufLen)
	{
		if (m_hSocket == INVALID_SOCKET || pData == NULL || uBufLen > MAX_SEND_SIZE)
		{
			return;
		}
		NetMessageHead * pNetHead = (NetMessageHead *)new char[uBufLen];
		//��������
		CopyMemory(pNetHead, pData, uBufLen);
		m_sendList.push_back(SendBuf((char*)pNetHead, uBufLen));
		CheckSendData();

	}

	//��ⷢ�����ݺ���
	bool CWJSSocket::CheckSendData()
	{
		DWORD dwThancferred = 0;
		if (!m_bSending && !m_sendList.empty())
		{
			m_bSending = true;
			SendBuf & sendBuf = m_sendList.front();
			m_SendOverData.WSABuffer.buf = sendBuf.m_buf;
			m_SendOverData.WSABuffer.len = sendBuf.m_len;
			
			if ((::WSASend(m_hSocket, &m_SendOverData.WSABuffer, 1, &dwThancferred, 0, (LPWSAOVERLAPPED)&m_SendOverData, nullptr) == SOCKET_ERROR))
			{
				if (::WSAGetLastError() != WSA_IO_PENDING)
				{
					//xlog_out("Log_AFCComEx", "CTCPSocket", P_ERROR, "��ⷢ�����ݺ��� ʧ��1");

					m_bSending = false;
					return false;
				}
			}
            IncRef();
		}
		return true;
	}

	//������ɺ���
	bool CWJSSocket::OnSendCompleted(DWORD dwThancferred)
	{
		//��������
		m_bSending = false;
		if (dwThancferred > 0)
		{
			if (!m_sendList.empty())
			{
				SendBuf sendbuf = m_sendList.front();
				m_sendList.pop_front();
				WJ_SAFE_DELETE_ARRAY(sendbuf.m_buf);
			}
			return CheckSendData();
		}

		return false;
	}

	//������ɺ���
	bool CWJSSocket::OnRecvCompleted()
	{
		//��������
		int iRecvCode = ::recv(m_hSocket, m_szRecvBuf + m_dwRecvBuffLen, MAX_SEND_SIZE - m_dwRecvBuffLen, 0);

		//TRACE(TEXT("RECEIVE DATA SIZE:%d\n"), iRecvCode);
		if (iRecvCode > 0)
		{
			//��������
			m_dwRecvBuffLen += iRecvCode;
			NetMessageHead * pNetHead = (NetMessageHead *)m_szRecvBuf;
			if (pNetHead->uMessageSize > MAX_SEND_SIZE) //��̫��
			{
				return false;
			}
			while ((m_dwRecvBuffLen >= sizeof(NetMessageHead)) && (m_dwRecvBuffLen >= pNetHead->uMessageSize))
			{
				//QK_InfoLog("pNetHead->uMessageSize=%d", pNetHead->uMessageSize);
				//Ч������
				if (pNetHead->uMessageSize < sizeof(NetMessageHead))
				{
					return false;
				}
				//�������
				UINT uMessageSize = pNetHead->uMessageSize;
				UINT uHandleSize = uMessageSize - sizeof(NetMessageHead);

				// �����߼��㴦��
                bool bRes = m_pManage->ProccessNetMessage(this, pNetHead, uHandleSize ? pNetHead + 1 : nullptr, uHandleSize);
				if (!bRes)
				{
					m_pManage->CloseSocket(m_sockid);
					return false;
				}
                if (m_hSocket == INVALID_SOCKET)  // �߼����������ôЩԭ��(�����⵽���װ���Ƶ��������)�ر��˵�ǰ�׽��ֶ���
                {
                    return false;
                }
				//ɾ����������
				::MoveMemory(m_szRecvBuf, m_szRecvBuf + uMessageSize, m_dwRecvBuffLen - uMessageSize);
				m_dwRecvBuffLen -= uMessageSize;
			}
			return RecvData();
		}
		else  // �Զ˹رջ�socket����
		{
			return false;
		}

	}

	//Ͷ�ݽ��� IO
	bool CWJSSocket::RecvData()
	{
		//std::lock_guard <std::recursive_mutex> autoLock(m_dataMutex_Recv);
		DWORD dwThancferred = 0, dwFlags = 0;
		m_RecvOverData.WSABuffer.buf = m_szRecvBuf + m_dwRecvBuffLen;
		m_RecvOverData.WSABuffer.len = 0;
		int ires = ::WSARecv(m_hSocket, &m_RecvOverData.WSABuffer, 1, &dwThancferred, &dwFlags, (LPWSAOVERLAPPED)&m_RecvOverData, nullptr);
		if (0 != ires)
		{
			int ierror = ::WSAGetLastError();
			if (ierror != WSA_IO_PENDING)
			{
				//xlog_out("Log_AFCComEx", "CTCPSocket", P_ERROR, "Ͷ�ݽ��� IO ʧ�� �Ͽ�����");
				OutputDebugStringA("sdp_test CloseSocket(14)");
				m_pManage->CloseSocket(m_sockid);
				return false;
			}
		}
        IncRef();

		return true;
	}

	//���õ�ǰ�׽�������ʹ��ʱ��
	void CWJSSocket::SetKeep()
	{
		m_Keep = (UINT)time(NULL);	//��ǰʱ��
	}
	//����׽������ʹ��ʱ��
	UINT CWJSSocket::GetKeep()
	{
		return m_Keep;
	}
}