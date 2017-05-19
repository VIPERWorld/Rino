#pragma once

#ifndef GETUPDATE_H
#define GETUPDATE_H

#include <set>
#include <map>
using namespace std;
#include "DownloadZip.h"

//�����б�
typedef pair<int, wstring> tpVerLink;
typedef set<pair<int, wstring> > tvVerLink;

class CGetUpdate {

private:
	CURL*																				m_pCurl;
	//�����б�
	tpVerLink																			m_pVerLink;
	tvVerLink																			m_VerLink;
	//������ָ��
	void*																				m_pMainWnd;
	//���汾���
	bool																				CheckMajor(LPCTSTR lptDirecotry, LPCTSTR lptConfig, int inVerions, LPCTSTR lptLink);
	//�Ӱ汾���
	bool																				CheckMinVer(LPCTSTR lptDirecotry, LPCTSTR lptConfig, int inVerions, LPCTSTR lptLink);
protected:
	//��ҳ���ݻ�ȡ
	static int																			WriteHttpData(char* pchData, size_t size, size_t nmemb, string* pWriterData);
	//�����汾����
	bool																				ParseVersion(string& strData);
	//��ȡ����·��
	wstring																				GetAppPath();
public:
	CGetUpdate();
	~CGetUpdate();

	//��ȡ���·�������Ϣ
	bool																				GetUpdate(void* pThis);

	//У��汾��Ϣ
	bool																				CheckVersion(void* pThis);
};

#endif