#ifndef _WindowMsgHandlerHS_H_
#define _WindowMsgHandlerHS_H_

#include <Windows.h>

extern WNDPROC g_wndProcDefault;

/*
* \brief:	¯ʯע�봰����Ϣ����ص�����
*/

LRESULT CALLBACK WndProcHS(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



#endif	// $END$ _WindowMsgHandlerHS_H_