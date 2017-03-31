#include "TeamManage.h"
#include <windows.h>
namespace WJ
{

	CTeamManage::CTeamManage()
	{

	}
	
	CTeamManage::~CTeamManage()
	{

	}

	//���
	bool CTeamManage::add(TeamStruct &data)
	{
		TeamStruct data2;
		if (find(data.m_uCaptain_userid, data2))
		{//������Ѿ����Ŷ���
			return false;
		}
		
		//�ŶӰ�����������ɵģ������ܺ����еĳ�ͻ
		std::string str_passwor;
		while (1)
		{
			if (0 < m_pass.size())
			{//���ų�һ���У����ٴ�ʹ��
				str_passwor = m_pass.at(0);
				m_pass.erase(m_pass.begin());
			}
			else
			{
				//��������ŶӰ���
				GetRandText(str_passwor, TEAM_PASS_LENGTH - 1);
			}

			if (bExistTeam_password(const_cast<char*>(str_passwor.c_str())))
			{//����Ŷӹ����б����Ѿ�������
				continue;
			}

			//�ϲ�ҵ��ȷ��������ȷ������ֻ�������ŶӰ���
			sprintf_s(data.m_Team_password, "%s", str_passwor.c_str());
			data.m_Userid[0] = data.m_uCaptain_userid;
			m_list.push_back(data);
			break;
		};

		return true;
	}

	//ɾ��
	void CTeamManage::del(UINT uCaptain_userid)
	{
		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			if ((*iter).m_uCaptain_userid == uCaptain_userid)
			{				
				if (!bExistTeam_password((*iter).m_Team_password))
				{
					std::string str_passwor;
					str_passwor = (*iter).m_Team_password;
					m_pass.push_back(str_passwor);
				}

				m_list.erase(iter);
				return;
			}
			++iter;
		}
	}

	void CTeamManage::del(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			if (0 == strcmp((*iter).m_Team_password, Team_password))
			{
				if (!bExistTeam_password((*iter).m_Team_password))
				{
					std::string str_passwor;
					str_passwor = (*iter).m_Team_password;
					m_pass.push_back(str_passwor);
				}

				m_list.erase(iter);
				return;
			}
			++iter;
		}
	}

	void CTeamManage::delAll()
	{
		m_list.clear();
	}

	//����
	bool CTeamManage::find(UINT userid, TeamStruct &data)
	{
		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			for (int i = 0; i < (*iter).m_uTemaPlayerNum; ++i)
			{
				if ((*iter).m_Userid[i] == userid)
				{
					std::memcpy(&data, &(*iter), sizeof(TeamStruct));
					return true;
				}
			}

			++iter;
		}

		return false;
	}

	bool CTeamManage::find(char* Team_password, TeamStruct &data)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		auto iter = m_list.begin();
		while (iter != m_list.end())
		{
			if (0==strcmp((*iter).m_Team_password, Team_password))
			{
				std::memcpy(&data, &(*iter), sizeof(TeamStruct));
				return true;
			}
			++iter;
		}

		return false;
	}

	//�Ƿ�����������ĳ�����иð���
	bool CTeamManage::bExistTeam_password(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//����Ŷӹ����б����Ѿ�������
				return true;
			}
		}

		return false;
	}

	//���������Ƿ�����
	bool CTeamManage::bFull(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//�ҵ��ö���
				UINT uTemaPlayerNum = m_list.at(i).m_uTemaPlayerNum < TEAM_USER_NUM ? m_list.at(i).m_uTemaPlayerNum : TEAM_USER_NUM;
				UINT uplayers = 0;
				for (int j = 0; j < uTemaPlayerNum; ++j)
				{//����ͳ��
					if (0 != m_list.at(i).m_Userid[j])
					{
						++uplayers;
					}
				}
				//��������������������
				return uplayers == uTemaPlayerNum;
			}
		}

		return false;
	}

	//����Ŷ��û���Ϣ
	bool CTeamManage::GetUsers(char* Team_password, UINT Userid[], UINT &uTemaPlayerNum)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//�ҵ��ö���
				uTemaPlayerNum = m_list.at(i).m_uTemaPlayerNum < TEAM_USER_NUM ? m_list.at(i).m_uTemaPlayerNum : TEAM_USER_NUM;

				for (int j = 0; j < uTemaPlayerNum; ++j)
				{
					Userid[j] = m_list.at(i).m_Userid[j];
				}
				return true;
			}
		}

		return false;
	}

	//�ӳ�ͬ�⿪ʼƥ��
	void CTeamManage::setButVS(char* Team_password, bool ballow)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//�ҵ��ö���
				m_list.at(i).bButVS = ballow;
				break;
			}
		}
	}

	//��ѯ�ӳ��Ƿ�ͬ�⿪ʼƥ��
	bool CTeamManage::getButVS(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//�ҵ��ö���
				return m_list.at(i).bButVS;
			}
		}

		return false;
	}

	//��Ҽ����Ŷ�
	bool CTeamManage::userAdd(UINT userid, char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		TeamStruct data2;
		if (find(userid, data2))
		{//������Ѿ��������Ŷ�
			return false;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//�ҵ��ö���
				UINT uTemaPlayerNum = m_list.at(i).m_uTemaPlayerNum < TEAM_USER_NUM ? m_list.at(i).m_uTemaPlayerNum : TEAM_USER_NUM;
				for (int j = 1; j < uTemaPlayerNum; ++j)
				{//�ӷǶӳ�λ�ÿ�ʼ��
					if (0 == m_list.at(i).m_Userid[j])
					{//�ڹ涨������Χ�ڶԿ�λ�ӽ��и�ֵ
						m_list.at(i).m_Userid[j] = userid;
						return true;
					}
				}
			}
		}

		return false;
	}

	//����뿪�Ŷ�
	bool CTeamManage::userDel(UINT userid, char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return false;
		}

		TeamStruct data2;
		if (find(userid, data2))
		{//������Ѿ��������Ŷ�
			return false;
		}
		
		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//�ҵ��ö���
				UINT uTemaPlayerNum = m_list.at(i).m_uTemaPlayerNum < TEAM_USER_NUM ? m_list.at(i).m_uTemaPlayerNum : TEAM_USER_NUM;
				for (int j = 1; j < uTemaPlayerNum; ++j)
				{//�ӷǶӳ�λ�ÿ�ʼ��
					if (userid == m_list.at(i).m_Userid[j])
					{//�ڹ涨������Χ�ڶԿ�λ�ӽ��и�ֵ
						m_list.at(i).m_Userid[j] = 0;
						return true;
					}
				}
			}
		}

		return false;
	}

	//�����Ŷӱ����䵽����
	void CTeamManage::AllotRoom(char* Team_password, UINT uroomid)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//�ҵ��ö���
				m_list.at(i).bAllot = true;
				m_list.at(i).uroomid = uroomid;
				return;
			}
		}
	}

	//�����Ŷ�δ�����䵽����
	void CTeamManage::unAllotRoom(char* Team_password)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//�ҵ��ö���
				m_list.at(i).bAllot = false;
				m_list.at(i).uroomid = 0;
				return;
			}
		}
	}

	//��øñ����ж������Ŷӻ�û�д���
	UINT CTeamManage::getteamcount(UINT ucontestid)
	{
		UINT ucount = 0;

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (ucontestid == m_list.at(i).m_uContest
				&& !m_list.at(i).bAllot)
			{//�ҵ��ö���
				++ucount;
			}
		}

		return ucount;
	}

	//����Ŷ����ж������Ŷӻ�û�д���
	UINT CTeamManage::getteamcount_unplaying()
	{
		UINT ucount = 0;

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (!m_list.at(i).bAllot)
			{//�ҵ��ö���
				++ucount;
			}
		}

		return ucount;
	}

	//����Ŷ����ж������Ŷ��Ѿ��ڱ�����
	UINT CTeamManage::getteamcount_playing()
	{
		UINT ucount = 0;

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (m_list.at(i).bAllot)
			{//�ҵ��ö���
				++ucount;
			}
		}

		return ucount;
	}

	//����Ŷ��б�����
	std::vector<TeamStruct> CTeamManage::getTeamStruct_list()
	{
		return m_list;
	}

	int CTeamManage::MyRand()
	{
		static int innerSeed = GetTickCount();

		return (((innerSeed = innerSeed * 214013L + 2531011L) >> 16) & 0x7fff);
	}

	//����������
	void CTeamManage::GetRandText(std::string &str, int strleng)
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

	//ƥ�������
	UINT CTeamManage::Count_VS()
	{
		return m_vs_list.size();
	}

	//���ƥ��ڵ�
	bool CTeamManage::add_VS(char* Team_password, TeamStruct_VS &_TeamStruct_VS, UINT uroomid)
	{
		TeamStruct data;
		//�Ƿ����ҵ�
		if (!find(Team_password, data))
		{
			return false;
		}
		//�Ƿ�����
		if (!bFull(Team_password))
		{
			return false;
		}

		if (data.bVS)
		{//�Ѿ�ƥ����
			return false;
		}

		if (data.bButVS)
		{//�ӳ�δͬ��
			return false;
		}
		
		for (int i = 0; i < m_list.size();++i)
		{
			if (m_list.at(i).m_uContest == data.m_uContest
				&& m_list.at(i).m_uCaptain_userid != data.m_uCaptain_userid
				&& bFull(m_list.at(i).m_Team_password)
				&& m_list.at(i).bButVS
				&& !m_list.at(i).bVS)
			{//ͬһ�����У�����ͬһ����,���������࣬�Ҷӳ�ͬ��ƥ�䣬��δƥ����Ϳ���ƥ��
				
				_TeamStruct_VS._TeamStruct[0] = data;
				_TeamStruct_VS._TeamStruct[1] = m_list.at(i);

				m_list.at(i).bVS = true;
				m_list.at(i).bAllot = true;
				m_list.at(i).uroomid = uroomid;
				setbVS(Team_password, true);
				AllotRoom(Team_password, uroomid);
				return true;
			}
		}
		return true;
	}

	//���ñ�ƥ����
	void CTeamManage::setbVS(char* Team_password, bool bVS)
	{
		if (nullptr == Team_password)
		{
			return;
		}

		for (int i = 0; i < m_list.size(); ++i)
		{
			if (0 == strcmp(Team_password, m_list.at(i).m_Team_password))
			{//�ҵ��ö���
				m_list.at(i).bVS = bVS;
				return;
			}
		}
	}

	//ɾ��ƥ��ڵ�
	void CTeamManage::del_VS(UINT index)
	{
		if (index >= m_vs_list.size())
		{
			return;
		}

		m_vs_list.erase(m_vs_list.begin() + index);
	}

	//�õ�ƥ��ڵ�
	void CTeamManage::at_vs(UINT index, TeamStruct_VS &_TeamStruct_VS)
	{
		if (index >= m_vs_list.size())
		{
			return;
		}

		_TeamStruct_VS = m_vs_list.at(index);
	}
}