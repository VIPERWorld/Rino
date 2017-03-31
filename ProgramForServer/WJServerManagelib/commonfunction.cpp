#include "common/commonfunction.h"
#include <windows.h>

int IsTextUTF8(const char* str, ULONGLONG length)
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
char* U2G(const char* utf8)
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
char* G2U(const char* gb2312)
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

void MyDebugString(char* lpPre, char* lpFormat, ...)
{
	try
	{
		int ilen = (int)strlen(lpFormat) + 1;
		//char* strMsg = new char[ilen];
		char strMsg[1024] = {0};
		std::memset(strMsg, 0, sizeof(strMsg));
		va_list pArg;
		va_start(pArg, lpFormat);

		//vsprintf_s(strMsg, ilen, lpFormat, pArg);
		vsprintf_s(strMsg, sizeof(strMsg), lpFormat, pArg);
		va_end(pArg);

		std::string str;
		str = lpPre;
		str += "------";
		str += strMsg;
		str += "\n";
		OutputDebugStringA(str.c_str());

		//delete[] strMsg;
	}
	catch (...)
	{
		//
	}
}

std::string GetAppPath()
{
	char lpBuffer[MAX_PATH * 2] = {0};
	ZeroMemory(lpBuffer, sizeof(lpBuffer));
	GetCurrentDirectoryA(MAX_PATH, lpBuffer);
	std::string ss = lpBuffer;
	ss += "\\";
	return ss;
}