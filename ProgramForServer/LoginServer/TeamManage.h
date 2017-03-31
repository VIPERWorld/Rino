#ifndef WJ_TeamManage_H
#define WJ_TeamManage_H

#include <vector>
#include <mutex>
#include "common/WJBaseType.h"
#include "server/comstruct.h"

namespace WJ
{//�Ŷӹ���
	class CTeamManage
	{
	public:
		CTeamManage();
		~CTeamManage();

		//���
		bool add(TeamStruct &data);
		//ɾ��
		void del(UINT uCaptain_userid);
		void del(char* Team_password);
		void delAll();
		//����
		bool find(UINT userid, TeamStruct &data);
		bool find(char* Team_password, TeamStruct &data);

		//��Ҽ����Ŷ�
		bool userAdd(UINT userid, char* Team_password);
		//����뿪�Ŷ�
		bool userDel(UINT userid, char* Team_password);

		//�����Ŷӱ����䵽����
		void AllotRoom(char* Team_password, UINT uroomid);
		//�����Ŷ�δ�����䵽����
		void unAllotRoom(char* Team_password);
		//��øñ����ж������Ŷӻ�û�д���
		UINT getteamcount(UINT ucontestid);
		//����Ŷ����ж������Ŷӻ�û�д���
		UINT getteamcount_unplaying();
		//����Ŷ����ж������Ŷ��Ѿ��ڱ�����
		UINT getteamcount_playing();

		//����Ŷ��б�����
		std::vector<TeamStruct> getTeamStruct_list();

		//�Ƿ�����������ĳ�����иð���
		bool bExistTeam_password(char* Team_password);

		//���������Ƿ�����
		bool bFull(char* Team_password);

		//����Ŷ��û���Ϣ
		bool GetUsers(char* Team_password, UINT Userid[], UINT &uTemaPlayerNum);

		//�ӳ�ͬ�⿪ʼƥ��
		void setButVS(char* Team_password, bool ballow);
		//��ѯ�ӳ��Ƿ�ͬ�⿪ʼƥ��
		bool getButVS(char* Team_password);

		//��ƥ�������
		UINT Count_VS();
		//Ϊĳ�Ŷ�����ƥ��
		bool add_VS(char* Team_password, TeamStruct_VS &_TeamStruct_VS, UINT uroomid);

		//ɾ��ƥ��ڵ�
		void del_VS(UINT index);
		//�õ�ƥ��ڵ�
		void at_vs(UINT index, TeamStruct_VS &_TeamStruct_VS);
	private:
		//���������
		int MyRand();
		//����������
		void GetRandText(std::string &str, int strleng);

		//���ñ�ƥ����
		void setbVS(char* Team_password, bool bVS);
	private:
		std::vector<TeamStruct> m_list;//�ŶӶ���
		std::vector<std::string> m_pass;//δʹ�õ��ŶӰ��Ŵ洢��

		std::vector<TeamStruct_VS> m_vs_list;//ƥ�����
	};
}

#endif//WJ_TeamManage_H