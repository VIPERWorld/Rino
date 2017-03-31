#include "stdafx.h"
#include "server/WJListenSocket.h"
namespace WJ
{
	//���캯��
	CWJListenSocket::CWJListenSocket(void)
	{
		m_hSocket = INVALID_SOCKET;
		m_uListenType = 0;
	}

	//��������
	CWJListenSocket::~CWJListenSocket(void)
	{
		OutputDebugStringA("sdp_test CloseSocket(10)");
		CloseSocket();
	}

	//���� SOCKET
	bool CWJListenSocket::Create()
	{
		m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
		return (m_hSocket != INVALID_SOCKET);
	}

	//�󶨵�ַ
	bool CWJListenSocket::Bind(const char *bindIp, UINT uPort)
	{
		::memset(&m_SocketAddr, 0, sizeof(m_SocketAddr));
		m_SocketAddr.sin_family = AF_INET;
		m_SocketAddr.sin_addr.s_addr = inet_addr(bindIp);
		m_SocketAddr.sin_port = htons(uPort);
		return (::bind(m_hSocket, (SOCKADDR*)&m_SocketAddr, sizeof(m_SocketAddr)) != SOCKET_ERROR);
	}

	//��������
	bool CWJListenSocket::Listen(UINT uListenType)
	{
		m_uListenType = uListenType;
		return (::listen(m_hSocket, 4096) != SOCKET_ERROR);
	}

	//�ر� SOCKET
	bool CWJListenSocket::CloseSocket()
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
		return true;
	}
}
