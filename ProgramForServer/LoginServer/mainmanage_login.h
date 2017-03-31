
#ifndef _WJ_mainmanage_login_H_
#define _WJ_mainmanage_login_H_
#include "TeamManage.h"
#include "server/WJServerManagelib.h"
#include "server/WJSSocketManage.h"
#include "LUserInfoManage.h"
#include "Gamelistmanage.h"
#include "ApplyManage.h"
#include "redisDataListManage.h"
#include <map>
#include <vector>
#include<mutex>
#include "server/WJTimerMgr.h"
namespace WJ
{
	class CMainManage_login : public  CWJSSocketManage, public CallBack_Time
	{
	
	public:
		CMainManage_login(ManageInfoStruct *pManageInfoStruct);
		virtual ~CMainManage_login();


		CLUserInfoManage* GetCLUserInfoManage();

		///��ʼ������
		virtual bool Init_subclass(const ManageInfoStruct &InitData);
		///ȡ����ʼ������
		virtual bool UnInit_subclass();
		///��������
		virtual bool Start_subclass();
		///ֹͣ����
		virtual bool Stop_subclass();

		//��ʱ���¼�����(�ú���ִ�����ϲ�ҵ�����߳���)
		int ProccessTimers();
		// ���ݿ��ѯ�������
		int ProccessRedisResults();

		//UINT uElapse ��λ��
		void SetTimer(TimerID uTimerID, UINT uElapse);

		void KillTimer(TimerID uTimerID);

	private:
		// ������Ϣ�߼�����
		virtual bool ProccessNetMessage(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//���¿ͻ��˼���
		virtual void OnNewConnection(CWJSSocket * pSocket, void *pUserData);
		///SOCKET �ر� ���������أ�
		virtual void OnEndConnection(CWJSSocket * pSocket);
		//[�ͻ��˽�ɫ]���ӶԷ�ʧ��
		virtual void OnConnectFailed(int type, void *pUserData)  {  }
		///���ݿ⴦���� ���������أ�
		virtual bool OnDataBaseResult(DataBaseResultLine * pResultData, UINT tag = 0);
		///��ʱ����Ϣ ���������أ�
		virtual bool OnTimerMessage(TimerID timerId);


		//�������ͺ���
		void SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode);

		//��½��Ϣ����
		bool OnSocketRead_hs_mess_login(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//��Ϸ�б���Ϣ����
		bool OnSocketRead_hs_mess_gamelist(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//������Ϣ����
		bool OnSocketRead_hs_mess_apply(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//�Ŷ���Ϣ����
		bool OnSocketRead_hs_mess_team(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		/////PtoRͨ����Ϣ//////////
		//������Ϣ����
		bool OnSocketRead_pr_mess_roominfo(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//�û���Ϣ����
		bool OnSocketRead_pr_mess_userinfo(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);

		//���ָ�������µ�roomlis�������͸��ͻ���
		void onSendRoomListForContest(CWJSSocket * pSocket, UINT contest, UINT bMainID, UINT bAssistantID, UINT bHandleCode_part, UINT bHandleCode_finish);

		//���͸��Ŷ��������Ϣ
		//UINT myuserid �ų��Լ�����
		void TeamSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);
		//���͸��Ŷ�VS�������Ϣ
		//UINT myuserid �ų��Լ�����
		void TeamVSSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);


		//redis������ �û���¼����
		void onRedisRes_Order_h_messid_load_hall(RedisResultLine * pResultData);
		//redis������ ����
		void onRedisRes_Order_h_messid_apply(RedisResultLine * pResultData);
		//redis������ �˷�
		void onRedisRes_Order_h_messid_apply_restitution(RedisResultLine * pResultData);
		//redis������ �����Ŷ�
		void onRedisRes_Order_h_messid_team_create(RedisResultLine * pResultData);
		//redis������ �����Ŷ�
		void onRedisRes_Order_h_messid_team_join(RedisResultLine * pResultData);
		//redis������ �˳��Ŷ�
		void onRedisRes_Order_h_messid_team_leave(RedisResultLine * pResultData);
		//redis������ ��ɢ�Ŷ�
		void onRedisRes_Order_h_messid_team_dissolve(RedisResultLine * pResultData);
		//redis������ �����û�����(��ҡ�����)
		void onRedisRes_Order_h_messid_update_userinfo(RedisResultLine * pResultData);
		//����
		void Test();
	private:
		CWJRedis * m_pRedis;

		//CLGsocketserver m_CLGsocketserver;//�����Ϸ��������ͨ����
		CLUserInfoManage m_CLUserInfoManage;//�û��б�
		CGameList		m_CGameList;//��Ϸ�б�
		CApplayManage	m_CApplayManage;//��������
		CTeamManage m_CTeamManage;//�ŶӴ�������

		ManageInfoStruct m_ManageInfoStruct;//��������(�����Ϸ��������ͨ����)
		std::vector<int> m_socket_vector;//��ǰ������������������Ϣ��GS��Ŀ

		CWJTimerMgr   m_timerMgr_h;//��ʱ��

		UINT m_uCheckTime;//�����߼������λ��s��

		UINT m_update_now_playercount;//��ǰ��������
	};
}

#endif //_WJ_mainmanage_login_H_
