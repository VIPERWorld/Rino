#pragma once

#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "GetHttpUrl.h"
#include "UnzipFile.h"
#include "InstallTask.h"
#include "Complete.h"

#define ID_TIMERIMAGEHIDE																		WM_USER + 1
#define ID_TIMERIMAGESHOW																		WM_USER + 2
#define ID_TIMERIMAGESWITCH																		WM_USER + 3

class CDownload : public WindowImplBase {

private:
	//����ͼ
	static CLabelUI*																		m_pBkImage;
	//��ǰͼ���
	static int																				m_CurIndex;
	//�´�ͼ���
	static int																				m_NextIndex;
	//͸����ֵ
	static int																				m_Opacity;
	//��������
	CTextUI*																				m_pProgrText;
	//������
	CProgressUI*																			m_pProgress;
	//�ٶ�����
	CTextUI*																				m_pSpeedText;
	//����
	CGetHttpUrl																				m_GetHttpUrl;
	//��ѹ
	CUnzipFile																				m_UnZipFile;
	//��װ����
	CInstallTask																			m_InstallTask;
	//��װ���
	CComplete*																				m_pComplete;
	//�ر�
	bool																					m_bClose;
protected:
	//������ͼ
	static VOID CALLBACK																	SwitchTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	//�������ر���ͼ
	static VOID CALLBACK																	HideTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	//������ʾ����ͼ
	static VOID CALLBACK																	ShowTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	//�����߳�
	static DWORD WINAPI																		DownLoadThreadProc(LPVOID lpParameter);
public:
	CDownload();
	~CDownload();

	//Ƥ���ļ�
	CDuiString																				GetSkinFile();
	//����������
	LPCTSTR																					GetWindowClassName() const;
	//��ʼ��
	void																					InitWindow();
	//����¼�
	void																					Notify(TNotifyUI& msg);
};

#endif