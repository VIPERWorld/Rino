#pragma once

#ifndef DOWNLOADZIP_H
#define DOWNLOADZIP_H

class CDownloadZip {

private:
	CURL*																				m_pCurl;
	//�ļ��ܳ���
	long																				m_lFileLen;
	//ȡ������
	bool																				m_bEndDownLoad;
	//������ָ��
	void*																				m_pMainWnd;
	//�����ܴ�С
	double																				m_dDownLen;
	//��·��
	wstring																				m_InstallPath;
protected:
	//д�ļ�
	static size_t																		WriteFunc(char* pstr, size_t size, size_t nmemb, void* pstream);
	//����
	static size_t																		ProgressFunc(void* pData, double dDwonLoadCount, double dCurDwonLoadCount, double uUpCount, double uUpCurCount);
	//У���ļ��Ƿ��������
	bool																				CheckFileComplete();
	//·���滻
	wstring																				RepPath(LPCTSTR lptPath);
public:
	CDownloadZip();
	~CDownloadZip();

	//����ZIP��
	bool																				DownLoadZip(LPCTSTR lptInstallPath, LPCTSTR lptLink, int inVersion, LPCTSTR lptConfig, void* pThis);

	//ȡ������
	void																				CancelDownLoad();

	//����ռ�ý���
	void																				ExitProcessList();

	//���
	bool																				UnpackZip(LPCTSTR lptInstallPath, int inVersion, LPCTSTR lptConfig, LPCTSTR lptTilName);
};

#endif