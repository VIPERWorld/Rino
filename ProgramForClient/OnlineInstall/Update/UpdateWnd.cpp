/*Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/
#include "stdafx.h"
#include "UpdateWnd.h"
#include <Shlwapi.h>
#include <ShellAPI.h>

#pragma comment(lib, "Shlwapi.lib")

/////////////////////////////ZIP////////////////////////////////////////////////////
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{ 
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{ 
	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);

#define ZIP_OK		 0
#define ZIP_HANDLE   1
#define ZIP_FILENAME 2
#define ZIP_MEMORY   3
/////////////////////////////////////////////////////////////////////////////////

#define IDT_SENDREQUEST		1	//���������б�
#define IDT_UPDATE_TIMER	2	//���½���
#define IDT_DOWNLOAD_NEXT	3	//������һ���ļ�
#define IDT_ERROR_QUIT		4	//���³����ӳ��˳�
#define IDT_MAKEFILES		5	//ת���ļ�

#define WM_INSTALL	WM_USER+2		//���ڰ�װ

//���غͰ�װ
const int g_Download_Max_Value = 90;//��װһ���ļ��Ĳ����Ϊ���غͽ�ѹ���ڴ˶������ز�����ռ����Ϊ90%
const int g_Unzip_Max_Value = 10;	//��ѹ���ļ�ռ10%

CString	CUpdateWnd::m_strFileName;
CDuiString	CUpdateWnd::m_str_version;//���汾
CDuiString	CUpdateWnd::m_str_smallversion;//�Ӱ汾
vector<_S_HALL_DOWNLOAD_DATA> CUpdateWnd::m_vecHallDownData;
CMyFileOperation CUpdateWnd::m_MyFileOperation;  //�ļ��в���

void MyDebugString(LPCTSTR lpPre, LPCTSTR lpFormat, ...)
{
	va_list Args;

	va_start(Args, lpFormat);

	int nLen, totalLen;
	TCHAR *szBuffer;

	nLen = _vsntprintf(NULL, 0, lpFormat, Args);
	totalLen = (nLen + 1)*sizeof(TCHAR);
	szBuffer = (TCHAR*)malloc(totalLen);
	ZeroMemory(szBuffer, totalLen);
	nLen = _vsntprintf(szBuffer, nLen + 1, lpFormat, Args);
	va_end(Args);

	CDuiString strMsg;
	strMsg.Format(_T("%s-----%s"), lpPre, szBuffer);
	OutputDebugString(strMsg);

	free(szBuffer);
}

CUpdateWnd::CUpdateWnd(LPTSTR lpCmdLine)
	:m_strCmdLine(lpCmdLine), 
	m_bDownloading(false)
{
	MyDebugString("sdp_test", "CUpdateWnd");
	m_vecDownFiles.clear();
	m_bIsErr = false;
	m_pCbackgroundWnd = nullptr;
}

CUpdateWnd::~CUpdateWnd()
{
	m_vecDownFiles.clear();
}

void CUpdateWnd::InitWindow()
{
	SetIcon(IDI_ICON1);

	CWndShadow::Initialize(CPaintManagerUI::GetInstance());
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(6);
	m_WndShadow.SetPosition(0,0);

	m_pProgress = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("Pro_Update")));
	m_pBtn_cancel = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("Btn_cancel")));
	m_pTxt_Progress = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("Txt_Progress")));
	m_pTxt_UpdateClient = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("Txt_UpdateClient")));
	if (nullptr != m_pTxt_UpdateClient)
	{
		m_pTxt_UpdateClient->SetShowHtml(true);
	}
	//��������ʱ���������б�
	SetTimer(m_hWnd, IDT_SENDREQUEST, 200, NULL);
	MyDebugString("sdp_test", "InitWindow");
}

LRESULT CUpdateWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	ExitProcess(0);
	return 0;
}

LRESULT CUpdateWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	case WM_INSTALL:
		{
			int iCurePos = (int)lParam;
			if (m_pProgress)
			{
				CDuiString strValue;
				strValue.Format(_T("%d%%"), iCurePos);
				//m_pProgress->SetText(strValue.GetData());
				m_pProgress->SetValue(iCurePos);

				onShowProgress(iCurePos);
				
			}
			bHandled = TRUE;
		}break;
	case WM_TIMER:
		{
			if (IDT_SENDREQUEST == wParam)
			{
				KillTimer(m_hWnd, IDT_SENDREQUEST);				
				bHandled = TRUE;
				if (!ParseCmdLine())
				{
					SetTimer(m_hWnd, IDT_ERROR_QUIT, 1000, NULL);
				}
			}
			else if (IDT_UPDATE_TIMER == wParam)
			{
				UpdateProcess();
				bHandled = TRUE;
			}
			else if (IDT_DOWNLOAD_NEXT == wParam)
			{
				KillTimer(m_hWnd, IDT_DOWNLOAD_NEXT);
				bHandled = TRUE;
				if (m_pProgress)
				{
					//m_pProgress->SetText(_T("0%"));
					m_pProgress->SetValue(0);
					onShowProgress(0);
				}
				std::string strFile = m_main_wait_download.front();
				m_main_wait_download.pop_front();
				MyDebugString("http", strFile.c_str());
				m_iCurTaskID = AddDownload(strFile.c_str());
			}
			else if (IDT_ERROR_QUIT == wParam)
			{
				KillTimer(m_hWnd, IDT_ERROR_QUIT);
				bHandled = TRUE;
				//ExitProcess(0);

				m_bIsErr = true;
				if (nullptr != m_pBtn_cancel)
				{
					m_pBtn_cancel->SetText("����");
				}
				if (nullptr != m_pTxt_Progress)
				{
					m_pTxt_Progress->SetText("��Ǹ��ƽ̨����ʧ��...");
					m_pTxt_Progress->SetTextColor(0xFFFF5252);
				}
				if (nullptr != m_pProgress)
				{
					m_pProgress->SetForeImage("Progress_err.png");
				}
			}
			else if (IDT_MAKEFILES == wParam)
			{
				KillTimer(m_hWnd, IDT_MAKEFILES);
				bHandled = TRUE;
				unsigned int uiThreadID = 0;
				_beginthreadex(NULL, 0, Thread_Install, this, 0, &uiThreadID);
			}
		}break;
	default:
		break;
	}
	return 0;
}

void CUpdateWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (0 == _tcsicmp(_T("Btn_cancel"), msg.pSender->GetName().GetData()))
		{
			if (m_bIsErr)
			{//����ʧ����,��Ϊ���Թ���
				CDuiString strValue;
				if (nullptr != m_pBtn_cancel)
				{
					m_pBtn_cancel->SetText("ȡ������");
				}
				if (nullptr != m_pTxt_Progress)
				{
					onShowProgress(0);
					m_pTxt_Progress->SetTextColor(0xFFFFFFFF);
				}
				if (nullptr != m_pProgress)
				{
					strValue.Format(_T("%d%%"), 0);
					m_pProgress->SetValue(0);
					m_pProgress->SetForeImage("Progress_suc.png");
				}
				m_bIsErr = false;
				m_bDownloading = false;
				m_main_wait_download.clear();
				DeleteRequest(m_iCurTaskID);
				//��������ʱ���������б�
				SetTimer(m_hWnd, IDT_SENDREQUEST, 200, NULL);
			}
			else
			{//ȡ������
				int scrWidth, scrHeight;  
				scrWidth = GetSystemMetrics(SM_CXSCREEN);  
				scrHeight = GetSystemMetrics(SM_CYSCREEN);  
				RECT rect; 

				if (nullptr == m_pCbackgroundWnd)
				{
					m_pCbackgroundWnd = new CbackgroundWnd();
					if( m_pCbackgroundWnd == NULL ) return;

					m_pCbackgroundWnd->CreateDuiWindow(m_hWnd, NULL, UI_WNDSTYLE_FRAME);

					GetWindowRect(m_pCbackgroundWnd->GetHWND(), &rect);
					SetWindowPos(m_pCbackgroundWnd->GetHWND(), HWND_TOPMOST, (scrWidth - rect.right) / 2, (scrHeight - rect.bottom) / 2, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW); 

					m_pCbackgroundWnd->ShowWindow(true);
					m_pCbackgroundWnd->ShowModal();

					delete m_pCbackgroundWnd;
					m_pCbackgroundWnd = nullptr;
				}
			}
			
		}
	}
	else if (msg.sType == DUI_MSGTYPE_LINK)
	{
		if (0 == _tcsicmp(_T("Txt_UpdateClient"), msg.pSender->GetName().GetData()))
		{
			if (nullptr != m_pTxt_UpdateClient)
			{
				CDuiString* str = m_pTxt_UpdateClient->GetLinkContent(0);

				//�������
				ShellExecute(NULL, "open", str->GetData(), NULL, NULL, SW_SHOWNORMAL);
				ExitProcess(0);
			}
		}
	}	

	__super::Notify(msg);
}

LRESULT CUpdateWnd::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN || wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

bool CUpdateWnd::ParseCmdLine()
{
	CDuiString strCmdLine = m_strCmdLine;
	if (strCmdLine.IsEmpty())
	{
		return false;
	}
	
	//����Ӧ�ó�����
	CDuiString strBegin = _T("/nb:");
	CDuiString strEnd = _T("/ne");
	int iBegin = strCmdLine.Find(strBegin);
	int iEnd = strCmdLine.Find(strEnd);
	if (0 > iBegin || 0 > iEnd)
	{
		return false;
	}
	m_strFileName = strCmdLine.Mid(iBegin+strBegin.GetLength(), iEnd-iBegin-strBegin.GetLength());
	
	char s[MAX_PATH] = { 0 };
	sprintf_s(s, "sdp_test update.exe ����Ӧ�ó�����=%s", m_strFileName.GetBuffer());
	OutputDebugStringA(s);

	//�������汾
	strBegin = _T("/v1b:");
	strEnd = _T("/v1e");
	iBegin = strCmdLine.Find(strBegin, iEnd);
	iEnd = strCmdLine.Find(strEnd, iBegin);
	if (0 > iBegin || 0 > iEnd)
	{
		return false;
	}
	m_str_version = strCmdLine.Mid(iBegin + strBegin.GetLength(), iEnd - iBegin - strBegin.GetLength());

	sprintf_s(s, "sdp_test update.exe �������汾=%s", m_str_version.GetData());
	OutputDebugStringA(s);

	//�����Ӱ汾
	strBegin = _T("/v2b:");
	strEnd = _T("/v2e");
	iBegin = strCmdLine.Find(strBegin, iEnd);
	iEnd = strCmdLine.Find(strEnd, iBegin);
	if (0 > iBegin || 0 > iEnd)
	{
		return false;
	}
	m_str_smallversion = strCmdLine.Mid(iBegin + strBegin.GetLength(), iEnd - iBegin - strBegin.GetLength());
	
	sprintf_s(s, "sdp_test update.exe �����Ӱ汾=%s", m_str_smallversion);
	OutputDebugStringA(s);

	//�������ظ���ַ
	strBegin = _T("/ub:");
	strEnd = _T("/ue");
	iBegin = strCmdLine.Find(strBegin, iEnd);
	iEnd = strCmdLine.Find(strEnd, iBegin);
	if (0 > iBegin || 0 > iEnd)
	{
		return false;
	}
	m_strMainUrl = strCmdLine.Mid(iBegin+strBegin.GetLength(), iEnd-iBegin-strBegin.GetLength()) + _T("/");
	
	sprintf_s(s, "sdp_test update.exe �������ظ���ַ=%s", m_strMainUrl.GetData());
	OutputDebugStringA(s);

	//��ȡ�����ļ�
	strBegin = _T("/pb:");
	strEnd = _T("/pe");
	iBegin = strCmdLine.Find(strBegin, iEnd);
	iEnd = strCmdLine.Find(strEnd, iBegin);
	if (0 > iBegin || 0 > iEnd)
	{
		return false;
	}
	CDuiString strFile = strCmdLine.Mid(iBegin+strBegin.GetLength(), iEnd-iBegin-strBegin.GetLength());

	sprintf_s(s, "sdp_test update.exe ��ȡ�����ļ�=%s", strFile.GetData());
	OutputDebugStringA(s);

	char* psource = (LPTSTR)strFile.GetData();
	char *result = NULL;

	result = strtok(psource, _T("|"));
	while( result != NULL ) {
		m_vecDownFiles.push_back(result);
		result = strtok( NULL,  _T("|"));
	}

	if (m_vecDownFiles.empty())
	{
		return false;
	}

	//��ȡ�ܵ��ļ���С
	strBegin = _T("/sb:");
	strEnd = _T("/se");
	iBegin = strCmdLine.Find(strBegin, iEnd);
	iEnd = strCmdLine.Find(strEnd, iBegin);
	if (0 > iBegin || 0 > iEnd)
	{
		return false;
	}
	CDuiString strSize = strCmdLine.Mid(iBegin+strBegin.GetLength(), iEnd-iBegin-strBegin.GetLength());
	m_dwTotalSize = strtoul(strSize.GetData(), NULL, 10);
	
	SetTimer(m_hWnd, IDT_UPDATE_TIMER, 50, NULL) ;

	CDuiString strFilePath;
	//��ӵ����ض�����(sdp������ѿ��ӷ�ס��ֻ��Ҫ����һ��zip�ͺ���)
	if (m_vecDownFiles.size() > 0)
	{
		MyDebugString("sdp_test", "m_vecDownFiles=%d", m_vecDownFiles.size());
		AddDownloadEx(m_vecDownFiles[0].GetData());
	}
	//for (int i=0; i<m_vecDownFiles.size(); ++i)
	//{
	//	AddDownloadEx(m_vecDownFiles[i].GetData());		
	//}
	return true;
}

void CUpdateWnd::UpdateProcess()
{
	FCHttpRequest *pTask = FindRequest(m_iCurTaskID);
	if (!pTask)
	{
		return;
	}

	int   nDownload = pTask->GetTotalReceiveByte();	
	int   nTotal = pTask->GetResponseInfo().m_content_length;
	if (nTotal)
	{
		int nPercent = (int)(g_Download_Max_Value * 100.0 * nDownload / ( g_Download_Max_Value + g_Unzip_Max_Value ) / nTotal);
		if (m_pProgress)
		{
			CDuiString strValue;
			strValue.Format(_T("%d%%"), nPercent);
			//m_pProgress->SetText(strValue.GetData());
			m_pProgress->SetValue(nPercent);
			onShowProgress(nPercent);
		}
	}
}

//������������ɺ���ô˺���
void CUpdateWnd::OnAfterRequestSend(FCHttpRequest& rTask)
{
	_E_RSULT e_r = OP_OK;
	const HTTP_RESPONSE_INFO   & resp = rTask.GetResponseInfo() ;
	if (resp.m_status_code == 0)
	{//�޷����ӷ�����		
		SetTimer(m_hWnd, IDT_ERROR_QUIT, 1000, NULL);
		//MessageBox(m_hWnd, GetStatusText(rTask), _T("����"), MB_OK);
		DeleteRequest(rTask.GetTaskID());
		return;
	}
	//��ʾ��ǰ���µ��ļ���
	CDuiString strURL = rTask.GetURL();
	strURL.Replace(m_strMainUrl, _T(""));
	//��ʾ�ļ���С
	CDuiString strTxt = GetFormatKB(rTask.GetResponseInfo().m_content_length);
}

//�ļ�������ɺ���ô˺���
void CUpdateWnd::OnAfterRequestFinish(FCHttpRequest& rTask)
{
	const HTTP_RESPONSE_INFO &r = rTask.GetResponseInfo();
	_E_RSULT e_r = OP_FAILD;
	if (r.m_status_code == HTTP_STATUS_OK)
	{
		if (r.m_content_length)
		{
			//��ֹ���ع����������жϵ������ݴ���
			if (r.m_content_length == rTask.GetTotalReceiveByte())
			{
				e_r = OP_OK;
			}
		}
		else
		{
			//���ش�СΪ0��˵�����ص����ݿ�������ҳ��HTML,ASP������
			if (r.m_final_read_result)
			{
				e_r = OP_OK;
			}
		}
	}

	if (OP_OK != e_r)
	{
		SetTimer(m_hWnd, IDT_ERROR_QUIT, 1000, NULL);
		//MessageBox(m_hWnd, GetStatusText(rTask), _T("����"), MB_OK);
		m_main_wait_download.clear();
		DeleteRequest(m_iCurTaskID);
		m_bDownloading = false;
		return;
	}

	std::string str_recv_data;

	//��ȡ�����������
	rTask.PopReceived(str_recv_data);

	CDuiString strUrl = rTask.GetURL();
	MyDebugString("httpss", strUrl.GetData());
	strUrl.Replace(m_strMainUrl, _T(""));
	std::string str_file_name = strUrl.GetData();
	MyDebugString("httpss", strUrl.GetData());
	m_vecHallDownData.push_back(_S_HALL_DOWNLOAD_DATA(str_file_name, str_recv_data));

	m_dwDownLoadSize += str_recv_data.size();

	if (m_pProgress)
	{
		char str[MAX_PATH] = {0};
		sprintf_s(str, "%d%%", g_Download_Max_Value*100/(g_Download_Max_Value+g_Unzip_Max_Value));
		//m_pProgress->SetText(str);
		m_pProgress->SetValue(g_Download_Max_Value*100/(g_Download_Max_Value+g_Unzip_Max_Value));
		onShowProgress(g_Download_Max_Value * 100 / (g_Download_Max_Value + g_Unzip_Max_Value));
	}
	
	//����ʣ���ļ���
	CDuiString strValue;
	strValue.Format(_T("%d/%d"), m_vecHallDownData.size(), m_vecDownFiles.size());

	//������һ���ļ�
	if (!m_main_wait_download.empty())
	{
		SetTimer(m_hWnd, IDT_DOWNLOAD_NEXT, 50, NULL);	
	}
	else
	{//�������޵ȴ�����ת���ļ�
		m_bDownloading = false;
		KillTimer(m_hWnd, IDT_UPDATE_TIMER);
		SetTimer(m_hWnd, IDT_MAKEFILES, 10, NULL);
	}
}

bool CUpdateWnd::AddDownloadEx(LPCTSTR strFile)
{
	//�ж�URL��ַ�Ƿ�Ϸ�
	if (!UrlIs(m_strMainUrl.GetData(), URLIS_URL))
	{
		return false;
	}
	CDuiString strCombin = m_strMainUrl + strFile;
	//������������ļ����򽫴��ļ���ӵ��ȴ�������
	if (m_bDownloading)
	{
		m_main_wait_download.push_back(std::string(strCombin));
		return true;
	}
	m_bDownloading = true;
	m_iCurTaskID = AddDownload(strCombin.GetData());
	return true;
}

CDuiString CUpdateWnd::GetFormatKB(LONGLONG nByte)
{
	TCHAR   t[128] ;
	StrFormatKBSize(nByte, t, 128) ;
	return t ;
}

CDuiString CUpdateWnd::GetStatusText(FCHttpRequest& rTask)
{
	const HTTP_RESPONSE_INFO &resp = rTask.GetResponseInfo();

	CDuiString strMsg;
	switch (resp.m_status_code)
	{
	case HTTP_STATUS_OK:
	case HTTP_STATUS_PARTIAL_CONTENT:		
		// range request
		if (rTask.GetRequestHeader().m_start && (resp.m_status_code == HTTP_STATUS_OK))
		{
			strMsg += " ( not support range )";
		}
		break;

	case HTTP_STATUS_NOT_FOUND:
		strMsg = "404 error : Not Found";
		break;

	case HTTP_STATUS_FORBIDDEN :
		strMsg = "403 error : Forbidden";
		break;

	case HTTP_STATUS_PROXY_AUTH_REQ:
		strMsg = "407 error : Proxy Authentication Required";
		break;

	default:
		strMsg.Format("Download Error, status code : %d", resp.m_status_code);
		break;
	}
	return strMsg;
}

unsigned int __stdcall CUpdateWnd::Thread_Install(LPVOID lpVoid)
{
	CUpdateWnd* pMainWnd = (CUpdateWnd*)lpVoid;
	CString strRoot = GetAppPath();
	CString strPath;
	CString strPath2;//��ʱ�ļ���
	CString strPath3;//����ĳ�ļ�����Ӧ������·��
	CString strMsg;
	CString strDir, strPre;
	vector<_S_HALL_DOWNLOAD_DATA>::iterator iter = m_vecHallDownData.begin();
	_E_RSULT iErrorFlag = OP_OK;
	int iIndex = 0;

	strPath2.Format(_T("%stemp"), strRoot);
	if (CreateDirectory(strPath2, NULL))
	{
		SetFileAttributes(strPath2, FILE_ATTRIBUTE_HIDDEN ); //����
	}

	while (iter != m_vecHallDownData.end())
	{//�����ļ�
		strPath.Format(_T("%s\\%s"), strPath2, iter->m_strFileName.c_str());
		strPath3.Format(_T("%s%s"), strRoot, iter->m_strFileName.c_str());
		/////�����ļ���·��
		//�ж�Ŀ¼�Ƿ����
		iIndex = strPath.ReverseFind('\\');
		if (0 >= iIndex)
		{
			iErrorFlag = OP_FAILD;
			break;
		}
		strDir = strPath.Left(iIndex);
		if (strDir != strPre)
		{
			if (!CreateMuliteDirectory(strDir))
			{
				SetTimer(pMainWnd->GetHWND(), IDT_ERROR_QUIT, 1000, NULL);
				strMsg.Format(_T("����Ŀ¼ʧ��-%s"), strDir);
				MessageBox(pMainWnd->GetHWND(), strMsg, _T("����"), MB_OK|MB_ICONERROR);
				iErrorFlag = OP_FAILD;
				break;
			}
			strPre = strDir;
		}
		////���ڴ��е��ļ�д��Ӳ��
		HANDLE   f = CreateFile(strPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) ;
		if (f != INVALID_HANDLE_VALUE)
		{
			DWORD   nWrite ;
			::WriteFile(f, iter->m_strRecvData.c_str(), iter->m_strRecvData.size(), &nWrite, NULL) ;			
			CloseHandle(f);
			if (((int)nWrite != iter->m_strRecvData.size()))
			{//д���ļ�ʧ��
				SetTimer(pMainWnd->GetHWND(), IDT_ERROR_QUIT, 1000, NULL);
				strMsg.Format(_T("д���ļ�ʧ��%d--%s"), GetLastError(), strPath);
				MessageBox(pMainWnd->GetHWND(), strMsg, _T("����"), MB_OK|MB_ICONERROR);
				iErrorFlag = OP_FAILD;
				break;
			}
			//���ļ����µ��ļ�����������·����
			MyDebugString("sdp_test", "update ��%s��->��%s��", strPath, strPath3);
			CopyFile(strPath, strPath3, false);

			///////////�ж��ļ��Ƿ�ΪZIP,�����ѹ////////////////////////////////
			string str1 = ".zip";
			string::size_type idx = iter->m_strFileName.find( str1 );
			if ( idx != string::npos )
			{
				//��ѹ���ļ�
				HZIP hz = OpenZip((char*)(iter->m_strRecvData.c_str()), iter->m_strRecvData.size(), ZIP_MEMORY);
				//	HZIP hz = OpenZip((char*)(strPath.GetData()), strPath.GetLength(), ZIP_FILENAME);
				ZIPENTRY ze;

				//��ȡѹ�������ļ�����
				GetZipItem(hz,-1,&ze);
				int numitems=ze.index;
				int iCurePos = 0;
				ZRESULT zr;
				for (int i=0; i!=numitems; ++i)
				{
					zr = GetZipItem(hz, i, &ze);
					if (0 != zr)
					{
						break; //û�и����ļ���
					}

#ifdef _DEBUG
					if (0 == strcmp("Update_d.exe", ze.name))
#else
					if (0 == strcmp("Update.exe", ze.name))
#endif // _DEBUG
					{//�������ѹ�����д���update.exe�Լ����������������
						MyDebugString("sdp_test", "�������ѹ�����д���update.exe�Լ����������������");
						continue;
					}
					if (0 == strcmp("0", m_str_smallversion.GetData()))
					{//˵������������
						std::string s = ze.name;
						
						if (s.find("RhinoClient/") != string::npos)
						{
							std::string ss = "RhinoClient/";
							int pos = s.find(ss.c_str());
							std::string s2 = s.erase(pos, ss.length());
							sprintf_s(ze.name, "%s", s2.c_str());
						}
					}
					//��ѹ��i���ļ�
					zr = UnzipItem(hz, i, ze.name, 0, ZIP_FILENAME);

					//���������Ͱ�װ����
					iCurePos = g_Unzip_Max_Value*1.0*(i+1)/numitems;
					::PostMessage(pMainWnd->GetHWND(), WM_INSTALL, (WPARAM)0, (LPARAM)(g_Download_Max_Value+iCurePos));
					//Sleep(20);
				}
				CloseZip(hz);	
			}
			//�ǵ�ɾ����ѹ���ļ�
			m_MyFileOperation.DeleteFolder(strPath3);
			///////////////////////////////////////////
		}
		else
		{
			SetTimer(pMainWnd->GetHWND(), IDT_ERROR_QUIT, 1000, NULL);
			strMsg.Format(_T("�����ļ�ʧ��%d--%s"), GetLastError(), strPath);
			MessageBox(pMainWnd->GetHWND(), strMsg, _T("����"), MB_OK|MB_ICONERROR);
			iErrorFlag = OP_FAILD;
			break;
		}

		++iter;
	}

	m_MyFileOperation.DeleteFolder(strPath2);

	if (OP_FAILD != iErrorFlag)
	{
		//������ɺ����ʱ��, �˳��Լ�����������
		CDuiString strIni = strRoot + _T("update.ini");
		::WritePrivateProfileString(_T("config"), _T("version"), m_str_version, strIni.GetData());
		::WritePrivateProfileString(_T("config"), _T("smallversion"), m_str_smallversion, strIni.GetData());
		MyDebugString("sdp_test", "update m_strFileName=%s", m_strFileName.GetBuffer());

		//::MessageBox(NULL, NULL, "CUpdateWnd", MB_OK);
		//bool bres = ::ShellExecute(NULL, _T("open"), m_strFileName, NULL, NULL, SW_SHOWNORMAL);

		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = m_strFileName.GetBuffer();
		ShExecInfo.lpParameters = "";
		ShExecInfo.lpDirectory = strRoot.GetBuffer();
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = NULL;
		BOOL bres = ::ShellExecuteEx(&ShExecInfo);

		DWORD dwError = GetLastError();
		MyDebugString("sdp_test", "m_strFileName=%s,bres=%d", m_strFileName.GetBuffer(), bres);
		ExitProcess(0);
	}

	return 0;
}

bool CUpdateWnd::CreateMuliteDirectory(CString P)
{   
	int len=P.GetLength();   
	if ( len <2 ) return false;    
	if('\\'==P[len-1])  
	{  
		P=P.Left(len-1);  
		len=P.GetLength();  
	}  
	if ( len <=0 ) return false;  
	if (len <=3)   
	{
		if (FolderExists(P))return true;  
		else return false;   
	}
	if (FolderExists(P))return true;  
	CString Parent;  
	Parent=P.Left(P.ReverseFind('\\') );   
	if(Parent.GetLength()<=0)return false;    
	BOOL Ret=CreateMuliteDirectory(Parent);    
	if(Ret)
	{
		SECURITY_ATTRIBUTES sa;   
		sa.nLength=sizeof(SECURITY_ATTRIBUTES);   
		sa.lpSecurityDescriptor=NULL;   
		sa.bInheritHandle=0;   
		Ret=(CreateDirectory(P,&sa)==TRUE);   
		return Ret==TRUE?true:false;   
	}
	else
		return false;   
}

bool CUpdateWnd::FolderExists(CString s)
{   
	DWORD attr;    
	attr = GetFileAttributes(s);    
	return (attr != (DWORD)(-1) ) &&  ( attr & FILE_ATTRIBUTE_DIRECTORY);    
} 

//��ʾ��ǰ���ؽ���
void CUpdateWnd::onShowProgress(int iprogress)
{
	CDuiString strValue;
	std::string s1 = m_str_version;
	std::string s2 = m_str_smallversion;
	strValue.Format(_T("(�ļ��汾:%s.%s)ƽ̨���ڸ�����...(%d%%)"), s1.c_str(), s2.c_str(), iprogress);
	m_pTxt_Progress->SetText(strValue.GetData());
}