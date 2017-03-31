#ifndef _WJ_ListenSocket_H_
#define _WJ_ListenSocket_H_
#include <WinSock2.h>
#include "common/WJBaseType.h"
namespace WJ
{
	//������ SOCKET
	class WJSERVERMANAGELIB_API CWJListenSocket
	{
		//��������
	private:
		SOCKET						m_hSocket;							//SOCKET ���
		SOCKADDR_IN					m_SocketAddr;						//����������Ϣ
		UINT                        m_uListenType;
		//��������
	public:
		//���캯��
		CWJListenSocket(void);
		//��������
		virtual ~CWJListenSocket(void);

		//���ܺ���
	public:
		//���� SOCKET
		bool Create();
		//��������
		bool Listen(UINT uListenType);
		//�󶨵�ַ
		bool Bind(const char *bindIp, UINT uPort);
		inline UINT GetListenType() { return m_uListenType; }
		//�ر� SOCKET
		inline bool CloseSocket();
		//��ȡ SOCKET ���
		inline SOCKET GetSafeSocket() { return m_hSocket; }
		//��ȡ����������Ϣ
		inline const sockaddr_in *GetConnectData() { return &m_SocketAddr; }
	};
}


#endif //_WJ_ListenSocket_H_