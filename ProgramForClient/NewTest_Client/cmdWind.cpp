#include "stdafx.h"
#include "cmdWind.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "message/WJNetPlatformMessage.h"
#include "clientconfig.h"

#define TIME_ID 100



CcmdWind::CcmdWind()
{
	m_hWindow = nullptr;
	m_bRunning = false;//��������
	std::thread recvThread_wind(std::bind(&CcmdWind::WindowMsgThread, this));
	recvThread_wind.detach();
}

CcmdWind::~CcmdWind()
{
	//�رմ��ھ��
	if ((m_hWindow != 0) && (::IsWindow((HWND)m_hWindow) == TRUE)) ::SendMessage((HWND)m_hWindow, WM_CLOSE, 0, 0);

	clear_socketlogic();
}

//����
void CcmdWind::clearWind()
{
	system("cls");
}

//���׽����߼��߳�
int CcmdWind::MakeSocketThread()
{
	std::cout << "==>�������������ˣ����Ժ�..." << std::endl;
	try
	{
		int inum = 0;
		while (1)
		{
			if (inum >= clientconfig().GetData()->AInum)
			{
				break;
			}

			{
				std::lock_guard <std::mutex> autoLock(m_dataMutex_test_client);
				WJ::WJPlatformLogic* plogic_struct = new WJ::WJPlatformLogic(inum);
				m_vector_logic_struct.push_back(plogic_struct);
			}

			++inum;
		}
		std::cout << "==>������������ɣ�����к���������" << std::endl;
	}
	catch (...)
	{
		clear_socketlogic();
		std::cout << "==>����������ʧ�ܣ��Ѿ��رջ�����" << std::endl;
	}

	std::cout << "==>������������ɣ�" << std::endl;
	return 0;
}

//��������߼�����
void CcmdWind::clear_socketlogic()
{
	std::lock_guard <std::mutex> autoLock(m_dataMutex_test_client);
	auto _iter = m_vector_logic_struct.begin();
	while (_iter != m_vector_logic_struct.end())
	{
		(*_iter)->close();
		WJ_SAFE_DELETE((*_iter));
		_iter = m_vector_logic_struct.erase(_iter);
	}
}

//�û�������ʱ��
void CcmdWind::OnTimeMesage(int itimeid)
{
	if (itimeid == TIME_ID)
	{
		for (UINT i = 0; i < m_vector_logic_struct.size(); ++i)
		{
			m_vector_logic_struct.at(i)->run();
		}
	}
}

//ִ�����Ӷ���
void CcmdWind::onsocketconnect()
{
	std::cout << "==>����ִ�����Ӷ���..." << std::endl;
	SetTimer(TIME_ID, 1000);

	//ִ�����Ӳ���
	for (UINT i = 0; i < m_vector_logic_struct.size(); ++i)
	{
		m_vector_logic_struct.at(i)->connect((char*)clientconfig().GetData()->hall_server_address.c_str(), 0);
	}
	std::cout << "==>ִ�����Ӷ�����ɡ�" << std::endl;
}

//ִ�йرն���
void CcmdWind::onsocketClose()
{
	std::cout << "==>����ִ�йرն���..." << std::endl;
	for (UINT i = 0; i < m_vector_logic_struct.size(); ++i)
	{
		m_vector_logic_struct.at(i)->close();
	}
	std::cout << "==>ִ�йرն�����ɡ�" << std::endl;
}

#define GWL_USERDATA        (-21)
//���ڻص�����
LRESULT CALLBACK WindowProcFunc_CcmdWind(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:		//���ڽ�����Ϣ
	{
		DWORD iIndex = TlsAlloc();
		CcmdWind * p = (CcmdWind *)(((CREATESTRUCT *)lParam)->lpCreateParams);
		TlsSetValue(iIndex, p);
		::SetWindowLong((HWND)hWnd, GWL_USERDATA, iIndex);
		break;
	}
	case WM_TIMER:		//��ʱ����Ϣ
	{
		DWORD iIndex = ::GetWindowLong((HWND)hWnd, GWL_USERDATA);
		CcmdWind * p = (CcmdWind *)::TlsGetValue(iIndex);//::TlsGetValue(iIndex);
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
		CcmdWind * p = (CcmdWind *)::TlsGetValue(iIndex);
		if (p != NULL) p->sethWindow(0);
		::TlsFree(iIndex);
		PostQuitMessage(0);
		//ExitProcess(0);
		break;
	}
	}
	return ::DefWindowProc((HWND)hWnd, uMsg, wParam, lParam);
}

HWND CcmdWind::gethWindow()
{
	return m_hWindow;
}

void CcmdWind::sethWindow(HWND hWindow)
{
	m_hWindow = hWindow;
}

//WINDOW ��Ϣѭ���߳�
void CcmdWind::WindowMsgThread()
{
	try
	{
		//ע�ᴰ����
		LOGBRUSH		LogBrush;
		WNDCLASS		WndClass;
		TCHAR			szClassName[] = TEXT("CcmdWindow");

		LogBrush.lbColor = RGB(0, 0, 0);
		LogBrush.lbStyle = BS_SOLID;
		LogBrush.lbHatch = 0;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hCursor = NULL;
		WndClass.hIcon = NULL;
		WndClass.lpszMenuName = NULL;
		WndClass.lpfnWndProc = WindowProcFunc_CcmdWind;
		WndClass.lpszClassName = szClassName;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		WndClass.hInstance = NULL;
		WndClass.hbrBackground = (HBRUSH)::CreateBrushIndirect(&LogBrush);
		::RegisterClass(&WndClass);

		//��������
		this->m_hWindow = ::CreateWindow(szClassName, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, this);
		if (this->m_hWindow == NULL) throw TEXT("���ڽ���ʧ��");
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
bool CcmdWind::SetTimer(UINT uTimerID, UINT uElapse)
{
	if ((m_hWindow != nullptr) && (IsWindow(m_hWindow) == TRUE))
	{
		::SetTimer(m_hWindow, uTimerID, uElapse, nullptr);
		return true;
	}
	return false;
}

//�����ʱ��
bool CcmdWind::KillTimer(UINT uTimerID)
{
	if ((m_hWindow != nullptr) && (::IsWindow(m_hWindow) == TRUE))
	{
		::KillTimer(m_hWindow, uTimerID);
		return true;
	}
	return false;
}

//��ʼ��ҳ��
void CcmdWind::ShowMainWind()
{
	char str[200] = { 0 };
	clearWind();
	std::cout << "################################��ҳ##################################" << std::endl;
	std::cout << "#��ӭʹ�ñ�������������������ʾѡ����в�������                    #" << std::endl;
	std::cout << "#1�����ý���                                                         #" << std::endl;
	std::cout << "#2�����н���                                                         #" << std::endl;
	std::cout << "#   �����˳�ϵͳ�������κ�ʱ�����롾q��                              #" << std::endl;
	std::cout << "######################################################################" << std::endl;
	do
	{
		std::cout << "��ѡ��";
		std::cin >> str;
		if (!ShowCommond(str))
		{
			if (0 == strcmp(str, "1"))
			{
				ShowSet();
				return;
			}
			else if (0 == strcmp(str, "2"))
			{
				ShowRun();
				return;
			}
			else
			{
				std::cout << "�������������ѡ��" << std::endl;
				continue;
			}
		}
		else
		{
			return;
		}

	} while (1);
}

//����ҳ��
void CcmdWind::ShowSet()
{
	char str[200] = { 0 };
	clearWind();
	std::cout << "###############################����ҳ��###############################" << std::endl;
	std::cout << "#1������                                                             #" << std::endl;
	std::cout << "#2����ѯ����                                                         #" << std::endl;
	std::cout << "#3����������                                                         #" << std::endl;
	std::cout << "#4��������ҳ                                                         #" << std::endl;
	std::cout << "######################################################################" << std::endl;

	do
	{
		std::cout << "==>�����룺";
		std::cin >> str;
		if (!ShowCommond(str))
		{
			if (0 == strcmp(str, "1"))
			{
				ShowSet();
				return;
			}
			else if (0 == strcmp(str, "2"))
			{
				std::cout << "==>��ǰ���ý����" << std::endl;
				std::cout << "��������Ŀ��" << clientconfig().GetData()->AInum << std::endl;
				std::cout << "��½������ip��" << clientconfig().GetData()->hall_server_address.c_str() << std::endl;
				std::cout << "����id��" << clientconfig().GetData()->contestid << std::endl;
				std::cout << "����id��" << clientconfig().GetData()->roomid << std::endl;
				continue;
			}
			else if (0 == strcmp(str, "3"))
			{
				std::cout << "��������Ŀ�����ܴ���" << AI_NUM << "����";
				std::cin >> str;
				clientconfig().GetData()->AInum = atoi(str) < AI_NUM ? atoi(str) : AI_NUM;
				std::cout << "��½������ip��";
				std::cin >> str;
				clientconfig().GetData()->hall_server_address = str;
				std::cout << "����id��";
				std::cin >> str;
				clientconfig().GetData()->contestid = atoi(str);
				std::cout << "����id��";
				std::cin >> str;
				clientconfig().GetData()->roomid = atoi(str);
				std::cout << "==>��������Ч��" << std::endl;
				continue;
			}
			else if (0 == strcmp(str, "4"))
			{
				ShowMainWind();
				return;
			}
			else
			{
				std::cout << "�������������ѡ��" << std::endl;
				continue;
			}
		}
		else
		{
			return;
		}

	} while (1);
}

//����ҳ��
void CcmdWind::ShowRun()
{
	char str[200] = { 0 };
	clearWind();
	std::cout << "###############################����ҳ��###############################" << std::endl;
	std::cout << "#1������                                                             #" << std::endl;
	std::cout << "#2����ѯ��������                                                     #" << std::endl;
	std::cout << "#3������                                                             #" << std::endl;
	std::cout << "#4��ֹͣ                                                             #" << std::endl;
	std::cout << "#5��������ҳ                                                         #" << std::endl;
	std::cout << "######################################################################" << std::endl;
	do
	{
		std::cout << "==>�����룺";
		std::cin >> str;
		if (!ShowCommond(str))
		{
			if (0 == strcmp(str, "1"))
			{
				ShowRun();
				return;
			}
			else if (0 == strcmp(str, "2"))
			{
				if (m_bRunning)
				{
					std::cout << "==>��ǰ��������..." << std::endl;
				}
				else
				{
					std::cout << "==>��ǰ�Ѿ�ֹͣ..." << std::endl;
				}
				continue;
			}
			else if (0 == strcmp(str, "3"))
			{
				if (m_bRunning)
				{
					std::cout << "==>��ǰ����������..." << std::endl;
					continue;
				}
				std::cout << "==>��ʼ����..." << std::endl;
				m_bRunning = true;
				MakeSocketThread();
				onsocketconnect();
				continue;
			}
			else if (0 == strcmp(str, "4"))
			{
				std::cout << "==>��ʼֹͣ..." << std::endl;
				onsocketClose();
				KillTimer(TIME_ID);
				m_bRunning = false;
				std::cout << "==>ֹͣ���..." << std::endl;
				continue;
			}
			else if (0 == strcmp(str, "5"))
			{
				ShowMainWind();
				return;
			}
			else
			{
				std::cout << "�������������ѡ��" << std::endl;
				continue;
			}
		}
		else
		{
			return;
		}
	} while (1);
}

bool CcmdWind::ShowCommond(char *str)
{
	if (nullptr != str)
	{
		if (0 == strcmp(str, "q"))
		{
			std::cout << "�˳�ϵͳ�ɹ���" << std::endl;
			return true;
		}
	}
	return false;
}