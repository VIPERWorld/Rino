/*Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/#pragma once
#include "http_request_manager.h"
#include "resource.h"
#include <vector>
#include <deque>
#include "MyFileOperation.h"
#include "backgroundWnd.h"

using namespace std;

enum _E_RSULT
{
	OP_OK=0,
	OP_FAILD
};

struct _S_HALL_DOWNLOAD_DATA
{
	_S_HALL_DOWNLOAD_DATA(std::string strFileName, std::string strRecvData):
		m_strFileName(strFileName), m_strRecvData(strRecvData)
	{
	}
	std::string m_strFileName;	//�ļ���
	std::string m_strRecvData;	//�ļ�����
};

class CUpdateWnd : public WindowImplBase, public FCHttpRequestManager
{
public:
	explicit CUpdateWnd(LPTSTR lpCmdLine);

	~CUpdateWnd();

	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("UpdateWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return _T("UpdateWnd.xml");
	}

	virtual CDuiString GetSkinFolder()
	{
		return  _T("skin\\");
	}

	virtual UILIB_RESOURCETYPE GetResourceType() const
	{
		return UILIB_ZIPRESOURCE;
		return UILIB_FILE;
	}

	virtual LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIPRES1);
		return _T("");
	}

	virtual void InitWindow();
	
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual void Notify(TNotifyUI& msg);
public:

	bool ParseCmdLine();

	void UpdateProcess();

public://ִ�п�����װ�ļ��߳�
	static unsigned int __stdcall Thread_Install(LPVOID lpVoid);

protected:
    
	//������������ɺ���ô˺���
    virtual void OnAfterRequestSend(FCHttpRequest& rTask);
    
	//�ļ�������ɺ���ô˺���
    virtual void OnAfterRequestFinish(FCHttpRequest& rTask);

	bool AddDownloadEx(LPCTSTR strFile);

	CDuiString GetFormatKB(LONGLONG nByte);

	CDuiString GetStatusText (FCHttpRequest& rTask);

	//void MakeFiles();

	static bool CreateMuliteDirectory(CString P);

	static bool FolderExists(CString s);

	//BOOL LoadImageFromResource(ATL::CImage *pImage, UINT nResID,LPCTSTR lpTyp);
	//��ʾ��ǰ���ؽ���
	void onShowProgress(int iprogress);
protected:
	CbackgroundWnd* m_pCbackgroundWnd;//�˳�����

	CTextUI*  m_pTxt_Progress;//��������
	CButtonUI* m_pBtn_cancel;//ȡ����ť
	CTextUI* m_pTxt_UpdateClient;//���������֣����ؿͻ���

	CProgressUI *m_pProgress;//�������ؼ�

	CWndShadow m_WndShadow;
	bool m_bIsErr;	//����ʧ�ܵı��
private:

	CDuiString m_strCmdLine;

	CDuiString m_strMainUrl;

	DWORD		m_dwTotalSize;

	DWORD		m_dwDownLoadSize;



	vector<CDuiString> m_vecDownFiles;


	std::deque<std::string>	m_main_wait_download;

	bool m_bDownloading;

	int m_iCurTaskID;
public:
	static CString	m_strFileName;

	static CDuiString	CUpdateWnd::m_str_version;//���汾
	static CDuiString	CUpdateWnd::m_str_smallversion;//�Ӱ汾
	static vector<_S_HALL_DOWNLOAD_DATA> m_vecHallDownData;
	static CMyFileOperation m_MyFileOperation;  //�ļ��в���
};