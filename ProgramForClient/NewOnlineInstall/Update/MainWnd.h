#pragma once

#ifndef MAINWND_H
#define MAINWND_H

#include "Cancel.h"
#include "GetUpdate.h"

class CMainWnd : public WindowImplBase {

private:
	//��ʾ�ı�
	CTextUI*																				m_pTilText;
	//�ٶ�
	CTextUI*																				m_pTilSpeed;
	//����
	CProgressUI*																			m_pProgress;
	//ȡ����������
	CButtonUI*																				m_pCancelReDownLoad;
protected:
	//�����߳�
	static DWORD WINAPI																		TaskThreadProc(LPVOID lpParameter);
	//����ƽ̨
	void																					RunMainroutine();
	//��Ȩ
	BOOL																					EnablePrivilege(LPCTSTR lptName); 
public:
	CMainWnd();
	~CMainWnd();

	//Ƥ��
	CDuiString																				GetSkinFile();
	//����
	LPCTSTR																					GetWindowClassName() const;
	//��ʼ��
	void																					InitWindow();
	//��Ӧ
	void																					Notify(TNotifyUI& msg);
};

#endif