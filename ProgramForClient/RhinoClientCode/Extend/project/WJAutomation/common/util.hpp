#ifndef _UTIL_H_
#define _UTIL_H_

#include <Windows.h>
#include <string>

//ѡ���ļ��жԻ���  
#include<Shlobj.h>  
#pragma comment(lib,"Shell32.lib") 

class Util
{
public:
	static std::string strToUtf8(const std::string& _str)
	{
		static char sBuffer[1024];
		static wchar_t sBufferTemp[1024];

		ZeroMemory(sBufferTemp, 1024);
		ZeroMemory(sBuffer, 1024);

		int len = MultiByteToWideChar(CP_ACP, 0, _str.c_str(), -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _str.c_str(), _str.length(), sBufferTemp, len);

		int length = WideCharToMultiByte(CP_UTF8, 0, sBufferTemp, -1, NULL, NULL, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, sBufferTemp, len, sBuffer, length, NULL, NULL);
		return std::string(sBuffer);
	}


	static bool WStringToString(const std::wstring &wstr, std::string &str)
	{
		int len = (int)wstr.length();
		str.resize(len, ' ');

		int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), len, (LPSTR)str.c_str(), len, NULL, NULL);
		if (nResult == 0)
		{
			return false;
		}

		return true;
	}

	static bool openFile(std::string& _file)
	{
		TCHAR szFileName[MAX_PATH] {0};
		OPENFILENAME openFileName = { 0 };
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.nMaxFile = MAX_PATH;  //����������ã������õĻ�������ִ��ļ��Ի���  
		openFileName.lpstrFilter = TEXT("�����ļ�(*.*)\0*.*\0\0");
		openFileName.lpstrFile = szFileName;
		openFileName.lpstrInitialDir = TEXT("F:\\01-Development\\lixySample\\lixySample\\bin\\images");

		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


		if (!::GetOpenFileName(&openFileName))
			return false;

		return WStringToString(szFileName, _file);
	}

	static bool openFolder(std::string& _dir)
	{
		TCHAR szBuffer[MAX_PATH] = { 0 };
		BROWSEINFO bi = { 0 };
		bi.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
		bi.pszDisplayName = szBuffer;//�����ļ��еĻ�����  
		bi.lpszTitle = TEXT("ѡ��һ���ļ���");//����  
		bi.ulFlags = BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
		if (!SHGetPathFromIDList(idl, szBuffer))
			return false;

		return WStringToString(szBuffer, _dir);
	}


};

#endif // _UTIL_H_