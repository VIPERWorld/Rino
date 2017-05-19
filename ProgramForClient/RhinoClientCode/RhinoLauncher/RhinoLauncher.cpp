#include "stdafx.h"
#include "RhinoLauncher.h"
#include <windows.h>
#include <winhttp.h> 
#include <Psapi.h>
#include "atlbase.h"
#include "commonfunction_c.h"
#include "json.h"
#pragma comment(lib,"winhttp.lib")
#pragma comment(lib,"user32.lib")

//
// GetDebugPriv
// �� Windows NT/2000/XP �п�����Ȩ�޲����������Ϻ���ʧ��
// ���ԡ�System Ȩ�����е�ϵͳ���̣��������
// �ñ�����ȡ�á�debug Ȩ�޼���,Winlogon.exe ��������ֹŶ :)
//
BOOL GetDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}

//
// FindProcess
// �������Ψһ�Ĳ�������ָ���Ľ���������:���Ŀ�����
// �� "Notepad.exe",����ֵ�Ǹý��̵�ID��ʧ�ܷ���0
//
DWORD FindProcess(char *strProcessName)
{
	DWORD aProcesses[1024], cbNeeded, cbMNeeded;
	HMODULE hMods[1024];
	HANDLE hProcess;
	char szProcessName[MAX_PATH];
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return 0;
	for (int i = 0; i < (int)(cbNeeded / sizeof(DWORD)); i++)
	{
		//_tprintf(_T("%d/t"), aProcesses[i]);  
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
		EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbMNeeded);
		GetModuleFileNameExA(hProcess, hMods[0], szProcessName, sizeof(szProcessName));

		if (strstr(szProcessName, strProcessName))
		{
			//_tprintf(_T("%s;"), szProcessName);  
			return (aProcesses[i]);
		}
		//_tprintf(_T("/n"));  
	}
	return 0;
}

//
// Function: ErrorForce
// �˺������������ FindProcess ����������Ŀ����̵�ID
// ��WIN API OpenPorcess ��ô˽��̵ľ��������TerminateProcess
// ǿ�ƽ����������
//
void KillProcess(char *strProcessName)
{
	GetDebugPriv();
	// When the all operation fail this function terminate the "winlogon" Process for force exit the system.  
	HANDLE hYourTargetProcess = OpenProcess(PROCESS_TERMINATE |
		PROCESS_QUERY_INFORMATION |   // Required by Alpha  
		PROCESS_CREATE_THREAD |   // For CreateRemoteThread  
		PROCESS_VM_OPERATION |   // For VirtualAllocEx/VirtualFreeEx  
		PROCESS_VM_WRITE,             // For WriteProcessMemory  
		FALSE, FindProcess(strProcessName));
	if (hYourTargetProcess == NULL)
	{
		return;
	}
	bool bres = TerminateProcess(hYourTargetProcess, 0);
	return;
}

CRhinoLauncher::CRhinoLauncher()
{

}

CRhinoLauncher::~CRhinoLauncher()
{

}

/*
std::string version ���汾
std::string smallversion �Ӱ汾
std::string _filename ���ص��ļ�
std::string strDownLoadUpdatepADDR ���صĸ���ַ
std::string startgamename �����ĳ�������
*/
bool CRhinoLauncher::updateOnline(std::string version, std::string smallversion, std::string _filename, std::string strDownLoadUpdatepADDR, std::string startgamename)
{
	// ����������
	char strCommonLine[MAX_PATH] = { 0 };
	std::string str_CommonLine;
	char _str_exe[100] = { 0 };
	std::string _strAppPath = G2U(GetAppPath().c_str());
#ifdef _DEBUG
	sprintf_s(_str_exe, "Update_d.exe");
#else
	sprintf_s(_str_exe, "Update.exe");
#endif // _DEBUG
	sprintf_s(strCommonLine, ("%s /nb:%s%s/ne /v1b:%s/v1e /v2b:%s/v2e /ub:%s/ue /pb:")
		, _str_exe, _strAppPath.c_str(), startgamename.c_str(), version.c_str(), smallversion.c_str(), strDownLoadUpdatepADDR.c_str());


	DWORD dwSize = 0;
	str_CommonLine += strCommonLine;
	str_CommonLine += _filename;

	char _str[MAX_PATH * 10] = { 0 };
	sprintf_s(_str, "%s/pe /sb:%lu/se", str_CommonLine.c_str(), dwSize);
	std::string strCommand = U2G(_str);
	std::string _address = strCommand;
	STARTUPINFOA StartInfo;
	memset(&StartInfo, 0, sizeof(StartInfo));
	StartInfo.cb = sizeof(StartInfo);
	StartInfo.wShowWindow = SW_SHOWNORMAL;
	PROCESS_INFORMATION 	m_ProcessInfo;
	MyDebugString("sdp_test", "���߸������� %s", _address.c_str());
	BOOL bSuccess = CreateProcessA(NULL, /*_str*/(LPSTR)_address.c_str(), NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &StartInfo, &m_ProcessInfo);

	if (bSuccess) {

		::CloseHandle(m_ProcessInfo.hThread);
		::CloseHandle(m_ProcessInfo.hProcess);
	}

	ExitProcess(0);
	return true;
}

#include <assert.h>
bool CRhinoLauncher::VersionUpdate()
{
	std::string route;		//���汾���ص�ַ
	std::string version;	//���汾��
	std::string info;		//���汾������Ϣ
	Json::Value _smallversion;	//�Ӱ汾����
	std::string _webroot;		//��ҳ��·��
	std::wstring _wwebroot;		//��ҳ��·��
#ifdef _DEBUG
	std::string _exe_name = "RhinoClient_d.exe";	//������������
#else
	std::string _exe_name = "RhinoClient.exe";		//������������
#endif // _DEBUG
////////////////
	//�ļ�����
	std::string str_file = GetAppPath();
	str_file += "Config\\clientconfig.json";

	//�����ļ�ֻ������
	DWORD dwAttributes = ::GetFileAttributesA(str_file.c_str());

	if (dwAttributes & FILE_ATTRIBUTE_READONLY) {

		DWORD dwTempAttribute = dwAttributes & (~FILE_ATTRIBUTE_READONLY);

		::SetFileAttributesA(str_file.c_str(), dwTempAttribute);
	}

	HANDLE hFile = ::CreateFileA(str_file.c_str(),		//�����ļ������ơ�
		GENERIC_READ,									// д�Ͷ��ļ���
		0,												// �������д��
		NULL,											// ȱʡ��ȫ���ԡ�
		OPEN_EXISTING,									// �ļ������Ѿ����ڡ����豸���Ҫ��
		FILE_ATTRIBUTE_NORMAL,							// һ����ļ���       
		NULL);											// ģ���ļ�Ϊ�ա�
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//��ȡ�����롣
		DWORD dwError = GetLastError();
		OutputDebugString(TEXT("CreateFile fail!\r\n"));
	}

	const int BUFSIZE = 4096;
	char chBuffer[BUFSIZE];
	DWORD dwReadSize = 0;
	memset(chBuffer, 0, 4096);
	BOOL bRet = ::ReadFile(hFile, chBuffer, BUFSIZE-1, &dwReadSize, NULL);
	if (bRet)
	{
		MyDebugString("sdp_test", "ReadFile ���ļ��ɹ�");
	}
	else
	{
		//��ȡ�����롣
		DWORD dwError = GetLastError();
		//�������            
		TCHAR chErrorBuf[1024];
		wsprintf(chErrorBuf, TEXT("GetLastError()=%d"), dwError);
		MyDebugString("sdp_test", "GetLastError()=%d", dwError);

		::MessageBox(NULL, chErrorBuf, L"�����ļ���ȡʧ��", MB_OK);
		return false;
	}

	Json::Reader reader;
	Json::Value my_value;
	if (reader.parse(chBuffer, my_value) && my_value.isMember("SysSet"))
	{
		//��ȡ����ַ
		_webroot = my_value["SysSet"]["web_server_address"].asString();
		if (_webroot.empty())
		{//����Ϊ��
			return false;
		}
		//Ҫ�ǵ�ȥ����http://��
		std::string del = "http://";
		std::string::size_type _npos = _webroot.find(del, 0);  // pΪ�ҵ���λ��,-1Ϊδ�ҵ�
		if (_npos != std::string::npos)
		{
			_webroot.erase(_npos, del.length());
		}
		//Ҫ�ǵð��ַ�������"/"����"\\"
		std::string _s = _webroot.substr(_webroot.size()-1, 1);
		if (0 == strcmp(_s.c_str(), "/") || 0 == strcmp(_s.c_str(), "\\"))
		{
			_webroot.erase(_webroot.size() - 1, _s.length());
		}
	}
	else
	{
		return false;
	}
	////////////////
	std::string str_res;//���ؽ��
	if (!StringToWString(_webroot, _wwebroot))
	{//ת��ʧ��
		MyDebugString("sdp_test", "_webroot(%s) ת��ʧ��", _webroot.c_str());
		return false;
	}

	GetHttpData(_wwebroot, INTERNET_DEFAULT_HTTP_PORT, L"/api/fight/version", str_res);
	MyDebugString("sdp_test", "�汾����=%s", str_res.c_str());

	Json::Value my_value2;
	if (reader.parse(str_res, my_value2) && my_value2.isMember("route") && my_value2.isMember("version"))
	{//������õķ��������������¿ͻ��˰汾����
		//���汾���ص�ַ
		route = my_value2["route"].asString();
		//���汾��
		version = my_value2["version"].asString();
		//���汾������Ϣ
		info = my_value2["info"].asString();
		//�Ӱ汾���ݣ���һ�����飬�������ǰ���汾�µ��Ӱ汾��
		_smallversion = my_value2["smallversions"];
	}

	//�鿴���ذ汾��Ϣ
	std::string str_file_update = GetAppPath().c_str();
	str_file_update += "\\update.ini";
	int iversion = GetPrivateProfileIntA("config", "version", 0, str_file_update.c_str());//���汾��
	int ismallversion = GetPrivateProfileIntA("config", "smallversion", 0, str_file_update.c_str());//�Ӱ汾��

	//�Ƚϰ汾
	if (iversion == atoi(version.c_str()))
	{//��汾��ͬ���Ƚ��Ӱ汾
		if (0 != _smallversion.size())
		{//�����Ӱ汾
			std::string version2;//�Ӱ汾��
			std::string route2;//�Ӱ汾���ص�ַ

			/////////////////////////////////
			bool bNeed = false;//�Ƿ���Ҫ����
			//��ʼ��,����������Ϊ�����ڵ�ǰ���ݵĽڵ������
			Json::Value str_ValueInfo;//�ڵ�����
			for (unsigned int i = 0; i < _smallversion.size(); ++i)
			{
				str_ValueInfo = _smallversion[i];
				if (str_ValueInfo.isMember("route") && str_ValueInfo.isMember("version"))
				{
					//�Ӱ汾���ص�ַ
					route2 = str_ValueInfo["route"].asString();
					//�Ӱ汾��
					version2 = str_ValueInfo["version"].asString();
				}

				if (atoi(version2.c_str()) > ismallversion)
				{//��һ���ڵ����ǰ���ذ汾��һ��˵�������а汾Ҫ����
					bNeed = true;
					break;
				}
			}

			if (bNeed)
			{
				MyDebugString("sdp_test", "���߸��� ��ʼ��,����������Ϊ��һ���ڵ������=====%d,%d", atoi(version2.c_str()), ismallversion);
				for (unsigned int i = 0; i < _smallversion.size(); ++i)
				{
					std::string version_midd;//�Ӱ汾��(��ʱ)
					std::string route_midd;//�Ӱ汾���ص�ַ(��ʱ)
					str_ValueInfo = _smallversion[i];
					if (str_ValueInfo.isMember("route") && str_ValueInfo.isMember("version"))
					{
						//�Ӱ汾���ص�ַ
						route_midd = str_ValueInfo["route"].asString();
						//�Ӱ汾��
						version_midd = str_ValueInfo["version"].asString();
					}

					MyDebugString("sdp_test", "���߸��� �����ӽڵ�=====%d", atoi(version_midd.c_str()));

					if (atoi(version_midd.c_str()) <= ismallversion)
					{//�ȵ�ǰ�Ӱ汾С�Ĳ�������
						MyDebugString("sdp_test", "���߸��� �ȵ�ǰ�Ӱ汾С�Ĳ�������=====%d,%d", atoi(version_midd.c_str()), ismallversion);
						continue;
					}

					MyDebugString("sdp_test", "���߸��� ѡ��ȵ�һ���ڵ�汾��ҪС�����Ҵ��ڵ�ǰ�汾�Ľڵ�˵��Ӧ�ø��¸ð汾=====%d,%d", atoi(version2.c_str()), atoi(version_midd.c_str()));

					if (atoi(version2.c_str()) > atoi(version_midd.c_str()))
					{//ѡ��ȵ�һ���ڵ�汾��ҪС�����Ҵ��ڵ�ǰ�汾�Ľڵ�˵��Ӧ�ø��¸ð汾
						version2 = version_midd;
						route2 = route_midd;
					}
				}

				int v1 = 0, v2 = 0;
				v1 = ismallversion;
				v2 = atoi(version2.c_str());
				MyDebugString("sdp_test", "���߸��� �����Ӱ汾�Ȳ�ѯ���Ӱ汾С�������� ��v1=%d < v2=%d��", v1, v2);
				std::string::size_type _npos = route2.rfind('/', route2.size() - 1);
				std::string _s;
				std::string _s2;
				if (_npos != std::string::npos)
				{
					_s = route2.substr(_npos, route2.size() - 1);
					_s2 = route2.substr(0, _npos);
				}
				return updateOnline(version, version2, _s, _s2, _exe_name.c_str());
			}

		}

		MyDebugString("sdp_test", "���߸��� �汾����Ҫ����");
		return false;
	}
	else
	{//��汾��ͬ��ֱ�����ش�汾����
		MyDebugString("sdp_test", "���߸��� ��汾��ͬ��ֱ�����ش�汾���� ��version=%s < virsiontime=%d��", version.c_str(), iversion);
		std::string::size_type _npos = route.rfind('/', route.size() - 1);
		std::string _s;
		std::string _s2;
		if (_npos != std::string::npos)
		{
			_s = route.substr(_npos, route.size() - 1);
			_s2 = route.substr(0, _npos);
		}
		return updateOnline(version, "0", _s, _s2, _exe_name.c_str());
	}

	return false;
}

void CRhinoLauncher::GetHttpData(std::wstring str_web_addr, int iport, std::wstring str_web_content, std::string &str_res)
{
	str_res = "";
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer = NULL;
	std::string str_res2;//�������
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	BOOL  bResults = FALSE;

	hSession = WinHttpOpen(L"User Agent", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	if (hSession)
	{
		hConnect = WinHttpConnect(hSession, str_web_addr.c_str(), /*INTERNET_DEFAULT_HTTP_PORT*/iport, 0);
		if (nullptr == hConnect)
		{
			DWORD res_err = GetLastError();
			printf("%ld", res_err);
		}
	}

	if (hConnect)
	{
		hRequest = WinHttpOpenRequest(hConnect, L"GET", str_web_content.c_str(), L"HTTP/1.1", WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
	}

	if (hRequest)
	{
		bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	}

	if (bResults)
	{
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	}

	if (bResults)
	{
		do
		{
			// Check for available data.

			dwSize = 0;

			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			{
				printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
				break;
			}

			if (!dwSize)
				break;

			pszOutBuffer = new char[dwSize + 1];

			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
			{
				printf("Error %u in WinHttpReadData.\n", GetLastError());
			}
			else
			{
				//printf("%s", pszOutBuffer);
				str_res2 += pszOutBuffer;
			}

			delete[] pszOutBuffer;

			if (!dwDownloaded)
				break;

		} while (dwSize > 0);
	}



	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	std::string strRet = U2G(str_res2.c_str());
	str_res = strRet;
}