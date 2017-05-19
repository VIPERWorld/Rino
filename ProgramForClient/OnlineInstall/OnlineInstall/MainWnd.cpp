#include "stdafx.h"
#include "MainWnd.h"
#include "Resource.h"
#include <process.h>
#include <Shlobj.h>
#include <io.h>
#include <atlconv.h>
#include <ShellAPI.h>
//#include <thread>
#include "json.h"
#include "HDSerial.h"
///////////////////////////////////////////////////////////////////////////////////////
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


const int g_MinFade = 20;	//����ʱ͸���ȴﵽ��ֵʱ�����أ���ʾ�����͸����
const int g_MaxFade = 255;	//��͸��
const int g_base = 5;		//͸���ȵ�������
const int g_change = 50;	//50ms����һ��͸����
const int g_BkCount = 3;	//����ͼ����
const int g_ShowTime = 2000;//һ�ű���ͼ��ʾ2S

const int g_MinHorValue = 1; //�Զ���������С�߶�
const int g_MaxHorValue = 80;//�Զ����������߶�
const int g_HorBase = 5;	 //�Զ�������߶ȱ仯����

//���غͰ�װ
const int g_Download_Max_Value = 900;//��װһ���ļ��Ĳ����Ϊ���غͽ�ѹ���ڴ˶������ز�����ռ����Ϊ90%
const int g_Unzip_Max_Value = 100;	//��ѹ���ļ�ռ10%

const int g_InstallSpace = 150;		//��װ��Ҫ50MB�ռ�
const DWORD g_dwWarnColor = 4294901760;//����ʱ������ɫ

#ifdef _DEBUG
const std::string g_strZipName = _T("RhinoClient");	//ѹ��������
const std::string g_strFolderName = _T("RhinoClient");	//ѹ�������ļ�������
const std::string g_strExeName = _T("RhinoClient");			//EXE����
#else
const std::string g_strZipName = _T("RhinoClient");	//ѹ��������
const std::string g_strFolderName = _T("RhinoClient");	//ѹ�������ļ�������
const std::string g_strExeName = _T("RhinoClient");			//EXE����
#endif

const std::string g_strUnInstallExeName = _T("RhinoUnInstall");			//ж��EXE����

const std::string g_strLinkName = _T("Ϭţ�羺");	//��ݷ�ʽ

#define TIMER_SMOOTH_SHOW		1	//��ʾ���ͼ
#define TIMER_SMOOTH_HIDE		2	//���ع��ͼ
#define TIMER_CHANGE			3	//�л���������ʾ
#define TIMER_DOWNLOAD_PROCESS	4	//��ȡ���ؽ���
#define TIMER_SPEED				5	//��ȡ�����ٶ�
#define TIMER_SHOW_CUSTOM		6	//��ʾ�������Զ�������
#define TIMER_EXIST_DX_EXE		7	//���ٰ�װ����exe�����Ƿ����
#define TIMER_DELAYED_UPDATE	8	//��ʱ����

#define WM_DOWNLOAD	WM_USER+1		//��������
#define WM_INSTALL	WM_USER+2		//���ڰ�װ


///////////////////////////////////////////////////////////////
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

CMainWnd::CMainWnd(void)
{
	m_iCurIndex = 0;
	m_iCurFade = g_MinFade;
	m_bToUp = true;
	m_bCanInstall = false;
	m_strWeb_download = "";
	m_NoWndInstall = false;
	m_iSource = 0;

	m_MyWinHttp.SetpCMainWnd(this);
}

CMainWnd::~CMainWnd(void)
{
}

LPCTSTR CMainWnd::GetWindowClassName(void) const
{
	return _T("MainWnd");
}

CDuiString CMainWnd::GetSkinFolder()
{
//	return _T("skin\\");
	return _T("");
}

CDuiString CMainWnd::GetSkinFile()
{
	return _T("skin.xml");
}

UILIB_RESOURCETYPE CMainWnd::GetResourceType() const
{
//	return __super::GetResourceType();
	return UILIB_ZIPRESOURCE;
}

LPCTSTR CMainWnd::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CMainWnd::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

void CMainWnd::InitWindow()
{
	SetIcon(IDI_ICON1);

	CWndShadow::Initialize(CPaintManagerUI::GetInstance());
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(6);
	m_WndShadow.SetPosition(0,0);

	m_pMainTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("Tab_MainSwitch")));
	if (m_pMainTab)
	{
		m_pMainTab->SelectItem(0);
	}
	
	m_pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("Tab_Switch")));
	if (m_pTab)
	{
		m_pTab->SelectItem(0);
	}
	m_pHorCustom = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("Hor_Custom")));
	if (m_pHorCustom)
	{
		m_pHorCustom->SetFixedHeight(g_MinHorValue);
	}
	m_pBtnCustom = static_cast<CFadeButtonUI*>(m_PaintManager.FindControl(_T("Btn_Custom")));
	if (m_pBtnCustom)
	{
		SwitchCustomImage(_T("Custom_Up.png"));
	}

	m_pTxtinfo = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("Txt_info")));
	if (m_pTxtinfo)
	{
		m_pTxtinfo->SetEnabled(false);
		m_pTxtinfo->SetText(char_info);
	}

	m_pBtnInstall = static_cast<CFadeButtonUI*>(m_PaintManager.FindControl(_T("Btn_Install")));
	//char_info
	m_pOptAgree = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("Opt_Agree")));
	if (m_pOptAgree)
	{
		m_pOptAgree->Selected(true);
		if (m_pBtnInstall)
		{
			m_pBtnInstall->SetEnabled(true);
		}
		//sdp_test
		m_pOptAgree->SetVisible(false);
		/////////////////////////////////////////////////////////////////////////////////////////////////
	}
	//sdp_test
	CFadeButtonUI* p = static_cast<CFadeButtonUI*>(m_PaintManager.FindControl(_T("Btn_agreement")));
	if (p)
	{
		p->SetInternVisible(false);
	}
	CLabelUI* p2 = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Label_agreement")));
	if (p2)
	{
		p2->SetInternVisible(false);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	m_pCtrlTheme = m_PaintManager.FindControl(_T("theme"));
	m_pProgress = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("Pro_Install")));
	m_pTxtStatues = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("Txt_Status")));
	m_pTxtError = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("Txt_Error")));
	m_pTxtShowMsg = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("Txt_ShowMsg")));
	if (m_pTxtShowMsg)
	{
		m_dwMsgColor = m_pTxtShowMsg->GetTextColor();
	}
	//����Ĭ�ϰ�װ·��
	m_pEdtPath = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edt_Path")));
	if (m_pEdtPath)
	{
		TCHAR szBuf[MAX_PATH];
		memset(szBuf, 0, sizeof(szBuf));
		UINT len=GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR), szBuf);//������е�Ӳ�̷���

		TCHAR szSysPath[MAX_PATH];
		GetSystemDirectory(szSysPath, MAX_PATH);//���ϵͳ��
		CDuiString strSys = szSysPath;
		CDuiString strValue;

		if (!Is64Bit_OS())
		{
			strValue.Format(_T("%sProgram Files"), strSys.Left(3));
		}
		else
		{
			strValue.Format(_T("%sProgram Files (x86)"), strSys.Left(3));
		}

		m_pEdtPath->SetText(strValue.GetData());
		VerifyPath(strValue.GetData());
	}
}

bool CMainWnd::Is64Bit_OS()
{
	SYSTEM_INFO si = { 0 };

	typedef VOID (WINAPI *LPFN_PGNSI)(LPSYSTEM_INFO lpSystemInfo);
	LPFN_PGNSI pGNSI = (LPFN_PGNSI)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetNativeSystemInfo");
	if (pGNSI == NULL)
	{
		return false;
	}
	pGNSI(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || 
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
	{
		return true;
	}
	return false;
}

int CMainWnd::GetDiskFree(LPCTSTR strDrive)
{
	_ULARGE_INTEGER lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
	
	if(GetDriveType(strDrive) == DRIVE_FIXED)
	{
		GetDiskFreeSpaceEx(strDrive, &lpFreeBytesAvailableToCaller, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes);

		return lpFreeBytesAvailableToCaller.QuadPart / (1024 *1024);
	}
	return 0;
}

void CMainWnd::VerifyPath(LPCTSTR strPath)
{
	m_bCanInstall = false;
	if (!strPath)
	{		
		return ;
	}
	//�ж�·���Ƿ���ڣ�����������ʾ�Ƿ�·��
	CDuiString strDisk = strPath;
	if (_access(strPath, 0) == -1 && 3 < strDisk.GetLength())
	{
		if (m_pTxtShowMsg)
		{
			m_pTxtShowMsg->SetTextColor(4294901760);
			m_pTxtShowMsg->SetText(_T("��������ȷ�İ�װ·����"));
		}
	}
	else
	{
		if (3 <= strDisk.GetLength())
		{					
			int iFreeSpace = GetDiskFree(strDisk.Left(3).GetData());
			CDuiString strValue;
			DWORD dwMsgColor = m_dwMsgColor;
			strValue.Format(_T("����ռ䣺%dMB�����ÿռ䣺%.1fGB"), g_InstallSpace, iFreeSpace/1024.f);
			if (iFreeSpace > g_InstallSpace)
			{
				m_bCanInstall = true;
			}
			else
			{
				dwMsgColor = g_dwWarnColor;
				strValue.Format(_T("����ռ䣺%dMB�����ÿռ䣺%.1fGB"), g_InstallSpace, iFreeSpace/1024.f);
			}
			if (m_pTxtShowMsg)
			{
				m_pTxtShowMsg->SetTextColor(dwMsgColor);
				m_pTxtShowMsg->SetText(strValue.GetData());
			}			
		}
		else
		{
			if (m_pTxtShowMsg)
			{
				m_pTxtShowMsg->SetTextColor(g_dwWarnColor);
				m_pTxtShowMsg->SetText(_T("��������ȷ�İ�װ·����"));
			}
		}
	}
}

void CMainWnd::CreateLinkFile(LPCTSTR szStartAppPath,LPCTSTR szDestLnkPath)
{
	IShellLink *pisl;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL,
		CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
	if (SUCCEEDED(hr))
	{
		IPersistFile* pIPF;
		//����������Ҫ������ݷ�ʽ��ԭʼ�ļ���ַ
		pisl->SetPath(szStartAppPath);
		CDuiString strPath = szStartAppPath;
		pisl->SetWorkingDirectory(strPath.Left(strPath.ReverseFind('\\')));  // ������ʼλ��
		hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
		if (SUCCEEDED(hr))
		{
			//����������Ҫ������ݷ�ʽ��Ŀ���ַ
			pIPF->Save(T2BSTR(szDestLnkPath), TRUE);
			pIPF->Release();
		}
		pisl->Release();
	}
}

void CMainWnd::AddInstall(LPCSTR lptAppPath, LPCTSTR lptDirectory)
{
	try {

		if ((NULL == lptAppPath) || (NULL == lptDirectory)) {

			return ;
		}

		HKEY hKey = NULL;

		LSTATUS lStatus = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0, KEY_ENUMERATE_SUB_KEYS, &hKey);

		if (ERROR_SUCCESS != lStatus) {

			return ;
		}

		HKEY hKeyValue = NULL;
		lStatus = ::RegOpenKey(hKey, TEXT("Rhino81D6A461-B578-4615-90B1-51675927B00D"), &hKeyValue);

		if (ERROR_SUCCESS != lStatus) {

			lStatus = ::RegCreateKey(hKey, TEXT("Rhino81D6A461-B578-4615-90B1-51675927B00D"), &hKeyValue);

			if (ERROR_SUCCESS != lStatus) {

				return ;
			}
		}

		char pchPathIcon[1024] = { 0 };
		::strcpy_s(pchPathIcon, ARRAYSIZE(pchPathIcon), lptDirectory);
		std::string s;
		s = "\\";
		s += g_strExeName;
		s += ".exe";
		::strcat_s(pchPathIcon, ARRAYSIZE(pchPathIcon), s.c_str());

		::RegSetValueEx(hKeyValue, TEXT("DisplayIcon"), 0, REG_SZ, (const BYTE*)pchPathIcon, ::strlen(pchPathIcon) + 1);

		char* pwchDisplay = "Ϭţ�羺ƽ̨";
		::RegSetValueEx(hKeyValue, TEXT("DisplayName"), 0, REG_SZ, (const BYTE*)pwchDisplay, ::strlen(pwchDisplay) + 1);

		char* pwchPublisher = "Ϭţ�羺";
		::RegSetValueEx(hKeyValue, TEXT("Publisher"), 0, REG_SZ, (const BYTE*)pwchPublisher, ::strlen(pwchPublisher) + 1);

		char pchUninallPath[1024] = { 0 };
		::strcpy_s(pchUninallPath, ARRAYSIZE(pchUninallPath), lptDirectory);

		std::string s2;
		s2 = "\\";
		s2 += g_strUnInstallExeName;
		s2 += ".exe";

		::strcat_s(pchUninallPath, ARRAYSIZE(pchUninallPath), s2.c_str());

		::RegSetValueEx(hKeyValue, TEXT("UninstallString"), 0, REG_SZ, (const BYTE*)pchUninallPath, ::strlen(pchUninallPath) + 1);

		::RegCloseKey(hKeyValue);
		::RegCloseKey(hKey);
	}
	catch (...) {

	}
}

void CMainWnd::NoWindowInstall()
{
	std::string strRoot = GetAppPath();

	//������ɺ����ʱ��
	CDuiString strIni;
	strIni.Format("%s\\%s", strRoot.c_str(), "set.ini");
	int itag = GetPrivateProfileInt("config", "delay", 1, strIni.GetData());

	//��ȡ�����ļ���ȷ���ӳٶ೤ʱ��ſ�ʼ����
	MyDebugString("sdp_test", "��ȡ�����ļ���ȷ���ӳ�%ds�ſ�ʼ����", itag);
	if (itag<=0)
	{
		itag = 1;
	}
	SetTimer(m_hWnd, TIMER_DELAYED_UPDATE, itag*1000, NULL);
}

void CMainWnd::SwitchCustomImage(LPCTSTR lpBkImage)
{
	if (!lpBkImage || !m_pBtnCustom)
	{
		return;
	}
	CDuiString strBk;
	strBk.Format(_T("file='%s' source='0,0,130,30'"), lpBkImage);
	m_pBtnCustom->SetBkImage(strBk.GetData());

	strBk.Format(_T("file='%s' source='0,0,130,30'"), lpBkImage);
	m_pBtnCustom->SetNormalImage(strBk.GetData());

	strBk.Format(_T("file='%s' source='0,30,130,60'"), lpBkImage);
	m_pBtnCustom->SetHotImage(strBk.GetData());

	strBk.Format(_T("file='%s' source='0,60,130,90'"), lpBkImage);
	m_pBtnCustom->SetPushedImage(strBk.GetData());
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (0 == _tcsicmp(_T("Btn_Back"), msg.pSender->GetName().GetData()))
		{
			if (m_pMainTab)
			{
				m_pMainTab->SelectItem(0);
			}
		}
		else if (0 == _tcsicmp(_T("Btn_agreement"), msg.pSender->GetName().GetData()))
		{
			if (m_pMainTab)
			{
				m_pMainTab->SelectItem(1);
			}
		}
		else if (0 == _tcsicmp(_T("Btn_Install"), msg.pSender->GetName().GetData()))
		{
			if (!m_bCanInstall)
			{
				if (m_pHorCustom)
				{
					int iValue = m_pHorCustom->GetFixedHeight();
					if (g_MinHorValue == iValue)
					{
						KillTimer(m_hWnd, TIMER_SHOW_CUSTOM);
						SetTimer(m_hWnd, TIMER_SHOW_CUSTOM, 20, NULL);
					}
				}
				return;
			}
			if (m_pTab)
			{
				CControlUI* pWindow = m_PaintManager.FindControl(_T("Hor_Window"));
				if (pWindow)
				{
					pWindow->SetBkImage(_T("bk1.png"));
				}
				m_pTab->SelectItem(1);
			}

			//��ȡ�������µ�ַ
			std::string _str;
			m_MyWinHttp.GetHttpDownAddress(_str);
			getDownloadAddressInfo(_str);
			CDuiString strUrl;
			strUrl.Format(_T("%s"), m_strWeb_download.c_str());
			m_task_id = AddDownload(strUrl.GetData());

			SetTimer(m_hWnd, TIMER_SMOOTH_SHOW, g_change, NULL);
		}
		else if (0 == _tcsicmp(_T("Btn_Custom"), msg.pSender->GetName().GetData()))
		{                                                 
			if (m_pHorCustom)
			{
				int iValue = m_pHorCustom->GetFixedHeight();
				if (g_MinHorValue == iValue || g_MaxHorValue == iValue)
				{
					KillTimer(m_hWnd, TIMER_SHOW_CUSTOM);
					SetTimer(m_hWnd, TIMER_SHOW_CUSTOM, 20, NULL);
				}
			}
		}
		else if (0 == _tcsicmp(_T("Btn_Change"), msg.pSender->GetName().GetData()))
		{
			TCHAR szBuffer[MAX_PATH] = {0};
			BROWSEINFO bi;
			ZeroMemory(&bi,sizeof(BROWSEINFO));
			bi.hwndOwner = NULL;
			bi.pszDisplayName = szBuffer;
			bi.lpszTitle = _T("������ѡ��װĿ¼��");
			bi.ulFlags = BIF_RETURNFSANCESTORS;
			LPITEMIDLIST idl = SHBrowseForFolder(&bi);
			if (NULL == idl)
			{
				return;
			}
			SHGetPathFromIDList(idl,szBuffer);
			if (m_pEdtPath)
			{
				m_pEdtPath->SetText(szBuffer);
			}
			VerifyPath(szBuffer);
		}
		else if (0 == _tcsicmp(_T("Btn_Start"), msg.pSender->GetName().GetData()))
		{
			CDuiString strExePath;//����������·��һ��
			strExePath.Format(_T("%s\\%s\\%s.exe"), m_pEdtPath->GetText().GetData(), g_strFolderName.c_str(), g_strExeName.c_str());
			char str[MAX_PATH];
			sprintf_s(str, "sdp_aaa ·��: %s", strExePath.GetData());
			OutputDebugStringA(str);
			CDuiString strExePath2;//��������·��
			strExePath2.Format(_T("%s\\%s"), m_pEdtPath->GetText().GetData(), g_strFolderName.c_str());
			ShellExecute(NULL, _T("open"), strExePath.GetData(), NULL, strExePath2.GetData(), SW_SHOW);
			Close();
		}
		else if (0 == _tcsicmp(_T("Btn_Finish"), msg.pSender->GetName().GetData()))
		{
			Close();
		}
	}
	else if (msg.sType == DUI_MSGTYPE_KILLFOCUS)
	{
		if (0 == _tcsicmp(_T("Edt_Path"), msg.pSender->GetName().GetData()))
		{
			VerifyPath(msg.pSender->GetText().GetData());
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (0 == _tcsicmp(_T("Opt_Agree"), msg.pSender->GetName().GetData()))
		{
			if (m_pOptAgree)
			{
				if (m_pBtnInstall)
				{
					bool a = m_pOptAgree->IsSelected();
					m_pBtnInstall->SetEnabled(a);
				}
			}
		}
	}
	__super::Notify(msg);
}

//���и�����Ҫ�����л�������
void CMainWnd::Exec_dx_exe()

{
	CDuiString strExePath2;//��������·��
	strExePath2.Format(_T("%s\\%s"), m_pEdtPath->GetText().GetData(), g_strFolderName.c_str());
	CDuiString strExe_dxPath;//vs2015���л�������
	//if (!Is64Bit_OS())
	//{//32λ
	strExe_dxPath.Format(_T("%s\\%s\\cp15.exe"), m_pEdtPath->GetText().GetData(), g_strFolderName.c_str());
	CDuiString strExe_dxPath2;
	strExe_dxPath2.Format(_T("%s\\%s\\RhinoMonitor.exe"), m_pEdtPath->GetText().GetData(), g_strFolderName.c_str());
	//}
	//else
	//{//64λ
	//	strExe_dxPath.Format(_T("%s\\%s\\cp15a.exe"), m_pEdtPath->GetText().GetData(), g_strFolderName.c_str());
	//}

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = strExe_dxPath2.GetData();
	ShExecInfo.lpParameters = "/uninstall";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	::ShellExecuteEx(&ShExecInfo);

	::Sleep(3000);
	
	::ZeroMemory(&ShExecInfo, sizeof(SHELLEXECUTEINFO));
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = strExe_dxPath2.GetData();
	ShExecInfo.lpParameters = "/install";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	::ShellExecuteEx(&ShExecInfo);
}

LRESULT CMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	case WM_TIMER:
		{
			OnTimer((int)wParam);
		}break;
	case WM_INSTALL:
		{
			int iCurePos = (int)lParam;
			SetProgress(iCurePos);
			if (m_pTxtStatues)
			{
				CDuiString strValue;
				strValue.Format(_T("%d%%"), iCurePos/10);
				m_pTxtStatues->SetText(strValue.GetData());
			}
			if (iCurePos >= (g_Download_Max_Value+g_Unzip_Max_Value))
			{//�˴���ʾ�������ز���װ���
				//�رն�����ʱ��
				KillTimer(m_hWnd, TIMER_SMOOTH_SHOW);
				KillTimer(m_hWnd, TIMER_SMOOTH_HIDE);
				KillTimer(m_hWnd, TIMER_CHANGE);

				//http֪ͨ�����������ذ�װ�ɹ�
				m_MyWinHttp.tellHttpServerDownFinish(m_iSource);
				
				//д��ע���
				std::string _str = m_pEdtPath->GetText().GetData();
				_str += "\\";
				_str += g_strFolderName.c_str();
				_str += "\\";
				//AddPath_registry((char*)_str.c_str());
				//������ݷ�ʽ
				CDuiString strSrc, strDest;
				strSrc.Format(_T("%s\\%s\\%s.exe"), m_pEdtPath->GetText().GetData(), g_strFolderName.c_str(), g_strExeName.c_str());
				strDest.Format(_T("%s\\%s.lnk"), GetDesktop().GetData(), g_strLinkName.c_str());
				CreateLinkFile(strSrc.GetData(), strDest.GetData());
				CDuiString strSrc2;
				strSrc2.Format(_T("%s\\%s"), m_pEdtPath->GetText().GetData(), g_strFolderName.c_str());

				//�ڰ�װ�б�����Ӳ�Ʒ
				AddInstall(g_strExeName.c_str(), strSrc2.GetData());
				
				//������ɺ����ʱ��
				CDuiString strIni;
				strIni.Format(_T("%s\\%s\\%s"), m_pEdtPath->GetText().GetData(), g_strFolderName.c_str(), _T("update.ini"));
				::WritePrivateProfileString(_T("config"), _T("version"), m_str_version.c_str(), strIni.GetData());
				CDuiString	_str_smallversion = "0";//�Ӱ汾
				::WritePrivateProfileString(_T("config"), _T("smallversion"), _str_smallversion, strIni.GetData());
				CDuiString	_str_Source;//�ƹ��־
				_str_Source.Format(_T("%d"), m_iSource);
				::WritePrivateProfileString(_T("config"), _T("source"), _str_Source, strIni.GetData());
				Exec_dx_exe();
				SetTimer(m_hWnd, TIMER_EXIST_DX_EXE, 5000, NULL);
			}
		}break;
	default:
		break;
	}
	return 0;
}

void CMainWnd::OnTimer(int iEventID)
{
	switch (iEventID)
	{
	case TIMER_DELAYED_UPDATE:
	{//��ʱ����
								 KillTimer(m_hWnd, TIMER_DELAYED_UPDATE);

								 try {

									 CButtonUI* pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(TEXT("Btn_Install")));

									 if (NULL != pButton) {

										 m_NoWndInstall = true;

										 m_PaintManager.SendNotify(pButton, DUI_MSGTYPE_CLICK);
									 }
								 }
								 catch (...) {

								 }
	}break;
	case TIMER_EXIST_DX_EXE:
	{
			//HWND hWnd = ::FindWindowA(NULL, "Microsoft Visual C++ 2015 Redistributable (x86) - 14.0.24123 ��װ����");
			//if (nullptr != hWnd)
			//{
			//	break;
			//}
			//û�д����ˣ�������ʾ�Լ��Ŀͻ�����
			KillTimer(m_hWnd, iEventID);

			//�л�������װ��ɡ�ҳ��
			CControlUI* pWindow = m_PaintManager.FindControl(_T("Hor_Window"));
			if (pWindow)
			{
				pWindow->SetBkImage(_T("bk2.png"));
			}
			if (m_pTab)
			{
				m_pTab->SelectItem(2);
			}

			if (true == m_NoWndInstall) {

				::PostQuitMessage(0);
			}

	}break;
	case TIMER_SMOOTH_SHOW:
		{
			m_iCurFade += g_base;
			SmoothSwitchBK();
		}break;
	case TIMER_SMOOTH_HIDE:
		{
			m_iCurFade -= g_base;
			SmoothSwitchBK();
		}break;
	case TIMER_CHANGE:
		{
			KillTimer(m_hWnd, TIMER_CHANGE);
			SetTimer(m_hWnd, TIMER_SMOOTH_HIDE, g_change, NULL);
		}break;
	case TIMER_DOWNLOAD_PROCESS:
		{
			DownloadProcess();
		}break;
	case TIMER_SHOW_CUSTOM:
		{
			if (m_pHorCustom)
			{
				int iValue = m_pHorCustom->GetFixedHeight();
				if (!m_bToUp)
				{
					iValue -= g_HorBase;
					if (iValue <= g_MinHorValue)
					{						
						iValue = g_MinHorValue;
						KillTimer(m_hWnd, TIMER_SHOW_CUSTOM);
						m_bToUp = !m_bToUp;
						//�޸İ�ť״̬ͼ
						SwitchCustomImage(_T("Custom_Up.png"));
					}
				}
				else
				{
					iValue += g_HorBase;
					if (iValue >= g_MaxHorValue)
					{						
						iValue = g_MaxHorValue;
						KillTimer(m_hWnd, TIMER_SHOW_CUSTOM);
						m_bToUp = !m_bToUp;
						//�޸İ�ť״̬ͼ
						SwitchCustomImage(_T("Custom_Down.png"));
					}					
				}				
				m_pHorCustom->SetFixedHeight(iValue);
			}
		}break;
	default:
		break;
	}
}

void CMainWnd::DownloadProcess()
{
	FCHttpRequest *pTask = FindRequest(m_task_id);
	if (!pTask)
	{
		SetErrorMsg(_T("��������ʧ�ܣ��޷���ȡ���ȣ�"));
		KillTimer(m_hWnd, TIMER_DOWNLOAD_PROCESS);
		return;
	}

	int   living_ms = pTask->GetRunningTime() ;
	int   nDownload = pTask->GetTotalReceiveByte() ;

	if (!living_ms)
		living_ms = 100 ;

	//�����ļ���С�������ش�С�������
	int nTotal = pTask->GetResponseInfo().m_content_length;
	
	if (nTotal)
	{
		int nPercent = (int)(g_Download_Max_Value * 1.0 * nDownload / nTotal);		
		SetProgress(nPercent);
		if (m_pTxtStatues)
		{
			CDuiString strValue;
			strValue.Format(_T("%d%%"), nPercent/10);
			m_pTxtStatues->SetText(strValue.GetData());
		}
	}
	static int iTimes = 0;
	iTimes += 20;
	if (0 == iTimes%1000)
	{
		CDuiString strSpeed;
		strSpeed.Format(_T("�����ĵȴ������Ͼͺ�! ��ǰ�ٶȣ�%s/s"), GetFormatKB((int)(1000.0*nDownload/living_ms)).GetData());
		SetErrorMsg(strSpeed.GetData());
	}	
}

void CMainWnd::SmoothSwitchBK()
{
	if (!m_pCtrlTheme)
	{
		return;
	}

	if (m_iCurFade > g_MaxFade)
	{
		m_iCurFade = g_MaxFade;
	}
	if (m_iCurFade < g_MinFade)
	{
		m_iCurFade = g_MinFade;		
	}

	if (m_iCurFade >= g_MaxFade)
	{
		KillTimer(m_hWnd, TIMER_SMOOTH_SHOW);
		SetTimer(m_hWnd, TIMER_CHANGE, g_ShowTime, NULL);
	}
	else if (m_iCurFade <= g_MinFade)
	{
		m_iCurIndex = ((++m_iCurIndex)%g_BkCount);
		KillTimer(m_hWnd, TIMER_SMOOTH_HIDE);
		SetTimer(m_hWnd, TIMER_SMOOTH_SHOW, g_change, NULL);
	}

	CDuiString strBg;
	strBg.Format(_T("file='%d.png' fade='%d'"), m_iCurIndex, m_iCurFade);
	m_pCtrlTheme->SetBkImage(strBg.GetData());	
}

unsigned int __stdcall CMainWnd::Thread_Install(LPVOID lpVoid)
{
	CMainWnd* pMainWnd = (CMainWnd*)lpVoid;
	if (!pMainWnd || 0 >= pMainWnd->m_receive.size())
	{
		return 0;
	}
	TCHAR szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szPath);
	if (pMainWnd->m_pEdtPath)
	{
		SetCurrentDirectory(pMainWnd->m_pEdtPath->GetText().GetData());
	}

	//��ѹ���ļ�
	HZIP hz = OpenZip((char*)(pMainWnd->m_receive.c_str()), pMainWnd->m_receive.size(), ZIP_MEMORY);
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
		//��ѹ��i���ļ�
		zr = UnzipItem(hz, i, ze.name, 0, ZIP_FILENAME);
		
		//���������Ͱ�װ����
		iCurePos = g_Unzip_Max_Value*1.0*(i+1)/numitems;
		::PostMessage(pMainWnd->GetHWND(), WM_INSTALL, (WPARAM)0, (LPARAM)(g_Download_Max_Value+iCurePos));
		//Sleep(20);
	}
	CloseZip(hz);
	SetCurrentDirectory(szPath);
	return 0;
}

//-------------------------------------------------------------------------------------
void CMainWnd::OnAfterRequestSend (FCHttpRequest& rTask)
{
	const HTTP_RESPONSE_INFO   & resp = rTask.GetResponseInfo() ;

	if (resp.m_status_code == 0)
	{
		SetErrorMsg(_T("�ܱ�Ǹ�����ӷ�����ʧ�ܣ����Ժ����ԡ�"));
		DeleteRequest (rTask.GetTaskID()) ;
		return ;
	}
	 
	::SetTimer(m_hWnd, TIMER_DOWNLOAD_PROCESS, 20, NULL);
}
//-------------------------------------------------------------------------------------
CDuiString CMainWnd::GetStatusText (FCHttpRequest& rTask)
{
	const HTTP_RESPONSE_INFO   & resp = rTask.GetResponseInfo() ;

	CDuiString   s ;
	switch (resp.m_status_code)
	{
	case HTTP_STATUS_OK :
	case HTTP_STATUS_PARTIAL_CONTENT :
		if (resp.m_final_read_result)
			s = "Success" ;
		else
			s = "Read Error" ;

		// range request
		if (rTask.GetRequestHeader().m_start && (resp.m_status_code == HTTP_STATUS_OK))
		{
			s += " ( not support range )" ;
		}
		break;

	case HTTP_STATUS_NOT_FOUND :
		s = "404 error : Not Found" ;
		break;

	case HTTP_STATUS_FORBIDDEN :
		s = "403 error : Forbidden" ;
		break;

	case HTTP_STATUS_PROXY_AUTH_REQ :
		s = "407 error : Proxy Authentication Required" ;
		break;

	default :
		s.Format("Download Error, status code : %d", resp.m_status_code) ;
		break;
	}
	return s ;
}
//-------------------------------------------------------------------------------------
void CMainWnd::OnAfterRequestFinish (FCHttpRequest& rTask)
{
	rTask.PopReceived(m_receive) ;
	KillTimer(m_hWnd, TIMER_DOWNLOAD_PROCESS);	
	if (0 < m_receive.size())
	{
		SetErrorMsg(_T(""));
		unsigned int uiThreadID = 0;
		_beginthreadex(NULL, 0, Thread_Install, this, 0, &uiThreadID);
	}
}

CDuiString CMainWnd::GetDesktop()
{
	CDuiString strValue;
	LPITEMIDLIST pidl;
	LPMALLOC pShellMalloc;
	TCHAR szDir[MAX_PATH];
	if(SUCCEEDED(SHGetMalloc(&pShellMalloc)))
	{
		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL,CSIDL_DESKTOPDIRECTORY,&pidl)))
		{
			// ����ɹ�����true
			if(SHGetPathFromIDList(pidl, szDir))
			{
				strValue = szDir;
			}
			pShellMalloc->Free(pidl);
		}
		pShellMalloc->Release();
	}
	return strValue;
}

//ע���д��ʱ���жϸ�·���ַ�����ԭpath�ַ������ǲ��Ǵ���
bool CMainWnd::IsPathExist(char *PerfData, char *myPath)
{
	std::string str_PerfData = PerfData;
	std::string str_myPath = myPath;
	std::size_t found = str_PerfData.find(str_myPath);
	if (found != std::string::npos)
	{
		return true;
	}

	return false;
}

//��ϵͳ��������path�м�����Ϸ��װ·��
bool CMainWnd::AddPath_registry(char *szPath)
{
	char s[MAX_PATH] = "SYSTEM\\ControlSet001\\Control\\Session Manager\\Environment";
	HKEY hKey;

	DWORD retCode = RegOpenKeyExA(
		HKEY_LOCAL_MACHINE,
		s,
		NULL,
		KEY_READ | KEY_WRITE | KEY_WOW64_64KEY,
		&hKey
		);

	if (ERROR_SUCCESS != retCode)
	{//�򿪼�environment  
		DWORD a = GetLastError();
		MessageBox(NULL, _T("����ע������"), _T("ERROR"), MB_ICONERROR);
		return 0;
	}

	DWORD dwType = 0, dw = 0;
	DWORD dwRet = RegQueryValueExA(hKey, "Path", NULL, &dwType, NULL, &dw);

	char* pStr = new char[dw];
	RegQueryValueExA(hKey, "Path", NULL, &dwType, (BYTE*)pStr, &dw);
	if (!IsPathExist(pStr, szPath))
	{//�����ڲ����
		std::string str = pStr;
		str += ";";
		str += szPath;
		//RegSetValueEx����Ҫ���õ�ע������²���Ĭ��ֵ����������ֵ�����ݺ�����ʱ�����Ե���RegSetValueEx�������ú�������ע�����������ָ��ֵ�����ݺ����͡�
		long setResult = RegSetValueExA(hKey, "Path", NULL, REG_EXPAND_SZ,
			(BYTE * const)str.c_str(), (str.length() + 1)*sizeof(char));
	}
	
	RegCloseKey(hKey);

	SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
		(LPARAM)"Environment", SMTO_ABORTIFHUNG, 5000, &dwRet);
	if (ERROR_SUCCESS == dwRet)
	{
		//MessageBox(NULL, _T("��������������ɣ�"), _T("��Ϣ"), MB_OK);
	}
	else
	{
		MessageBox(NULL, _T("������������ʧ�ܣ�"), _T("��Ϣ"), MB_OK);
	}

	return TRUE;
}

void CMainWnd::getDownloadAddressInfo(string strValue)
{
	Json::Reader reader;
	Json::Value my_value;

	if (reader.parse(strValue, my_value)
		&& my_value.isMember("route")
		&& my_value.isMember("version"))
	{
		m_strWeb_download = my_value["route"].asString();
		m_str_version = my_value["version"].asString();
	}
	string str = my_value.toStyledString();
}