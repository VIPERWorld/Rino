
#define WM_HALL_MESS		WM_USER+1000	//������Ϣ
#define WM_GAME_MESS		WM_USER+2000	//������Ϣ

////////////////////////////////////////////////////////////////////////////////////////
//�����ڴ�����Ϣ
////////////////////////////////////////////////////////////////////////////////////////
#define WM_HALL_MESS_1001	WM_HALL_MESS+1						//������ICO ��Ӧ��Ϣ

#define WM_HALL_MESS_1002	WM_HALL_MESS+2						//�������������ӳ�ʱ
#define WM_HALL_MESS_1003	WM_HALL_MESS+3						//��������������ʧ�ܣ������������Ͽ�
#define WM_HALL_MESS_1004	WM_HALL_MESS+4						//�����������ӳɹ�
#define WM_HALL_MESS_1005	WM_HALL_MESS+5						//��Ϸ��Ϣ�������
#define WM_HALL_MESS_1006	WM_HALL_MESS+6						//����ʧ��_δ���뷿��
#define WM_HALL_MESS_1007	WM_HALL_MESS+7						//����ʧ��_�Ѽ��뷿��
#define WM_HALL_MESS_1008	WM_HALL_MESS+8						//��Ϸ��������(������)
#define WM_HALL_MESS_1010	WM_HALL_MESS+10						//lol solo��ƥ��ҳ��
//������ҳ��������
#define WM_HALL_MESS_1009	WM_HALL_MESS+9						//CrossProxy.exe������
#define WM_HALL_MESS_1012	WM_HALL_MESS+12						//�������״̬����ʧ��
#define WM_HALL_MESS_1013	WM_HALL_MESS+13						//�ڶ����ͻ���֪ͨ��������ʾ
#define WM_HALL_MESS_1014	WM_HALL_MESS+14						//�յ�����LOL���̹ر���Ϣ��ת��Socket3013��Ϣ��H5
#define WM_HALL_MESS_1015	WM_HALL_MESS+15					    //LOL ���������ҳ�ɹ�����

////////////////////////////////////////////////////////////////////////////////////////
//�������Ϸ�ڴ�����Ϣ
////////////////////////////////////////////////////////////////////////////////////////
#define WM_GAME_MESS_2001	WM_GAME_MESS+1						//¯ʯע������Ϣ
#define WM_GAME_MESS_2002	WM_GAME_MESS+2						//¯ʯ�Ӻ�������
#define WM_GAME_MESS_2003	WM_GAME_MESS+3						//¯ʯ�Ӻ��������Ӧ
#define WM_GAME_MESS_2004	WM_GAME_MESS+4						//¯ʯ������ս����
#define WM_GAME_MESS_2005	WM_GAME_MESS+5						//¯ʯ������ս��Ӧ
#define WM_GAME_MESS_2006	WM_GAME_MESS+6						//¯ʯ������Ϸ������

#define WM_GAME_MESS_2007	WM_GAME_MESS+7						//��Ϸ���������ӳ�ʱ
#define WM_GAME_MESS_2008	WM_GAME_MESS+8						//��Ϸ����������ʧ��
#define WM_GAME_MESS_2009	WM_GAME_MESS+9						//��Ϸ��������½�ɹ�
#define WM_GAME_MESS_2010	WM_GAME_MESS+10						//��Ϸ�����������ػ�
#define WM_GAME_MESS_2011	WM_GAME_MESS+11						//solo����Ϸ������ƥ��ɹ�
#define WM_GAME_MESS_2012	WM_GAME_MESS+12						//��Ϸ��������Ϸ��ʽ��ʼ
#define WM_GAME_MESS_2013	WM_GAME_MESS+13						//��Ϸ��������Ҿ���ͬ����Ϣ
#define WM_GAME_MESS_2014	WM_GAME_MESS+14						//��Ϸ��������Ϸ������Ϣ����

#define WM_GAME_MESS_2015	WM_GAME_MESS+15						//¯ʯ�Զ����ֹ���Ԥ

#define WM_GAME_MESS_2016	WM_GAME_MESS+16						//¯ʯ��ʾ��������Ƕ����ҳ��
#define WM_GAME_MESS_2017	WM_GAME_MESS+17						//��Ϸ�������쳣������Ҫ������ȷ�����ݣ���֤�����ٴε��һ������(��ʼ��������������)

#define WM_GAME_MESS_2018	WM_GAME_MESS+18						//¯ʯ�Զ����ֹ���Ԥ������ɣ�ɱ���ղŵļ�ʱ������ֹ�жϳ�ʱ

#define WM_GAME_MESS_2019	WM_GAME_MESS+19						//¯ʯ��Ƕ���ڵ�������չ��Ч����ָ��
#define WM_GAME_MESS_2020	WM_GAME_MESS+20						//¯ʯ��Ƕ���ڴ�������ʾ

#define WM_GAME_MESS_2021	WM_GAME_MESS+21						//�յ������б���Ϣ

#define WM_GAME_MESS_2022	WM_GAME_MESS+22						//����ʧ������

#define WM_GAME_MESS_2023	WM_GAME_MESS+23						//LOL�˳���Ϸ��Ϣ

#define WM_GAME_MESS_2024	WM_GAME_MESS+24						//LOL SOLO����Ѻע���ҳ




/////////////////////////////////////////////////////////////////////////////////////////
//������Ϣ���ͺ�����װ
#include <windows.h>
//#include <WinUser.h>
#include <qwidget.h>
#define WJ_SAFE_QSendMessage(_p, _messid, _wParam, _lParam) \
	if (nullptr != _p)\
		::SendMessage((HWND)((QWidget*)_p->winId()), _messid, _wParam, _lParam);

#define WJ_SAFE_QPostMessage(_p, _messid, _wParam, _lParam) \
	if (nullptr != _p)\
	::PostMessage((HWND)((QWidget*)_p->winId()), _messid, _wParam, _lParam);

