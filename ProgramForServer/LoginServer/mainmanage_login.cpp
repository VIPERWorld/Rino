
#include "mainmanage_login.h"
#include <vector>
#include "loginserver.h"
#include "message/WJNetPlatformMessage.h"
#include "message/WJNetPtoR.h"
#include "server/comtimetag.h"
#include "Redis/WJR_agreement_hall.h"
#include "common/commonfunction.h"
#include "common/WJGameInfo.h"
#include "common\QkLogger.h"
#include "lsconfig.h"
#include "message/WJNetRoomMessage.h"

enum ClientType
{
	ePCClient,//pc�ͻ���
	eGameServer,//��Ϸ������
};
namespace WJ
{
	CMainManage_login::CMainManage_login(ManageInfoStruct *pManageInfoStruct):CWJSSocketManage(70000)
	{
		m_pRedis = NULL;
		m_ManageInfoStruct = *pManageInfoStruct;
		m_uCheckTime = 180;
		m_update_now_playercount = 0;
	}

	CMainManage_login::~CMainManage_login()
	{
		UnInit_subclass();
	}

	CLUserInfoManage* CMainManage_login::GetCLUserInfoManage()
	{
		return &m_CLUserInfoManage;
	}

	///��ʼ������
	bool CMainManage_login::Init_subclass(const ManageInfoStruct &InitData)
	{
		m_CLUserInfoManage.Init_CLUserInfoManage(InitData.uMaxCount);
		m_pRedis = new WJ::CWJRedis(lsconfig().GetData()->redis_address,
			lsconfig().GetData()->redis_pass,
			lsconfig().GetData()->redis_port,
			8);
		assert(m_pRedis);

		return true;
	}
	///ȡ����ʼ������
	bool CMainManage_login::UnInit_subclass()
	{
		m_CLUserInfoManage.UnInit_CLUserInfoManage();
		m_CGameList.clear();
		m_CApplayManage.clear();
		m_socket_vector.clear();
		return true; 
	}
	///��������
	bool CMainManage_login::Start_subclass()
	{
		Init_subclass(m_ManageInfoStruct);
		// ����ʱ ��ȡ��Ϸ�б�
		m_CGameList.InitLoadFromDB(m_pRedis);
		// ע��Ҫ������socketmanage Ȼ������˿ڣ����򣬼����߳̽�����ѭ�� ����������
		Start_WJSSocketManage(1);

		QK_InfoLog("ls ���� = (0.0.0.0:%d)", m_ManageInfoStruct.uListenPort);
		AsyncListen("0.0.0.0", m_ManageInfoStruct.uListenPort, ePCClient);

		QK_InfoLog("ls (��gs)���� = (0.0.0.0:%d)", PORT_LS_TO_GS);
		AsyncListen("0.0.0.0", PORT_LS_TO_GS, eGameServer);

		SetTimer(E_P_TIME_ID_TEAM_VS, 4);
		SetTimer(E_P_TIME_ID_UPDATE_ONLINE_COUNT, 10);
		SetTimer(E_P_TIME_ID_HEART, 10);
		SetTimer(E_P_TIME_ID_UPDATE_USER_INFO, 5);
		return true;
	}
	///ֹͣ����
	bool CMainManage_login::Stop_subclass()
	{
		Stop_WJSSocketManage();
		if (m_pRedis)
		{
			m_pRedis->StopWorkers();
		}
		m_CLUserInfoManage.clear();
		m_CGameList.clear();
		m_CApplayManage.clear();
		m_socket_vector.clear();

		KillTimer(E_P_TIME_ID_TEAM_VS);
		KillTimer(E_P_TIME_ID_UPDATE_ONLINE_COUNT);
		KillTimer(E_P_TIME_ID_HEART);

		m_CTeamManage.delAll();
		return true;
	}

	//UINT uElapse ��λ��
	void CMainManage_login::SetTimer(TimerID uTimerID, UINT uElapse)
	{
		CWJTimer _Timer;

		_Timer.m_timerID = uTimerID;
		_Timer.m_interval = uElapse; // seconds
		_Timer.m_pCallBack_Time = this;
		m_timerMgr_h.AddTimer(&_Timer);
	}

	void CMainManage_login::KillTimer(TimerID uTimerID)
	{
		m_timerMgr_h.DelTimer(uTimerID);
	}

	int CMainManage_login::ProccessTimers()
	{
		return m_timerMgr_h.ProcessTimerEvents();
	}

	int CMainManage_login::ProccessRedisResults()
	{
		list<const CRedisCmdResultBase*>  results;
		m_pRedis->ProcessDbResults(results);
		int count = 0;
		while (!results.empty())
		{
			const CRedisCmdResultBase* pCmdRes = results.front();
			assert(pCmdRes);
			results.pop_front();
			if (pCmdRes->m_cmdType == E_TRANSACTION)
			{
				CRedisTransactionResult *pTrans = (CRedisTransactionResult*)pCmdRes;
				assert(pTrans);
				switch (pTrans->m_cmdId)
				{
					case Redis_Order_h_messid_load_hall:
					{
						CTransaction_load_hall *pTransObj = (CTransaction_load_hall *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_load_hall(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_apply:
					{
						CTransaction_apply *pTransObj = (CTransaction_apply *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_apply(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_apply_restitution:
					{
						CTransaction_apply_restitution *pTransObj = (CTransaction_apply_restitution *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_apply_restitution(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_team_create:
					{
						CTransaction_team_create *pTransObj = (CTransaction_team_create *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_team_create(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_team_join:
					{
						CTransaction_team_join *pTransObj = (CTransaction_team_join *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_team_join(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_team_leave:
					{
						CTransaction_team_leave *pTransObj = (CTransaction_team_leave *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_team_leave(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_team_dissolve:
					{
						CTransaction_team_dissolve *pTransObj = (CTransaction_team_dissolve *)pTrans->m_pArg;
						onRedisRes_Order_h_messid_team_dissolve(&pTransObj->m_out);
						WJ_SAFE_DELETE(pTransObj);
						break;
					}
					case Redis_Order_h_messid_update_userinfo:
					{
																 CTransaction_Update_Userinfo *pTransObj = (CTransaction_Update_Userinfo *)pTrans->m_pArg;
																 auto _iter = pTransObj->m_out_vector.begin();
																 while (_iter != pTransObj->m_out_vector.end())
																 {
																	 onRedisRes_Order_h_messid_update_userinfo(&(*_iter));
																	 ++_iter;
																 }																 
																 WJ_SAFE_DELETE(pTransObj);
					}break;
					default:
						break;
				}
			}
			delete pCmdRes;
			++count;
		}
		return count;
	}

	void CMainManage_login::OnNewConnection(CWJSSocket * pNewTCPSocket, void *pUserData)
	{
		UINT uIndex = pNewTCPSocket->GetSockID();
		pNewTCPSocket->SetKeep();

		// ���������һЩͳ�ƣ����������Լ���Ӵ���
		int nTag = pNewTCPSocket->GetType();
		if (nTag == ePCClient)
		{

		}
		else if (nTag == eGameServer)
		{

		}

		//�������ӳɹ���Ϣ
		Net_ConnectSuccess Message;
		Message.bLessVer = (BYTE)MAKELONG(3, 4);
		Message.bReserve[0] = 0;
		Message.bMaxVer = (BYTE)MAKELONG(3, 4);
		Message.bReserve[1] = 0;
		//�˴���hSocket���м��ܣ��ɿͻ��˽��ܲ��ڰ�ͷ��bReserve�ֶ��з�����У�顣����SECRET_KEYΪ��Կ�ɿͻ��Լ�������
		Message.i64CheckCode = (int)uIndex * 23 + SECRECTKEY;
		pNewTCPSocket->SendData(&Message, sizeof(Message), NET_M_CONNECT, ASS_NET_CONNECT_2, 0);

		QK_InfoLog("sdp_test �������ӳɹ���Ϣ");
	}

	bool CMainManage_login::ProccessNetMessage(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		// �ײ�socket��������ô��麯��  
		if (nullptr == pSocket || nullptr == pNetHead)
		{
			return false;
		}

		//	ULONG uAccessIP =1 ;
		UINT uIndex = pSocket->GetSockID();
		UINT tag = pSocket->GetType();

		//if (pNetHead->bMainID != NET_M_CONNECT && pNetHead->bAssistantID != ASS_NET_CONNECT_1)
		//{
		//	QK_InfoLog("ls ProccessNetMessage(bMainID=%d,bAssistantID=%d,bHandleCode=%d)��", pNetHead->bMainID, pNetHead->bAssistantID, pNetHead->bHandleCode);
		//	QK_InfoLog("ls ProccessNetMessage(uIndex=%d,tag=%d)��", uIndex, tag);
		//}

		//Ч�������������
		if ((pNetHead->bMainID == NET_M_CONNECT) && (pNetHead->bAssistantID == ASS_NET_CONNECT_1))
		{
			//������Ϣ�ݲ�����
			return true;
		}

		if (tag == ePCClient)
		{
			switch (pNetHead->bMainID)
			{
			case e_hs_mess_login:
			{
									return OnSocketRead_hs_mess_login(pSocket, pNetHead, pData, uSize);
			}break;
			case e_hs_mess_gamelist:
			{
									   return OnSocketRead_hs_mess_gamelist(pSocket, pNetHead, pData, uSize);
			}break;
			case e_hs_mess_apply:
			{
									return OnSocketRead_hs_mess_apply(pSocket, pNetHead, pData, uSize);
			}break;
			case e_hs_mess_team:
			{
								   return OnSocketRead_hs_mess_team(pSocket, pNetHead, pData, uSize);
			}break;
			default:
				return false;
			}
		}
		else if (tag == eGameServer)
		{
			switch (pNetHead->bMainID)
			{
			case e_pr_mess_roominfo:
			{
									   return OnSocketRead_pr_mess_roominfo(pSocket, pNetHead, pData, uSize);
			}break;
			case e_pr_mess_userinfo:
			{
									   return OnSocketRead_pr_mess_userinfo(pSocket, pNetHead, pData, uSize);
			}break;
			default:
				return false;
			}
		}

		return true;
	}

	//��½��Ϣ����
	bool CMainManage_login::OnSocketRead_hs_mess_login(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_hs_ass_login:
		{//�����ͨ��½
							   QK_InfoLog("sdp_test hall �յ������ͨ��½��Ϣ��");
							   struct_data_hs_mess_login* pCmd = (struct_data_hs_mess_login*)pData;
							   if (nullptr == pCmd || uSize != sizeof(struct_data_hs_mess_login))
							   {
								   return false;
							   }

							   CLoginUserInfo* puserold = m_CLUserInfoManage.GetOnLineUserInfo(pCmd->username);
							   if (nullptr != puserold)
							   {//����Ѿ�����
								   QK_InfoLog("sdp_test hall ����Ѿ����ߡ�");
								   pSocket->SendData(e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_online);
								   return true;
							   }

							   //��������
							   CTransaction_load_hall *pLoadHall = new CTransaction_load_hall;
							   assert(pLoadHall);
							   pLoadHall->m_in.uIndex = uIndex;
							   std::memcpy(&pLoadHall->m_in.data, pCmd, sizeof(struct_data_hs_mess_login));
							   m_pRedis->do_transaction(Redis_Order_h_messid_load_hall,
								   pSocket->GetSockID(),
								   onRedis_Order_h_messid_load_hall,
								   pLoadHall);

							   return true;
		}break;
		case e_hs_ass_login_WX:
		{//΢�ŵ�½
							   QK_InfoLog("sdp_test hall �յ����΢�ŵ�½��Ϣ��");
							   struct_data_hs_mess_login_WX* pCmd = (struct_data_hs_mess_login_WX*)pData;
							   if (nullptr == pCmd || uSize != sizeof(struct_data_hs_mess_login_WX))
							   {
								   QK_InfoLog("sdp_test hall �յ����΢�ŵ�½��Ϣ err1��");
								   return false;
							   }
							   QK_InfoLog("sdp_test hall �û���¼����wx userid=%d,username=%s,wx_key=%s", pCmd->userid, pCmd->username, pCmd->wx_key);
							   CLoginUserInfo* puserold = m_CLUserInfoManage.GetOnLineUserInfo(pCmd->userid);
							   if (nullptr != puserold)
							   {//����Ѿ�����
								   QK_InfoLog("sdp_test hall ����Ѿ����ߡ�");
								   pSocket->SendData(e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_online);
								   return true;
							   }

							   //��������
							   CTransaction_load_hall *pLoadHall = new CTransaction_load_hall;
							   assert(pLoadHall);
							   pLoadHall->m_in.uIndex = uIndex;
							   pLoadHall->m_in.bWX = true;
							   std::memcpy(&pLoadHall->m_in.data2, pCmd, sizeof(struct_data_hs_mess_login_WX));
							   m_pRedis->do_transaction(Redis_Order_h_messid_load_hall,
								   pSocket->GetSockID(),
								   onRedis_Order_h_messid_load_hall,
								   pLoadHall);
							   return true;
		}break;
		case e_hs_ass_login_c_finish:
		{
										QK_InfoLog("sdp_test hall �յ��ͻ��˸��߷�������ҵ�½�����Ϣ��");
										CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
										if (nullptr == puser)
										{
											QK_InfoLog("sdp_test hall ��Ҳ����ڡ�");
											return true;
										}

										//�������Ƿ��ڱ����У�����û�м��뷿��
										struct_applynode data;
										bool bexit_apply = m_CApplayManage.at(puser->GetUserData()->basedata.dwUserID, data);
										if (bexit_apply)
										{
											if (data.bAllot)
											{//�ڷ�����
												struct_data_hs_mess_apply_login_recome_playing cmd;
												cmd.ucontestid = data.ucontestid;
												cmd.roomid = data.uroomid;
												const ComRoomInfo* pRoom = m_CGameList.GetComRoomInfo(cmd.roomid);
												const ComContestInfo* pC = m_CGameList.GetComContestInfo(pRoom->contestid);
												if (nullptr != pRoom)
												{
													std::memcpy(cmd.strIP, pRoom->strIP, sizeof(cmd.strIP));
													cmd.uport = pRoom->uport;
													cmd.ereaid = pRoom->ereaid;
													cmd.gameid = pRoom->gameid;
													cmd.uContestKind = pC->uContestKind;//������������
												}
												MyDebugString("sdp_test", "hall ��Ҷ��ߺ���������Ѿ����뷿�䣬���͸���Ϣ���ͻ��ˡ�ereaid=%d, roomid=%d��%s:%d��", cmd.ereaid, cmd.roomid, cmd.strIP, cmd.uport);
												pSocket->SendData(&cmd, sizeof(struct_data_hs_mess_apply_login_recome_playing), e_hs_mess_apply, e_hs_ass_apply_login_recome_playing, 0);
											}
											else
											{//��û�ڷ�����
												QK_InfoLog("sdp_test hall ��Ҷ��ߺ�������ֻ�δ���뷿�䣬���͸���Ϣ���ͻ��ˡ�");
												onSendRoomListForContest(pSocket, data.ucontestid, e_hs_mess_apply, e_hs_ass_apply_login_recome_apply, e_hs_hcid_login_recome_apply_roomlist_part, e_hs_hcid_login_recome_apply_roomlist_finish);
											}
										}
										else
										{//���߿ͻ��ˣ���Ҳ�û�б����κα���
											QK_InfoLog("sdp_test hall ���߿ͻ��ˣ���Ҳ�û�б����κα�����");
											pSocket->SendData( e_hs_mess_apply, e_hs_ass_unapply, 0);
										}
										return true;
		}break;
		case e_gs_ass_login_c_force_out:
		{
										   //ֱ�ӶϿ��׽���
										   OutputDebugStringA("sdp_test CloseSocket(21)");
										   pSocket->CloseSocket();
										   return true;
		}break;
		default:
		{
				   break;
		}
		}

		return true;
	}

	//���ָ�������µ�roomlis�������͸��ͻ���
	void CMainManage_login::onSendRoomListForContest(CWJSSocket * pSocket, UINT contest, UINT bMainID, UINT bAssistantID, UINT bHandleCode_part, UINT bHandleCode_finish)
	{
		//��ȡ��������
		UINT uBeginPos = 0;
		bool bFinish = false;
		char bBuffer[1500] = { 0 };
		UINT uCopyCount = 0;
		UINT uCount = m_CGameList.Getroom_nodeCount();
		do
		{
			UINT uCopyPos = m_CGameList.FillGameRoom(bBuffer, sizeof(bBuffer), contest, uBeginPos, bFinish, uCopyCount);
			uBeginPos += uCopyPos;

			if (0 == uCount)
			{
				bFinish = true;
			}

			if (bFinish)
			{
				pSocket->SendData(bBuffer, uCopyCount*sizeof(ComRoomInfo), bMainID, bAssistantID, bHandleCode_finish);
			}
			else
			{
				pSocket->SendData(bBuffer, uCopyCount*sizeof(ComRoomInfo), bMainID, bAssistantID, bHandleCode_part);
			}
			--uCount;
		} while (bFinish == false);
	}

	//��Ϸ�б���Ϣ����
	bool CMainManage_login::OnSocketRead_hs_mess_gamelist(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		switch (pNetHead->bAssistantID)
		{
		case e_hs_ass_gamelist_game:
		{//��ȡ��Ϸ	
									   //��ȡ��������
									   UINT uBeginPos = 0;
									   bool bFinish = false;
									   char bBuffer[1500] = { 0 };
									   UINT uCopyCount = 0;
									   UINT uCount = m_CGameList.Getgame_nodeCount();
									   do
									   {
										   UINT uCopyPos = m_CGameList.FillGameName(bBuffer, sizeof(bBuffer), uBeginPos, bFinish, uCopyCount);
										   uBeginPos += uCopyPos;

										   if (0 == uCount)
										   {
											   bFinish = true;
										   }

										   if (bFinish)
										   {
											   //OutputDebugStringA("sdp_test2 ��ȡ��Ϸ(bFinish)");
											   pSocket->SendData(bBuffer, uCopyCount*sizeof(ComGameinfo), e_hs_mess_gamelist, e_hs_ass_gamelist_game, e_hs_hcid_gamelist_finish);
										   }
										   else
										   {
											   //OutputDebugStringA("sdp_test2 ��ȡ��Ϸ(bunFinish)");
											   pSocket->SendData(bBuffer, uCopyCount*sizeof(ComGameinfo), e_hs_mess_gamelist, e_hs_ass_gamelist_game, e_hs_hcid_gamelist_part);
										   }
										   --uCount;
									   } while (bFinish == false);

									   return true;
		}break;
		case e_hs_ass_gamelist_erea:
		{//	��ȡ����
									   struct_data_hs_ass_gamelist_erea* pCmd = (struct_data_hs_ass_gamelist_erea*)pData;
									   if (nullptr == pCmd)
									   {
										   return false;
									   }

									   //��ȡ��������
									   UINT uBeginPos = 0;
									   bool bFinish = false;
									   char bBuffer[1500] = { 0 };
									   UINT uCopyCount = 0;
									   UINT uCount = m_CGameList.Geterea_nodeCount();
									   do
									   {
										   UINT uCopyPos = m_CGameList.FillErea(bBuffer, sizeof(bBuffer), pCmd->gameid, uBeginPos, bFinish, uCopyCount);
										   uBeginPos += uCopyPos;

										   if (0 == uCount)
										   {
											   bFinish = true;
										   }

										   if (bFinish)
										   {
											   //OutputDebugStringA("sdp_test2 ��ȡ����(bFinish)");
											  pSocket->SendData(bBuffer, uCopyCount*sizeof(ComEreainfo), e_hs_mess_gamelist, e_hs_ass_gamelist_erea, e_hs_hcid_gamelist_finish);
										   }
										   else
										   {
											   //OutputDebugStringA("sdp_test2 ��ȡ����(bunFinish)");
											   pSocket->SendData(bBuffer, uCopyCount*sizeof(ComEreainfo), e_hs_mess_gamelist, e_hs_ass_gamelist_erea, e_hs_hcid_gamelist_part);
										   }

										   --uCount;
									   } while (bFinish == false);

									   return true;
		}break;
		case e_hs_ass_gamelist_contest:
		{//	��ȡ����
										  struct_data_hs_ass_gamelist_contest* pCmd = (struct_data_hs_ass_gamelist_contest*)pData;
										  if (nullptr == pCmd)
										  {
											  return false;
										  }

										  //��ȡ��������
										  UINT uBeginPos = 0;
										  bool bFinish = false;
										  char bBuffer[1500] = { 0 };
										  UINT uCopyCount = 0;
										  UINT uCount = m_CGameList.Getcontest_nodeCount();
										  do
										  {
											  UINT uCopyPos = m_CGameList.FillGameContest(bBuffer, sizeof(bBuffer), pCmd->ereaid, uBeginPos, bFinish, uCopyCount);
											  uBeginPos += uCopyPos;

											  if (0 == uCount)
											  {
												  bFinish = true;
											  }

											  if (bFinish)
											  {
												  //OutputDebugStringA("sdp_test2 ��ȡ����(bFinish)");
												  pSocket->SendData(bBuffer, uCopyCount*sizeof(ComContestInfo), e_hs_mess_gamelist, e_hs_ass_gamelist_contest, e_hs_hcid_gamelist_finish);
											  }
											  else
											  {
												  //OutputDebugStringA("sdp_test2 ��ȡ����(bunFinish)");
												  pSocket->SendData(bBuffer, uCopyCount*sizeof(ComContestInfo), e_hs_mess_gamelist, e_hs_ass_gamelist_contest, e_hs_hcid_gamelist_part);
											  }
											  --uCount;
										  } while (bFinish == false);

										  return true;
		}break;
		case e_hs_ass_gamelist_contest_online:
		{//��ȡ�����ڵ�(���ڵ�����)
												 struct_data_hs_ass_gamelist_contest_online* pCmd = (struct_data_hs_ass_gamelist_contest_online*)pData;
												 if (nullptr == pCmd)
												 {
													 return false;
												 }

												 //��ñ�������
												 const ComContestInfo*p = m_CGameList.GetComContestInfo(pCmd->contestid);
												 if (nullptr == p)
												 {
													 return true;
												 }

												 ComContestInfo cmd;
												 std::memcpy(&cmd, p, sizeof(ComContestInfo));
												 pSocket->SendData( &cmd, sizeof(ComContestInfo), e_hs_mess_gamelist, e_hs_ass_gamelist_contest, e_hs_hcid_gamelist_finish);
												 return true;
		}break;
		case e_hs_ass_gamelist_room:
		{//��ȡ����									  
									   struct_data_hs_ass_gamelist_room* pCmd = (struct_data_hs_ass_gamelist_room*)pData;
									   if (nullptr == pCmd)
									   {
										   return false;
									   }

									   onSendRoomListForContest(pSocket, pCmd->contestid, e_hs_mess_gamelist, e_hs_ass_gamelist_room, e_hs_hcid_gamelist_part, e_hs_hcid_gamelist_finish);

									   return true;
		}break;
		case e_hs_ass_gamelist_onlien_count:
		{
				pSocket->SendData(nullptr, 0, e_hs_mess_gamelist, e_hs_ass_gamelist_onlien_count, m_update_now_playercount);
				return true;
		}break;
		default:
		{
				   return false;
		}
		}

		return true;
	}

	//������Ϣ����
	bool CMainManage_login::OnSocketRead_hs_mess_apply(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_hs_ass_apply:
		{
							   QK_InfoLog("sdp_test hall �����������");
							   struct_data_hs_mess_apply* p = (struct_data_hs_mess_apply*)pData;
							   if (nullptr == p || uSize != sizeof(struct_data_hs_mess_apply))
							   {
								   return false;
							   }

							   //����Ƿ��˲�����
							   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
							   if (nullptr == puser)
							   {
								   QK_InfoLog("sdp_test hall err ��Ҳ����ڡ�");
								   return false;
							   }

							   if (!puser->bCanChangeUserStation(US_P_Applying))
							   {
								   QK_InfoLog("sdp_test hall err ��Ҳ��ܴ��ڱ�����%d", puser->GetUserData()->basedata.bUserState);
								   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_unknown);
								   return true;
							   }

							   //�������Ƿ����
							   const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
							   if (nullptr == pContest)
							   {
								   QK_InfoLog("sdp_test hall err ���������ڣ�%d����", p->ucontestid);
								   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_contest_inexistence);
								   return true;
							   }
							   //�������Ƿ��з���
							   if (!m_CGameList.ExistRoomForContest(p->ucontestid))
							   {
								   QK_InfoLog("sdp_test hall err ����û�з��䡣");
								  SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_room_inexistence);
								   return true;
							   }

							   //�ѱ�����δ����
							   if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
							   {
								   QK_InfoLog("sdp_test hall err ����ѱ�����δ������");
								   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_applyed);
								   return true;
							   }

							   QK_InfoLog("sdp_test hall ������(userid=%d,uContestKind=%d,i64Money=%I64d,dwPoint=%d,_i64cost=%I64d)��"
								   , puser->GetUserData()->basedata.dwUserID
								   , pContest->uContestKind
								   , puser->GetUserData()->basedata.i64Money
								   , puser->GetUserData()->basedata.dwPoint
								   , pContest->_i64cost);
							   if (US_ContestKind_free == pContest->uContestKind)
							   {
								   struct_applynode cmd;
								   cmd.ucontestid = p->ucontestid;
								   cmd.userid = puser->GetUserData()->basedata.dwUserID;
								   m_CApplayManage.add(cmd);
								   QK_InfoLog("sdp_test hall �����ɹ���");
								   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_suc);
		
							   }
							   else if (US_ContestRule_happy == pContest->uContestKind)
							   {//��ⱨ���ѹ�����(���)
								   if (puser->GetUserData()->basedata.i64Money < pContest->_i64cost)
								   {
									   QK_InfoLog("sdp_test hall err �����ѹ�����(���)��");
									   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_nomoney);
									   return true;
								   }						   
								   
								   puser->SetUserState(US_P_Applying);
								   /*
								   RRLine_c_apply rediscommand;
								   rediscommand.changemoney = pContest->_i64cost;
								   rediscommand.uContestKind = pContest->uContestKind;
								   rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
								   rediscommand.ucontestid = p->ucontestid;
								   m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply), uIndex);
								   */
								   CTransaction_apply *pTransApply = new CTransaction_apply;
								   assert(pTransApply);
								   pTransApply->m_in.uIndex = uIndex;
								   pTransApply->m_in.changemoney = pContest->_i64cost;
								   pTransApply->m_in.uContestKind = pContest->uContestKind;
								   pTransApply->m_in.userid = puser->GetUserData()->basedata.dwUserID;
								   pTransApply->m_in.ucontestid = p->ucontestid;
								   m_pRedis->do_transaction(Redis_Order_h_messid_apply,
									   uIndex,
									   onRedis_Order_h_messid_apply,
									   pTransApply);
							   }
							   else if (US_ContestRule_point == pContest->uContestKind)
							   {//��ⱨ���ѹ�����(����)
								   if (puser->GetUserData()->basedata.dwPoint < pContest->_i64cost)
								   {
									   QK_InfoLog("sdp_test hall err �����ѹ�����(����)��");
									   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_nomoney);
									   return true;
								   }
								   puser->SetUserState(US_P_Applying);
								   /*
								   puser->SetUserState(US_P_Applying);
								   RRLine_c_apply rediscommand;
								   rediscommand.changepoint = pContest->_i64cost;
								   rediscommand.uContestKind = pContest->uContestKind;
								   rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
								   rediscommand.ucontestid = p->ucontestid;
								   m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply), uIndex);
								   */
								   CTransaction_apply *pTransApply = new CTransaction_apply;
								   assert(pTransApply);
								   pTransApply->m_in.uIndex = uIndex;
								   pTransApply->m_in.changepoint = pContest->_i64cost;
								   pTransApply->m_in.uContestKind = pContest->uContestKind;
								   pTransApply->m_in.userid = puser->GetUserData()->basedata.dwUserID;
								   pTransApply->m_in.ucontestid = p->ucontestid;
								   m_pRedis->do_transaction(Redis_Order_h_messid_apply,
									   uIndex,
									   onRedis_Order_h_messid_apply,
									   pTransApply);
							   }

							   return true;
		}break;
		case e_hs_ass_apply_restitution:
		{
										   QK_InfoLog("sdp_test hall �յ�����˷�����");
										   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
										   if (nullptr == puser)
										   {
											   QK_InfoLog("sdp_test hall �˷��У���Ҳ����ڣ�����");
											   return false;
										   }

										   if (!puser->bCanChangeUserStation(US_P_unApplying))
										   {
											   QK_InfoLog("sdp_test hall err ��Ҳ��ܴ����˷ѡ�%d", puser->GetUserData()->basedata.bUserState);
											   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_err);
											   return true;
										   }

										   struct_applynode data;
										   if (!m_CApplayManage.at(puser->GetUserData()->basedata.dwUserID, data))
										   {
											   QK_InfoLog("sdp_test hall ���߿ͻ��ˣ���Ҳ�û�б����κα�����");
											   //���߿ͻ��ˣ���Ҳ�û�б����κα���					
											  SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_unapply, 0);
											   return true;
										   }

										   UINT usocketindex;										   
										   if (m_CGameList.GetRoomSocketID(data.uroomid, usocketindex))
										   {//�Ѿ����뷿�䣬Ҫѯ�ʷ���
											   QK_InfoLog("sdp_test hall �Ѿ����뷿�䣬Ҫѯ�ʷ��䡣");
											   struct_data_pr_userinfo_apply_restitution cmd;
											   cmd.userid = puser->GetUserData()->basedata.dwUserID;
											   SendData(usocketindex, &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_apply_restitution, 0);
										   }
										   else
										   {//û�ڷ��䣬��ֱ��ͬ��
											   //�������Ƿ����
											   const ComContestInfo* pContest = m_CGameList.GetComContestInfo(data.ucontestid);
											   if (nullptr != pContest)
											   {//Ǯ�˸����
												   puser->SetUserState(US_P_unApplying);
												   /*
												   RRLine_c_apply_restitution rediscommand;
												   rediscommand.changepoint = pContest->_i64cost;
												   rediscommand.uContestKind = pContest->uContestKind;
												   rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
												   rediscommand.ucontestid = data.ucontestid;
												   QK_InfoLog("sdp_test hall �б�����û�ڷ��䣬���ѯredis�����˷ѡ�");
												   m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply_restitution), uIndex);
												   */
												   CTransaction_apply_restitution *pTransResti = new CTransaction_apply_restitution;
												   assert(pTransResti);
												   pTransResti->m_in.uIndex = uIndex;
												   pTransResti->m_in.changepoint = pContest->_i64cost;
												   pTransResti->m_in.uContestKind = pContest->uContestKind;
												   pTransResti->m_in.userid = puser->GetUserData()->basedata.dwUserID;
												   pTransResti->m_in.ucontestid = data.ucontestid;
												   QK_InfoLog("sdp_test hall �б�����û�ڷ��䣬���ѯredis�����˷ѡ�");
												   m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
													   uIndex,
													   onRedis_Order_h_messid_apply_restitution,
													   pTransResti);

											   }
											   else
											   {
												   QK_InfoLog("sdp_test hall û�����������ܾ��˷ѡ�");
												   SendData(uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_err);
											   }											   
										   }

										   return true;
		}break;
		default:
		{
				   break;
		}
		}

		return false;
	}

	//�Ŷ���Ϣ����
	bool CMainManage_login::OnSocketRead_hs_mess_team(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_hs_ass_team_create:
		{//�����Ŷ�
									 QK_InfoLog("sdp_test hall �յ���Ҵ����Ŷ�����");
									 struct_data_hs_mess_team_create* p = (struct_data_hs_mess_team_create*)pData;
									 if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_create))
									 {
										 return false;
									 }

									 //����Ƿ��˲�����
									 CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									 if (nullptr == puser)
									 {
										 return false;
									 }

									 if (!puser->bCanChangeUserStation(US_P_CreateTeaming))
									 {
										 QK_InfoLog("sdp_test hall err ��Ҳ��ܴ��ڴ����Ŷӡ�%d", puser->GetUserData()->basedata.bUserState);
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_unknown);
										 return true;
									 }

									 //�������Ƿ����
									 const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									 if (nullptr == pContest)
									 {
										 QK_InfoLog("sdp_test hall err ���������ڡ�");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_contest_inexistence);
										 return true;
									 }

									 //����ģʽ����
									 if (US_ContestRule_team != pContest->uContestRule)
									 {
										 QK_InfoLog("sdp_test hall err ����ģʽ�����Ŷ�����%d����", pContest->uContestRule);
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_contest_rule);
										 return true;
									 }

									 //�������Ƿ��з���
									 if (!m_CGameList.ExistRoomForContest(p->ucontestid))
									 {
										 QK_InfoLog("sdp_test hall err ����û�з��䡣");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_room_inexistence);
										 return true;
									 }

									 //�ѱ�����δ����
									 if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
									 {
										 QK_InfoLog("sdp_test hall err ����ѱ�����δ������");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_applyed);
										 return true;
									 }

									 TeamStruct _TeamStruct;
									 if (m_CTeamManage.find(puser->GetUserData()->basedata.dwUserID, _TeamStruct))
									 {//�����Ŷ��У�δ��ɢ
										 QK_InfoLog("sdp_test hall err �����Ŷ��У�δ��ɢ��");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_exist);
										 return true;
									 }

									 LLONG _i64cost = 0;
									 if (TeamPayType_AA == p->m_TeamPayType)
									 {
										 _i64cost = pContest->_i64cost / (UINT)pContest->uRoomRule;
									 }
									 else if (TeamPayType_Captain == p->m_TeamPayType)
									 {
										 _i64cost = pContest->_i64cost;
									 }
									 else
									 {
										 QK_InfoLog("sdp_test hall err ֧������δ֪(%d)��", p->m_TeamPayType);
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_PayType);
										 return true;
									 }

									 if (US_ContestKind_free == pContest->uContestKind)
									 {
										 TeamStruct cmd;
										 cmd.m_uCaptain_userid = puser->GetUserData()->basedata.dwUserID;
										 cmd.m_TeamPayType = p->m_TeamPayType;
										 cmd.m_uContest = p->ucontestid;
										 cmd.m_uTemaPlayerNum = (UINT)pContest->uRoomRule;
										 if (!m_CTeamManage.add(cmd))
										 {//�Ѵ�����δ��ɢ
											 QK_InfoLog("sdp_test hall err �Ѵ�����δ��ɢ2��");
											SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_exist);
											 return true;
										 }

										 QK_InfoLog("sdp_test hall �����Ŷӳɹ���");
										 SendData(uIndex, &cmd, sizeof(struct_data_hs_mess_team_create_res), e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_suc);
									 }
									 else if (US_ContestRule_happy == pContest->uContestKind)
									 {//��ⴴ���Ŷӷѹ�����(���)
										 if (puser->GetUserData()->basedata.i64Money < _i64cost)
										 {
											 QK_InfoLog("sdp_test hall err �����Ŷӷѹ�����(���)��");
											 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_nomoney);
											 return true;
										 }

										 QK_InfoLog("sdp_test hall ��ʼִ��redis������");
										 puser->SetUserState(US_P_CreateTeaming);
										 /*
										 RRLine_c_team_create rediscommand;
										 rediscommand.changemoney = _i64cost;
										 rediscommand.uContestKind = pContest->uContestKind;
										 rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
										 rediscommand.ucontestid = p->ucontestid;
										 rediscommand.m_TeamPayType = p->m_TeamPayType;
										 rediscommand.uRoomRule = pContest->uRoomRule;
										 m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_create), uIndex);
										 */
										 CTransaction_team_create *pTransTeamCreat = new CTransaction_team_create;
										 assert(pTransTeamCreat);
										 pTransTeamCreat->m_in.uIndex = uIndex;
										 pTransTeamCreat->m_in.changemoney = _i64cost;
										 pTransTeamCreat->m_in.uContestKind = pContest->uContestKind;
										 pTransTeamCreat->m_in.userid = puser->GetUserData()->basedata.dwUserID;
										 pTransTeamCreat->m_in.ucontestid = p->ucontestid;
										 pTransTeamCreat->m_in.m_TeamPayType = p->m_TeamPayType;
										 pTransTeamCreat->m_in.uRoomRule = pContest->uRoomRule;
										 m_pRedis->do_transaction(Redis_Order_h_messid_team_create,
											 uIndex,
											 onRedis_Order_h_messid_team_create,
											 pTransTeamCreat);
									 }
									 else if (US_ContestRule_point == pContest->uContestKind)
									 {//��ⴴ���Ŷӷѹ�����(����)
										 if (puser->GetUserData()->basedata.dwPoint < _i64cost)
										 {
											 QK_InfoLog("sdp_test hall err �����Ŷӷѹ�����(����)��");
											SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_nomoney);
											 return true;
										 }

										 QK_InfoLog("sdp_test hall ��ʼִ��redis������");
										 puser->SetUserState(US_P_CreateTeaming);
										 /*
										 RRLine_c_team_create rediscommand;
										 rediscommand.changepoint = _i64cost;
										 rediscommand.uContestKind = pContest->uContestKind;
										 rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
										 rediscommand.ucontestid = p->ucontestid;
										 rediscommand.m_TeamPayType = p->m_TeamPayType;
										 rediscommand.uRoomRule = pContest->uRoomRule;
										 m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_create), uIndex);
										 */
										 CTransaction_team_create *pTransTeamCreat = new CTransaction_team_create;
										 assert(pTransTeamCreat);
										 pTransTeamCreat->m_in.uIndex = uIndex;
										 pTransTeamCreat->m_in.changepoint = _i64cost;
										 pTransTeamCreat->m_in.uContestKind = pContest->uContestKind;
										 pTransTeamCreat->m_in.userid = puser->GetUserData()->basedata.dwUserID;
										 pTransTeamCreat->m_in.ucontestid = p->ucontestid;
										 pTransTeamCreat->m_in.m_TeamPayType = p->m_TeamPayType;
										 pTransTeamCreat->m_in.uRoomRule = pContest->uRoomRule;
										 m_pRedis->do_transaction(Redis_Order_h_messid_team_create,
											 uIndex,
											 onRedis_Order_h_messid_team_create,
											 pTransTeamCreat);
									 }
		}break;
		case e_hs_ass_team_join:
		{//�����Ŷ�
									QK_InfoLog("sdp_test hall �յ���Ҽ����Ŷ�����");
									struct_data_hs_mess_team_join* p = (struct_data_hs_mess_team_join*)pData;
									if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_join))
									{
										return false;
									}

									//����Ƿ��˲�����
									CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									if (nullptr == puser)
									{
										return false;
									}

									if (!puser->bCanChangeUserStation(US_P_AddTeaming))
									{
										QK_InfoLog("sdp_test hall err ��Ҳ��ܴ��ڼ����Ŷӡ�%d", puser->GetUserData()->basedata.bUserState);
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_unknown);
										return true;
									}

									//�������Ƿ����
									const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									if (nullptr == pContest)
									{
										QK_InfoLog("sdp_test hall err ���������ڡ�");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_contest_inexistence);
										return true;
									}

									//����ģʽ����
									if (US_ContestRule_team != pContest->uContestRule)
									{
										QK_InfoLog("sdp_test hall err ����ģʽ�����Ŷ�����%d����", pContest->uContestRule);
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_contest_rule);
										return true;
									}

									//�ѱ�����δ����
									if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
									{
										QK_InfoLog("sdp_test hall err ����ѱ�����δ������");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_applyed);
										return true;
									}

									TeamStruct _TeamStruct;
									if (m_CTeamManage.find(puser->GetUserData()->basedata.dwUserID, _TeamStruct))
									{//�����Ŷ��У�δ��ɢ
										QK_InfoLog("sdp_test hall err �����Ŷ��У�δ��ɢ��");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_exist);
										return true;
									}

									std::memset(&_TeamStruct, 0, sizeof(TeamStruct));

									LLONG _i64cost = 0;
									if (!m_CTeamManage.find(p->m_Team_password, _TeamStruct))
									{//�ŶӰ��Ŵ���
										QK_InfoLog("sdp_test hall err �ŶӰ��Ŵ���");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_password);
										return true;
									}
									else
									{
										if (TeamPayType_AA == _TeamStruct.m_TeamPayType)
										{
											_i64cost = pContest->_i64cost / (UINT)pContest->uRoomRule;
										}
										else if (TeamPayType_Captain == _TeamStruct.m_TeamPayType)
										{
											_i64cost = 0;
										}
									}
																		
									if (US_ContestKind_free == pContest->uContestKind)
									{										
										if (m_CTeamManage.bFull(_TeamStruct.m_Team_password))
										{//��������������ʧ��
											QK_InfoLog("sdp_test hall err ��������������ʧ�ܡ�");
											SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_full);
											return true;
										}

										QK_InfoLog("sdp_test hall �����Ŷӳɹ���");
										m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password);
										std::memset(&_TeamStruct, 0, sizeof(TeamStruct));
										m_CTeamManage.find(p->m_Team_password, _TeamStruct);
										SendData(uIndex, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
										TeamSendData(puser->GetUserData()->basedata.dwUserID, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
									}
									else if (US_ContestRule_happy == pContest->uContestKind)
									{//�������Ŷӷѹ�����(���)
										if (0 == _i64cost)
										{
											if (m_CTeamManage.bFull(_TeamStruct.m_Team_password))
											{//��������������ʧ��
												QK_InfoLog("sdp_test hall err ��������������ʧ��2��");
												SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_full);
												return true;
											}

											QK_InfoLog("sdp_test hall �����Ŷӳɹ�2��");
											m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password);
											std::memset(&_TeamStruct, 0, sizeof(TeamStruct));
											m_CTeamManage.find(p->m_Team_password, _TeamStruct);
											SendData(uIndex, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
											TeamSendData(puser->GetUserData()->basedata.dwUserID, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
										}
										else
										{
											if (puser->GetUserData()->basedata.i64Money < _i64cost)
											{
												QK_InfoLog("sdp_test hall err �����Ŷӷѹ�����(���)��");
												SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_nomoney);
												return true;
											}

											QK_InfoLog("sdp_test hall ��ʼִ��redis������");
											if (m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password))
											{
												puser->SetUserState(US_P_AddTeaming);
												/*
												RRLine_c_team_join rediscommand;
												rediscommand.changemoney = _i64cost;
												rediscommand.uContestKind = pContest->uContestKind;
												rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
												rediscommand.ucontestid = pContest->contestid;
												std::memcpy(rediscommand.m_Team_password, p->m_Team_password, sizeof(rediscommand.m_Team_password));
												m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_join), uIndex);
												*/
												CTransaction_team_join *pTransTeamJoin = new CTransaction_team_join;
												assert(pTransTeamJoin);
												pTransTeamJoin->m_in.uIndex = uIndex;
												pTransTeamJoin->m_in.changemoney = _i64cost;
												pTransTeamJoin->m_in.uContestKind = pContest->uContestKind;
												pTransTeamJoin->m_in.userid = puser->GetUserData()->basedata.dwUserID;
												pTransTeamJoin->m_in.ucontestid = pContest->contestid;
												std::memcpy(pTransTeamJoin->m_in.m_Team_password, p->m_Team_password, sizeof(pTransTeamJoin->m_in.m_Team_password));
												m_pRedis->do_transaction(Redis_Order_h_messid_team_join,
													uIndex,
													onRedis_Order_h_messid_team_join,
													pTransTeamJoin);
											}
										}										
									}
									else if (US_ContestRule_point == pContest->uContestKind)
									{//�������Ŷӷѹ�����(����)
										if (0 == _i64cost)
										{
											if (m_CTeamManage.bFull(_TeamStruct.m_Team_password))
											{//��������������ʧ��
												QK_InfoLog("sdp_test hall err ��������������ʧ��3��");
												SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_full);
												return true;
											}

											QK_InfoLog("sdp_test hall �����Ŷӳɹ�3��");
											m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password);
											std::memset(&_TeamStruct, 0, sizeof(TeamStruct));
											m_CTeamManage.find(p->m_Team_password, _TeamStruct);
											SendData(uIndex, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
											TeamSendData(puser->GetUserData()->basedata.dwUserID, &_TeamStruct, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
										}
										else
										{
											if (puser->GetUserData()->basedata.dwPoint < _i64cost)
											{
												QK_InfoLog("sdp_test hall err �����Ŷӷѹ�����(����)��");
												SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_nomoney);
												return true;
											}

											QK_InfoLog("sdp_test hall ��ʼִ��redis������");
											if (m_CTeamManage.userAdd(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password))
											{//�ȼӽ�ȥ���Ƚ�����������ж�ʧ�ܾ�ɾ��Ҳ����
												puser->SetUserState(US_P_AddTeaming);
												/*
												RRLine_c_team_join rediscommand;
												rediscommand.changepoint = _i64cost;
												rediscommand.uContestKind = pContest->uContestKind;
												rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
												rediscommand.ucontestid = pContest->contestid;
												std::memcpy(rediscommand.m_Team_password, p->m_Team_password, sizeof(rediscommand.m_Team_password));
												m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_join), uIndex);
												*/
												CTransaction_team_join *pTransTeamJoin = new CTransaction_team_join;
												assert(pTransTeamJoin);
												pTransTeamJoin->m_in.uIndex = uIndex;
												pTransTeamJoin->m_in.changepoint = _i64cost;
												pTransTeamJoin->m_in.uContestKind = pContest->uContestKind;
												pTransTeamJoin->m_in.userid = puser->GetUserData()->basedata.dwUserID;
												pTransTeamJoin->m_in.ucontestid = pContest->contestid;
												std::memcpy(pTransTeamJoin->m_in.m_Team_password, p->m_Team_password, sizeof(pTransTeamJoin->m_in.m_Team_password));
												m_pRedis->do_transaction(Redis_Order_h_messid_team_join,
													uIndex,
													onRedis_Order_h_messid_team_join,
													pTransTeamJoin);
											}
										}										
									}
		}break;
		case e_hs_ass_team_leave:
		{//�˳��Ŷ�									
									QK_InfoLog("sdp_test hall �յ�����˳��Ŷ�����");
									struct_data_hs_mess_team_leave* p = (struct_data_hs_mess_team_leave*)pData;
									if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_leave))
									{
										return false;
									}

									//����Ƿ��˲�����
									CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									if (nullptr == puser)
									{
										return false;
									}

									if (!puser->bCanChangeUserStation(US_P_LeaveTeaming))
									{
										QK_InfoLog("sdp_test hall err ��Ҳ��ܴ����˳��Ŷӡ�%d", puser->GetUserData()->basedata.bUserState);
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_unknown);
										return true;
									}

									//�������Ƿ����
									const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									if (nullptr == pContest)
									{
										QK_InfoLog("sdp_test hall err ���������ڡ�");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_contest_inexistence);
										return true;
									}

									//����ģʽ����
									if (US_ContestRule_team != pContest->uContestRule)
									{
										QK_InfoLog("sdp_test hall err ����ģʽ�����Ŷ�����%d����", pContest->uContestRule);
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_contest_rule);
										return true;
									}

									//�ѱ�����δ����
									if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
									{
										QK_InfoLog("sdp_test hall err ����ѱ�����δ������");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_applyed);
										return true;
									}

									TeamStruct _TeamStruct;
									bool bfind = m_CTeamManage.find(puser->GetUserData()->basedata.dwUserID, _TeamStruct);
									if (bfind && _TeamStruct.m_uCaptain_userid == puser->GetUserData()->basedata.dwUserID)
									{//�����Ŷ��У����Լ��Ƕӳ����Լ��������Լ��������Ŷ�
										QK_InfoLog("sdp_test hall err �����Ŷ��У����Լ��Ƕӳ����Լ��������Լ��������Ŷӡ�");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_created);
										return true;
									}

									//�ж�����ǲ������ڵ��Ŷ��Ѿ���ʼvs��
									for (int i = 0; i < m_CTeamManage.Count_VS(); ++i)
									{
										TeamStruct_VS _TeamStruct_VS;
										m_CTeamManage.at_vs(i, _TeamStruct_VS);
										if (0 == strcmp(_TeamStruct_VS._TeamStruct[0].m_Team_password, p->m_Team_password)
											|| 0 == strcmp(_TeamStruct_VS._TeamStruct[1].m_Team_password, p->m_Team_password))
										{
											QK_InfoLog("sdp_test hall �Ŷ�����vs,�����뿪��");
											SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_playing);
											return true;
										}
									}

									TeamStruct _TeamStruct2;
									bool bfind2 = m_CTeamManage.find(p->m_Team_password, _TeamStruct2);
									if (0 != strcmp(p->m_Team_password, _TeamStruct.m_Team_password)
										|| 0 != strcmp(p->m_Team_password, _TeamStruct2.m_Team_password)
										)
									{//����û������İ��������⣬�;ܾ�
										QK_InfoLog("sdp_test hall err ����û������İ��������⣬�;ܾ�,�ŶӰ��Ŵ���");
										bfind2 = false;
									}

									LLONG _i64cost = 0;
									if (!bfind2)
									{//�ŶӰ��Ŵ���
										QK_InfoLog("sdp_test hall err �ŶӰ��Ŵ���");
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_password);
										return true;
									}
									else
									{
										if (TeamPayType_AA == _TeamStruct.m_TeamPayType)
										{
											_i64cost = pContest->_i64cost / (UINT)pContest->uRoomRule;
										}
										else if (TeamPayType_Captain == _TeamStruct.m_TeamPayType)
										{
											_i64cost = 0;
										}
									}

									bool bUnNeedMoney = false;
									if (US_ContestKind_free == pContest->uContestKind)
									{
										bUnNeedMoney = true;										
									}
									else if (US_ContestRule_happy == pContest->uContestKind)
									{//�������Ŷӷѹ�����(���)
										if (0 == _i64cost)
										{
											bUnNeedMoney = true;
										}
									}
									else if (US_ContestRule_point == pContest->uContestKind)
									{//�������Ŷӷѹ�����(����)
										if (0 == _i64cost)
										{
											bUnNeedMoney = true;
										}
									}
									else
									{//�������������
										SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_unknown);
										return true;
									}

									if (bUnNeedMoney)
									{
										QK_InfoLog("sdp_test hall �˳��Ŷӳɹ���");
										struct_data_hs_mess_team_leave_res cmd;
										cmd.userid = puser->GetUserData()->basedata.dwUserID;
										m_CTeamManage.userDel(puser->GetUserData()->basedata.dwUserID, _TeamStruct.m_Team_password);
										SendData(uIndex, &cmd, sizeof(struct_data_hs_mess_team_leave_res), e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_suc);
										UINT uTemaPlayerNum;//��������
										UINT Userid[TEAM_USER_NUM];//ÿ���˵�userid
										m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
										for (int i = 0; i < uTemaPlayerNum; ++i)
										{
											CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
											if (nullptr != puser)
											{
												SendData(puser->GetSocketIndex(), &cmd, sizeof(struct_data_hs_mess_team_leave_res), e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_suc);
											}
										}
									}
									else
									{
										QK_InfoLog("sdp_test hall err ��ʼִ��redis������");	
										puser->SetUserState(US_P_LeaveTeaming);
										/*
										RRLine_c_team_leave rediscommand;
										if (US_ContestRule_happy == pContest->uContestKind)
										{//�����Ŷӷ�(���)
											rediscommand.changemoney = _i64cost;
										}
										if (US_ContestRule_point == pContest->uContestKind)
										{//�����Ŷӷ�(����)
											rediscommand.changepoint = _i64cost;
										}
										rediscommand.uContestKind = pContest->uContestKind;
										rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
										rediscommand.ucontestid = pContest->contestid;
										std::memcpy(rediscommand.m_Team_password, p->m_Team_password, sizeof(rediscommand.m_Team_password));										
										m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_leave), uIndex);
										*/
										CTransaction_team_leave *pTransTeamLeave = new CTransaction_team_leave;
										assert(pTransTeamLeave);
										pTransTeamLeave->m_in.uIndex = uIndex;
										if (US_ContestRule_happy == pContest->uContestKind)
										{//�����Ŷӷ�(���)
											pTransTeamLeave->m_in.changemoney = _i64cost;
										}
										if (US_ContestRule_point == pContest->uContestKind)
										{//�����Ŷӷ�(����)
											pTransTeamLeave->m_in.changepoint = _i64cost;
										}
										pTransTeamLeave->m_in.uContestKind = pContest->uContestKind;
										pTransTeamLeave->m_in.userid = puser->GetUserData()->basedata.dwUserID;
										pTransTeamLeave->m_in.ucontestid = pContest->contestid;
										std::memcpy(pTransTeamLeave->m_in.m_Team_password, p->m_Team_password, sizeof(pTransTeamLeave->m_in.m_Team_password));
										m_pRedis->do_transaction(Redis_Order_h_messid_team_leave,
											uIndex,
											onRedis_Order_h_messid_team_leave,
											pTransTeamLeave);
									}
		}break;
		case e_hs_ass_team_dissolve:
		{//��ɢ�Ŷ�
									   QK_InfoLog("sdp_test hall �յ���ҽ�ɢ�Ŷ�����");
									   struct_data_hs_mess_team_dissolve* p = (struct_data_hs_mess_team_dissolve*)pData;
									   if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_dissolve))
									   {
										   return false;
									   }

									   //����Ƿ��˲�����
									   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									   if (nullptr == puser)
									   {
										   return false;
									   }

									   if (!puser->bCanChangeUserStation(US_P_DissolveTeaming))
									   {
										   QK_InfoLog("sdp_test hall err ��Ҳ��ܴ��ڽ�ɢ�Ŷӡ�%d", puser->GetUserData()->basedata.bUserState);
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_unknown);
										   return true;
									   }

									   //�������Ƿ����
									   const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									   if (nullptr == pContest)
									   {
										   QK_InfoLog("sdp_test hall err ���������ڡ�");
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_contest_inexistence);
										   return true;
									   }

									   //����ģʽ����
									   if (US_ContestRule_team != pContest->uContestRule)
									   {
										   QK_InfoLog("sdp_test hall err ����ģʽ�����Ŷ�����%d����", pContest->uContestRule);
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_contest_rule);
										   return true;
									   }

									   //�ѱ�����δ����
									   if (m_CApplayManage.bExist(puser->GetUserData()->basedata.dwUserID))
									   {
										   QK_InfoLog("sdp_test hall err ����ѱ�����δ������");
										  SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_applyed);
										   return true;
									   }

									   TeamStruct _TeamStruct;
									   bool bfind = m_CTeamManage.find(puser->GetUserData()->basedata.dwUserID, _TeamStruct);
									   if (!bfind || _TeamStruct.m_uCaptain_userid != puser->GetUserData()->basedata.dwUserID)
									   {//��ɢ�Ŷ�ʧ��_�Լ���û�д����Ŷ�
										   QK_InfoLog("sdp_test hall err ��ɢ�Ŷ�ʧ��_�Լ���û�д����Ŷӡ�");
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_uncreated);
										   return true;
									   }

									   //�ж�����ǲ������ڵ��Ŷ��Ѿ���ʼvs��
									   for (int i = 0; i < m_CTeamManage.Count_VS(); ++i)
									   {
										   TeamStruct_VS _TeamStruct_VS;
										   m_CTeamManage.at_vs(i, _TeamStruct_VS);
										   if (0 == strcmp(_TeamStruct_VS._TeamStruct[0].m_Team_password, p->m_Team_password)
											   || 0 == strcmp(_TeamStruct_VS._TeamStruct[1].m_Team_password, p->m_Team_password))
										   {
											   QK_InfoLog("sdp_test hall �Ŷ�����vs,���ܽ�ɢ��");
											   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_playing);
											   return true;
										   }
									   }

									   TeamStruct _TeamStruct2;
									   bool bfind2 = m_CTeamManage.find(p->m_Team_password, _TeamStruct2);
									   if (0 != strcmp(p->m_Team_password, _TeamStruct.m_Team_password)
										   || 0 != strcmp(p->m_Team_password, _TeamStruct2.m_Team_password)
										   )
									   {//����û������İ��������⣬�;ܾ�
										   QK_InfoLog("sdp_test hall err ����û������İ��������⣬�;ܾ�,�ŶӰ��Ŵ���");
										   bfind2 = false;
									   }

									   LLONG _i64cost = 0;
									   if (!bfind2)
									   {//�ŶӰ��Ŵ���
										   QK_InfoLog("sdp_test hall err �ŶӰ��Ŵ���");
										 SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_password);
										   return true;
									   }
									   else
									   {
										   if (TeamPayType_AA == _TeamStruct.m_TeamPayType)
										   {
											   _i64cost = pContest->_i64cost / (UINT)pContest->uRoomRule;
										   }
										   else if (TeamPayType_Captain == _TeamStruct.m_TeamPayType)
										   {
											   _i64cost = 0;
										   }
									   }

									   bool bUnNeedMoney = false;
									   if (US_ContestKind_free == pContest->uContestKind)
									   {
										   bUnNeedMoney = true;
									   }
									   else if (US_ContestRule_happy == pContest->uContestKind)
									   {//�������Ŷӷѹ�����(���)
										   if (0 == _i64cost)
										   {
											   bUnNeedMoney = true;
										   }
									   }
									   else if (US_ContestRule_point == pContest->uContestKind)
									   {//�������Ŷӷѹ�����(����)
										   if (0 == _i64cost)
										   {
											   bUnNeedMoney = true;
										   }
									   }
									   else
									   {//�������������
										   SendData(uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_unknown);
										   return true;
									   }

									   if (bUnNeedMoney)
									   {
										   QK_InfoLog("sdp_test hall ��ɢ�Ŷӳɹ���");
										   UINT uTemaPlayerNum;//��������
										   UINT Userid[TEAM_USER_NUM];//ÿ���˵�userid
										   m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
										   for (int i = 0; i < uTemaPlayerNum; ++i)
										   {//֪ͨ�����˽�ɢ�Ŷ�
											   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
											   if (nullptr != puser)
											   {
												   SendData(puser->GetSocketIndex(), nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_suc);
											   }
										   }
										   m_CTeamManage.del(p->m_Team_password);
									   }
									   else
									   {
										   QK_InfoLog("sdp_test hall ��ʼִ��redis������");
										   puser->SetUserState(US_P_DissolveTeaming);
										   /*
										   RRLine_c_team_dissolve rediscommand;
										   for (int i = 0; i < _TeamStruct.m_uTemaPlayerNum; ++i)
										   {
											   if (US_ContestRule_happy == pContest->uContestKind)
											   {//�����Ŷӷ�(���)
												   rediscommand.changemoney[i] = _i64cost;
											   }
											   if (US_ContestRule_point == pContest->uContestKind)
											   {//�����Ŷӷ�(����)
												   rediscommand.changepoint[i] = _i64cost;
											   }
											   rediscommand.userid[i] = _TeamStruct.m_Userid[i];
										   }

										   rediscommand.uContestKind = pContest->uContestKind;
										   rediscommand.m_uTemaPlayerNum = _TeamStruct.m_uTemaPlayerNum;
										   rediscommand.ucontestid = pContest->contestid;
										   std::memcpy(rediscommand.m_Team_password, p->m_Team_password, sizeof(rediscommand.m_Team_password));
										   m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_team_dissolve), uIndex);
										   */
										   CTransaction_team_dissolve *pTransTeamDis = new CTransaction_team_dissolve;
										   assert(pTransTeamDis);
										   pTransTeamDis->m_in.uIndex = uIndex;
										   for (int i = 0; i < _TeamStruct.m_uTemaPlayerNum; ++i)
										   {
											   if (US_ContestRule_happy == pContest->uContestKind)
											   {//�����Ŷӷ�(���)
												   pTransTeamDis->m_in.changemoney[i] = _i64cost;
											   }
											   if (US_ContestRule_point == pContest->uContestKind)
											   {//�����Ŷӷ�(����)
												   pTransTeamDis->m_in.changepoint[i] = _i64cost;
											   }
											   pTransTeamDis->m_in.userid[i] = _TeamStruct.m_Userid[i];
										   }

										   pTransTeamDis->m_in.uContestKind = pContest->uContestKind;
										   pTransTeamDis->m_in.m_uTemaPlayerNum = _TeamStruct.m_uTemaPlayerNum;
										   pTransTeamDis->m_in.ucontestid = pContest->contestid;
										   std::memcpy(pTransTeamDis->m_in.m_Team_password, p->m_Team_password, sizeof(pTransTeamDis->m_in.m_Team_password));
										   m_pRedis->do_transaction(Redis_Order_h_messid_team_dissolve,
											   uIndex,
											   onRedis_Order_h_messid_team_dissolve,
											   pTransTeamDis);
									   }
		}break;
		case e_hs_ass_team_start:
		{//�ӳ�����ʼ����
									struct_data_hs_mess_team_start* p = (struct_data_hs_mess_team_start*)pData;
									if (nullptr == p || uSize != sizeof(struct_data_hs_mess_team_start))
									{
										return false;
									}

									//����Ƿ��˲�����
									CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uIndex);
									if (nullptr == puser)
									{
										return false;
									}

									//�������Ƿ����
									const ComContestInfo* pContest = m_CGameList.GetComContestInfo(p->ucontestid);
									if (nullptr == pContest)
									{
										return true;
									}

									TeamStruct _TeamStruct;
									if (!m_CTeamManage.find(p->m_Team_password, _TeamStruct))
									{//�������Ŷ�
										return true;
									}

									if (!m_CTeamManage.bFull(p->m_Team_password))
									{//�˻�û��
										return true;
									}

									if (_TeamStruct.m_Userid[0] != puser->GetUserData()->basedata.dwUserID)
									{//���Ƕӳ���������
										return true;
									}

									//����ͬ��ƥ��
									m_CTeamManage.setButVS(p->m_Team_password, true);									
		}break;
		default:
		{
				   break;
		}
		}
		return true;
	}

	//������Ϣ����
	bool CMainManage_login::OnSocketRead_pr_mess_roominfo(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_pr_ass_roominfo_update:
		{//�������ϵķ���������ڸ��������·�������
										 //QK_InfoLog("ls �������ϵķ���������ڸ��������·������ݡ�");
										 struct_data_pr_roominfo_update* p = (struct_data_pr_roominfo_update*)pData;
										 if (nullptr == p || uSize != sizeof(struct_data_pr_roominfo_update))
										 {
											 return false;
										 }

										 ComRoomInfo_node cmd;
										 cmd.usocketindex = uIndex;
										 std::memcpy(&cmd.room_node, p, sizeof(ComRoomInfo));
										 //QK_InfoLog("ls �������ϵķ���������ڸ��������·�������roomid=%d,(%s:%d)", cmd.room_node.roomid, cmd.room_node.strIP, cmd.room_node.uport);
										 m_CGameList.addroom_node(cmd);

										 bool bexist = false;
										 auto iter = m_socket_vector.begin();
										 while (iter != m_socket_vector.end())
										 {
											 if ((*iter) == uIndex)
											 {
												 bexist = true;
												 break;
											 }
											 ++iter;
										 }

										 if (!bexist)
										 {//���ھͲ�������
											 m_socket_vector.push_back(uIndex);
										 }
		}break;
		default:
		{
				   break;
		}
		}

		return true;
	}

	//�û���Ϣ����
	bool CMainManage_login::OnSocketRead_pr_mess_userinfo(CWJSSocket * pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize)
	{
		UINT uIndex = pSocket->GetSockID();
		switch (pNetHead->bAssistantID)
		{
		case e_pr_ass_userinfo_login:
		{
										QK_InfoLog("sdp_test hall ��Ϸ��������֪��½������������ҵ�¼��Ϸ������������ȷ�ϡ�");
										struct_data_pr_userinfo_login* p = (struct_data_pr_userinfo_login*)pData;
										if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_login))
										{
											return false;
										}

										struct_data_pr_userinfo_login_res_err cmd;
										cmd.userid = p->uuserid;
										cmd.uIndex = p->uIndex;
										struct_data_pr_userinfo_login_res_ok cmd2;
										cmd2.uIndex = p->uIndex;
										cmd2.contestid = p->contestid;
										const ComContestInfo* pComContestInfo = m_CGameList.GetComContestInfo(p->contestid);
										if (nullptr == pComContestInfo)
										{

											QK_WarnLog("sdp_test hall ����������Ϸ����������ͬ�����ҵ�¼��Ϸ�����������������ڡ�(%d)", p->contestid);
											SendData(uIndex, &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_login, e_pr_hcid_login_unallow);
											return true;
										}

										if (US_ContestRule_solo == pComContestInfo->uContestRule)
										{//����solo��
											struct_applynode data;
											bool bRes = m_CApplayManage.at(p->uuserid, data);
											QK_InfoLog("sdp_test hall ���������ж�(userid=%d,bRes=%d,bAllot=%d)", p->uuserid, bRes, data.bAllot);
											if (!bRes || data.ucontestid != p->contestid)
											{
												QK_WarnLog("sdp_test hall ����������Ϸ����������ͬ�����ҵ�¼��Ϸ������������solo�� ���û�ڸñ�����(%d, %d)��"
													, data.ucontestid, p->contestid);
												SendData(uIndex, &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_login, e_pr_hcid_login_unallow);
												return true;
											}
										}
																				
										CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->uuserid);
										if (nullptr == puser)
										{
											QK_WarnLog("sdp_test hall ����������Ϸ����������ͬ�����ҵ�¼��Ϸ����������Ҳ����ߡ�(%d)", p->uuserid);
											SendData(uIndex, &cmd, sizeof(cmd), e_pr_mess_userinfo, e_pr_ass_userinfo_login, e_pr_hcid_login_unallow);
											return true;
										}
										QK_InfoLog("sdp_test hall ��½������ͬ�����ҵ�¼��Ϸ��������");
										std::memcpy(&cmd2.data, puser->GetUserData(), sizeof(UserInfoInHallStruct));
										SendData(uIndex, &cmd2, sizeof(cmd2), e_pr_mess_userinfo, e_pr_ass_userinfo_login, e_pr_hcid_login_allow);
		}break;
		case e_pr_ass_userinfo_allot_room:
		{//��Ϸ��������֪��½������������Ҽ���÷��䣬����ȷ��	
										QK_InfoLog("sdp_test hall ��Ϸ��������֪��½������������Ҽ���÷��䣬����ȷ�ϡ�");
										struct_data_pr_userinfo_allot_room* p = (struct_data_pr_userinfo_allot_room*)pData;
										if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_allot_room))
										{
											return false;
										}

										const ComContestInfo* pComContestInfo = m_CGameList.GetComContestInfo(p->contestid);
										const ComRoomInfo* pComRoomInfo = m_CGameList.GetComRoomInfoEx(p->roomid);
										if (nullptr == pComContestInfo
											|| nullptr == pComRoomInfo)
										{
											return false;
										}

										if (US_ContestRule_solo == pComContestInfo->uContestRule)
										{//����solo��
											struct_applynode data;
											bool bRes = m_CApplayManage.at(p->userid, data);
											QK_InfoLog("sdp_test hall ���������ж�(userid=%d,bRes=%d,bAllot=%d)", p->userid, bRes, data.bAllot);
											if (bRes)
											{//�ﵽ�����������½����
												m_CApplayManage.AllotRoom(p->userid, p->roomid);
											}
										}
										else if (US_ContestRule_team == pComContestInfo->uContestRule)
										{//�Ŷ���

										}

										return true;
		}break;
		case e_pr_ass_userinfo_apply_restitution:
		{//��Ϸ���������ߴ�������Ƿ�����˷�
													QK_InfoLog("sdp_test hall ��Ϸ���������ߴ�������Ƿ�����˷ѡ�");
													struct_data_pr_userinfo_apply_restitution* p = (struct_data_pr_userinfo_apply_restitution*)pData;
													if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_apply_restitution))
													{//���ݰ��쳣
														return false;
													}

													struct_applynode data;
													if (!m_CApplayManage.at(p->userid, data))
													{//�����û����
														return true;
													}

													CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->userid);

													if (e_pr_hcid_apply_restitution_suc == pNetHead->bHandleCode)
													{//�˷ѳɹ�
														//�������Ƿ����
														const ComContestInfo* pContest = m_CGameList.GetComContestInfo(data.ucontestid);
														if (nullptr != pContest)
														{//Ǯ�˸����
															if (nullptr != puser)
																puser->SetUserState(US_P_unApplying);
															/*
															RRLine_c_apply_restitution rediscommand;
															rediscommand.changepoint = pContest->_i64cost;
															rediscommand.uContestKind = pContest->uContestKind;
															rediscommand.userid = puser->GetUserData()->basedata.dwUserID;
															rediscommand.ucontestid = data.ucontestid;
															QK_InfoLog("sdp_test hall �б�����û�ڷ��䣬���ѯredis�����˷ѡ�");
															m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply_restitution), puser->GetSocketIndex());
															*/
															CTransaction_apply_restitution *pTransApplyResti = new CTransaction_apply_restitution;
															assert(pTransApplyResti);
															pTransApplyResti->m_in.uIndex = (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID;
															pTransApplyResti->m_in.changepoint = pContest->_i64cost;
															pTransApplyResti->m_in.uContestKind = pContest->uContestKind;
															pTransApplyResti->m_in.userid = p->userid;
															pTransApplyResti->m_in.ucontestid = data.ucontestid;
															m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
																(nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID,
																onRedis_Order_h_messid_apply_restitution,
																pTransApplyResti);
														}
													}
													else if (e_pr_hcid_apply_restitution_err == pNetHead->bHandleCode)
													{//�˷�ʧ��
														QK_InfoLog("sdp_test hall �˷�ʧ�ܡ�");
														if (nullptr != puser)
															SendData(puser->GetSocketIndex(), nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_err);
													}
		}break;
		case e_pr_ass_userinfo_unplay:
		{//���û���ڷ��������棬����������Ч
										 QK_InfoLog("sdp_test hall ��Ϸ���������ߴ������û���ڷ��������棬����������Ч��");
										 struct_data_pr_userinfo_unplay* p = (struct_data_pr_userinfo_unplay*)pData;
										 if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_unplay))
										 {
											 return false;
										 }
										 struct_applynode data;
										 if (!m_CApplayManage.at(p->userid, data))
										 {//�����û����
											 return true;
										 }

										 CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->userid);

										 if (data.bAllot)
										 {
											 //������Ҽ����ȴ�����
											 m_CApplayManage.unAllotRoom(p->userid);
											 //�뿪������˷�
											 struct_applynode data;
											 if (m_CApplayManage.at(p->userid, data))
											 {//�б���
												 UINT usocketindex;
												 if (!m_CGameList.GetRoomSocketID(data.uroomid, usocketindex))
												 {//û�ڷ��䣬���������
													 //�������Ƿ����
													 const ComContestInfo* pContest = m_CGameList.GetComContestInfo(data.ucontestid);
													 if (nullptr != pContest)
													 {//Ǯ�˸����
														 /*
														 RRLine_c_apply_restitution rediscommand;
														 rediscommand.changepoint = pContest->_i64cost;
														 rediscommand.uContestKind = pContest->uContestKind;
														 rediscommand.userid = p->userid;
														 rediscommand.ucontestid = data.ucontestid;
														 QK_InfoLog("sdp_test hall �б�����û�ڷ��䣬���ѯredis�����˷ѡ�");
														 m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply_restitution), ERROR_SOCKETID);
														 */
														 CTransaction_apply_restitution *pTransApplyResti = new CTransaction_apply_restitution;
														 assert(pTransApplyResti);
														 pTransApplyResti->m_in.uIndex = (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID;
														 pTransApplyResti->m_in.changepoint = pContest->_i64cost;
														 pTransApplyResti->m_in.uContestKind = pContest->uContestKind;
														 pTransApplyResti->m_in.userid = p->userid;
														 pTransApplyResti->m_in.ucontestid = data.ucontestid;
														 QK_InfoLog("sdp_test hall �б�����û�ڷ��䣬���ѯredis�����˷ѡ�");
														 m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
															 (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID,
															 onRedis_Order_h_messid_apply_restitution,
															 pTransApplyResti);
													 }
												 }
											 }
										 }
										 return true;
		}break;
		case e_pr_ass_userinfo_gameover:
		{//��Ϸ������֪ͨ����������Ѿ�������Ϸ
										   QK_InfoLog("sdp_test hall ��Ϸ������֪ͨ����������Ѿ�������Ϸ��");
										   struct_data_pr_userinfo_gameover* p = (struct_data_pr_userinfo_gameover*)pData;
										   if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_gameover))
										   {
											   QK_WarnLog("sdp_test hall ��Ϸ������֪ͨ����������Ѿ�������Ϸ��err-----------------!");
											   return false;
										   }
										   QK_InfoLog("sdp_test hall ��Ϸ������֪ͨ����������Ѿ�������Ϸ2222222��");
										   struct_applynode data;
										   bool bexist = m_CApplayManage.at(p->userid, data);
										   if (false == bexist)
										   {//����Ҳ����ڣ����ô���
											   QK_WarnLog("sdp_err hall ��Ϸ������֪ͨ����������Ѿ�������Ϸ,����Ҳ����ڣ����ô���");
											   return true;
										   }
										   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->userid);

										   const ComContestInfo* pContestInfo = m_CGameList.GetComContestInfo(data.ucontestid);
										   MyDebugString("sdp_test", "hall ������Ϸ�ж��˷����⣨%d,%d,%d,%d��", p->userid, data.ucontestid, nullptr != pContestInfo, pNetHead->bHandleCode);
										   if (nullptr != pContestInfo && GAME_FINISH_TYPE_FORCE == pNetHead->bHandleCode)
										   {
											   //Ǯ�˸����
											   CTransaction_apply_restitution *pTransApplyResti = new CTransaction_apply_restitution;
											   assert(pTransApplyResti);
											   pTransApplyResti->m_in.uIndex = (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID;
											   pTransApplyResti->m_in.changepoint = pContestInfo->_i64cost;
											   pTransApplyResti->m_in.uContestKind = pContestInfo->uContestKind;
											   pTransApplyResti->m_in.userid = p->userid;
											   pTransApplyResti->m_in.ucontestid = data.ucontestid;
											   QK_InfoLog("sdp_test hall �б�����û�ڷ��䣬���ѯredis�����˷ѡ�");
											   m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
												   (nullptr != puser) ? puser->GetSocketIndex() : ERROR_SOCKETID,
												   onRedis_Order_h_messid_apply_restitution,
												   pTransApplyResti);
											   return true;
										   }
										   QK_InfoLog("sdp_test hall ��Ϸ������֪ͨ����������Ѿ�������Ϸ33333333333��");
										   //������Ҵӱ���������ɾ��
										   bool bres = m_CApplayManage.del(p->userid);
										   QK_InfoLog("sdp_err hall ������Ҵӱ���������ɾ��(userid=%d, bres=%d)��", p->userid, bres);
										
										   return true;
		}break;
		case e_pr_ass_userinfo_team_vs_finish:
		{//��Ϸ������֪ͨ�������Ŷ��Ѿ�������Ϸ
												 QK_InfoLog("sdp_test hall ��Ϸ������֪ͨ�������Ŷ��Ѿ�������Ϸ��");
												 struct_data_pr_userinfo_team_vs_finish* p = (struct_data_pr_userinfo_team_vs_finish*)pData;
												 if (nullptr == p || uSize != sizeof(struct_data_pr_userinfo_team_vs_finish))
												 {
													 return false;
												 }

												 for (int i = 0; i < m_CTeamManage.Count_VS(); ++i)
												 {
													 TeamStruct_VS _TeamStruct_VS;
													 m_CTeamManage.at_vs(i, _TeamStruct_VS);
													 if (0 == strcmp(_TeamStruct_VS._TeamStruct[0].m_Team_password, p->_data._TeamStruct[0].m_Team_password)
														 && 0 == strcmp(_TeamStruct_VS._TeamStruct[1].m_Team_password, p->_data._TeamStruct[1].m_Team_password)
														 && _TeamStruct_VS._TeamStruct[0].m_uCaptain_userid == p->_data._TeamStruct[0].m_uCaptain_userid
														 && _TeamStruct_VS._TeamStruct[1].m_uCaptain_userid == p->_data._TeamStruct[1].m_uCaptain_userid)
													 {
														 QK_InfoLog("sdp_test hall �Ŷ�ʹ����ϣ����������ݡ�");
														 m_CTeamManage.del(_TeamStruct_VS._TeamStruct[0].m_Team_password);
														 m_CTeamManage.del(_TeamStruct_VS._TeamStruct[1].m_Team_password);
														 m_CTeamManage.del_VS(i);
														 break;
													 }
												 }

												 return true;
		}break;
		default:
		{
				   break;
		}
		}

		return true;
	}
	void CMainManage_login::OnEndConnection(CWJSSocket * pSocket)
	{
		int tag = pSocket->GetType();
		UINT uSocketIndex = pSocket->GetSockID();
		QK_InfoLog("sdp_test hall OnSocketClose uSocketIndex = %d ,tag=%d", uSocketIndex, tag);
		if (tag == ePCClient)
		{
			char cip[30] = { 0 };
			char str[MAX_PATH] = { 0 };
			//����
			UINT userid = 0;
			char username[NAME_LENGTH] = { 0 };
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(uSocketIndex);
			if (nullptr != puser)
			{
				userid = puser->GetUserData()->basedata.dwUserID;
				//////////////////////////////////////////
				struct_applynode data;
				if (m_CApplayManage.at(userid, data))
				{//������б�����
					if (!data.bAllot)
					{//��û�н�����
						//�������Ƿ����
						const ComContestInfo* pContest = m_CGameList.GetComContestInfo(data.ucontestid);
						if (nullptr != pContest)
						{//Ǯ�˸����
							/*
							RRLine_c_apply_restitution rediscommand;
							rediscommand.changepoint = pContest->_i64cost;
							rediscommand.uContestKind = pContest->uContestKind;
							rediscommand.userid = userid;
							rediscommand.ucontestid = data.ucontestid;
							QK_InfoLog("sdp_test hall �б�����û�ڷ��䣬���ѯredis�����˷ѡ�");
							m_pCRedisDataListManage->PushLine(&rediscommand, sizeof(RRLine_c_apply_restitution), ERROR_SOCKETID);
							*/
							CTransaction_apply_restitution *pTransApplyResti = new CTransaction_apply_restitution;
							assert(pTransApplyResti);
							pTransApplyResti->m_in.uIndex = INVALID_SOCKET_ID;
							pTransApplyResti->m_in.changepoint = pContest->_i64cost;
							pTransApplyResti->m_in.uContestKind = pContest->uContestKind;
							pTransApplyResti->m_in.userid = userid;
							pTransApplyResti->m_in.ucontestid = data.ucontestid;
							m_pRedis->do_transaction(Redis_Order_h_messid_apply_restitution,
								uSocketIndex,
								onRedis_Order_h_messid_apply_restitution,
								pTransApplyResti);

						}
					}					
				}
				//////////////////////////////////////////////////////////
				std::memcpy(username, puser->GetUserData()->basedata.szName, sizeof(username));				
				m_CLUserInfoManage.FreeUser(uSocketIndex);
				sprintf_s(str, "disconnect!IP:%s, index=%d, player=(%d,%s)", cip, uSocketIndex, userid, username);
			}
			else
			{
				LUserStruct* pLUserStruct = m_CLUserInfoManage.GetLUserStructEx_TEST(uSocketIndex);	
				if (nullptr != pLUserStruct)
					sprintf_s(str, "disconnect!IP:%s, index=%d, player=(%d), usertype=%d", cip, uSocketIndex, userid, pLUserStruct->usertype);
				else
					sprintf_s(str, "disconnect!IP:%s, index=%d, player=(%d), usertype=null", cip, uSocketIndex, userid);
				QK_InfoLog("sdp_test %s", std::string(str).c_str());
			}

		
		}
		else if (tag == eGameServer)
		{			
			
			
		}
		return; 
	}
	///���ݿ⴦���� ���������أ�
	bool CMainManage_login::OnDataBaseResult(DataBaseResultLine * pResultData, UINT tag)
	{
		if (tag == e_UserDataTag_def)
		{

		}
		else if (tag == e_UserDataTag_LGsocketserver)
		{

		}

		return true; 
	}

	double random(double start, double end)
	{
		double _min = 0.0;
		double _max = 0.0;
		_min = start >= end ? end : start;
		_max = start >= end ? start : end;
		return _min + (_max - _min)*rand() / (RAND_MAX + 1.0);

		srand((unsigned)time(NULL));
	}

	///��ʱ����Ϣ  ע�� ���ﻹû�н��д�����Ҫ����ҵ����д�����֪��ҵ��֪ʶ��
	bool CMainManage_login::OnTimerMessage(TimerID timerId)
	{
		switch (timerId)
		{
		case E_P_TIME_ID_HEART:
		{
							 UINT uNowTime = (UINT)time(NULL);	//��ǰʱ��

							 map<long, LUserStruct*>* pMap = m_CLUserInfoManage.GetLUserStruct_map();
							 auto iter = pMap->begin(), end = pMap->end();
							 for (; iter != end; ++iter)
							 {//�鿴��������Ƿ�������
								 if (usertype_online == iter->second->usertype)
								 {
									 CWJSSocket* pSocket = getCWJSSocket(iter->second->userinfo.GetSocketIndex());
									 if (nullptr != pSocket)
									 {
										 UINT itag = uNowTime - pSocket->GetKeep();
										 if (itag > m_uCheckTime)
										 {
											 QK_WarnLog("��¼������ (uNowTime = %d, m_Keep = %d)ʱ���=%d ���� %d,��Ͽ��ÿͻ����� socketindex = %d", uNowTime, pSocket->GetKeep(), itag, m_uCheckTime, iter->second->userinfo.GetSocketIndex());
											 CloseSocket(pSocket->GetSockID());
										 }
									 }
								 }
							 }
		}break;
		case E_P_TIME_ID_UPDATE_ONLINE_COUNT:
		{
											static int irandcount = 0;
											if ((rand() % 10 > 5) || irandcount == 0)
											{
												irandcount = int(random(lsconfig().GetData()->iMinVirtualCount, lsconfig().GetData()->iMaxVirtualCount));
											}
											m_update_now_playercount = m_CLUserInfoManage.GetOnLineUserCount() + irandcount;											
		}break;
		case  E_P_TIME_ID_UPDATE_USER_INFO:
		{
											  CTransaction_Update_Userinfo *pCmd = new CTransaction_Update_Userinfo;
											  assert(pCmd);
											  pCmd->m_in.uIndex = 0;
											  m_pRedis->do_transaction(Redis_Order_h_messid_update_userinfo,
												  0,
												  onRedis_Order_h_messid_update_userinfo,
												  pCmd);
		}break;
		case E_P_TIME_ID_TEAM_VS:
		{
							   std::vector<TeamStruct> _vector = m_CTeamManage.getTeamStruct_list();
							   auto _iter_vector = _vector.begin();
							   while (_iter_vector != _vector.end())
							   {
								   std::vector<ComRoomInfo_node> room_node;
								   m_CGameList.GetComRoomList(room_node, (*_iter_vector).m_uContest);
								   for (auto iter = room_node.begin(); iter != room_node.end(); ++iter)
								   {
									   bool bexist = false;
									   {
										   auto _iter = m_socket_vector.begin();
										   while (_iter != m_socket_vector.end())
										   {
											   if ((*_iter) == (*iter).usocketindex)
											   {//��������Ч�ķ���
												   bexist = true;
												   break;
											   }
											   ++_iter;
										   }
									   }									   

									   if (bexist && ((*iter).uTeamCount * (*_iter_vector).m_uTemaPlayerNum + ((*_iter_vector).m_uTemaPlayerNum * 2) <= PORT_GS_COUNT))
									   {//ƥ��ֻ��Ҫһ������,�������������ܳ�����Ϸ��������������
										   TeamStruct_VS _TeamStruct_VS;
										   bool bres = m_CTeamManage.add_VS((*_iter_vector).m_Team_password, _TeamStruct_VS, (*iter).room_node.roomid);
										   if (bres)
										   {//֪ͨ����
											   SendData((*iter).usocketindex, &_TeamStruct_VS, sizeof(TeamStruct_VS), e_pr_mess_userinfo, e_pr_ass_userinfo_team_start, 0);
											   struct_data_hs_hcid_team_start_suc cmd;
											   cmd.data = _TeamStruct_VS;
											   std::memcpy(&cmd.strIP, (*iter).room_node.strIP, sizeof(cmd.strIP));
											   cmd.uport = (*iter).room_node.uport;
											   //֪ͨ�����Ŷӵ�������ң�ƥ��ɹ�
											   for (int i = 0; i < 2;++i)
											   {
												   for (int j = 0; j < _TeamStruct_VS._TeamStruct[i].m_uTemaPlayerNum; ++j)
												   {
													   CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(_TeamStruct_VS._TeamStruct[i].m_Userid[j]);
													   if (nullptr != puser)
													   {
														   SendData(puser->GetSocketIndex(), &cmd, sizeof(struct_data_hs_hcid_team_start_suc), e_hs_mess_team, e_hs_ass_team_start, e_hs_hcid_team_start_suc);
													   }
												   }
											   }												
										   }
										   break;
									   }
								   }								   

								   ++_iter_vector;
							   }
		}break;
		default:
			break;
		}
		return true;
	}
	
	//redis������ �û���¼����
	void CMainManage_login::onRedisRes_Order_h_messid_load_hall(RedisResultLine * pResultData)
	{
		RRLine_s_load_hall* pCmd = (RRLine_s_load_hall*)pResultData;

		//�����Ѿ��жϣ������账��
		CWJSSocket* psocket = getCWJSSocket(pCmd->uIndex);
		if (nullptr != psocket && psocket->IsConnect())
		{
			CLoginUserInfo* p = nullptr;
			p = m_CLUserInfoManage.GetOnLineUserInfoEx(pCmd->uIndex);
			if (nullptr != p)
			{//���ָ��׽���id�´������
				QK_WarnLog("sdp_test hall ��½�������� ���ָ��׽���id�´�����ҡ�");
				pResultData->uHandleRusult = RRLine_s_load_hall::e_login_err_online;
			}

			p = m_CLUserInfoManage.GetOnLineUserInfo(pCmd->_data.basedata.dwUserID);
			if (nullptr != p)
			{//���ָ�userid�´������
				QK_WarnLog("sdp_test hall ��½�������� ���ָ�userid�´�����ҡ�");
				pResultData->uHandleRusult = RRLine_s_load_hall::e_login_err_online;
			}

			switch (pResultData->uHandleRusult)
			{
			case RRLine_s_load_hall::e_login_err_unexist:
			{//��Ҳ�����
															QK_WarnLog("sdp_test hall ��½�������� ��Ҳ����ڡ�");
															SendData(pCmd->uIndex, e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_unexist);
			}break;
			case RRLine_s_load_hall::e_login_err_password:
			{//�������
															 QK_WarnLog("sdp_test hall ��½�������� �������");
															 SendData(pCmd->uIndex, e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_password);
			}break;
			case RRLine_s_load_hall::e_login_err_online:
			{//����Ѿ�����
															 QK_WarnLog("sdp_test hall ��½�������� ����Ѿ����ڡ�");
															 SendData(pCmd->uIndex, e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_err_online);
			}break;
			case RRLine_s_load_hall::e_login_suc:
			{//�ɹ�
													QK_InfoLog("sdp_test hall ��½�����ɹ���");
													//ע�������û�
													CLoginUserInfo* puser = m_CLUserInfoManage.ActiveUser(&pCmd->_data, pCmd->uIndex);
													if (nullptr == puser)
													{//ע�������û�ʧ�ܣ����ܽ������
														CloseSocket(pCmd->uIndex);
														return;
													}
													SendData(pCmd->uIndex, &pCmd->_data, sizeof(struct_data_hs_hcid_login_suc), e_hs_mess_login, e_hs_ass_login, e_hs_hcid_login_suc);							
			}break;
			default:
				break;
			}
		}
	}


	//redis������ ����
	void CMainManage_login::onRedisRes_Order_h_messid_apply(RedisResultLine * pResultData)
	{
		RRLine_s_apply* p = (RRLine_s_apply*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
			if (!p->bSuc)
			{
				QK_InfoLog("sdp_test hall �����Ѳ�����");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_err_nomoney);
				
				return;
			}

			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money -= p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint -= p->changepoint;
			}
		}
		
		struct_applynode cmd;
		cmd.ucontestid = p->ucontestid;
		cmd.userid = p->userid;
		m_CApplayManage.add(cmd);
		QK_InfoLog("sdp_test hall �����ɹ���");	
		if (nullptr != puser)
		{
			SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply, e_hs_hcid_apply_suc);
		}

	}

	//redis������ �����Ŷ�
	void CMainManage_login::onRedisRes_Order_h_messid_team_create(RedisResultLine * pResultData)
	{
		RRLine_s_team_create* p = (RRLine_s_team_create*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
			if (!p->bSuc)
			{
				QK_InfoLog("sdp_test hall �����Ѳ�����");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_err_nomoney);
				return;
			}

			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money -= p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint -= p->changepoint;
			}
		}

		TeamStruct cmd;
		cmd.m_uCaptain_userid = p->userid;
		cmd.m_TeamPayType = p->m_TeamPayType;
		cmd.m_uContest = p->ucontestid;
		cmd.m_uTemaPlayerNum = (UINT)p->uRoomRule;

		m_CTeamManage.add(cmd);

		QK_InfoLog("sdp_test hall �����Ŷӳɹ���");
		if (nullptr != puser)
		{
			SendData(pResultData->uIndex, &cmd, sizeof(struct_data_hs_mess_team_create_res), e_hs_mess_team, e_hs_ass_team_create, e_hs_hcid_team_create_suc);
		}
	}

	//redis������ �����Ŷ�
	void CMainManage_login::onRedisRes_Order_h_messid_team_join(RedisResultLine * pResultData)
	{
		RRLine_s_team_join* p = (RRLine_s_team_join*)pResultData;

		if (!p->bSuc)
		{
			QK_WarnLog("sdp_test hall �����Ѳ�����");
			m_CTeamManage.userDel(p->userid, p->m_Team_password);//һ��Ҫ�ǵ�ɾ������ң���Ϊ�����Ѿ����
		}

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
			if (!p->bSuc)
			{
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_err_nomoney);
				return;
			}

			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money -= p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint -= p->changepoint;
			}
		}

		TeamStruct cmd;

		m_CTeamManage.find(p->m_Team_password, cmd);

		QK_InfoLog("sdp_test hall �����Ŷӳɹ���");
		
		UINT uTemaPlayerNum;//��������
		UINT Userid[TEAM_USER_NUM];//ÿ���˵�userid
		m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
		for (int i = 0; i < uTemaPlayerNum; ++i)
		{
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
			if (nullptr != puser)
			{
				SendData(puser->GetSocketIndex(), &cmd, sizeof(struct_data_hs_mess_team_join_res), e_hs_mess_team, e_hs_ass_team_join, e_hs_hcid_team_join_suc);
			}
		}
	}

	//redis������ �˳��Ŷ�
	void CMainManage_login::onRedisRes_Order_h_messid_team_leave(RedisResultLine * pResultData)
	{
		RRLine_s_team_leave* p = (RRLine_s_team_leave*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (!p->bSuc)
		{//ʧ�ܾͲ����������ˣ�����ֻ�е�������߲�֪ͨ�Է�
			if (nullptr != puser)
			{
				QK_WarnLog("sdp_test hall �˳��Ŷ�ʧ�ܡ�");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_err_money);	
				puser->SetUserState(US_P_Wait);
			}
			return;
		}

		QK_InfoLog("sdp_test hall �˳��Ŷӳɹ���");
		struct_data_hs_mess_team_leave_res cmd;
		cmd.userid = p->userid;
		m_CTeamManage.userDel(p->userid, p->m_Team_password);
		
		if (nullptr != puser)
		{
			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money += p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint += p->changepoint;
			}
			//ֻ�������ߵ�ʱ��Ż�֪ͨ����Ҳ����ɹ�
			SendData(pResultData->uIndex, &cmd, sizeof(struct_data_hs_mess_team_leave_res), e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_suc);
			puser->SetUserState(US_P_Wait);
		}
		
		UINT uTemaPlayerNum;//��������
		UINT Userid[TEAM_USER_NUM];//ÿ���˵�userid
		m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
		for (int i = 0; i < uTemaPlayerNum; ++i)
		{
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
			if (nullptr != puser)
			{
				SendData(puser->GetSocketIndex(), &cmd, sizeof(struct_data_hs_mess_team_leave_res), e_hs_mess_team, e_hs_ass_team_leave, e_hs_hcid_team_leave_suc);
			}
		}
	}

	//redis������ �����û�����(��ҡ�����)
	void CMainManage_login::onRedisRes_Order_h_messid_update_userinfo(RedisResultLine * pResultData)
	{
		RRLine_s_Update_Userinfo* p = (RRLine_s_Update_Userinfo*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->iuserid);
		if (nullptr != puser)
		{
			if (1 == p->itype)
			{//����
				puser->GetUserData()->basedata.dwPoint = p->dwPoint;
			}
			else if (2 == p->itype)
			{//���
				puser->GetUserData()->basedata.i64Money = p->i64Money;
			}
		}
		return;
	}

	void CMainManage_login::onRedisRes_Order_h_messid_team_dissolve(RedisResultLine * pResultData)
	{
		RRLine_s_team_dissolve* p = (RRLine_s_team_dissolve*)pResultData;

		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(p->userid[0]);
		if (!p->bSuc)
		{//ʧ�ܾͲ����������ˣ�����ֻ�е�������߲�֪ͨ�Է�
			if (nullptr != puser)
			{
				QK_InfoLog("sdp_test hall ��ɢ�Ŷ�ʧ�ܡ�");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_err_money);
				puser->SetUserState(US_P_Wait);
			}
			return;
		}

		UINT uTemaPlayerNum;//��������
		UINT Userid[TEAM_USER_NUM];//ÿ���˵�userid
		m_CTeamManage.GetUsers(p->m_Team_password, Userid, uTemaPlayerNum);
		for (int i = 0; i < uTemaPlayerNum; ++i)
		{//֪ͨ�����˽�ɢ�Ŷ�
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(Userid[i]);
			if (nullptr != puser)
			{
				if (US_ContestRule_happy == p->uContestKind)
				{
					puser->GetUserData()->basedata.i64Money += p->changemoney[i];
				}
				else if (US_ContestRule_point == p->uContestKind)
				{
					puser->GetUserData()->basedata.dwPoint += p->changepoint[i];
				}

				SendData(puser->GetSocketIndex(), nullptr, 0, e_hs_mess_team, e_hs_ass_team_dissolve, e_hs_hcid_team_dissolve_suc);
			}
		}
		m_CTeamManage.del(p->m_Team_password);

		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
		}
		QK_InfoLog("sdp_test hall ��ɢ�Ŷӳɹ���");
	}

	//redis������ �˷�
	void CMainManage_login::onRedisRes_Order_h_messid_apply_restitution(RedisResultLine * pResultData)
	{
		RRLine_s_apply_restitution* p = (RRLine_s_apply_restitution*)pResultData;
		
		CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfoEx(pResultData->uIndex);
		if (nullptr != puser)
		{
			if (!p->bSuc)
			{
				QK_WarnLog("sdp_test hall ��ѯredis�˷�ʧ�ܡ�");
				SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_err);
				puser->SetUserState(US_P_Wait);
				return;
			}

			if (US_ContestRule_happy == p->uContestKind)
			{
				puser->GetUserData()->basedata.i64Money += p->changemoney;
			}
			else if (US_ContestRule_point == p->uContestKind)
			{
				puser->GetUserData()->basedata.dwPoint += p->changepoint;
			}

			SendData(pResultData->uIndex, nullptr, 0, e_hs_mess_apply, e_hs_ass_apply_restitution, e_hs_hcid_apply_restitution_suc);
		}
		QK_InfoLog("sdp_test hall redis��ѯͬ���˷ѡ�");
		m_CApplayManage.del(p->userid);
		if (nullptr != puser)
		{
			puser->SetUserState(US_P_Wait);
		}
	}

	//���͸��Ŷ��������Ϣ
	//UINT myuserid �ų��Լ�����
	void CMainManage_login::TeamSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{
		TeamStruct _TeamStruct;
		if (!m_CTeamManage.find(myuserid, _TeamStruct))
		{
			return;
		}

		for (int k = 0; k < _TeamStruct.m_uTemaPlayerNum; ++k)
		{
			if (_TeamStruct.m_Userid[k] == myuserid)
			{
				continue;
			}
			CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(_TeamStruct.m_Userid[k]);
			if (nullptr != puser)
			{//Ҫ����vs�е��������,���˻�����
				SendData(puser->GetSocketIndex(), pData, uBufLen, bMainID, bAssistantID, bHandleCode);
			}
		}
	}

	//���͸��Ŷ�VS�������Ϣ
	//UINT myuserid �ų��Լ�����
	void CMainManage_login::TeamVSSendData(UINT myuserid, void * pData, UINT uBufLen, UINT bMainID, UINT bAssistantID, UINT bHandleCode)
	{//�Ŷ�����Ҫ����vs�е��������
		UINT ucount_vs = m_CTeamManage.Count_VS();
		TeamStruct_VS in_TeamStruct_VS;
		bool bInVS = false;//����Ƿ����Ŷ���VS��
		for (int i = 0; i < ucount_vs; i++)
		{
			TeamStruct_VS _TeamStruct_VS;
			m_CTeamManage.at_vs(i, _TeamStruct_VS);
			for (int j = 0; j < 2; ++j)
			{
				for (int k = 0; k < _TeamStruct_VS._TeamStruct[j].m_uTemaPlayerNum; ++k)
				{
					if (_TeamStruct_VS._TeamStruct[j].m_Userid[k] == myuserid)
					{
						bInVS = true;
						in_TeamStruct_VS = _TeamStruct_VS;
						break;
					}
				}

				if (bInVS)
				{
					break;
				}
			}

		}

		if (bInVS)
		{//��vs��						
			for (int j = 0; j < 2; ++j)
			{
				for (int k = 0; k < in_TeamStruct_VS._TeamStruct[j].m_uTemaPlayerNum; ++k)
				{
					if (in_TeamStruct_VS._TeamStruct[j].m_Userid[k] == myuserid)
					{
						continue;
					}
					CLoginUserInfo* puser = m_CLUserInfoManage.GetOnLineUserInfo(in_TeamStruct_VS._TeamStruct[j].m_Userid[k]);
					if (nullptr != puser)
					{//Ҫ����vs�е��������,���˻�����
						SendData(puser->GetSocketIndex(), pData, uBufLen, bMainID, bAssistantID, bHandleCode);
					}
				}
			}
		}
	}

	//����
	void CMainManage_login::Test()
	{
		//�����ã���ʱ������Ϸ�б�������ڵ���Ҫ��ʵ����Ϸ���������½������ͨ��
		{//��Ϸ
			ComGameinfo cmd[2];
			cmd[0].gameid = 10000001;
			sprintf_s(cmd[0].gamename, "Ӣ������");
			m_CGameList.addgame_node(cmd[0]);

			cmd[1].gameid = 10000002;
			sprintf_s(cmd[1].gamename, "¯ʯ��˵");
			m_CGameList.addgame_node(cmd[1]);
		}

		{//����
		ComEreainfo cmd[5];
		cmd[0].ereaid = 0;
		sprintf_s(cmd[0].ereaname, "��ŷ����");
		cmd[0].gameid = 10000001;
		m_CGameList.adderea_node(cmd[0]);

		cmd[1].ereaid = 1;
		sprintf_s(cmd[1].ereaname, "�氲");
		cmd[1].gameid = 10000001;
		m_CGameList.adderea_node(cmd[1]);

		cmd[2].ereaid = 2;
		sprintf_s(cmd[2].ereaname, "�����");
		cmd[2].gameid = 10000001;
		m_CGameList.adderea_node(cmd[2]);

		cmd[3].ereaid = 3;
		sprintf_s(cmd[3].ereaname, "���ү");
		cmd[3].gameid = 10000002;
		m_CGameList.adderea_node(cmd[3]);

		cmd[4].ereaid = 4;
		sprintf_s(cmd[4].ereaname, "�������");
		cmd[4].gameid = 10000002;
		m_CGameList.adderea_node(cmd[4]);
	}

		{//����
			ComContestInfo cmd[6];
			cmd[0].contestid = 1;
			cmd[0].gameid = 10000001;
			cmd[0].uContestRule = US_ContestRule_solo;
			cmd[0].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[0].contestname, "����solo1v1��׼��");
			sprintf_s(cmd[0].cContestDescribe, "����solo1v1��׼�����ǳ�����˼���Ͻ����棡");
			cmd[0].ereaid = 0;
			cmd[0]._i64cost = 1000;
			cmd[0].award = 1;
			m_CGameList.addcontest_node(cmd[0]);

			cmd[1].contestid = 2;
			cmd[1].gameid = 10000001;
			cmd[1].uContestRule = US_ContestRule_solo;
			cmd[1].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[1].contestname, "����solo2v2��׼��");
			sprintf_s(cmd[1].cContestDescribe, "����solo2v2��׼�����ǳ�����˼���Ͻ����棡");
			cmd[1].ereaid = 0;
			cmd[1]._i64cost = 1000;
			cmd[1].award = 2;
			m_CGameList.addcontest_node(cmd[1]);

			cmd[2].contestid = 3;
			cmd[2].gameid = 10000001;
			cmd[2].uContestRule = US_ContestRule_solo;
			cmd[2].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[2].contestname, "����solo��׼��");
			sprintf_s(cmd[2].cContestDescribe, "����solo��׼�����ǳ�����˼���Ͻ����棡");
			cmd[2].ereaid = 1;
			cmd[2]._i64cost = 1000;
			cmd[2].award = 3;
			m_CGameList.addcontest_node(cmd[2]);

			cmd[3].contestid = 4;
			cmd[3].gameid = 10000001;
			cmd[3].uContestRule = US_ContestRule_solo;
			cmd[3].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[3].contestname, "����solo5v5��ʦ��");
			sprintf_s(cmd[3].cContestDescribe, "����solo5v5��ʦ�����ǳ�����˼���Ͻ����棡");
			cmd[3].ereaid = 2;
			cmd[3]._i64cost = 1000;
			cmd[3].award = 1;
			m_CGameList.addcontest_node(cmd[3]);

			cmd[4].contestid = 5;
			cmd[4].gameid = 10000002;
			cmd[4].uContestRule = US_ContestRule_solo;
			cmd[4].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[4].contestname, "����solo1v1������");
			sprintf_s(cmd[4].cContestDescribe, "����solo1v1���������ǳ�����˼���Ͻ����棡");
			cmd[4].ereaid = 3;
			cmd[4]._i64cost = 1000;
			cmd[4].award = 100;
			m_CGameList.addcontest_node(cmd[4]);

			cmd[5].contestid = 6;
			cmd[5].gameid = 10000002;
			cmd[5].uContestRule = US_ContestRule_solo;
			cmd[5].uContestKind = US_ContestRule_happy;
			sprintf_s(cmd[5].contestname, "����solo1v1��ʦ��");
			sprintf_s(cmd[5].cContestDescribe, "����solo1v1��ʦ�����ǳ�����˼���Ͻ����棡");
			cmd[5].ereaid = 4;
			cmd[5]._i64cost = 1000;
			cmd[5].award = 100;
			m_CGameList.addcontest_node(cmd[5]);
		}
	}

	//�������ͺ���
	void CMainManage_login::SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode)
	{
		map<long, LUserStruct*>* pMap = m_CLUserInfoManage.GetLUserStruct_map();
		auto iter = pMap->begin(), end = pMap->end();
		for (; iter != end; ++iter)
		{
			if (usertype_online == iter->second->usertype)
			{
				SendData(iter->second->userinfo.GetSocketIndex(), pData, uSize, uMainID, uAssistantID, uHandleCode);
			}
		}
	}

}
