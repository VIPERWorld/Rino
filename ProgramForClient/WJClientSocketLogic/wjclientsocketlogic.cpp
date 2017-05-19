
#include "clientsocket/wjclientsocketlogic.h"
#include "common/WJCommonMarco.h"
#include "common/WJUIDelegateQueue.h"
//#include "clientsocket/WJSocketMessageDelegate.h"
#include "WJSocketProtocolData.h"
#include "common/WJCommonMarco.h"
#include <windows.h>
#include "clientsocket/commonfunction_c.h"
#ifdef _UNUSE_QT
//
#else
#include <QtWidgets/QApplication>
#endif

namespace WJ
{
#ifdef _UNUSE_QT
#define TIME_ID_SOCKET_DISPATCH 100 //������Ϣ֪ͨ������
#endif

#define TIME_ID_SOCKET_HEART 101 //��������������


	WJClientSocketLogic::WJClientSocketLogic(ISocketMessageDelegate* delegate, const std::string& tag) : _delegate(delegate)
	{
		//_bReadComplete = false;
		_socketThread = new WJSocketThread(this);
		//_socketThread->setTag(tag);
		_selectorQueue = new WJSocketMessageSelectorHandler();

		m_hWindow = 0;

		std::thread recvThread_wind(std::bind(&WJClientSocketLogic::WindowMsgThread, this));
		recvThread_wind.detach();

#ifdef _UNUSE_QT

#else
		QTimer *timer = new QTimer(this);
		bool bres = connect(timer, SIGNAL(timeout()), this, SLOT(socketDataDispatch()));//������ʱ�������źź���Ӧ�Ĳۺ���
		timer->start(1000);//��ʱ����ʼ��ʱ������1000��ʾ1000ms��1��
#endif

	}

	WJClientSocketLogic::~WJClientSocketLogic(void)
	{
		//�رմ��ھ��
		if ((m_hWindow != 0) && (::IsWindow((HWND)m_hWindow) == TRUE)) ::SendMessage((HWND)m_hWindow, WM_CLOSE, 0, 0);

		resetData();
		if (nullptr != _socketThread)
		{
			_socketThread->close();
		}

		WJ_SAFE_DELETE(_selectorQueue);
		WJ_SAFE_DELETE(_socketThread);
	}

	bool WJClientSocketLogic::init()
	{
		return true;
	}

	void WJClientSocketLogic::resetData()
	{
		if (nullptr != _selectorQueue)
		{
			_selectorQueue->clear();
		}
	}

	bool WJClientSocketLogic::openWithIp(const char* ip, int port)
	{
		return _socketThread->openWithIp(ip, port);
	}

	bool WJClientSocketLogic::openWithHost(const char* host, int port)
	{
		return _socketThread->openWithHost(host, port);
	}

	bool WJClientSocketLogic::close()
	{
		OutputDebugStringA("sdp_test client �ر� SOCKET close3");
		resetData();
		return _socketThread->close();
	}

	BYTE WJClientSocketLogic::GetSocketState()
	{
		return _socketThread->GetSocketState();
	}

	bool WJClientSocketLogic::connected()
	{
		return _socketThread->connected();
	}

	//��������
	INT WJClientSocketLogic::send(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{
		return send(MainID, AssistantID, 0, object, objectSize);
	}

	INT WJClientSocketLogic::send(UINT MainID, UINT AssistantID, UINT uHandleCode, void* object, INT objectSize)
	{
		if (connected())
		{
			return _socketThread->sendData(object, objectSize, MainID, AssistantID, uHandleCode);
		}

		return 0;
	}

	void WJClientSocketLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		assert(nullptr != selector && "selector is nullptr");
		if (nullptr != selector)
		{
			CHAR messageKey[16] = { 0 };
			sprintf(messageKey, SELECTER_KEY_FORMAT, MainID, AssistantID);
			_selectorQueue->addSelector(messageKey, selector);
		}
	}

	void WJClientSocketLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		CHAR messageKey[16] = { 0 };
		sprintf(messageKey, SELECTER_KEY_FORMAT, MainID, AssistantID);
		_selectorQueue->removeSelector(messageKey);
	}

	void WJClientSocketLogic::heartBeat()
	{
		if (connected())
		{
			int ret = send(NET_M_CONNECT, ASS_NET_CONNECT_1, 0, nullptr, 0);
			if (ret >= 0)
			{
				//HNLOG_WARNING("%s >> heartBeat", _socketThread->getTag().c_str());
			}
		}
	}

	//void WJClientSocketLogic::onReadComplete()
	//{
	//	_bReadComplete = true;
	//}

	void WJClientSocketLogic::socketDataDispatch()
	{
		_socketThread->transform_socket([this](WJSocketMessage* socketMessage)
		{
			assert(nullptr != _delegate);
			switch (socketMessage->socketStatus)
			{
			case SocketStatus_UNKNOW:
			{
				_selectorQueue->clear();
				_delegate->onConnected(false, SocketStatus_UNKNOW);
			}break;
			case SocketStatus_CONNECT:
				break;
			case SocketStatus_OUTTIME:
			{
				_selectorQueue->clear();
				_delegate->onConnected(false, SocketStatus_OUTTIME);
			}break;
			case SocketStatus_SUCCESS:
			{
				_delegate->onConnected(true, SocketStatus_SUCCESS);
			}break;
			case SocketStatus_RECV:
			{
				//MyDebugString("sdp_test2", "client �ײ���Ϣ2(%d,%d,%d)", socketMessage->messageHead.bMainID, socketMessage->messageHead.bAssistantID, socketMessage->messageHead.bHandleCode);
				bool ret = _selectorQueue->executeSelector(socketMessage->strKey, socketMessage);
				if (!ret)
				{
					_delegate->onSocketMessage(socketMessage);
				}
			}break;
			case SocketStatus_DISCONNECT:
			{
				_selectorQueue->clear();
				_delegate->onDisConnect();
			}break;
			default:
				break;
			}
		});

		//_bReadComplete = false;
	}

	void WJClientSocketLogic::socketHeartBeatSend()
	{
		if (connected())
		{
			heartBeat();
		}
	}


	//��ʱ��
	void WJClientSocketLogic::OnTimeMesage(int itimeid)
	{
#ifdef _UNUSE_QT
		if (TIME_ID_SOCKET_DISPATCH == itimeid)
		{
			socketDataDispatch();
		}
#endif	
		if (TIME_ID_SOCKET_HEART == itimeid)
		{
			socketHeartBeatSend();
		}
	}

#define GWL_USERDATA        (-21)
	//���ڻص�����
	LRESULT CALLBACK WindowProcFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:		//���ڽ�����Ϣ
		{
			DWORD iIndex = TlsAlloc();
			WJClientSocketLogic * p = (WJClientSocketLogic *)(((CREATESTRUCT *)lParam)->lpCreateParams);
			TlsSetValue(iIndex, p);
			::SetWindowLong((HWND)hWnd, GWL_USERDATA, iIndex);
			break;
		}
		case WM_TIMER:		//��ʱ����Ϣ
		{
			DWORD iIndex = ::GetWindowLong((HWND)hWnd, GWL_USERDATA);
			WJClientSocketLogic * p = (WJClientSocketLogic *)::TlsGetValue(iIndex);//::TlsGetValue(iIndex);
			if (p != nullptr){ p->OnTimeMesage((int)wParam); }
			break;
		}
		case WM_CLOSE:		//���ڹر���Ϣ
		{
			DestroyWindow((HWND)hWnd);
			break;
		}
		case WM_DESTROY:	//���ڹر���Ϣ
		{
			DWORD iIndex = ::GetWindowLong((HWND)hWnd, GWL_USERDATA);
			WJClientSocketLogic * p = (WJClientSocketLogic *)::TlsGetValue(iIndex);
			if (p != NULL) p->sethWindow(0);
			::TlsFree(iIndex);
			PostQuitMessage(0);
			//ExitProcess(0);
			break;
		}
		}
		return ::DefWindowProc((HWND)hWnd, uMsg, wParam, lParam);
	}

	int WJClientSocketLogic::gethWindow()
	{
		return m_hWindow;
	}

	void WJClientSocketLogic::sethWindow(int hWindow)
	{
		m_hWindow = hWindow;
	}

	//WINDOW ��Ϣѭ���߳�
	void WJClientSocketLogic::WindowMsgThread()
	{
		try
		{
			//ע�ᴰ����
			LOGBRUSH		LogBrush;
			WNDCLASS		WndClass;
			TCHAR			szClassName[] = TEXT("WJClientSocketLogicWindow");

			LogBrush.lbColor = RGB(0, 0, 0);
			LogBrush.lbStyle = BS_SOLID;
			LogBrush.lbHatch = 0;
			WndClass.cbClsExtra = 0;
			WndClass.cbWndExtra = 0;
			WndClass.hCursor = NULL;
			WndClass.hIcon = NULL;
			WndClass.lpszMenuName = NULL;
			WndClass.lpfnWndProc = WindowProcFunc;
			WndClass.lpszClassName = szClassName;
			WndClass.style = CS_HREDRAW | CS_VREDRAW;
			WndClass.hInstance = NULL;
			WndClass.hbrBackground = (HBRUSH)::CreateBrushIndirect(&LogBrush);
			::RegisterClass(&WndClass);

			//��������
			this->m_hWindow = (int)::CreateWindow(szClassName, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, this);
			if (this->m_hWindow == NULL) throw TEXT("���ڽ���ʧ��");

#ifdef _UNUSE_QT
			SetTimer(TIME_ID_SOCKET_DISPATCH, 1000);
#endif
			SetTimer(TIME_ID_SOCKET_HEART, 1000);
		}
		catch (...)
		{
			//TRACE("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
			//��������
		}

		//��Ϣѭ��
		MSG	Message;
		while (::GetMessage(&Message, NULL, 0, 0))
		{
			if (!::TranslateAccelerator(Message.hwnd, NULL, &Message))
			{
				::TranslateMessage(&Message);
				::DispatchMessage(&Message);
			}
			Sleep(0);
		}
	}

	//�趨��ʱ��
	bool WJClientSocketLogic::SetTimer(UINT uTimerID, UINT uElapse)
	{
		if ((m_hWindow != 0) && (IsWindow((HWND)m_hWindow) == TRUE))
		{
			::SetTimer((HWND)m_hWindow, uTimerID, uElapse, nullptr);
			return true;
		}
		return false;
	}

	//�����ʱ��
	bool WJClientSocketLogic::KillTimer(UINT uTimerID)
	{
		if ((m_hWindow != 0) && (::IsWindow((HWND)m_hWindow) == TRUE))
		{
			::KillTimer((HWND)m_hWindow, uTimerID);
			return true;
		}
		return false;
	}

}