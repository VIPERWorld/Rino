#include "redisDataListManage.h"

#include "json/json.h"


#include "common/WJBaseType.h"
#include "server/MD5.h"
#include "common/commonfunction.h"
#include "common/WJGameInfo.h"
#include "common\QkLogger.h"
#include "server\MD5.h"
#include "Redis/WJR_agreement_hall.h"

#include <vector>
#include <assert.h>
#include <string>
#include <string.h>

using namespace std;



namespace WJ
{
	void onRedis_Order_h_messid_load_hall(CRedisWorkerThread *pDbWorker, void *pArg)
	{
		CTransaction_load_hall * pTransObj = (CTransaction_load_hall*)pArg;
		assert(pTransObj);
		RRLine_c_load_hall* pIn = &pTransObj->m_in;

		RRLine_s_load_hall* pOut = &pTransObj->m_out;
		pOut->uIndex = pIn->uIndex;
		char key[MAX_PATH] = { 0 };

		string str = "";
		if (!pIn->bWX)
		{//��ͨ��¼
			//�û���
			memcpy(pOut->_data.basedata.szName, pIn->data.username, sizeof(pOut->_data.basedata.szName));
			//userid
			sprintf_s(key, "username:%s", pIn->data.username);
			pDbWorker->get(key, str);
			pOut->_data.basedata.dwUserID = atoi(str.c_str());
			if (0 == pOut->_data.basedata.dwUserID)
			{//��Ҳ�����
				QK_WarnLog("sdp_test hall redis���� �û���¼���� ��Ҳ����� key=%s", string(key).c_str());
				pOut->uHandleRusult = RRLine_s_load_hall::e_login_err_unexist;
				return;
			}

			//����
			MD5 md5;
			string str_passwd = pIn->data.password;
			string str_passwd_md5 = md5.ToMD5(str_passwd);
			sprintf_s(key, "user:%d passwd", pOut->_data.basedata.dwUserID);
			pDbWorker->hget(key, str);
			sprintf_s(pOut->_data.basedata.password, "%s", pIn->data.password);//ֻ����δ���ܵ�����
			if (0 != strcmp(str.c_str(), str_passwd_md5.c_str()))
			{
				QK_WarnLog("sdp_test hall redis���� �û���¼���� ���벻�� %s��%s", str.c_str(), str_passwd_md5.c_str());
				//���벻��
				pOut->uHandleRusult = RRLine_s_load_hall::e_login_err_password;
				return;
			}
		}
		else
		{//΢��
			//�û���
			memcpy(pOut->_data.basedata.szName, pIn->data2.username, sizeof(pOut->_data.basedata.szName));
			//userid
			sprintf_s(key, "username:%s", pIn->data2.username);
			pDbWorker->get(key, str);
			pOut->_data.basedata.dwUserID = atoi(str.c_str());
			if (0 == pOut->_data.basedata.dwUserID)
			{//��Ҳ�����
				QK_WarnLog("sdp_test hall redis���� �û���¼����wx ��Ҳ����� key=%s", string(key).c_str());
				pOut->uHandleRusult = RRLine_s_load_hall::e_login_err_unexist;
				return;
			}

			sprintf_s(key, "redis_weixinuser_yan%d", pIn->data2.userid);
			QK_InfoLog("sdp_test hall redis���� �û���¼���� wx key=%s��", string(key).c_str());
			//int ires = get_int(key);
			pDbWorker->get(key, str);
			int ires = atoi(str.c_str());
			if (1 != ires)
			{
				//���벻��(΢�ŵ�½ʧ��)
				pOut->uHandleRusult = RRLine_s_load_hall::e_login_err_password;
				QK_WarnLog("sdp_test %s%d ΢�ŵ�½ʧ�ܣ�", string(pIn->data2.wx_key).c_str(), pIn->data2.userid);
				return;
			}
		}
		//�ǳ�
		sprintf_s(key, "user:%d nickname", pOut->_data.basedata.dwUserID);
		//str = hget_str(key);
		pDbWorker->hget(key, str);
		if (IsTextUTF8(str.c_str(), str.length()))
		{
			char* res = nullptr;
			res = U2G(str.c_str());
			memcpy(pOut->_data.basedata.nickName, res, sizeof(pOut->_data.basedata.nickName));
			WJ_SAFE_DELETE(res);
		}
		else
		{
			memcpy(pOut->_data.basedata.nickName, str.c_str(), sizeof(pOut->_data.basedata.nickName));
		}
		//����
		sprintf_s(key, "redis_user_coin%d", pOut->_data.basedata.dwUserID);
		pDbWorker->get(key, str);
		pOut->_data.basedata.dwPoint = atoi(str.c_str());
		//���
		sprintf_s(key, "redis_user_chip%d", pOut->_data.basedata.dwUserID);
		pDbWorker->get(key, str);
		pOut->_data.basedata.i64Money = atoi(str.c_str());
		//�绰����
		sprintf_s(key, "user:%d telephone idcard icon robot", pOut->_data.basedata.dwUserID);
		vector<string> userInfoStrs;
		pDbWorker->hmget(key, userInfoStrs);
		memcpy(pOut->_data.telephone, userInfoStrs[0].c_str(), sizeof(pOut->_data.telephone));
		//���֤����
		memcpy(pOut->_data.IDCard, userInfoStrs[1].c_str(), sizeof(pOut->_data.IDCard));
		//����ͷ����Ϣ
		memcpy(pOut->_data.basedata.icon, userInfoStrs[2].c_str(), min(userInfoStrs[2].size(), sizeof(pOut->_data.basedata.icon)));
		//�Ƿ�Ϊ������
		pOut->_data.basedata.bRobot = (atoi(userInfoStrs[3].c_str()) != 0);
		//��½�ɹ�
		pOut->uHandleRusult = RRLine_s_load_hall::e_login_suc;
	}

	//redis���� ��Ҵ����Ŷ�
	void onRedis_Order_h_messid_team_create(CRedisWorkerThread *pDbWorker, void *pArg)
	{//ʵ���ϴ����ŶӶ���redis��˵��Ȼ�Ǳ���
		CTransaction_team_create *pTransObj = (CTransaction_team_create *)pArg;
		assert(pTransObj);
		RRLine_c_team_create* pIn = &pTransObj->m_in;

		RRLine_s_team_create* pOut = &pTransObj->m_out;
		if (US_ContestKind_free == pIn->uContestKind)
		{
			pOut->bSuc = true;
		}
		else
		{
			Json::Value root;
			root["type"] = (int)pIn->uContestKind;
			root["userid"] = (int)pIn->userid;
			root["contestid"] = (int)pIn->ucontestid;

			if (US_ContestRule_happy == pIn->uContestKind)
			{
				root["change_type"] = (int)e_chip_change_type_apply;
				root["money"] = (int)pIn->changemoney;
			}
			else if (US_ContestRule_point == pIn->uContestKind)
			{
				root["change_type"] = (int)e_coin_change_type_apply;
				root["money"] = (int)pIn->changepoint;
			}
			char value_str[MAX_PATH] = { 0 };
			Json::FastWriter fast_writer;
			std::string json_str = fast_writer.write(root);
			//std::string json_str = root.toStyledString();
			sprintf_s(value_str, "%s", json_str.c_str());	
			pDbWorker->lpush("apply", value_str);

			//bool brun = command_LPUSH("apply", value_str);//���Ͷ���
			/*
			if (brun)
			{
				std::memset(value_str, 0, sizeof(value_str));
				sprintf_s(value_str, "apply_res_%d_%d", p->userid, p->uContestKind);
				bool bres = command_BRPOP_bool(value_str);//��ȡ���
				cmd.bSuc = bres;
			}
			else
			{
				cmd.bSuc = false;
			}
			*/
		}
		pOut->bSuc = true;
		pOut->changemoney = pIn->changemoney;
		pOut->changepoint = pIn->changepoint;
		pOut->ucontestid = pIn->ucontestid;
		pOut->uContestKind = pIn->uContestKind;
		pOut->userid = pIn->userid;
		pOut->uIndex = pIn->uIndex;
		pOut->m_TeamPayType = pIn->m_TeamPayType;
		pOut->uRoomRule = pIn->uRoomRule;
	}

	//redis���� ��Ҽ����Ŷ�
	void onRedis_Order_h_messid_team_join(CRedisWorkerThread *pDbWorker, void *pArg)
	{//ʵ���ϴ����ŶӶ���redis��˵��Ȼ�Ǳ���
		CTransaction_team_join *pTransObj = (CTransaction_team_join *)pArg;
		assert(pTransObj);
		RRLine_c_team_join* pIn = &pTransObj->m_in;
		RRLine_s_team_join* pOut = &pTransObj->m_out;
		if (US_ContestKind_free == pIn->uContestKind)
		{
			pOut->bSuc = true;
		}
		else
		{
			Json::Value root;
			root["type"] = (int)pIn->uContestKind;
			root["userid"] = (int)pIn->userid;
			root["contestid"] = (int)pIn->ucontestid;

			if (US_ContestRule_happy == pIn->uContestKind)
			{
				root["change_type"] = (int)e_chip_change_type_apply;
				root["money"] = (int)pIn->changemoney;
			}
			else if (US_ContestRule_point == pIn->uContestKind)
			{
				root["change_type"] = (int)e_coin_change_type_apply;
				root["money"] = (int)pIn->changepoint;
			}

			char value_str[MAX_PATH] = { 0 };
			Json::FastWriter fast_writer;
			std::string json_str = fast_writer.write(root);
			//std::string json_str = root.toStyledString();
			sprintf_s(value_str, "%s", json_str.c_str());
			/*
			bool brun = command_LPUSH("apply", value_str);//���Ͷ���
			if (brun)
			{
				std::memset(value_str, 0, sizeof(value_str));
				sprintf_s(value_str, "apply_res_%d_%d", p->userid, p->uContestKind);
				bool bres = command_BRPOP_bool(value_str);//��ȡ���
				cmd.bSuc = bres;
			}
			else
			{
				cmd.bSuc = false;
			}
			*/
			pDbWorker->lpush("apply", value_str);
		}
		pOut->bSuc = true;
		pOut->changemoney = pIn->changemoney;
		pOut->changepoint = pIn->changepoint;
		pOut->ucontestid = pIn->ucontestid;
		pOut->uContestKind = pIn->uContestKind;
		pOut->userid = pIn->userid;
		pOut->uIndex = pIn->uIndex;
		std::memcpy(pOut->m_Team_password, pIn->m_Team_password, sizeof(pOut->m_Team_password));
	}

	//redis���� ����˳��Ŷ�
	void onRedis_Order_h_messid_team_leave(CRedisWorkerThread *pDbWorker, void *pArg)
	{//ʵ�����˳��ŶӶ���redis��˵��Ȼ���˷�
		CTransaction_team_leave *pTransObj = (CTransaction_team_leave *)pArg;
		assert(pTransObj);
		RRLine_c_team_leave* pIn = &pTransObj->m_in;
		RRLine_s_team_leave* pOut = &pTransObj->m_out;
		if (US_ContestKind_free == pIn->uContestKind)
		{
			pOut->bSuc = true;
		}
		else
		{

			Json::Value root;
			root["type"] = (int)pIn->uContestKind;
			root["userid"] = (int)pIn->userid;
			root["contestid"] = (int)pIn->ucontestid;

			if (US_ContestRule_happy == pIn->uContestKind)
			{
				root["change_type"] = (int)e_chip_change_type_apply_restitution;
				root["money"] = (int)pIn->changemoney;
			}
			else if (US_ContestRule_point == pIn->uContestKind)
			{
				root["change_type"] = (int)e_coin_change_type_apply_restitution;
				root["money"] = (int)pIn->changepoint;
			}

			char value_str[MAX_PATH] = { 0 };
			Json::FastWriter fast_writer;
			std::string json_str = fast_writer.write(root);
			//std::string json_str = root.toStyledString();
			sprintf_s(value_str, "%s", json_str.c_str());
			/*
			bool brun = command_LPUSH("restitution", value_str);//���Ͷ���
			if (brun)
			{
				std::memset(value_str, 0, sizeof(value_str));
				sprintf_s(value_str, "restitution_res_%d_%d", p->userid, p->uContestKind);
				bool bres = command_BRPOP_bool(value_str);//��ȡ���
				cmd.bSuc = bres;
			}
			else
			{
				cmd.bSuc = false;
			}
			*/
			pDbWorker->lpush("restitution", value_str);
		}
		pOut->bSuc = true;
		pOut->changemoney = pIn->changemoney;
		pOut->changepoint = pIn->changepoint;
		pOut->ucontestid = pIn->ucontestid;
		pOut->uContestKind = pIn->uContestKind;
		pOut->userid = pIn->userid;
		pOut->uIndex = pIn->uIndex;
		std::memcpy(pOut->m_Team_password, pIn->m_Team_password, sizeof(pOut->m_Team_password));
	}

	//redis���� ��ҽ�ɢ�Ŷ�
	void onRedis_Order_h_messid_team_dissolve(CRedisWorkerThread *pDbWorker, void *pArg)
	{
		CTransaction_team_dissolve *pTransObj = (CTransaction_team_dissolve *)pArg;
		assert(pTransObj);
		RRLine_c_team_dissolve* pIn = &pTransObj->m_in;
		RRLine_s_team_dissolve* pOut = &pTransObj->m_out;
		bool bSuc = false;//ֻҪ��һ���˿۷ѳɹ������ǳɹ�
		if (US_ContestKind_free == pIn->uContestKind)
		{
			bSuc = true;
		}
		else
		{
			for (int i = 0; i < pIn->m_uTemaPlayerNum; ++i)
			{
				Json::Value root;
				root["type"] = (int)pIn->uContestKind;
				root["userid"] = (int)pIn->userid[i];
				root["contestid"] = (int)pIn->ucontestid;

				if (US_ContestRule_happy == pIn->uContestKind)
				{
					root["change_type"] = (int)e_chip_change_type_apply_restitution;
					root["money"] = (int)pIn->changemoney[i];
				}
				else if (US_ContestRule_point == pIn->uContestKind)
				{
					root["change_type"] = (int)e_coin_change_type_apply_restitution;
					root["money"] = (int)pIn->changepoint[i];
				}

				
				char value_str[MAX_PATH] = { 0 };
				Json::FastWriter fast_writer;
				std::string json_str = fast_writer.write(root);
				//std::string json_str = root.toStyledString();
				sprintf_s(value_str, "%s", json_str.c_str());
				/*
				bool brun = command_LPUSH("restitution", value_str);//���Ͷ���
				if (brun)
				{
					std::memset(value_str, 0, sizeof(value_str));
					sprintf_s(value_str, "restitution_res_%d_%d", p->userid[i], p->uContestKind);
					bool bres = command_BRPOP_bool(value_str);//��ȡ���
					if (bres)
					{
						bSuc = true;
					}
				}
				*/
				pDbWorker->lpush("restitution", value_str);
			}
		}

		pOut->bSuc = true;
		std::memcpy(pOut->changemoney, pIn->changemoney, sizeof(pOut->changemoney));
		std::memcpy(pOut->changepoint, pIn->changepoint, sizeof(pOut->changepoint));
		std::memcpy(pOut->userid, pIn->userid, sizeof(pOut->userid));
		pOut->ucontestid = pIn->ucontestid;
		pOut->uContestKind = pIn->uContestKind;
		pOut->uIndex = pIn->uIndex;
		pOut->m_uTemaPlayerNum = pIn->m_uTemaPlayerNum;
		std::memcpy(pOut->m_Team_password, pIn->m_Team_password, sizeof(pOut->m_Team_password));
	}

	//redis���� �����û�����(��ҡ�����)
	void onRedis_Order_h_messid_update_userinfo(CRedisWorkerThread *pDbWorker, void *pArg)
	{
		CTransaction_Update_Userinfo *pTransObj = (CTransaction_Update_Userinfo *)pArg;
		assert(pTransObj);
		RedisDataBaseLine* pIn = &pTransObj->m_in;

		std::vector<std::string> res;
		pDbWorker->rpop("money_cue", res);

		Json::Reader reader;
		Json::Value root;
		auto _iter = res.begin();
		while (_iter != res.end())
		{
			if (reader.parse((*_iter), root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
			{
				RRLine_s_Update_Userinfo cmd;

				cmd.iuserid = root["userId"].asInt();
				cmd.itype = root["type"].asInt();
				if (1 == cmd.itype)
				{//����
					cmd.dwPoint = root["amount"].asInt();
				}
				else if (2 == cmd.itype)
				{//���
					cmd.i64Money = root["amount"].asInt();
				}
				cmd.uIndex = pIn->uIndex;
				pTransObj->m_out_vector.push_back(cmd);
			}

			++_iter;
		}	
	}

	//redis���� ��ұ���
	void onRedis_Order_h_messid_apply(CRedisWorkerThread *pDbWorker, void *pArg)
	{
		CTransaction_apply *pTransObj = (CTransaction_apply *)pArg;
		assert(pTransObj);
		RRLine_c_apply* pIn = &pTransObj->m_in;
		RRLine_s_apply* pOut = &pTransObj->m_out;
		if (US_ContestKind_free == pIn->uContestKind)
		{
			pOut->bSuc = true;
		}
		else
		{
			Json::Value root;
			root["type"] = (int)pIn->uContestKind;
			root["userid"] = (int)pIn->userid;
			root["contestid"] = (int)pIn->ucontestid;


			if (US_ContestRule_happy == pIn->uContestKind)
			{
				root["change_type"] = (int)e_chip_change_type_apply;
				root["money"] = (int)pIn->changemoney;
			}
			else if (US_ContestRule_point == pIn->uContestKind)
			{
				root["change_type"] = (int)e_coin_change_type_apply;
				root["money"] = (int)pIn->changepoint;
			}

			
			char value_str[MAX_PATH] = { 0 };
			Json::FastWriter fast_writer;
			std::string json_str = fast_writer.write(root);
			//std::string json_str = root.toStyledString();
			sprintf_s(value_str, "%s", json_str.c_str());
			/*
			bool brun = command_LPUSH("apply", value_str);//���Ͷ���
			if (brun)
			{
 				std::memset(value_str, 0, sizeof(value_str));
				sprintf_s(value_str, "apply_res_%d_%d", p->userid, p->uContestKind);
 				bool bres = command_BRPOP_bool(value_str);//��ȡ���
				cmd.bSuc = brun;
			}
			else
			{
				cmd.bSuc = false;
			}
			*/
			pDbWorker->lpush("apply", value_str);
		}
		pOut->bSuc = true;
		pOut->changemoney = pIn->changemoney;
		pOut->changepoint = pIn->changepoint;
		pOut->ucontestid = pIn->ucontestid;
		pOut->uContestKind = pIn->uContestKind;
		pOut->userid = pIn->userid;
		pOut->uIndex = pIn->uIndex;
	}

	//redis���� ����˷�
	void onRedis_Order_h_messid_apply_restitution(CRedisWorkerThread *pDbWorker, void *pArg)
	{
		CTransaction_apply_restitution *pTransObj = (CTransaction_apply_restitution *)pArg;
		assert(pTransObj);
		RRLine_c_apply_restitution* pIn = &pTransObj->m_in;
		RRLine_s_apply_restitution* pOut = &pTransObj->m_out;
		if (US_ContestKind_free == pIn->uContestKind)
		{
			pOut->bSuc = true;
		}
		else
		{
			Json::Value root;
			root["type"] = (int)pIn->uContestKind;
			root["userid"] = (int)pIn->userid;
			root["contestid"] = (int)pIn->ucontestid;
			if (US_ContestRule_happy == pIn->uContestKind)
			{
				root["change_type"] = (int)e_chip_change_type_apply_restitution;
				root["money"] = (int)pIn->changemoney;
			}
			else if (US_ContestRule_point == pIn->uContestKind)
			{
				root["change_type"] = (int)e_coin_change_type_apply_restitution;
				root["money"] = (int)pIn->changepoint;
			}

			char value_str[MAX_PATH] = { 0 };
			Json::FastWriter fast_writer;
			std::string json_str = fast_writer.write(root);
			//std::string json_str = root.toStyledString();
			sprintf_s(value_str, "%s", json_str.c_str());
			/*
			bool brun = command_LPUSH("restitution", value_str);//���Ͷ���
			if (brun)
			{
 				std::memset(value_str, 0, sizeof(value_str));
 				sprintf_s(value_str, "restitution_res_%d_%d", p->userid, p->uContestKind);
 				bool bres = command_BRPOP_bool(value_str);//��ȡ���
			}
			else
			{
				cmd.bSuc = false;
			}
			*/
			pDbWorker->lpush("restitution", value_str);
		}

		pOut->bSuc = true;
		pOut->changemoney = pIn->changemoney;
		pOut->changepoint = pIn->changepoint;
		pOut->ucontestid = pIn->ucontestid;
		pOut->uContestKind = pIn->uContestKind;
		pOut->userid = pIn->userid;
		pOut->uIndex = pIn->uIndex;
	}

}
