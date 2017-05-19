#ifndef _WJ_commonfunction_H_
#define _WJ_commonfunction_H_

#include <string>
#include <xstring>
#include "common/WJBaseType.h"
int IsTextUTF8(const char* str, ULONGLONG length);

//Utf8ToGb2312
std::string U2G(const char* utf8);

//GB2312��UTF-8��ת��
std::string G2U(const char* gb2312);

//��ӡ��־debugview
void MyDebugString(char* lpPre, char* lpFormat, ...);

//��ȡ��ǰ·��
std::string GetAppPath();

//wstring���ֽڲ�Ϊ0������FALSE
bool WStringToString(const std::wstring &wstr, std::string &str);
bool StringToWString(const std::string &str, std::wstring &wstr);
#endif//_WJ_commonfunction_H_