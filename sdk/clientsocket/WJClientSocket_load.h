#ifndef _WJ_CTCPClientSocket_load_H_
#define _WJ_CTCPClientSocket_load_H_

#include <vector>
#include "common/WJBaseType.h"
#include "message/WJNetMessageHead.h"
#include "WJSocketMessage.h"
namespace WJ
{
#define NO_CONNECT				0								//û������
#define CONNECTING				1								//��������
#define CONNECTED				2								//�ɹ�����
	class IClientSocketService_load;
	//�ͻ��� SOKET
	class WJCLIENTSOCKETLOGIC_EXPORT CTCPClientSocket_load
	{
		//��������
		private:
		int							m_iReadBufLen;					//����������
		BYTE						m_bConnectState;				//����״̬
		/*SOCKET*/LLONG				m_hSocket;						//SOCKET ���
		IClientSocketService_load		* m_pIService;					//����ӿ�
		BYTE						m_szBuffer[MAX_SEND_SIZE];			//���ݻ�����

		int							m_iCheckCode;

		/*HWND*/int					m_hWindow;					///���ھ��
		bool							m_bRunSafeThreading_c_wind;			//�Ƿ񴰿��߳�����
		std::vector<char>				m_vRecvBuf;

		long m_dwServerIP;//ip
		UINT m_uPort;//�˿�

		int								m_nKeepAliveCount;//���ӵ���ʱ
		//��������
	public:
		//���캯��
		explicit CTCPClientSocket_load(IClientSocketService_load * pIService);
		//��������
		virtual ~CTCPClientSocket_load();
		//�Ƿ��Ѿ�����
		inline BYTE GetSocketState() { return m_bConnectState; }
		//�ر� SOCKET
		inline bool CloseSocket(bool bNotify = true);
		//���ӷ�����
		inline bool Connect(const char * szServerIP, int uPort);
		//���ӷ�����
		inline bool Connect(long int dwServerIP, int uPort);
		//���ͺ���
		inline int SendData(void * pData, UINT uSize, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//������ͺ���
		inline int SendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode);

		//����У���룬��������Կ
		void SetCheckCode(__int64 iCheckCode, int isecretkey);

		bool CheckReConnect();
		bool CheckKeepLive();
		int gethWindow();
		void sethWindow(int hWindow);
		//SOCKET ��Ϣ�������
		/*LRESULT*/long	OnSocketNotifyMesage(/*WPARAM*/int wParam, /*LPARAM*/long lParam);
		//�ڲ�����
	private:
		
		///WINDOW ��Ϣѭ���߳�
		void WindowMsgThread();
	};

	///�ͻ����������ӿ�
	class IClientSocketService_load
	{
		///�ӿں��� 
	public:
		///�����ȡ��Ϣ
		virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket_load * pClientSocket) = 0;
		///����������Ϣ
		virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket_load * pClientSocket) = 0;
		///����ر���Ϣ
		virtual bool OnSocketCloseEvent() = 0;
	};
}

#endif //_WJ_CTCPClientSocket_load_H_