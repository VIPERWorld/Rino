// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// Update.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

/*Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������


CString GetAppPath()
{
	//char lpBuffer[MAX_PATH];
	//ZeroMemory(lpBuffer, sizeof(lpBuffer));
	//GetCurrentDirectoryA(MAX_PATH, lpBuffer);

	char szFilePath[MAX_PATH + 1];
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //ɾ���ļ�����ֻ���·��
	CString ss = szFilePath;

	//CString ss = lpBuffer;
	//ss += "\\";
	return ss;
}