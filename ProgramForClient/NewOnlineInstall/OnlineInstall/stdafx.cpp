// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// OnlineInstall.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H �������κ�����ĸ���ͷ�ļ���
//�������ڴ��ļ�������
//����
HANDLE																						g_hSetupMutex = NULL;
//��װ·��
wchar_t																						g_wchInstallPath[1024] = { 0 };
//����ZIP·��
wchar_t																						g_wchDownloadPath[1024] = { 0 };
//������ָ��
CPaintManagerUI*																			g_pMainWndManager = NULL;
//��Ĭ��װ
bool																						g_bNoWindow = false;