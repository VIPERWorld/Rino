/*Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/
#include "stdafx.h"
#include "MyFileOperation.h"

#include<shellapi.h>

CMyFileOperation::CMyFileOperation()
{
}

CMyFileOperation::~CMyFileOperation()
{
}

/////////////////////////////////////
//������:DeleteFolder
//���������LpszPath Ҫɾ����·��ָ��
//���ã�ɾ��ָ���ļ����Լ�������ļ�
//
/////////////////////////////////////
BOOL CMyFileOperation::DeleteFolder(LPCTSTR lpszPath)
{
	int nLength = strlen(lpszPath);
	char *NewPath = new char[nLength+2];
	strcpy(NewPath,lpszPath);
	NewPath[nLength] = '\0';
	NewPath[nLength+1] = '\0';
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPath;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}

/////////////////////////////////////
//��������CopyFolder
//������lpszFromPath Դ�ļ��е�·�� �� lpszToPath Ŀ���ļ��е�·��
//���ã������ļ��м����ļ����е���������
//
//////////////////////////////////////
BOOL CMyFileOperation::CopyFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm+2];
	strcpy(NewPathFrm,lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm+1] = '\0';
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_COPY;
	return SHFileOperation(&FileOp) == 0;
}

/////////////////////////////////////
//��������MoveFolder
//������lpszFromPath Դ�ļ���·�� ��lpszToPath Ŀ���ļ���·��
//���ã��ƶ�ԭ�ļ��м������ļ���ָ����·����
//
/////////////////////////////////////
BOOL CMyFileOperation::MoveFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm+2];
	strcpy(NewPathFrm,lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm+1] = '\0';
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_MOVE;
	return SHFileOperation(&FileOp) == 0;
}

/////////////////////////////////////
//ReNameFolder
//������lpszFromPath Դ�ļ���·�� ��lpszToPath Ŀ���ļ���·��
//���ã��޸�ԭ�ļ��е����֡�
//
/////////////////////////////////////
BOOL CMyFileOperation::ReNameFolder(LPCTSTR lpszFromPath,LPCTSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm+2];
	strcpy(NewPathFrm,lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm+1] = '\0';
	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.fFlags = FOF_NOCONFIRMATION ;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_RENAME;
	return SHFileOperation(&FileOp) == 0;
}