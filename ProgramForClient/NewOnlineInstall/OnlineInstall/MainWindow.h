#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Download.h"

class CMainWindow : public WindowImplBase {

private:
	//·���޸İ�ť
	static CButtonUI*																		m_pBtnShow;
	//�Զ��尲װ
	static CVerticalLayoutUI*																m_pVerlayout;
	//·����
	CEditUI*																				m_pPath;
	//�̷��ռ�M
	DWORD64																					m_dwMSize;
	//���ؽ���
	CDownload*																				m_pDownload;
protected:
	//��ʾ
	static VOID CALLBACK																	ShowTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	//����
	static VOID CALLBACK																	HideTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
public:
	CMainWindow();
	~CMainWindow();

	//Ƥ���ļ�
	CDuiString																				GetSkinFile();
	//����������
	LPCTSTR																					GetWindowClassName() const;
	//��ʼ��
	void																					InitWindow();
	//����¼�
	void																					Notify(TNotifyUI& msg);
	//��ʾ��������·��
	void																					ShowHidePath(CButtonUI* pBtnShow);
	//���ð�װ����·��
	void																					SetInstallPath();
	//ѡ��Ŀ¼
	void																					SwitchDirectory();
	//��ȡ�̷���Ϣ
	void																					GetDiskInfo();
};

#endif