#pragma once

#ifndef INSTALLTASK_H
#define INSTALLTASK_H

#include "GetHttpUrl.h"

class CInstallTask {

private:
	//���
	wstring																					m_SetupFlags;
	//POST�ύ
	CGetHttpUrl																				m_HttpPost;
	//��������ݷ�ʽ
	bool																					AddDesktopLink(LPCTSTR lptLink);
	//��ȡ�����ݷ�ʽ����
	bool																					GetDesktopLinkName(LPTSTR lptName, int inLen);
	//��Ʒ�б���������ǵĲ�Ʒ
	void																					AddInstall(LPCTSTR lptAppPath, LPCTSTR lptDirectory);
	//д��汾�Ͱ�װ��Ϣ
	void																					WriteVersionInfo(int inVersion, LPCTSTR lptDirectory);
	//������ط���
	void																					StartMonitor(LPCTSTR lptDirectory);
	//�ϱ���װ����
	void																					ReportSetup();
protected:
	//��ȡӲ�����к�
	bool																					GetDiskKey(LPWSTR lpwstr, int inLen);
public:
	CInstallTask();
	~CInstallTask();

	//ִ�а�װ����
	bool																					InstallTask(LPCTSTR lptInstallDir, CPaintManagerUI* pManager, int inVersion);
};

#endif