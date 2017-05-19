#include "Log.h"
#include <io.h>
#include <iostream>
#include <qstring.h>
#include "common/WJCommonMarco.h"
#include "common/WJEncrypt.h"
#include "common/WJSpinlock.h"
using namespace std;

// ��̬��ʵ��ָ��
CSystemLog* g_mspInstance = nullptr;

WJ::CSpinlock g_logLock;

	// �ļ�����  
void encrypt(std::string &str)
{
	std::string str2;
	char a;
	int i = 0, s, t;

	while (i < str.length())
	{
		s = str.at(i);
		t = genFunEx(ENCRYPT_KEY) ^ s;  // ����  // ���� (һģһ����������һ�ξ��ǽ��ܡ�����)^����������ڼ򵥵ļ��ܣ�����ԭʼֵint a = 19880516;��Կ int key =1314520; ���м��� int data=key^a = 20665500;���� data^key == a��
		a = t;
		str2 += a;

		++i;
	}

	str = str2;
}

int CSystemLog::IsTextUTF8(const char* str, ULONGLONG length)
{
	int i;
	DWORD nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
	UCHAR chr;
	BOOL bAllAscii = TRUE; //���ȫ������ASCII, ˵������UTF-8
	for (i = 0; i < length; i++)
	{
		chr = *(str + i);
		if ((chr & 0x80) != 0) // �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx
			bAllAscii = FALSE;
		if (nBytes == 0) //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		else //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
		{
			if ((chr & 0xC0) != 0x80)
			{
				return FALSE;
			}
			nBytes--;
		}
	}
	if (nBytes > 0) //Υ������
	{
		return FALSE;
	}
	if (bAllAscii) //���ȫ������ASCII, ˵������UTF-8
	{
		return FALSE;
	}
	return TRUE;
}

//Utf8ToGb2312
char* CSystemLog::U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312��UTF-8��ת��
char* CSystemLog::G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

CSystemLog::CSystemLog()
{

}

CSystemLog::~CSystemLog()
{
	g_logLock.Lock();
	if (mFSream.is_open())
		mFSream.close();
	g_logLock.UnLock();
};

CSystemLog* CSystemLog::getInstance()
{
	if (!g_mspInstance)
		g_mspInstance = new CSystemLog();
	return g_mspInstance;
}

void CSystemLog::release()
{
	WJ_SAFE_DELETE(g_mspInstance);
}

void CSystemLog::initialize()
{
	g_logLock.Lock();
	//�Ƚ���log����Ŀ¼·��
	char path[MAX_PATH] = { 0 };
	memset(path, 0, sizeof(path));
	HMODULE  handle = GetModuleHandle(NULL);
	GetModuleFileNameA(handle, path, MAX_PATH);
	char* pwchExeName = strrchr(path, '\\');
	::ZeroMemory(pwchExeName + 1, ::strlen(pwchExeName));
	strcat_s(path, "Log");
	if (_access(path, 0))
		CreateDirectoryA(path, NULL);

	//�ٴ��������Ե�������Ϊ���Ƶ���־�ļ�
	char buffer[64] = { 0 };
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	sprintf_s(buffer, "%4d-%02d-%02d", sys.wYear, sys.wMonth, sys.wDay);
	strcat_s(buffer, ".log");
	strcat_s(path, "\\");
	strcat_s(path, buffer);

	
	if (mFSream.is_open())
		mFSream.close();

	mFSream.open(path, std::ios::out | std::ios::app);
	memset(mBuffer, 0, BUFFERSIZE);
	g_logLock.UnLock();
}

char* CSystemLog::getSysTimeBuffer()
{
	static char buffer[64] = {0};
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	sprintf_s(buffer, "%4d-%02d-%02d %02d:%02d:%02d",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	return buffer;
};

void CSystemLog::LogToFile(char* format, ...)
{
	try
	{
		g_logLock.Lock();
		va_list va;
		va_start(va, format);
		vsprintf_s(mBuffer, BUFFERSIZE, format, va);

		std::string str;
		if (IsTextUTF8(mBuffer, sizeof(mBuffer)))
		{
			char* p = U2G(mBuffer);
			if (nullptr != p)
			{
				str = p;
				delete p;
			}
		}
		else
		{
			str = mBuffer;
		}
		
		std::string sss;
		sss = "[";
		sss += getSysTimeBuffer();
		sss += "][1]" + str + "\r\n";

		encrypt(sss);
		mFSream.write(sss.c_str(), sss.length());
		mFSream.flush();

		OutputDebugStringA(str.c_str());
		g_logLock.UnLock();
	}
	catch (...)
	{

	}
};

void CSystemLog::LogToFile(const char* buf)
{
	try
	{
		g_logLock.Lock();
		std::string str;
		if (IsTextUTF8(buf, strlen(buf)))
		{
			char* p = U2G(buf);
			if (nullptr != p)
			{
				str = p;
				delete p;
			}
		}
		else
		{
			str = buf;
		}

		std::string sss;
		sss = "[";
		sss += getSysTimeBuffer();
		sss += "][2]" + str + "\r\n";

		encrypt(sss);

		mFSream.write(sss.c_str(), sss.length());
		mFSream.flush();

		OutputDebugStringA(str.c_str());
		g_logLock.UnLock();
	}
	catch (...)
	{

	}
}

void CSystemLog::LogToFile(std::string str)
{
	try
	{
		g_logLock.Lock();
		std::string str2;
		if (IsTextUTF8(str.c_str(), str.length()))
		{
			char* p = U2G(str.c_str());
			if (nullptr != p)
			{
				str2 = p;
				delete p;
			}
		}

		std::string sss;
		sss = "[";
		sss += getSysTimeBuffer();
		sss += "][3]" + str2 + "\r\n";

		encrypt(sss);
		mFSream.write(sss.c_str(), sss.length());
		mFSream.flush();

		OutputDebugStringA(str2.c_str());
		g_logLock.UnLock();
	}
	catch (...)
	{

	}
}

void CSystemLog::LogToFile(QString str)
{
	std::string str2 = str.toStdString();
	LogToFile(str2);
}

void CSystemLog::LogToDebug(char* lpPre, char* format, ...)
{
	try
	{
		g_logLock.Lock();
		int ilen = strlen(format) + 1;
		//char* strMsg = new char[ilen];
		char strMsg[1024] = { 0 };
		std::memset(strMsg, 0, sizeof(strMsg));
		va_list pArg;
		va_start(pArg, format);

		//vsprintf_s(strMsg, ilen, lpFormat, pArg);
		vsprintf_s(strMsg, sizeof(strMsg), format, pArg);
		va_end(pArg);

		std::string str;
		str = lpPre;
		str += "------";
		str += strMsg;
		str += "\n";
		OutputDebugStringA(str.c_str());
		printf("%s", str.c_str());
		//delete[] strMsg;
		g_logLock.UnLock();
	}
	catch (...)
	{
		//
	}
}