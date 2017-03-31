#ifndef _WJ_CTCPClientSocket_H_
#define _WJ_CTCPClientSocket_H_

#include <vector>
#include <WinSock2.h>
#include "common/WJBaseType.h"
#include "server/WJInterface.h"

namespace WJ
{
#define NO_CONNECT				0								//û������
#define CONNECTING				1								//��������
#define CONNECTED				2								//�ɹ�����

	//�ͻ��� SOKET
	class WJSERVERMANAGELIB_API CTCPClientSocket
	{
		//��������
	private:
		int							m_iReadBufLen;					//����������
		BYTE						m_bConnectState;				//����״̬
		SOCKET						m_hSocket;						//SOCKET ���
		IClientSocketService		* m_pIService;					//����ӿ�
		BYTE						m_szBuffer[MAX_SEND_SIZE];			//���ݻ�����

		int							m_iCheckCode;

		HWND							m_hWindow;					///���ھ��
		bool							m_bRunSafeThreading_c_wind;			//�Ƿ񴰿��߳�����
		std::vector<char>				m_vRecvBuf;

		long m_dwServerIP;//ip
		UINT m_uPort;//�˿�

		int								m_nKeepAliveCount;//���ӵ���ʱ
		//��������
	public:
		//���캯��
		explicit CTCPClientSocket(IClientSocketService * pIService);
		//��������
		virtual ~CTCPClientSocket();
		//�Ƿ��Ѿ�����
		inline BYTE GetSocketState() { return m_bConnectState; }
		//�ر� SOCKET
		inline bool CloseSocket(bool bNotify = true);
		//���ӷ�����
		inline bool Connect(const char * szServerIP, UINT uPort);
		//���ӷ�����
		inline bool Connect(long int dwServerIP, UINT uPort);
		//���ͺ���
		inline int SendData(void * pData, UINT uSize, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//������ͺ���
		inline int SendData(UINT bMainID, UINT bAssistantID, UINT bHandleCode);

		//����У���룬��������Կ
		void SetCheckCode(__int64 iCheckCode, int isecretkey);

		bool CheckReConnect();
		bool CheckKeepLive();
		//�ڲ�����
	private:
		//SOCKET ��Ϣ�������
		inline LRESULT	OnSocketNotifyMesage(WPARAM wParam, LPARAM lParam);

		///WINDOW ��Ϣѭ���߳�
		void WindowMsgThread();
		///���ڻص�����
		static LRESULT CALLBACK WindowProcFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	};
}

#endif //_WJ_CTCPClientSocket_H_