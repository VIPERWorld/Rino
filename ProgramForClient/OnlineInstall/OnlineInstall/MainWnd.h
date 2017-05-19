/*Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/#pragma once
#include "http_request_manager.h"
#include "MyWinHttp.h"
#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")

class CMainWnd : public WindowImplBase, public FCHttpRequestManager
{
public:
	CMainWnd(void);
	~CMainWnd(void);

	virtual LPCTSTR GetWindowClassName(void) const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;

	virtual void InitWindow();
	virtual void OnFinalMessage( HWND hWnd );
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Notify(TNotifyUI& msg);
	void SwitchCustomImage(LPCTSTR lpBkImage);
public:
	void OnAfterRequestSend (FCHttpRequest& rTask);
	void OnAfterRequestFinish (FCHttpRequest& rTask);
	CDuiString GetStatusText (FCHttpRequest& rTask);
	void SetProcess();

	void DownloadProcess();
	void SmoothSwitchBK();
	void OnTimer(int iEventID);

	void SetErrorMsg(LPCTSTR lpStr)
	{
		if (m_pTxtError)
		{
			m_pTxtError->SetText(lpStr);
		}
	}

	CDuiString GetFormatKB (LONGLONG nByte)
	{
		TCHAR strRes[128] ;
		StrFormatKBSize(nByte, strRes, sizeof(strRes)) ;
		return strRes;
	}

	void SetProgress(int iValue)
	{
		if (m_pProgress)
		{
			m_pProgress->SetValue(iValue);
		}
	}

public:
	static unsigned int __stdcall Thread_Install(LPVOID lpVoid);

public:
	//������ݷ�ʽ
	void CreateLinkFile(LPCTSTR szStartAppPath,LPCTSTR szDestLnkPath);
	//��ȡ����·��
	CDuiString GetDesktop();
	//��ȡϵͳ��64λ����32λ
	bool Is64Bit_OS();
	//��ȡ����ʣ��ռ�
	int GetDiskFree(LPCTSTR strDrive);
	//��֤·���Ϸ����Լ����̿ռ��С
	void VerifyPath(LPCTSTR strPath);
	//��Ʒ�б���������ǵĲ�Ʒ
	void AddInstall(LPCSTR lptAppPath, LPCTSTR lptDirectory);
	//��Ĭ��װ
	void NoWindowInstall();
private:
	//ע���д��ʱ���жϸ�·���ַ�����ԭpath�ַ������ǲ��Ǵ���
	bool IsPathExist(char *PerfData, char *myPath);
	//��ϵͳ��������path�м�����Ϸ��װ·��
	bool AddPath_registry(char *szPath);
	//��ȡ���ص�ַ��Ϣjson
	void getDownloadAddressInfo(string strValue);

	//���и�����Ҫ�����л�������
	void Exec_dx_exe();
protected:
	CWndShadow m_WndShadow;
	CHorizontalLayoutUI* m_pHorCustom;
	CTabLayoutUI* m_pMainTab;//�л�����������û�Э�����
	CTabLayoutUI* m_pTab;
	CControlUI* m_pCtrlTheme;
	CProgressUI* m_pProgress;
	CRichEditUI*	m_pTxtinfo;//�û�Э����Ϣ
	CTextUI*	m_pTxtStatues;
	CTextUI*	m_pTxtError;
	CTextUI*	m_pTxtShowMsg;
	CEditUI*	m_pEdtPath;
	CFadeButtonUI* m_pBtnCustom;
	COptionUI* m_pOptAgree;//�û�Э�鵥ѡ��
	CFadeButtonUI* m_pBtnInstall;//���ذ�ť
	//�ж��Ƿ��Ǿ�Ĭ��װ
	bool			m_NoWndInstall;

	CMyWinHttp m_MyWinHttp;
	std::string m_strWeb_download;//��ǰ���°汾���ص�ַ//ѹ�������ص�ַhttp://172.16.2.18:8080/download
	std::string	m_str_version;//���汾

	int m_iSource;//�ƹ�ƽ̨��־
	//��������ص�����
private:
	int   m_task_id ;
	std::string   m_receive ;
	HTTP_REQUEST_HEADER   m_header ;

	//�������ص�����
private:
	int		m_iCurFade;	//��ǰ����ͼ͸����
	int		m_iCurIndex;//��ǰͼƬ����
	bool	m_bToUp;	//�Զ��尴ť�Ƿ���µ���(false-���ϵ��£�true-���µ��ϣ�
	bool	m_bCanInstall;	//�Ƿ���԰�װ
	DWORD	m_dwMsgColor;	//Ĭ����Ϣ����ɫ
};

