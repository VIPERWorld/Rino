// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

/*Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "UIlib.h"
using namespace DuiLib;

using namespace std;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

char char_info[];
int IsTextUTF8(const char* str, ULONGLONG length);
//Utf8ToGb2312
char* U2G(const char* utf8);
//GB2312��UTF-8��ת��
char* G2U(const char* gb2312);

//std::string ת UTF - 8��
char *  string_To_UTF8(const std::string & str);
//std::wstring ת UTF - 8��
char * wstring_To_UTF8(const std::wstring & str);
//UTF - 8 ת std::string��
char* UTF8_To_string(const std::string & str);

//��ȡ��·��
std::string GetAppPath();
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
