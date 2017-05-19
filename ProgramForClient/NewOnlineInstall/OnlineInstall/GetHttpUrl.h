#pragma once

#ifndef GETHTTPURL_H
#define GETHTTPURL_H

class CGetHttpUrl {

private:
	CURL*																				m_pCurl;
	//�ļ��ܳ���
	long																				m_lFileLen;
	//���ؽ���
	double																				m_dDownLen;
	//�������
	CPaintManagerUI*																	m_pManager;
	//��������
	bool																				m_bEndDownLoad;
	//��֤���ص��ļ�
	bool																				CheckDownloadFile();
protected:
	//��������
	static int																			WriteHttpData(char* pchData, size_t size, size_t nmemb, string* pWriterData);
	//������ȡ���ӵ�ַ
	bool																				CheckHttpLink(string strData, wstring* pwstrData, int* pinVerison);
	//д���ļ�
	static size_t																		WriteFunc(char* pstr, size_t size, size_t nmemb, void* pstream);
	//���ؽ���
	static size_t																		ProgressFunc(void* pData, double dDwonLoadCount, double dCurDwonLoadCount, double uUpCount, double uUpCurCount);
public:
	CGetHttpUrl();
	~CGetHttpUrl();

	//��ȡ���ص�ַ
	bool																				GetHttpUrl(wstring* pwstrUrl, int& inVersion);

	//�����ļ�
	bool																				GetInstallZip(LPCTSTR lptFileName, LPCTSTR lptLink, CPaintManagerUI* pManager, DWORD& dwFileSize);

	//�ر�ֹͣ����
	void																				CloseDownload();

	//�ύ��װ����
	bool																				RepointInstall(LPCTSTR lptData);
};

#endif