// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣


#include "LOLExtend.h"
#include "hook/EventSignal.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <atlimage.h>
#include <process.h>

GameInfoLOL g_info;
CLOLExtend g_lolextend;
extern bool isRead;
void NotifyMainWnd(HWND hWnd, UINT32 msg, void* buf, int len);
LRESULT CALLBACK WindowProcFunc_CTCPClientSocket_load(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define MSG_LOLINFO_HALL					(0x1001)	//����
#define MSG_LOLINFO_CLIENT					(0x1002)	//League of Legends.exe
//Ӣ�����˵�¼����
//PVP.net �ͻ���
//League of Legends(TM) Client
void HookProc(void* r)
{

	HWND hWnd = NULL;
	while (true)
	{
		hWnd = ::FindWindow(NULL, L"Ϭţ�羺");						
		if (hWnd && isRead)
		{
			g_lolextend.getUserInfo();
			OutputDebugString(L"Ϭţ�羺���ڴ���");
			NotifyMainWnd(hWnd, MSG_LOLINFO_CLIENT, &g_info, sizeof(g_info));
		}
		Sleep(1000);
	}

	return;
}
void WindowMsgThread(void* r)
{
	try
	{
		//ע�ᴰ����
		LOGBRUSH		LogBrush;
		WNDCLASS		WndClass;
		TCHAR			szClassName[] = TEXT("LOL Socket Window");

		LogBrush.lbColor = RGB(0, 0, 0);
		LogBrush.lbStyle = BS_SOLID;
		LogBrush.lbHatch = 0;
		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hCursor = NULL;
		WndClass.hIcon = NULL;
		WndClass.lpszMenuName = NULL;
		WndClass.lpfnWndProc = WindowProcFunc_CTCPClientSocket_load;
		WndClass.lpszClassName = szClassName;
		WndClass.style = CS_HREDRAW | CS_VREDRAW;
		WndClass.hInstance = NULL;
		WndClass.hbrBackground = (HBRUSH)::CreateBrushIndirect(&LogBrush);
		::RegisterClass(&WndClass);

		//��������
		g_lolextend.m_hWindow = ::CreateWindow(szClassName, NULL, 0, 0, 0, 0, 0, NULL, NULL, NULL, &g_lolextend);
		if (g_lolextend.m_hWindow == nullptr) throw TEXT("���ڽ���ʧ��");
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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{

		OutputDebugString(L"DllMain::DLL_PROCESS_ATTACH\n");
		
		HANDLE hThread = (HANDLE)_beginthread(HookProc, 0, NULL);
		HANDLE hThread2 = (HANDLE)_beginthread(WindowMsgThread, 0, NULL);

		break;
	}
	case DLL_THREAD_ATTACH:
	{
		OutputDebugString(L"DllMain::DLL_THREAD_ATTACH\n");

		break;
	}
	case DLL_THREAD_DETACH:
	{
		OutputDebugString(L"DllMain::DLL_THREAD_DETACH\n");

		break;
	}

	case DLL_PROCESS_DETACH:
	{
		OutputDebugString(L"DllMain::DLL_PROCESS_DETACH\n");

		break;
	}
	}
	return TRUE;
}


void NotifyMainWnd(HWND hWnd, UINT32 msg, void* buf, int len)
{
	COPYDATASTRUCT cpd;
	cpd.dwData = msg;
	cpd.lpData = buf;
	cpd.cbData = len;

	SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cpd);
}