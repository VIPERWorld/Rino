#ifndef __WJ_RhinoLauncher_H__
#define __WJ_RhinoLauncher_H__
#include <string>
#include <windows.h>
DWORD FindProcess(char *strProcessName);
void KillProcess(char *strProcessName);

class CRhinoLauncher
{
public:
	CRhinoLauncher();
	~CRhinoLauncher();

	//���汾�Ƿ���Ը���
	bool VersionUpdate();
	//����update.exe
	/*
	std::string version ���汾
	std::string smallversion �Ӱ汾
	std::string _filename ���ص��ļ�
	std::string strDownLoadUpdatepADDR ���صĸ���ַ
	std::string startgamename �����ĳ�������
	*/
	bool updateOnline(std::string version, std::string smallversion, std::string _filename, std::string strDownLoadUpdatepADDR, std::string startgamename);
	//��ȡhttp����
	void GetHttpData(std::wstring str_web_addr, int iport, std::wstring str_web_content, std::string &str_res);
private:

};

#endif//__WJ_RhinoLauncher_H__