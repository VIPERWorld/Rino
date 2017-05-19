#ifndef __cmdWind_H__
#define __cmdWind_H__

#include <vector>
#include "WJPlatformLogic.h"
#include <windows.h>

class CcmdWind
{
public:
	CcmdWind();
	~CcmdWind();

	//����
	void clearWind();
	//����ҳ��
	void ShowSet();
	//��ҳ
	void ShowMainWind();
	//ȥ��ʼ��ҳ��

	//����ҳ��
	void ShowRun();

	//ͨ��ȫ���������
	bool ShowCommond(char *str);

	HWND gethWindow();
	void sethWindow(HWND hWindow);
	//��ʱ��
	void OnTimeMesage(int itimeid);
	//�ڲ�����
private:


	//ִ�����Ӷ���
	void onsocketconnect();

	//ִ�йرն���
	void onsocketClose();

	//��������߼�����
	void clear_socketlogic();

	///WINDOW ��Ϣѭ���߳�
	void WindowMsgThread();

	///�趨��ʱ��
	bool SetTimer(UINT uTimerID, UINT uElapse);
	///�����ʱ��
	bool KillTimer(UINT uTimerID);

	//���׽����߼��߳�
	int MakeSocketThread();

private:
	std::vector<WJ::WJPlatformLogic*> m_vector_logic_struct;
	std::mutex m_dataMutex_test_client;//����ͬ�����������������̶߳Ի���������
	HWND m_hWindow;//���ھ��
	bool m_bRunning;//��������
};

#endif//__cmdWind_H__