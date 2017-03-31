#include "GameDesk.h"
#include "GameRoom.h"
#include "message/WJNetRoomMessage.h"
#include "server/comtimetag.h"
#include "message/WJNetPtoR.h"
#include "common/commonfunction.h"
#include "common/QkLogger.h"
#include "gameserver.h"

namespace WJ
{
    extern CGameServer * g_pGameServer;

	CGameDesk::CGameDesk()
	{
		m_pGameRoom = nullptr;
		m_bPlayGame = false;
		m_bLock = false;
		std::memset(m_szLockPass, 0, sizeof(m_szLockPass));
		std::memset(m_UserEndGame, 0, sizeof(m_UserEndGame));
		m_dwOwnerUserID = 0;
		m_uDeskIndex = 0;
		m_uMaxPeople = 0;
		m_dwBeginTime = 0;
		for (int i = 0; i < MAX_PEOPLE; ++i)
		{
			m_pUserInfo[i] = nullptr;
		}

		m_uGameStation = 0;
	}

	CGameDesk::~CGameDesk()
	{

	}

	///�ж���Ϸ���ϵ�ĳ������Ƿ�ʼ��Ϸ��
	bool CGameDesk::IsPlayGame()
	{
		return m_bPlayGame;
	}

	///�û�ͬ�⿪ʼ
	bool CGameDesk::UserAgreeGame(UINT uDeskStation)
	{
		QK_InfoLog("sdp_test desk �û�ͬ����Ϣ��");
		//����Ϸ�����оܾ�׼����Ϣ
		if (IsPlayGame())
		{
			return true;
		}

		if (uDeskStation >= m_uMaxPeople)
		{
			return false;
		}

		if (nullptr == m_pUserInfo[uDeskStation])
		{
			return false;
		}

		//��������
		m_pUserInfo[uDeskStation]->GetUserData()->basedata.bUserState = US_R_Agress;

		//����ͬ����Ϣ
		//������Ϸͬ����Ϣ
		RoomMessDate_user_frame_agree UserAgree;
		UserAgree.udeskid = m_uDeskIndex;
		UserAgree.udeskstation = uDeskStation;
		std::memcpy(&UserAgree.userdata, m_pUserInfo[uDeskStation]->GetUserData(), sizeof(UserInfoInRoomStruct));
		SendDeskData(&UserAgree, sizeof(UserAgree), e_gs_mess_game_frame, e_gs_ass_game_frame_agree, 0);
		QK_InfoLog("sdp_test desk Ⱥ���û�ͬ����Ϣ��");
		//�ж��Ƿ���Կ�ʼ
		if (Sys_CanBeginGame() == true)
		{
			Sys_GameBegin(0);
		}

		return true;
	}

	///�����û����ͽ�����Ϣ
	bool CGameDesk::Sys_UserEndGame(UINT uDeskStation, void* pData, UINT DataSize)
	{
		QK_InfoLog("sdp_test desk �û�������Ϣ��");
		if (uDeskStation >= m_uMaxPeople)
		{
			return false;
		}
		if (m_UserEndGame[uDeskStation])
		{
			return false;
		}
		//Ŀǰ���ֻҪ��һ���ͻ��˸�֪��������Ϸ��������Ϸ�Ϳ��Խ���
		if (IsPlayGame())
		{
			QK_InfoLog("sdp_test desk �û�������Ϣ ���ڴ�������");	
			UserEndGame(uDeskStation, pData, DataSize);
			m_UserEndGame[uDeskStation] = true;
			//ת����������
			for (UINT i = 0; i < GetMaxPeople(); ++i)
			{
				if (uDeskStation != i)
				{
					SendGameData(i, pData, DataSize, e_gs_mess_game_frame, e_gs_ass_game_frame_end_c, 0);
				}
			}
			//�鿴�ǲ��������˶���ʾ������
			for (int i = 0; i < m_uMaxPeople;++i)
			{
				if (!m_UserEndGame[i])
				{//������û�и��߷����������������ȴ�
					//����������ʱ������ֹ��������Ҷ��߲����ͽ�����Ϣ��������
					SetTimer(GAME_TIME_GAMEFINISH_WAIT, 10);
					return false;
				}
			}

			GameFinish(GAME_FINISH_TYPE_NORMAL);
		}
		return true;
	}

	/// ��������е�һ��û���˵���λ����
	bool CGameDesk::GetDeskNoPlayerStation(UINT &uDeskStation)
	{
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (m_pUserInfo[i] == nullptr)
			{
				uDeskStation = i;
				return true;
			}
		}

		return false;
	}

	/// ����������ж�����
	UINT CGameDesk::GetDeskPlayerNum()
	{
		UINT iPeopoleCount = 0;
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (m_pUserInfo[i] != nullptr)
			{
				iPeopoleCount++;
			}
		}

		return iPeopoleCount;
	}

	///�û�����״̬�뿪��Ϸ
	bool CGameDesk::UserOnlineStationNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo)
	{
		return true;
	}

	///�û�����״̬�ػ���Ϸ
	bool CGameDesk::UserOnlineStationReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo)
	{
		return true;
	}

	///�Ƿ�����
	bool CGameDesk::IsLock()
	{
		return m_bLock;
	}

	///����
	void CGameDesk::LockDesk()
	{
		m_bLock = true;
	}

	///����
	void CGameDesk::UnlockDesk()
	{
		m_bLock = false;
	}

	//�Ƿ���ڸ����
	bool CGameDesk::IsExistPlayer(CGameUserInfo * pUserInfo)
	{
        if (pUserInfo == nullptr)
        {
            return false;
        }
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (m_pUserInfo[i] != nullptr && m_pUserInfo[i]->GetUserData()->basedata.dwUserID == pUserInfo->GetUserData()->basedata.dwUserID)
			{
				return true;
			}
		}
		return false;
	}

	///��ʼ������
	bool CGameDesk::Init(UINT uDeskStation, UINT bMaxPeople, void* pDataManage)
	{
		m_uDeskIndex = uDeskStation;
		m_uMaxPeople = bMaxPeople;
		m_pGameRoom = (CGameRoom*)pDataManage;
		m_bPlayGame = false;
		m_bLock = false;
		std::memset(m_szLockPass, 0, sizeof(m_szLockPass));
		std::memset(m_UserEndGame, 0, sizeof(m_UserEndGame));
		m_dwOwnerUserID = 0;
		for (int i = 0; i < MAX_PEOPLE; ++i)
		{
			m_pUserInfo[i] = nullptr;
		}

		m_uGameStation = 0;
		InitDeskGameStation();
		return true;
	}

	//�ж������λ�Ƿ�����������
	bool CGameDesk::bExistPlayerForDeskStation(UINT uDeskStation)
	{
		if (uDeskStation >= m_uMaxPeople)
		{
			return false;
		}

		return (m_pUserInfo[uDeskStation] != nullptr);
	}

	/// ���������е������Ϣ
	void CGameDesk::SetDeskUserInfo(CGameUserInfo * pUserInfo, UINT uDeskStation)
	{
		if (uDeskStation >= m_uMaxPeople)
		{
			return;
		}

		if (nullptr != m_pUserInfo[uDeskStation] && nullptr == pUserInfo)
		{//�������������ڵ��������Ϊ���������ڣ��϶�����ҵ���ϢҲҪ����һ��
			m_pUserInfo[uDeskStation]->GetUserData()->uDeskid = ERROR_DESKID;
			m_pUserInfo[uDeskStation]->GetUserData()->uDeskStation = ERROR_DESKSTATION;
		}
		m_pUserInfo[uDeskStation] = pUserInfo;
	}

	//��ȡ�û���Ϣָ��
	CGameUserInfo* CGameDesk::GetUserInfo(UINT uDeskStation)
	{
		if (uDeskStation >= m_uMaxPeople)
		{
			return nullptr;
		}

		return m_pUserInfo[uDeskStation];
	}

	//��ȡ�������
	const UINT CGameDesk::GetMaxPeople()
	{
		return m_uMaxPeople;
	}

	//��ȡ����id
	const UINT CGameDesk::GetDeskIndex()
	{
		return m_uDeskIndex;
	}

	///������Ϸ״̬
	bool CGameDesk::SendGameStation(UINT uDeskStation, UINT uSocketID, void* pStationData, UINT uSize)
	{
		//������Ϸ״̬
		return SendGameData(uDeskStation, pStationData, uSize, e_gs_mess_game_frame, e_gs_ass_game_frame_station, 0);
	}

	///���Ϳ�ʼ��Ϣ
	bool CGameDesk::SendBeginData(void * pData, UINT uSize)
	{
		return SendDeskData(pData, uSize, e_gs_mess_game_frame, e_gs_ass_game_frame_begin, 0);
	}

	///���ͽ�����Ϣ
	bool CGameDesk::SendFinishData(void * pData, UINT uSize)
	{
		return SendDeskData(pData, uSize, e_gs_mess_game_frame, e_gs_ass_game_frame_end_s, 0);
	}

	//������������
	bool CGameDesk::SendDeskData(void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode)
	{
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if ((m_pUserInfo[i] != nullptr) && (m_pUserInfo[i]->GetUserData()->basedata.bUserState != US_R_NetCut))
			{
                g_pGameServer->SendData(m_pUserInfo[i]->GetSocketId(), pData, uSize, uMainID, uAssID, uHandleCode);
			}
		}

		return true;
	}

	bool CGameDesk::SendDeskData(UINT uMainID, UINT uAssID, UINT uHandleCode)
	{
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if ((m_pUserInfo[i] != nullptr) && (m_pUserInfo[i]->GetUserData()->basedata.bUserState != US_R_NetCut))
			{
				g_pGameServer->SendData(m_pUserInfo[i]->GetSocketId(), uMainID, uAssID, uHandleCode);
			}
		}

		return true;
	}

	///�������ݺ���
	bool CGameDesk::SendGameData(UINT uDeskStation, UINT uMainID, UINT uAssID, UINT uHandleCode)
	{
		if (uDeskStation < m_uMaxPeople)
		{
			if ((m_pUserInfo[uDeskStation] != nullptr) && (m_pUserInfo[uDeskStation]->GetUserData()->basedata.bUserState != US_R_NetCut))
			{
				UINT uSocketId = m_pUserInfo[uDeskStation]->GetSocketId();
				g_pGameServer->SendData(uSocketId, uMainID, uAssID, uHandleCode);
                return true;
			}
		}

		return false;
	}

	///�������ݺ���
	bool CGameDesk::SendGameData(UINT uDeskStation, void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode)
	{
		if (uDeskStation < m_uMaxPeople)
		{
			if ((m_pUserInfo[uDeskStation] != nullptr) && (m_pUserInfo[uDeskStation]->GetUserData()->basedata.bUserState != US_R_NetCut))
			{
				UINT uSocketIndex = m_pUserInfo[uDeskStation]->GetSocketId();
				g_pGameServer->SendData(uSocketIndex, pData, uSize, uMainID, uAssID, uHandleCode);
                return true;
			}
		}

		return false;
	}

	//�������ͺ���
	void CGameDesk::SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode)
	{
		m_pGameRoom->SendDataBatch(pData, uSize, uMainID, uAssistantID, uHandleCode);
	}

	//����Ƿ��κ�ʱ�򶼿����뿪����ζ��һ�뿪��Ϸ�ͽ����ˣ�����Ϸ״ֻ̬��һ����
	bool CGameDesk::SysCanLeaveEveryTime()
	{
		return CanLeaveEveryTime();
	}

	//���ö�ʱ��
	bool CGameDesk::SetTimer(UINT uTimerID, int uElapseSeconds)
	{
		UINT globalTimerId = 0;
		if (uTimerID >= TIMER_GAME_TIMERID_MASK && uTimerID < TIMER_GAME_TIMERID_MASK_SYS)
		{
			globalTimerId = ((m_pGameRoom->GetRoom_ManageInfoStruct()->uRoomID << 20) & TIMER_ROOMID_MASK)
				| ((m_uDeskIndex << 8) & TIMER_DESKID_MASK)
				| (uTimerID & TIMER_GAME_TIMERID_MASK_SYS);
		}
		else if (uTimerID < TIMER_GAME_TIMERID_MASK)
		{
			globalTimerId = ((m_pGameRoom->GetRoom_ManageInfoStruct()->uRoomID << 20) & TIMER_ROOMID_MASK)
				| ((m_uDeskIndex << 8) & TIMER_DESKID_MASK)
				| uTimerID;
		}
		else
		{
			return false;
		}

		
		g_pGameServer->SetTimer(globalTimerId, uElapseSeconds);
        return true;
	}

	//ɾ����ʱ��
	bool CGameDesk::KillTimer(UINT uTimerID)
	{
		UINT globalTimerId = 0;
		if (uTimerID >= TIMER_GAME_TIMERID_MASK && uTimerID < TIMER_GAME_TIMERID_MASK_SYS)
		{
			globalTimerId = ((m_pGameRoom->GetRoom_ManageInfoStruct()->uRoomID << 20) & TIMER_ROOMID_MASK)
				| ((m_uDeskIndex << 8) & TIMER_DESKID_MASK)
				| (uTimerID & TIMER_GAME_TIMERID_MASK_SYS);
		}
		else if (uTimerID < TIMER_GAME_TIMERID_MASK)
		{
			globalTimerId = ((m_pGameRoom->GetRoom_ManageInfoStruct()->uRoomID << 20) & TIMER_ROOMID_MASK)
				| ((m_uDeskIndex << 8) & TIMER_DESKID_MASK)
				| uTimerID;
		}
		else
		{
			return false;
		}

		g_pGameServer->KillTimer(globalTimerId);
		return true;

	}

	//��ȡ����ָ��
	const Room_ManageInfoStruct* CGameDesk::GetRoom_ManageInfoStruct()
	{
		return m_pGameRoom->GetRoom_ManageInfoStruct();
	}

	//��ȡͬ����� 
	UINT CGameDesk::FillDeskUserInfo(char * pBuffer, UINT uBufferSize, UINT& uindex, bool & bFinish)
	{
		return 0;
	}
	//��������,ǿ������(�Ƿ��������·Ż��Ŷӻ���)
	void CGameDesk::ClearDesk(bool bInqueue)
	{
		if (US_ContestRule_team == m_pGameRoom->GetRoom_ManageInfoStruct()->uContestRule)
		{
			bInqueue = false;
		}

		KillTimer(GAME_TIME_GAMEFINISH_WAIT);
		KillTimer(GAME_TIME_AGREE_OUTTIME);
		KillTimer(GAME_TIME_GAMEFINISH_OUTTIME);
		//������Ϸ�ڲ�����
		ReSetGameState(m_uGameStation);
		//�������������
		CleanCutGameInfoAll();//����������û�
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (nullptr != m_pUserInfo[i])
			{
				m_pUserInfo[i]->GetUserData()->basedata.bUserState = US_R_Wait;
				m_pUserInfo[i]->GetUserData()->uDeskid = ERROR_DESKID;
				m_pUserInfo[i]->GetUserData()->uDeskStation = ERROR_DESKSTATION;
				if (bInqueue)
				{//�����������
					g_pGameServer->GetCQueueManage(m_pGameRoom->GetRoom_ManageInfoStruct()->uContestid)->Add(m_pUserInfo[i]);
				}
				m_pUserInfo[i] = nullptr;
			}
		}

		std::memset(m_szLockPass, 0, sizeof(m_szLockPass));
		std::memset(m_UserEndGame, 0, sizeof(m_UserEndGame));
		m_bLock = false;
		m_dwOwnerUserID = 0;
		m_bPlayGame = false;
	}
	///��Ϸ����
	/// @param bCloseFlag  �쳣������Ϸʱ�ı�־����������ʱΪ0
	bool CGameDesk::GameFinish(UINT uCloseFlag, UINT uDeskStation)
	{
		if (!m_bPlayGame)
		{
			QK_ErrorLog("sdp_error desk ��Ϸ���ڽ����У�����������¼���");
			return false;
		}
		QK_InfoLog("sdp_test desk ��Ϸ����============the end========================��");
		KillTimer(GAME_TIME_GAMEFINISH_WAIT);
		KillTimer(GAME_TIME_GAMEFINISH_OUTTIME);
		UINT uiPlayTime = (UINT)time(NULL) - (UINT)m_dwBeginTime;	//������Ϸʱ��
		m_bPlayGame = false;
		std::memset(m_UserEndGame, 0, sizeof(m_UserEndGame));
		//��Ϸ�ڲ����㴦��
		std::string _Data;
		GameOverSettlement(uCloseFlag, _Data);
		//������Ϸ�ڲ�����
		ReSetGameState(m_uGameStation);

		if (US_ContestRule_solo == m_pGameRoom->GetRoom_ManageInfoStruct()->uContestRule)
		{//������֪ͨ����
			//֪ͨ��½����������ҽ�����Ϸ��
			for (UINT i = 0; i < m_uMaxPeople; i++)
			{
				if (nullptr != m_pUserInfo[i])
				{
					struct_data_pr_userinfo_gameover cmd;
					cmd.userid = m_pUserInfo[i]->GetUserData()->basedata.dwUserID;
					QK_InfoLog("sdp_test desk ֪ͨ������Ϸ������");
					g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &cmd, sizeof(struct_data_pr_userinfo_gameover), e_pr_mess_userinfo, e_pr_ass_userinfo_gameover, uCloseFlag);
				}
			}
		}
		else if (US_ContestRule_team == m_pGameRoom->GetRoom_ManageInfoStruct()->uContestRule)
		{//�Ŷ���֪ͨ����
			//������Ŷ����ǵ�ɾ����VS��¼
			unordered_map<UINT, R_TeamStruct_VS * > * _pTeamStruct_VS = g_pGameServer->GetR_TeamStruct_VS();
			bool bdelVS = false;//�Ƿ����ɾ��vs��¼
			for (UINT i = 0; i < m_uMaxPeople; i++)
			{
				if (nullptr != m_pUserInfo[i])
				{
                    auto iter = _pTeamStruct_VS->find(m_pUserInfo[i]->GetUserData()->basedata.teamid);
					if (iter != _pTeamStruct_VS->end())
					{
						for (int j = 0; j < 2; ++j)
						{
                            for (int k = 0; k < iter->second->_data._TeamStruct[j].m_uTemaPlayerNum; ++k)
							{
								if (iter->second->_data._TeamStruct[j].m_Userid[k] == m_pUserInfo[i]->GetUserData()->basedata.dwUserID)
								{
									bdelVS = true;
									break;
								}
							}
							if (bdelVS)
							{
								break;
							}
						}
						if (bdelVS)
						{
							QK_InfoLog("sdp_test desk ֪ͨ������Ϸ������");
							struct_data_pr_userinfo_team_vs_finish cmd;
							cmd._data = iter->second->_data;
                            g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &cmd, sizeof(struct_data_pr_userinfo_team_vs_finish), e_pr_mess_userinfo, e_pr_ass_userinfo_team_vs_finish, 0);
							_pTeamStruct_VS->erase(iter);
							break;
						}
					}
				}

				if (bdelVS)
				{
					break;
				}
			}
		}
		
		//�������������
		CleanCutGameInfoAll();//����������û�
		char data_str[MAX_SEND_SIZE];
		QK_InfoLog("sdp_test desk Ⱥ����Ϸ����[1]length=%d", _Data.length());
		if (_Data.length()+1 <= sizeof(data_str))
		{
			sprintf_s(data_str, "%s", _Data.c_str());
		}
        else
        {
            QK_ErrorLog("game finish result data is too long : len=%d", _Data.length());
        }
		
		if (GAME_FINISH_TYPE_FLEE == uCloseFlag)
		{//��������ܵ��½�����,���������userid���ݸ�������
			SendDeskData(&uDeskStation, sizeof(int), e_gs_mess_game_frame, e_gs_ass_game_frame_end_s, uCloseFlag);
		}
		else
		{
			SendDeskData(nullptr, 0, e_gs_mess_game_frame, e_gs_ass_game_frame_end_s, uCloseFlag);
		}

		QK_InfoLog("sdp_test desk Ⱥ����Ϸ����[2]��uCloseFlag=%d", uCloseFlag);
		//���������һ����Ϸ��¼
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (nullptr != m_pUserInfo[i])
			{//������������״̬
				g_pGameServer->GetCGameUserInfoManage()->FreeUserEx(m_pUserInfo[i]->GetUserData()->basedata.dwUserID);
			}
		}
		
		ClearDesk(false);

		
		//������Ϸ������Ϣ�����ݿ�
        //CGameServer::m_pRedis->hmset("game_result", );  //todo

		QK_InfoLog("sdp_test", "desk Ⱥ����Ϸ����[3]��");
		return true;
	}

	///�������ж������
	bool CGameDesk::CleanCutGameInfoAll()
	{
		for (BYTE i = 0; i < m_uMaxPeople; i++)
		{
			CleanCutGameInfo(i);
		}
		return true;
	}

	//�������ָ�������������
	bool CGameDesk::CleanCutGameInfo(UINT bDeskStation)
	{
		/// ���û�ҵ������ң��򷵻� false
		if (bDeskStation >= m_uMaxPeople)
		{
			return false;
		}

		if ((m_pUserInfo[bDeskStation] != nullptr) && US_R_NetCut == m_pUserInfo[bDeskStation]->GetUserData()->basedata.bUserState)
		{
			//��������
			g_pGameServer->GetCGameUserInfoManage()->FreeUserEx(m_pUserInfo[bDeskStation]->GetUserData()->basedata.dwUserID);
			m_pUserInfo[bDeskStation] = nullptr;
		}

		return false;
	}

	int CGameDesk::MyRand()
	{
		static int innerSeed = GetTickCount();

		return (((innerSeed = innerSeed * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	///�û������뿪
	bool CGameDesk::Sys_UserNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo)
	{
		UserNetCut(uDeskStation, pLostUserInfo);

		if (!SysCanLeaveEveryTime())
		{
			//������Ҷ������ˣ��ͽ�����Ϸ
			bool bAllNetCut = true;
			for (int i = 0; i < MAX_PEOPLE; ++i)
			{
				if (nullptr != m_pUserInfo[i] && US_R_NetCut != m_pUserInfo[i]->GetUserData()->basedata.bUserState)
				{
					bAllNetCut = false;
					break;
				}
			}

			if (bAllNetCut)
			{
				QK_InfoLog("sdp_test ������Ҷ������ˣ��ͽ�����Ϸ");
				GameFinish(GAME_FINISH_TYPE_FORCE);
			}
		}
		else
		{//��ʱ�������뿪�Ǿͽ���
			QK_InfoLog("sdp_test ��ʱ�������뿪�Ǿͽ���!!!!!!!!!!!!!!");
			GameFinish(GAME_FINISH_TYPE_FLEE, uDeskStation);
		}

		return true;
	}

	///�û���������
	bool CGameDesk::Sys_UserReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo)
	{
		assert(nullptr != pNewUserInfo);
		m_pUserInfo[uDeskStation] = pNewUserInfo;
		return UserReCome(uDeskStation, pNewUserInfo);
	}

	///��Ϸ��ʼ
	bool CGameDesk::Sys_GameBegin(UINT uBeginFlag)
	{
		QK_InfoLog("sdp_test desk ��Ϸ��ʼ��");
		m_bPlayGame = true;
		m_dwBeginTime = (long int)time(NULL);
		KillTimer(GAME_TIME_AGREE_OUTTIME);
		SetTimer(GAME_TIME_GAMEFINISH_OUTTIME, 60 * 60 * 2);
		if (!GameBegin(uBeginFlag))
		{
			QK_InfoLog("sdp_test ��ʼʧ�ܣ��ͽ�����Ϸ");
			return GameFinish(GAME_FINISH_TYPE_FORCE);
		}
		
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (nullptr != m_pUserInfo[i])
			{
				m_pUserInfo[i]->GetUserData()->basedata.bUserState = US_R_Playing;
			}
		}

		QK_InfoLog("sdp_test desk Ⱥ����Ϸ��ʼ��Ϣ��");
		SendDeskData(e_gs_mess_game_frame, e_gs_ass_game_frame_begin, 0);
		return true;
	}

	///�û��뿪��Ϸ��
	UINT CGameDesk::Sys_UserLeftDesk(UINT uDeskStation, CGameUserInfo * pUserInfo, UINT uLeftCause)
	{
		QK_InfoLog("sdp_test desk �û��뿪��Ϸ����");
		assert(nullptr != pUserInfo);
		UINT userid = pUserInfo->GetUserData()->basedata.dwUserID;
		bool res = UserLeftDesk(uDeskStation, pUserInfo, uLeftCause);
		pUserInfo->GetUserData()->uDeskid = ERROR_DESKID;
		pUserInfo->GetUserData()->uDeskStation = ERROR_DESKSTATION;
		m_pUserInfo[uDeskStation] = nullptr;

		if (GetDeskPlayerNum() == 0)
		{//û��Ӧ��Ҫ����������
			ClearDesk(false);
		}

		SendDeskData(e_gs_mess_game_frame, e_gs_ass_game_frame_user_leave, userid);
		return res;
	}

	///�����Ϣ������
	bool CGameDesk::Sys_HandleFrameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID)
	{
		switch (pNetHead->bAssistantID)
		{
		case e_gs_ass_game_frame_info:
		{
			QK_InfoLog("sdp_test desk �յ����������Ϸ����������Ϣ��");
			//��ȡ��Ϸ��Ϣ
			UINT uSendSize = 0;
			RoomMessDate_deskinfo deskinfo;
			deskinfo.bGameStation = m_uGameStation;///< ��Ϸ״̬
			QK_InfoLog("sdp_test desk �ظ����������Ϸ����������Ϣ��");
			//������Ϸ��Ϣ
			SendGameData(uDeskStation, &deskinfo, sizeof(deskinfo), e_gs_mess_game_frame, e_gs_ass_game_frame_info, 0);

			//�����û���Ϸ״̬
			return Sys_OnGetGameStation(uDeskStation, uSocketID);
		}break;
		case e_gs_ass_game_frame_agree:
		{
			UserAgreeGame(uDeskStation);
		}break;
		case e_gs_ass_game_frame_end_c:
		{
			Sys_UserEndGame(uDeskStation, pData, uSize);
		}break;
		default:
			break;
		}

		return HandleFrameMessage(uDeskStation, pNetHead, pData, uSize, uSocketID);
	}

	///��Ϸ���ݰ�������
	bool CGameDesk::Sys_HandleGameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID)
	{
		return HandleGameMessage(uDeskStation, pNetHead, pData, uSize, uSocketID);
	}

	///��ʱ����Ϣ
	bool CGameDesk::Sys_OnTimer(UINT uTimerID)
	{
		UINT timeId = (uTimerID & (TIMER_GAME_TIMERID_MASK_SYS | TIMER_GAME_TIMERID_MASK));

		if (timeId >= TIMER_GAME_TIMERID_MASK && timeId < TIMER_GAME_TIMERID_MASK_SYS)
		{//ϵͳ��ʱ��
			timeId = uTimerID & TIMER_GAME_TIMERID_MASK_SYS;

			if (GAME_TIME_GAMEFINISH_WAIT == timeId
				|| GAME_TIME_GAMEFINISH_OUTTIME == timeId)
			{//��ʱ�����ͽ�����Ϣ
				QK_InfoLog("sdp_test ��ʱ�����ͽ�����Ϣ(%d)", timeId);
				GameFinish(GAME_FINISH_TYPE_FORCE);
				return true;
			}
			else if (GAME_TIME_AGREE_OUTTIME == timeId)
			{
				ClearDesk(true);
			}
		}
		else if (timeId < TIMER_GAME_TIMERID_MASK)
		{//��ͨ��ʱ��
			timeId = uTimerID & TIMER_GAME_TIMERID_MASK;

			OnTimer(timeId);
			return true;
		}

		return false;
	}

	///��ȡ��Ϸ״̬��Ϣ
	bool CGameDesk::Sys_OnGetGameStation(UINT uDeskStation, UINT uSocketID)
	{
		OnGetGameStation(uDeskStation, uSocketID);
		QK_InfoLog("sdp_test", "desk ���߸���ң�������������ҵ����ݡ�");
		//���߸���ң�������������ҵ�����
		SendUserListForDesk(m_uDeskIndex, uSocketID, m_pUserInfo[uDeskStation]->GetUserData()->basedata.dwUserID);
		return true;
	}

	//���÷���
	void CGameDesk::SetdwOwnerUserID(UINT uuserid)
	{
		m_dwOwnerUserID = uuserid;
	}

	//��ȡ����
	UINT CGameDesk::GetdwOwnerUserID()
	{
		return m_dwOwnerUserID;
	}

	//�����������
	void CGameDesk::QueueOver()
	{
		SetTimer(GAME_TIME_AGREE_OUTTIME, 30*60);
	}

	//�ж��Ƿ���Կ�ʼ��Ϸ
	bool CGameDesk::Sys_CanBeginGame()
	{
		//�ж��������Ƿ�ͬ��
		UINT bPeopoleCount = 0;
		for (UINT i = 0; i < m_uMaxPeople; i++)
		{
			if (m_pUserInfo[i] != nullptr && US_R_Agress == m_pUserInfo[i]->GetUserData()->basedata.bUserState)
			{
				bPeopoleCount++;
			}
		}

		//�ж��ʺϿ�ʼ����
		if (bPeopoleCount == m_uMaxPeople)
		{
			return CanBeginGame();
		}

		return false;
	}

	//����������
	void CGameDesk::GetRandText(std::string &str, int strleng)
	{
		str.clear();
		do
		{
			int a = MyRand() % 10;
			char s[2] = "0";
			_itoa_s(a, s, 2, 10);
			str += s;
			--strleng;
		} while (strleng > 0);
	}

	//��ȡһ����������������б�,���Լ���Ϣ��
	UINT CGameDesk::FillDeskUsers(char * pBuffer, UINT uBufferSize, UINT deskid, UINT myuserid, UINT uBeginPos, bool & bFinish, UINT &uCopyCount)
	{
		//��������
		uCopyCount = 0;//�����˼���
		UINT i = uBeginPos;//�ƶ��Ľڵ�
		if (uBeginPos < m_uMaxPeople)
		{
			int i = 0;
			for (; i<m_uMaxPeople; ++i)
			{
				if (((uCopyCount + 1)*sizeof(UserInfoInRoomStruct)) >= uBufferSize)
				{
					break;
				}

				if (nullptr != m_pUserInfo[i]
					&& myuserid != m_pUserInfo[i]->GetUserData()->basedata.dwUserID)
				{
					std::memcpy(pBuffer + uCopyCount*sizeof(UserInfoInRoomStruct), m_pUserInfo[i]->GetUserData(), sizeof(UserInfoInRoomStruct));
					++uCopyCount;
				}
			}
		}

		//�������
		bFinish = (i >= m_uMaxPeople);

		return i;
	}

	//��ָ����Ҹ��¸�����������������г��Լ���������û��б�
	void CGameDesk::SendUserListForDesk(UINT deskid, UINT socketindex, UINT myuserid)
	{
		//��ȡ��������
		UINT uBeginPos = 0;
		bool bFinish = false;
		char *pBuffer = new char[sizeof(UserInfoInRoomStruct)*m_uMaxPeople];
		assert(pBuffer);
		int uCopyCount = 0;
		for (int i = 0; i < m_uMaxPeople; ++i)
		{
			if (nullptr != m_pUserInfo[i]
				&& myuserid != m_pUserInfo[i]->GetUserData()->basedata.dwUserID)
			{
				std::memcpy(pBuffer + uCopyCount*sizeof(UserInfoInRoomStruct), m_pUserInfo[i]->GetUserData(), sizeof(UserInfoInRoomStruct));
				ZeroMemory(((UserInfoInRoomStruct*)(pBuffer + uCopyCount*sizeof(UserInfoInRoomStruct)))->basedata.password, PASSWORD_MD5_LENGTH);
				++uCopyCount;
			}
		}
		g_pGameServer->SendData(socketindex, pBuffer, uCopyCount*sizeof(UserInfoInRoomStruct), e_gs_mess_user_list, e_gs_ass_user_list_get_desk_userinfo, e_gs_hcid_userlist_finish);
		if (pBuffer)
		{
			delete[]pBuffer;
			pBuffer = NULL;
		}
	}
}