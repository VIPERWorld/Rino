// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>
#include <string>
#include "LOLCore.h"
#include <thread>
using namespace std;



TGameInfo info;
void GetGameInfo(TGameInfo& info);
wstring GetAreaName(DWORD id);
void NotifyMainWnd(HWND hWnd, UINT32 msg, void* buf, int len);

DWORD WINAPI ThreadSendProc(LPVOID lpParameter);
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {

		OutputDebugString(L"DllMain::DLL_PROCESS_ATTACH\n");

		HANDLE hThread = CreateThread(NULL, 0, ThreadSendProc, 0, 0, NULL);

		if (NULL != hThread) {

			::CloseHandle(hThread);
		}
	}
		break;
	case DLL_THREAD_ATTACH:
		OutputDebugString(L"DllMain::DLL_THREAD_ATTACH\n");

		break;
	case DLL_THREAD_DETACH:
		OutputDebugString(L"DllMain::DLL_THREAD_DETACH\n");

		break;
	case DLL_PROCESS_DETACH:
		OutputDebugString(L"DllMain::DLL_PROCESS_DETACH\n");

		break;
	}
	return TRUE;
}
DWORD WINAPI ThreadSendProc(LPVOID lpParameter)
{
	HWND hWnd;
	memset(&info, 0, sizeof(TGameInfo));
	GetGameInfo(info);


	while (true)
	{
		//OutputDebugString(L"111");
		hWnd = ::FindWindow(NULL, L"Ϭţ�羺");
		if (hWnd)
		{
			//OutputDebugString(L"Ϭţ�羺���ڴ���");
			NotifyMainWnd(hWnd, MSG_LOLINFO_HALL, &info, sizeof(TGameInfo));
			//MessageBox(NULL, info.area, TEXT(""), NULL);
		}
		Sleep(3000);
	}
	return 0;
}

void GetGameInfo(TGameInfo& info)
{
	LPWSTR *szArgList;
	int argCount;
	LPWSTR  cmdline = GetCommandLine();
	if (cmdline == NULL)
	{
		return;
	}

	OutputDebugString(cmdline);
	szArgList = CommandLineToArgvW(cmdline, &argCount);

	//QQ��
	if (NULL != szArgList[2]) {

		info.dwUin = wcstoul(szArgList[2], 0, 10);
	}
	//����
	if (NULL != szArgList[6]) {

		DWORD dwArea = wcstoul(szArgList[6], 0, 10);
		wstring strArea = GetAreaName(dwArea);
		wmemcpy(info.area, strArea.c_str(), strArea.length());
	}

	LocalFree(szArgList);

}
void NotifyMainWnd(HWND hWnd, UINT32 msg, void* buf, int len)
{
	COPYDATASTRUCT cpd;
	cpd.dwData = msg;
	cpd.lpData = buf;
	cpd.cbData = len;

	SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cpd);
}
wstring GetAreaName(DWORD id)
{
	wstring a;
	switch (id)
	{
		//����
	case  AiOuNiYa:
		a = L"��ŷ����";
		break;
	case ZhuAng:
		a = L"�氲";
		break;
	case NuoKeShaShi:
		a = L"ŵ����˹";
		break;
	case BanDeErCheng:
		a = L"��¶���";
		break;
	case PiErTeWoFu:
		a = L"Ƥ�����ַ�";
		break;
	case ZhangZhengXueYuan:
		a = L"ս��ѧԺ";
		break;
	case JuShengFeng:
		a = L"�����";
		break;
	case LeiSheShouBei:
		a = L"��ɪ�ر�";
		break;
	case GangTieLieYang:
		a = L"��������";
		break;
	case ChaiJueZhiDi:
		a = L"�þ�֮��";
		break;
	case HeiSheMeiZhui:
		a = L"��ɫõ��";
		break;
	case AngYingDao:
		a = L"��Ӱ��";
		break;
	case JunHengJiaoPai:
		a = L"�������";
		break;
	case ShuiJinZhiHeng:
		a = L"ˮ��֮��";
		break;
	case YinLiu:
		a = L"Ӱ��";
		break;
	case ShouWanZhiHai:
		a = L"����֮��";
		break;
	case ZhengFuZhiHai:
		a = L"����֮��";
		break;
	case KaLaMangDa:
		a = L"��������";
		break;
	case PiChengJinBei:
		a = L"Ƥ�Ǿ���";
		break;
	case Nanjuelingyu:
		a = L"�о�����";
		break;

		//��ͨ
	case BiErJiWoTe:
		a = L"�ȶ�������";
		break;
	case DeMaXiYa:
		a = L"��������";
		break;
	case FeiLeiErZhuoDe:
		a = L"���׶�׿��";
		break;
	case WuWeiXianFeng:
		a = L"��η�ȷ�";
		break;
	case ShuRuiMa:
		a = L"ˡ����";
		break;
	case NiuQuChongLin:
		a = L"Ť������";
		break;
	case JuLongZhiChao:
		a = L"����֮��";
		break;

		//����
	case JiaoYuWangZhuanQu:
		a = L"������ר��";
		break;

	default:
		break;
	}
	return  a;
}