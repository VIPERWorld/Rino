#pragma once

#include "resource.h"

//������װ
void													SetupRhino(HINSTANCE hInstance, bool bShow = true);

//��Ĭ��װ
VOID CALLBACK											InstallTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

//У�������ļ�
bool													CheckConfigFile();