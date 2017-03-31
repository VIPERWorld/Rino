#ifndef WJ_RTeamManage_H
#define WJ_RTeamManage_H

#include "common/WJBaseType.h"
#include "server/comstruct.h"
#include <vector>

/*
�����ڵ��Ŷ��������崦��ģ��������Ŷ��ڲ������뿪���߿�ʼ
*/

namespace WJ
{//�Ŷӹ���
	class CRTeamManage
	{
	public:
		CRTeamManage();
		~CRTeamManage();

		//���
		bool add(TeamStruct &data);
		//ɾ��
		void del(UINT uCaptain_userid);
		void del(char* Team_password);
		void delAll();
		//����
		bool find(UINT uCaptain_userid, TeamStruct &data);
		bool find(char* Team_password, TeamStruct &data);

		//���������Ƿ�����
		bool bFull(char* Team_password);

		//����Ŷ��û���Ϣ
		bool GetUsers(char* Team_password, UINT Userid[], UINT &uTemaPlayerNum);
	private:
		std::vector<TeamStruct> m_list;//�ŶӶ���
	};
}

#endif//WJ_RTeamManage_H