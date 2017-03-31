#include <atltime.h>
#include <algorithm>
#include "QueueManage.h"
#include "GameRoom.h"
#include "message/WJNetRoomMessage.h"
#include "common/commonfunction.h"
#include "gameserver.h"
#include "common/QkLogger.h"
#include "message/WJNetPtoR.h"

namespace WJ
{
    
    extern CGameServer *g_pGameServer;

	///////////////////////////////////////////////////////////////////////////////////
	CQueueManage::CQueueManage()
	{
		m_ucontestid = ERROR_CONTESTID;
		m_uContestRule = US_ContestRule_mission;
	}

	CQueueManage::~CQueueManage()
	{
		DeleteAll();
	}

	void CQueueManage::run()
	{
		m_QueueUserList.sort();

		switch (m_uContestRule)
		{
		case US_ContestRule_solo:
		{
								 run_RoomRule_solo();
		}break;
		case US_ContestRule_team:
		{
								 run_RoomRule_team();
		}break;
		default:
			break;
		}
	}

	//����soloģʽ���г���
	void CQueueManage::run_RoomRule_solo()
	{
		if (2 > m_QueueUserList.size())
		{//����Ҫ������
			return;
		}

		//��û�з��䡢������
		ISysDeskInterface* pDesk = nullptr;
		CGameRoom* pRoom = g_pGameServer->m_roomMgr.GetRoom_ForDeskid(m_ucontestid, &pDesk);
		if (nullptr == pDesk || nullptr == pRoom)
		{//û�з��䡢�����ӾͲ���ƥ����
			return;
		}

		UINT max_palyers = pDesk->GetMaxPeople();
		if (0 == max_palyers)
		{//�����ǲ����Ե�
			return;
		}

		bool bOK = false;
		//�˲����Ͳ���ƥ����
		while (max_palyers <= m_QueueUserList.size())
		{
			bOK = false;
			pRoom = g_pGameServer->m_roomMgr.GetRoom_ForDeskid(m_ucontestid, &pDesk);
			if (nullptr == pDesk || nullptr == pRoom)
			{
				return;
			}
			//ȡ�û�������������
			vector<QueueInfo_t>	QueueUservector;//ָ�����뷿�����
			QueueUservector.clear();
			auto iter = m_QueueUserList.begin();
			while (iter != m_QueueUserList.end())
			{
				CGameUserInfo * puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo((*iter).m_dwUserID);
				if (nullptr == puser)
				{//��ȷ�����Ƿ����ߣ����߾ʹ��������߾Ͷ���
					m_userid2iter.erase(iter->m_dwUserID);
					iter = m_QueueUserList.erase(iter);
					continue;
				}

				QueueUservector.push_back((*iter));
				m_userid2iter.erase(iter->m_dwUserID);
				iter = m_QueueUserList.erase(iter);
				if (QueueUservector.size() == pDesk->GetMaxPeople())
				{//�����ﵽ��
					bOK = true;
					QK_InfoLog("sdp_test room ����ɹ����������������========= (deskid=%d,%d,%d)��"
						, pDesk->GetDeskIndex(), QueueUservector.at(0).m_dwUserID, QueueUservector.at(1).m_dwUserID);
					break;
				}
			}//while (iter != m_QueueUserList.end())

			if (bOK)
			{//�������ˣ���Ҳ�Һ��ˣ���ʼ֪ͨ��Щ�˽������ӿ���
				for (UINT j = 0; j < QueueUservector.size(); ++j)
				{
					CGameUserInfo * puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo(QueueUservector.at(j).m_dwUserID);
					//�����������������ڷ���id
					puser->GetUserData()->uRoomid = pRoom->GetRoom_ManageInfoStruct()->uRoomID;
					struct_data_pr_userinfo_allot_room _cmd;
					_cmd.userid = puser->GetUserData()->basedata.dwUserID;
					_cmd.contestid = puser->GetUserData()->uContestid;
					_cmd.roomid = puser->GetUserData()->uRoomid;
					_cmd.uIndex = puser->GetSocketId();
					QK_InfoLog("gs solo�� �����������������ڷ���id��(roomid=%d, userid=%d,contestid=%d,uIndex=%d)", _cmd.roomid, _cmd.userid, _cmd.contestid, _cmd.uIndex);
					g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &_cmd, sizeof(_cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_allot_room, 0);

					if (0 == j)
					{//��һ�����õ����Ϊ����
						pDesk->SetDeskUserInfo(puser, 0);
						pDesk->QueueOver();
						puser->GetUserData()->uDeskStation = 0;
						puser->GetUserData()->uDeskid = pDesk->GetDeskIndex();
						puser->GetUserData()->basedata.bUserState = US_R_GetStation;

						//֪ͨ��ұ����䵽ָ�������ָ����λ�ϣ�������Ϊ����
						QK_InfoLog("sdp_test room ֪ͨ��ұ����䵽ָ�������ָ����λ�ϣ�������Ϊ������");
						RoomMessDate_queue_allot cmd;
						cmd.bOwerUserid = true;
						cmd.udeskid = pDesk->GetDeskIndex();
						cmd.udeskstation = 0;
						cmd.uuserid = QueueUservector.at(j).m_dwUserID;
						g_pGameServer->SendData(puser->GetSocketId(), &cmd, sizeof(cmd), e_gs_mess_queue, e_gs_ass_queue_allot, 0);
					}//if (0 == j)
					else
					{//�ǵ�һ����ң�ֱ�ӷ������λ��
						UINT udeskstation = 0;
						bool res = pDesk->GetDeskNoPlayerStation(udeskstation);
						if (!res)
						{//������λʧ��
							continue;
						}
						pDesk->SetDeskUserInfo(puser, udeskstation);
						pDesk->QueueOver();
						puser->GetUserData()->uDeskStation = udeskstation;
						puser->GetUserData()->uDeskid = pDesk->GetDeskIndex();
						puser->GetUserData()->basedata.bUserState = US_R_GetStation;

						//֪ͨ��ұ����䵽ָ�������ָ����λ��
						QK_InfoLog("sdp_test room ֪ͨ��ұ����䵽ָ�������ָ����λ�ϡ�");
						RoomMessDate_queue_allot cmd;
						cmd.bOwerUserid = false;
						cmd.udeskid = pDesk->GetDeskIndex();
						cmd.udeskstation = udeskstation;
						cmd.uuserid = QueueUservector.at(j).m_dwUserID;
						g_pGameServer->
							SendData(puser->GetSocketId(), &cmd, sizeof(cmd), e_gs_mess_queue, e_gs_ass_queue_allot, 0);
					}
				}
			}//if (bOK)
			else
			{
				//û��һ���ͻ���ȥ
				auto iter_vector = QueueUservector.begin();
				while (iter_vector != QueueUservector.end())
				{
					m_QueueUserList.push_front((*iter_vector));
					m_userid2iter.insert(make_pair(iter_vector->m_dwUserID, m_QueueUserList.begin()));
					++iter_vector;
				}
			}
		}
	}

	//���ģʽģʽ���г���
	void CQueueManage::run_RoomRule_team()
	{
		unordered_map<UINT, R_TeamStruct_VS * > * _pTeamStruct_VS2 = g_pGameServer->GetR_TeamStruct_VS();
		vector<R_TeamStruct_VS *> vector_VS;

		auto _it = _pTeamStruct_VS2->begin();
		while (_it != _pTeamStruct_VS2->end())
		{
			assert(_it->second);
			if (_it->second->_E_R_TeamStruct_VS_station == E_R_TeamStruct_VS_station_wait)
			{//���ռ���ƥ����Ŷ�
				vector_VS.push_back(_it->second);
			}
			++_it;
		}

		if (0 == vector_VS.size())
		{//û���Ŷ���Ϣ
			return;
		}

		//��û�з��䡢������
		ISysDeskInterface* pDesk = nullptr;
		CGameRoom* pRoom = g_pGameServer->m_roomMgr.GetRoom_ForDeskid(m_ucontestid, &pDesk);
		if (nullptr == pDesk || nullptr == pRoom)
		{//û�з��䡢�����ӾͲ���ƥ����
			return;
		}

		bool bOK = false;
		//�˲����Ͳ���ƥ����
		while (0 < vector_VS.size())
		{
			bOK = false;
			pRoom = g_pGameServer->m_roomMgr.GetRoom_ForDeskid(m_ucontestid, &pDesk);
			if (nullptr == pDesk || nullptr == pRoom)
			{
				return;
			}
			//���Ű��ŶӷŽ�����
			R_TeamStruct_VS _VS_data;
			UINT players_count = 0;//�����Ŷ��������ܺ�
			auto it = vector_VS.begin(), end = vector_VS.end();
			while (it != end)
			{
				players_count = (*it)->_data._TeamStruct[0].m_uTemaPlayerNum + (*it)->_data._TeamStruct[1].m_uTemaPlayerNum;
				if (players_count > pDesk->GetMaxPeople())
				{//�����Ŷ��������ܼ�����������������
					it = vector_VS.erase(it);
					continue;
				}

				bool bOnLine = true;//�Ƿ������Ŷ��е���Ҷ�����
				for (int j = 0; j < 2; ++j)
				{
					for (int k = 0; k < (*it)->_data._TeamStruct[j].m_uTemaPlayerNum; ++k)
					{
						CGameUserInfo * puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo((*it)->_data._TeamStruct[j].m_Userid[k]);
						if (nullptr == puser)
						{//ֻҪ�Ŷ�VS������һ���˲����߾Ͳ��ܿ�ʼ
							bOnLine = false;
							break;
						}
					}

					if (!bOnLine)
					{
						break;
					}
				}

				if (!bOnLine)
				{//������һ���Ŷ�vs�ڵ�
					it = vector_VS.erase(it);
					continue;
				}

				//�����ﵽ��
				bOK = true;
				(*it)->_E_R_TeamStruct_VS_station = E_R_TeamStruct_VS_station_playing;
				_VS_data = *(*it);

				it = vector_VS.erase(it);
				break;
			}

			if (bOK)
			{//�������ˣ���Ҳ�Һ��ˣ���ʼ֪ͨ��Щ�˽������ӿ���
				std::vector<UINT> vec_userid;//�û�id����
				for (int j = 0; j < 2; ++j)
				{
					for (int k = 0; k < _VS_data._data._TeamStruct[j].m_uTemaPlayerNum; ++k)
					{
						vec_userid.push_back(_VS_data._data._TeamStruct[j].m_Userid[k]);
					}
				}

				for (UINT j = 0; j < vec_userid.size(); ++j)
				{
					CGameUserInfo * puser = g_pGameServer->GetCGameUserInfoManage()->GetOnLineUserInfo(vec_userid.at(j));
					if (nullptr == puser)
					{
						continue;
					}

					//�����������������ڷ���id
					puser->GetUserData()->uRoomid = pRoom->GetRoom_ManageInfoStruct()->uRoomID;
					struct_data_pr_userinfo_allot_room _cmd;
					_cmd.userid = puser->GetUserData()->basedata.dwUserID;
					_cmd.contestid = puser->GetUserData()->uContestid;
					_cmd.roomid = puser->GetUserData()->uRoomid;
					_cmd.uIndex = puser->GetSocketId();
					QK_InfoLog("gs �Ŷ��� �����������������ڷ���id��(roomid=%d, userid=%d,contestid=%d,uIndex=%d)", _cmd.roomid, _cmd.userid, _cmd.contestid, _cmd.uIndex);
					g_pGameServer->SendData(g_pGameServer->GetSockidOfLoginServer(), &_cmd, sizeof(_cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_allot_room, 0);

					if (0 == j)
					{//��һ�����õ����Ϊ����
						pDesk->SetDeskUserInfo(puser, 0);
						pDesk->QueueOver();
						puser->GetUserData()->uDeskStation = 0;
						puser->GetUserData()->uDeskid = pDesk->GetDeskIndex();
						puser->GetUserData()->basedata.bUserState = US_R_GetStation;

						//֪ͨ��ұ����䵽ָ�������ָ����λ�ϣ�������Ϊ����
						QK_InfoLog("sdp_test room ֪ͨ��ұ����䵽ָ�������ָ����λ�ϣ�������Ϊ������");
						RoomMessDate_queue_allot cmd;
						cmd.bOwerUserid = true;
						cmd.udeskid = pDesk->GetDeskIndex();
						cmd.udeskstation = 0;
						cmd.uuserid = vec_userid.at(j);
						g_pGameServer->SendData(puser->GetSocketId(), &cmd, sizeof(cmd), e_gs_mess_queue, e_gs_ass_queue_allot, 0);
					}//if (0 == j)
					else
					{//�ǵ�һ����ң�ֱ�ӷ������λ��
						UINT udeskstation = 0;
						bool res = pDesk->GetDeskNoPlayerStation(udeskstation);
						if (!res)
						{//������λʧ��
							continue;
						}
						pDesk->SetDeskUserInfo(puser, udeskstation);
						pDesk->QueueOver();
						puser->GetUserData()->uDeskStation = udeskstation;
						puser->GetUserData()->uDeskid = pDesk->GetDeskIndex();
						puser->GetUserData()->basedata.bUserState = US_R_GetStation;

						//֪ͨ��ұ����䵽ָ�������ָ����λ��
						QK_InfoLog("sdp_test room ֪ͨ��ұ����䵽ָ�������ָ����λ�ϡ�");
						RoomMessDate_queue_allot cmd;
						cmd.bOwerUserid = false;
						cmd.udeskid = pDesk->GetDeskIndex();
						cmd.udeskstation = udeskstation;
						cmd.uuserid = vec_userid.at(j);
						g_pGameServer->SendData(puser->GetSocketId(), &cmd, sizeof(cmd), e_gs_mess_queue, e_gs_ass_queue_allot, 0);
					}
				}//if (bOK)
			}//if (0 == uplayernum)
		}//for (UINT i = 0; i < ucount; ++i)
	}

	//��������Ϣ��������
	bool CQueueManage::Add(CGameUserInfo* pGameUserInfo)
	{
		if (nullptr == pGameUserInfo)
		{
			return false;
		}

		QueueInfo_t _t;
		_t.m_dwUserID = pGameUserInfo->GetUserData()->basedata.dwUserID;
		_t.m_uContestid = pGameUserInfo->GetUserData()->uContestid;
		_t.m_EnterTime = CTime::GetCurrentTime().GetTime();// ��¼�����Ŷӻ���ʱ�䣬�����ж����˶೤ʱ��Ķ�

		//�����ͨ��Ҷ���
		//�жϸ�����Ƿ������
        auto iter = m_userid2iter.find(_t.m_dwUserID);
		if (iter == m_userid2iter.end())//δ��ӵ������ӵ��Ŷ��б�
		{
            auto insertPos = m_QueueUserList.insert(m_QueueUserList.end(), _t);
            m_userid2iter.insert(make_pair(_t.m_dwUserID, insertPos));
			return true;
		}
		QK_ErrorLog("sdp_err room ���ܽ�����ظ����뵽�ŶӶ�������(%d)��", _t.m_dwUserID);
		return false;
	}

	//ɾ��������ָ�������Ϣ
	void CQueueManage::Delete(CGameUserInfo* pGameUserInfo)
	{
        if (nullptr == pGameUserInfo)
        {
            return;
        }
		auto iter = m_userid2iter.find(pGameUserInfo->GetUserData()->basedata.dwUserID);
		if (iter != m_userid2iter.end())
		{
			m_QueueUserList.erase(iter->second);
            m_userid2iter.erase(iter);
		}
	}

	//ɾ�����������������Ϣ
	void CQueueManage::DeleteAll()
	{
		m_QueueUserList.clear();
        m_userid2iter.clear();
	}
}