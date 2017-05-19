// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LOLEXTEND_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LOLEXTEND_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef LOLEXTEND_EXPORTS
#define LOLEXTEND_API __declspec(dllexport)
#else
#define LOLEXTEND_API __declspec(dllimport)
#endif


#include <string>
#include "hook/GameInfo.h"
#include <windows.h>
#include <map>
#include <process.h>
class LOLEXTEND_API CLOLExtend
{
public:
	CLOLExtend(void);
	// TODO:  �ڴ�������ķ�����

	/**
	* @date: 2016/5/13
	* @author: Steven
	* @func:   getUserInfo		���ڴ��в���ָ�����û�����Ϣ
	* @version: V1.0.0.1
	* @param: std::string& imageFile	�ļ�����

	* @return: void
	**/
	GameInfoLOL& getUserInfo();
	void ExitGame();

public:
	void IsLOLExist();//���LOL�Ƿ������˳�
	void CloseLol();//�رտ�����lol
	DWORD GetGameKey(DWORD dwCall, int inOffset);//��Ϸ���
	HWND m_hWindow;	///���ھ��
	
};

extern LOLEXTEND_API int nLOLExtend;

LOLEXTEND_API int fnLOLExtend(void);
