// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// OnlineInstall.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������


char char_info[] = {"\
��һ˲�伴����\n\
\n\
һ˲������ָо�������ĸо�����һ˲��ӿ�ֵ���������ǧ���ֵ���С���һ˲���Ǿ��ʵģ������õģ�Ҳ������ġ�\n\
\n\
�������\n\
\n\
���޽��ϣ���һҶ���ۣ�����ʶ��ͣ�����������������������š����������Ҷ��壬���˽����Ҷ��ѡ��ĸ�ҥ�����ϵĽ�����������׵����ޣ������ڽ��ߣ�����ã����һ˿���ݵĹ��������š���ɧ�����ϵĴ�ֻ��ҡ�����Ÿ衷�͡����¡���˫�����������ʡ����������������޽�ˮ�ߣ�����˭�����������н�֯��������Ŀ�ʹ�أ�����פ���ڽ��ߣ��������������������֮�䣬�����˵���ѡ���ڼ����ķ����У��ڱ���Զ���ڽ��ߵ���һ˲�䣬�ѻ������㡣\n\
\n\
��·��������Զ�⣬�Ὣ���¶���������������ԭ���Լ���������ڹ�͡���Ϊһ������֮�ˣ������Լ����ȳ��׸����Լ��Ȱ����������Ϊһ������֮����������������α���֮����Լ��������׸���������Ϊһ�����ˣ����ѡ����������δ�ڡ���Ϊ�Լ���׼�򡣡����Ͷ�Ԯ���⣬ѭ��ī�����ˡ�������׷�󡣡��̶������ᡱ������̬�ȡ�����Ī����Ϊ�����⡣�Ὣ������֮���ӡ��������������������Լ�������ڹ�����Լ������һ˲�䡣\n\
\n\
��һ˲�伴���㡣\n\
\n\
	�����ƾɵľ����д���һ����������ɡ�����һ���ԼᶨΪ����������Ϊ�׵�����֮�衣����ң��ϵ�Ϊ�������˴���ҲΪ���������ţ�����ȴ���Լ������������һ�������ڸ��ٱ߿���������ס���˵��ʺ�һ���˳е����޾��Ĵ����ѹ������������������ǣ�����֣������������趯�������ļ�ֵ���ڸ��ټ��ϴ�������Եĸ߽ࡣ����ÿһ�����Ӷ��������ǿ�߶����˷�������ս�飬�����������־�����ǰ���˽Զ���Ĥ�ݡ�����һ�����ھŽ������������������һ˲�䣬�ѻ������㡣\n\
\n\
	����Ҫ��ס���˵��ʺ�������ʹ������������԰첻���������Ǳ����һ������������������һ������ʵ��д�ա������ָ��˿��֣��Ȱ����ֵ���һ���Ȱ��������һ���Ȱ����ֵ��ˣ�����һ���Ȱ�������ˡ�\n\
\n\
	��һ˲�伴���㡣\n\
\n\
	ʱ�ӿ��Ե�ת����ʱ�����ô�����������ؿ������˻����ֻ�ô������Ķ���������ʱ����ݣ��������������һɲ��������ע����ô��������ô��ƽ�������⡰һ˲�䡱������ڲ��������������ʷ�����ĩ�ˣ�����ʷ���ǿջ���Ҳ����Ϊ��������һ˲�������������ͺ��أ�\n\
\n\
	��ʵ����һ˲�伴���㡣\
"};

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

//std::string ת UTF - 8��
char *  string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	return pBuf;
}
//std::wstring ת UTF - 8��
char * wstring_To_UTF8(const std::wstring & str)
{
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), nLen, pBuf, nLen, NULL, NULL);
	return pBuf;
}

//UTF - 8 ת std::string��
char* UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	return pBuf;

}

 std::string GetAppPath()
{
	//char lpBuffer[MAX_PATH];
	//ZeroMemory(lpBuffer, sizeof(lpBuffer));
	//GetCurrentDirectoryA(MAX_PATH, lpBuffer);

	char szFilePath[MAX_PATH + 1];
	GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //ɾ���ļ�����ֻ���·��
	std::string ss = szFilePath;

	//std::string ss = lpBuffer;
	//ss += "\\";
	return ss;
}