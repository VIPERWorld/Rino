// RhinoLauncher.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <ShellAPI.h>

#include "commonfunction_c.h"
#include "RhinoLauncher.h"

//����dos����
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main(int argc, _TCHAR* argv[])
{
	//::MessageBox(NULL, NULL, _T("I am comeing!"), MB_OK);
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, L"RhinoLauncher");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{//ֻ���п�һ��ʵ��
		return 0;
	}


	//����ж����߸��´�������������Ҫ���������߸�����
	std::string str_Update = GetAppPath().c_str();
#ifdef _DEBUG
	str_Update += "Update_d.exe";
#else
	str_Update += "Update.exe";
#endif // _DEBUG
	
	while (true)
	{
		if (0 != FindProcess((char*)str_Update.c_str()))
		{//����update��һֱ�ȴ�
			OutputDebugStringA("sdp_aaa ����Update.exe");
			continue;
		}
		else
		{//�����ھͿ���ȥ����update��
			break;
		}
	}

	std::string s_Client = GetAppPath().c_str();
#ifdef _DEBUG
	s_Client += "Client_d.exe";
#else
	s_Client += "Client.exe";
#endif // _DEBUG

	CRhinoLauncher cmd;
	if (!cmd.VersionUpdate())
	{//����Ҫ�����������ͻ���
		SHELLEXECUTEINFOA ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = s_Client.c_str();
		ShExecInfo.lpParameters = NULL;
		ShExecInfo.lpDirectory = GetAppPath().c_str();
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = NULL;
		::ShellExecuteExA(&ShExecInfo);
	}
	return 0;
}