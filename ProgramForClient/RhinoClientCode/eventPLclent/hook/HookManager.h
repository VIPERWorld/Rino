#ifndef _HOOKMANAGER_H
#define _HOOKMANAGER_H

#include <Windows.h>

enum EnHookStatus
{
	E_HOOK_SUCCESS = 0,				/* �ɹ� */
	E_HOOK_DLL_NULLPTR,				/* ָ����DLLΪ�� */
	E_HOOK_DLL_NOT_EXIST,			/* DLL������ */
	E_HOOK_INJECT_FAILE,			/* ע��ʧ�� */
	E_HOOK_SET_PRIVILEGE_FAILED,	/* ���ð�ȫ����ʧ�� */
	E_HOOK_OPEN_PROCESS_FAILED,		/* �򿪽���ʧ�� */
	E_HOOK_CREATE_REMOTE_THREAD_FAILED,	/* ����Զ���߳�ʧ�� */
	E_HOOK_WINDOW_NOT_EXIST,	/* ���ڲ����� */
};


class HookManager
{
public:
	/**
	* @date:	2016/08/20
	* @brief:	Զ��ע��DLL
	* @param:	[_processId]:	����ID
	* @return:	[_dllName]:		DLL����
	* @usage:	
	*/
	static EnHookStatus injectDll(unsigned long _processId, TCHAR* _dllName);

	
	static EnHookStatus ganmetaskinjectDll(HWND _hWnd, const char* _path, const char* _dllName);

private:
	static BOOL setPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

	static bool MyCreateRemoteThread(HANDLE _hProcess, LPTHREAD_START_ROUTINE _pThreadProc, LPVOID _pRemoteBuf);
};

#endif // !_HOOKMANAGER_H
