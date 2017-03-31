#include "GameRoom.h"
#include "server/comtimetag.h"
#include "message/WJNetRoomMessage.h"
#include "common/WJUserInfoBaseStruct.h"
#include "message/WJNetPtoR.h"
#include "common/commonfunction.h"
#include "common/QkLogger.h"
#include "GameServer.h"
#include "gsconfig.h"
#include "server/WJSSocketManage.h"

using namespace std;

namespace WJ
{
	extern CGameServer * g_pGameServer;

	CGameRoom::CGameRoom()
	{
	}

	CGameRoom::~CGameRoom()
	{
	}

	const Room_ManageInfoStruct* CGameRoom::GetRoom_ManageInfoStruct()
	{
		return &m_initRoomData;
	}

	CGameDeskManage* CGameRoom::GetCGameDeskManage()
	{
		return &m_GameDeskManage;
	}

	void CGameRoom::Init_Room(Room_ManageInfoStruct &initRoomData)
	{
		m_initRoomData = initRoomData;
	}

	///��ʼ������
	bool CGameRoom::Init_subclass()
	{
		m_GameDeskManage.InitGameDesk(m_initRoomData.uDeskCount, m_initRoomData.uDeskPeople, &m_initRoomData, this);

		return true;
	}

	///ȡ����ʼ������
	bool CGameRoom::UnInit_subclass()
	{
		return true;
	}

	///��������
	bool CGameRoom::Start_subclass()
	{
		return true;
	}

	///ֹͣ����
	bool CGameRoom::Stop_subclass()
	{
		return true;
	}

	//���·�����Ϣ����½������
	void CGameRoom::UpdateInfoToLS()
	{
		struct_data_pr_roominfo_update cmd;
		cmd.roomid = m_initRoomData.uRoomID;
		cmd.gameid = m_initRoomData.uGameID;
		cmd.contestid = m_initRoomData.uContestid;
		cmd.uOlineCount = g_pGameServer->GetCGameUserInfoManage()->GetGameRoomCount(m_initRoomData.uRoomID);
		int copyLen = min(sizeof(cmd.strIP), gsconfig()->ip_for_client.size());
		memcpy(cmd.strIP, gsconfig()->ip_for_client.c_str(), copyLen);
		cmd.strIP[copyLen] = '\0';
		cmd.uport = gsconfig()->port_for_client;
		std::memcpy(cmd.roomname, m_initRoomData.cRoomName, sizeof(cmd.roomname));

		g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(),
								&cmd, 
								sizeof(cmd), 
								e_pr_mess_roominfo, 
								e_pr_ass_roominfo_update, 
								0);
	}

	///SOCKET �ر� ���������أ�
	bool CGameRoom::OnSocketClose(CWJSSocket *pSocket)
	{
		QK_InfoLog("sdp_test room ��Ϸ������ OnSocketClose ");
		if (pSocket->GetType() == E_CONN_Client)
		{
			CGameUserInfo *puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfoEx(pSocket->GetSockID());
			if (nullptr != puser)
			{//������������Ҳ��ܽ��д���				
				//���Ŷӻ���ɾ��
				if (US_ContestRule_solo == m_initRoomData.uContestRule)
				{
					g_pGameServer->GetCQueueManage(m_initRoomData.uContestid)->Delete(puser);
				}

				UINT deskid = puser->GetUserData()->uDeskid;
				UINT uDeskStation = puser->GetUserData()->uDeskStation;
				if (ERROR_DESKID != deskid)
				{
					ISysDeskInterface* pdesk = m_GameDeskManage.At(deskid);
					if (nullptr != pdesk)
					{
						if (pdesk->IsPlayGame())
						{//����Ϸ�У���ע������û�
							CGameUserInfo* pNetCutUser = g_pGameServer->GetCGameUserInfoManage()->RegNetCutUser(pSocket->GetSockID());
							if (nullptr != pNetCutUser)
							{//֪ͨ��Ϸ�ڲ���Ҷ���
								QK_InfoLog("sdp_tes room ֪ͨ��Ϸ�ڲ���Ҷ��ߡ�");
								pdesk->Sys_UserNetCut(uDeskStation, pNetCutUser);
								return true;
							}
						}
						else
						{//��Ϸδ��ʼ������ֱ���뿪����
							QK_InfoLog("sdp_test room ��Ϸδ��ʼ������ֱ���뿪���ӡ�");
							pdesk->Sys_UserLeftDesk(uDeskStation, puser);
						}
					}
				}

				if (US_ContestRule_solo == m_initRoomData.uContestRule)
				{
					//֪ͨ��½�����������û���ڸ÷����Ŷ���Ϸ
					struct_data_pr_userinfo_unplay cmd;
					cmd.userid = puser->GetUserData()->basedata.dwUserID;
					QK_InfoLog("sdp_test room ֪ͨ��½�����������û���ڸ÷����Ŷ���Ϸ��");
					g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &cmd, sizeof(struct_data_pr_userinfo_unplay), e_pr_mess_userinfo, e_pr_ass_userinfo_unplay, 0);
				}
				else if (US_ContestRule_team == m_initRoomData.uContestRule)
				{//�Ŷ�����Ҫ����vs�е��������
					g_pGameServer->RTeamVSSendData(puser->GetUserData()->basedata.dwUserID, nullptr, 0, e_gs_mess_user_action, e_gs_ass_user_action_team_out, puser->GetUserData()->basedata.dwUserID);
				}

				//��ȫ�����������
				g_pGameServer->GetCGameUserInfoManage()->FreeUser(pSocket->GetSockID());

			}
			else
			{
				QK_InfoLog("sdp_err CGameRoom::OnSocketClose uSocketId=%d", pSocket->GetSockID());
			}
		}
		else if (pSocket->GetType() == E_CONN_LoginServer)
		{
			//������
		}

		return true;
	}


	///��ʱ����Ϣ ���������أ�
	bool CGameRoom::OnTimerMessage(UINT uTimerID)
	{
		UINT deskId = (uTimerID & TIMER_DESKID_MASK) >> 8;
		if(deskId < m_GameDeskManage.GetCount())
		{
			m_GameDeskManage.At(deskId)->Sys_OnTimer(uTimerID);
		}
		else
		{
			return false;
		}

		return true;
	}

	//�û��б���Ϣ
	bool CGameRoom::OnSocketRead(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bMainID)
		{
		case e_gs_mess_user_list:
		{//�û��б���Ϣ
									return OnSocketRead_user_list(pSocket, pNetHead, pData, uSize);
		}break;
		case e_gs_mess_user_action:
		{//�û�������Ϣ
									  return OnSocketRead_user_action(pSocket, pNetHead, pData, uSize);
		}break;
		case e_gs_mess_room:
		{//������Ϣ
							   return OnSocketRead_room(pSocket, pNetHead, pData, uSize);	   
		}break;
		case e_gs_mess_queue:
		{//�Ŷ���Ϣ
								return OnSocketRead_queue(pSocket, pNetHead, pData, uSize);
		}break;
		case e_gs_mess_game_frame:
		{//��Ϸ�����Ϣ
									 return OnSocketRead_game_frame(pSocket, pNetHead, pData, uSize);
		}break;
		case e_gs_mess_game_notify:
		{//��Ϸ��ͨ��Ϣ
									  return OnSocketRead_game_notify(pSocket, pNetHead, pData, uSize);
		}break;
		default:
			break;
		}

		return false;
	}

	//�û��б���Ϣ
	bool CGameRoom::OnSocketRead_user_list(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bAssistantID)
		{
		default:
			break;
		}

		return true;
	}

	//�û�������Ϣ
	bool CGameRoom::OnSocketRead_user_action(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		//switch (pNetHead->bAssistantID)
		//{
		//default:
		//	break;
		//}

		return true;
	}

	//������Ϣ
	bool CGameRoom::OnSocketRead_room(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bAssistantID)
		{
		case e_gs_ass_room_chat:
		{
			RoomMessDate_room_chat* pCmd = (RoomMessDate_room_chat*)pData;
			if (nullptr == pCmd)
			{
				return false;
			}

			CGameUserInfo *puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfoEx(pSocket->GetSockID());
			if (nullptr == puser)
				return true;

			if (e_gs_hcid_gs_room_chat_for_room == pNetHead->bHandleCode)
			{//�㲥��������, �㲥���������ٷ�
                SendDataBatch(pData, uSize, pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
				return true;
			}
			else if (e_gs_hcid_gs_room_chat_for_desk == pNetHead->bHandleCode)
			{//�鲥������
			UINT deskid = puser->GetUserData()->uDeskid;
				if (ERROR_DESKID != deskid)
				{
					ISysDeskInterface* pdesk = m_GameDeskManage.At(deskid);
					if (nullptr != pdesk)
					{
						pdesk->SendDeskData(pData, uSize, pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
						return true;
					}
				}
			}
			else if (e_gs_hcid_gs_room_chat_for_player == pNetHead->bHandleCode)
			{//����������									   
				CGameUserInfo* to_puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo(pCmd->to_username);
				if (nullptr != to_puser)
				{
                    g_pGameServer->SendData(to_puser->GetSocketId(), pData, uSize, pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
					return true;
				}
			}

			return true;
		}break;
		default:
			break;
		}

		return true;
	}

	//�Ŷ���Ϣ
	bool CGameRoom::OnSocketRead_queue(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bAssistantID)
		{
		default:
			break;
		}

		return true;
	}

	//��Ϸ�����Ϣ
	bool CGameRoom::OnSocketRead_game_frame(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		QK_InfoLog("sdp_test room ��Ϸ�����Ϣ");
		CGameUserInfo *puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfoEx(pSocket->GetSockID());
		if (nullptr == puser)
			return true;

		if (ERROR_DESKID == puser->GetUserData()->uDeskid || ERROR_DESKSTATION == puser->GetUserData()->uDeskStation)
		{//û������������
			QK_InfoLog("sdp_test room û������������(%d,%d)��", puser->GetUserData()->uDeskid, puser->GetUserData()->uDeskStation);
			return false;
		}

		ISysDeskInterface* pdesk = m_GameDeskManage.At(puser->GetUserData()->uDeskid);
		if (nullptr == pdesk)
		{//�����쳣
			QK_InfoLog("sdp_test room �����쳣��");
			return false;
		}

		return pdesk->Sys_HandleFrameMessage(puser->GetUserData()->uDeskStation, pNetHead, pData, uSize, pSocket->GetSockID());
	}

	//��Ϸ��ͨ��Ϣ
	bool CGameRoom::OnSocketRead_game_notify(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		CGameUserInfo *puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfoEx(pSocket->GetSockID());
		if (nullptr == puser)
			return true;

		if (ERROR_DESKID == puser->GetUserData()->uDeskid || ERROR_DESKSTATION == puser->GetUserData()->uDeskStation)
		{//û������������
			return false;
		}

		ISysDeskInterface* pdesk = m_GameDeskManage.At(puser->GetUserData()->uDeskid);
		if (nullptr == pdesk)
		{//�����쳣
			return false;
		}

		return pdesk->Sys_HandleGameMessage(puser->GetUserData()->uDeskStation, pNetHead, pData, uSize, pSocket->GetSockID());
	}

	//�������ͺ���
	void CGameRoom::SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode)
	{
		std::unordered_map<UINT, GameUserStruct*>* pMap = g_pGameServer->GetCGameUserInfoManage()->GetGameUserStruct_map();
		auto iter = pMap->begin(), end = pMap->end();
		for (; iter != end; ++iter)
		{
			if (usertype_online == iter->second->usertype
				&& m_initRoomData.uRoomID == iter->second->userinfo.GetUserData()->uRoomid)
			{
				g_pGameServer->SendData(iter->second->userinfo.GetSocketId(), pData, uSize, uMainID, uAssistantID, uHandleCode);
			}
		}
	}
}