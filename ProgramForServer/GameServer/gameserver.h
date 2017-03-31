#pragma  once

#include <mutex>
#include "common/WJBaseType.h"
#include "server/comstruct.h"
#include "server/WJSSocketManage.h"
#include "redis/WJRedis.h"
#include "RoomManage.h"
#include "server/WJTimerMgr.h"
#include "server/comtimetag.h"

enum E_DB_BUSID //���ݿ�
{
	E_DB_BUSID_query_roomlist = 1
};

// ��������
enum E_CONN_TYPE
{
	E_CONN_LoginServer = 1,//���ӵ�¼������
	E_CONN_Client = 2//�����û�
};



namespace WJ
{
	struct ContestInfo_locad
	{
		char cContestName[CONTESTNAME_LENGTH];//������
		ENUM_ContestRule uContestRule = (ENUM_ContestRule)0;//����ģʽ
		ENUM_ContestKind uContestKind = (ENUM_ContestKind)0;//������������
		LLONG _i64cost = 0;//��������
		ENUM_AwardType awardType = (ENUM_AwardType)0;//��������
		UINT award = 0;//����

		ContestInfo_locad()
		{
			std::memset(cContestName, 0, sizeof(cContestName));
		}
	};

	class CGameServer : public CWJSSocketManage, public CallBack_Time
	{
	public:
		CGameServer(unsigned int maxSocketNum);
		~CGameServer();
		void StartServer();
		void StopServer();

		static CWJRedis * GetRedis() { return m_pRedis; }
        //��ʱ���¼�����(�ú���ִ�����ϲ�ҵ�����߳���)
        int ProccessTimers();
		// ���ݿ��ѯ�������(�ú���ִ�����ϲ�ҵ�����߳���)
		int ProccessRedisResults();
		// ������Ϣ�߼�����
		virtual bool ProccessNetMessage(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		virtual void OnNewConnection(CWJSSocket * pSocket, void *pUserData);
		virtual void OnEndConnection(CWJSSocket * pSocket);
		virtual void OnConnectFailed(int type, void *pUserData);

		virtual bool OnTimerMessage(TimerID timerId);

		//������ҿͻ�����Ϣ
		bool ProccessNetMessage_player(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//����ls��������Ϣ
		bool ProccessNetMessage_ls(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);

        void OnReconnectLoginServerTimer();
		void OnKeepaliveLoginServerTimer();

		//UINT uElapse ��λ��
		void SetTimer(TimerID uTimerID, UINT uElapse);

		void KillTimer(TimerID uTimerID);

		UINT GetSockidOfLoginServer() { return m_sockidOfLoginServer; }
		CGameUserInfoManage* GetCGameUserInfoManage();
		CQueueManage* GetCQueueManage(UINT contestid);
		std::unordered_map<UINT, R_TeamStruct_VS * >* GetR_TeamStruct_VS();

		//���͸��Ŷ�VS�������Ϣ
		//UINT myuserid �ų��Լ�����
		void RTeamVSSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode);

		//��ӱ���
		void AddContInfo_locad(UINT contestid, ContestInfo_locad &data);
		//��ñ�������
		ContestInfo_locad* GetContInfo_locad(UINT contestid);
	private:
		/////PtoRͨ����Ϣ////////// ����loginserver
		//������Ϣ����
		bool OnSocketRead_pr_mess_roominfo(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//�û���Ϣ����///// ����loginserver
		bool OnSocketRead_pr_mess_userinfo(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);

	public:
		static  CWJRedis *m_pRedis;
		CRoomManage   m_roomMgr;//���������

	private:
		std::string   m_redisIp;
		std::string   m_redisPass;
		int           m_nRedisPort;
		int           m_nListenPort;
        CWJTimerMgr   m_timerMgr;//��ʱ��
		
		UINT          m_sockidOfLoginServer;//���¼���������ӵ��׽���id
		CGameUserInfoManage m_UserInfoManage;	//�û�����

		std::unordered_map<UINT, R_TeamStruct_VS * >  m_teamid2team_vs; // �����Ŷ�id��ֵ���Ŷӱ����ṹ��ָ��. �����ڴ��ڵ��Ŷӱ���
		std::unordered_map<UINT, ContestInfo_locad> m_contestid2info;  //����id�����ı������ݶ�Ӧ��ϵ
		std::unordered_map<UINT, CQueueManage>	m_contestid2Queue;//���Ǳ���id��ֵ�Ƕ�Ӧ���Ŷӻ�
		UINT m_uCheckTime;//�����߼������λ��s��
	};

}

