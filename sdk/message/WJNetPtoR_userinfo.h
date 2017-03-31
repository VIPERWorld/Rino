#ifndef _WJ_WJNetPtoR_userinfo_H_
#define _WJ_WJNetPtoR_userinfo_H_

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"
namespace WJ
{
	//e_pr_mess_userinfo = 501,//�û���Ϣ
	enum E_PtoR_ASSID_enum_userinfo
	{
		e_pr_ass_userinfo_login = 1,//��ҵ�¼��Ϸ������
		e_pr_ass_userinfo_allot_room = 2,//gs����ls��Ҽ��뷿��
		e_pr_ass_userinfo_apply_restitution = 3,//����ѯ�ʷ��䣬����Ƿ�����˷�
		e_pr_ass_userinfo_unplay = 4,//����뿪����
		e_pr_ass_userinfo_gameover = 5,//��Ϸ������֪ͨ����������Ѿ�������Ϸ
		e_pr_ass_userinfo_team_leave = 6,//����ѯ�ʷ��䣬����Ƿ�����˳��Ŷ�
		e_pr_ass_userinfo_team_dissolve = 7,//����ѯ�ʷ��䣬����Ƿ���Խ�ɢ�Ŷ�
		e_pr_ass_userinfo_team_start = 8,//����֪ͨ��ر����ķ��䣬�Ŷ�׼������
		e_pr_ass_userinfo_team_vs_finish = 9,//��Ϸ������֪ͨ�������Ŷ��Ѿ�������Ϸ
	};

	//��Ϸ��������֪��½������������ҵ�¼��Ϸ������������ȷ��
	struct struct_data_pr_userinfo_login
	{
		UINT uuserid;//�û�id
		UINT contestid;
		UINT uIndex;//��ҵ�ʱ������Ϸ������������׽���id
		struct_data_pr_userinfo_login()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_login));
			uIndex = ERROR_SOCKETID;
		}
	};

	enum E_hcid_enum_pr_userinfo_login
	{
		e_pr_hcid_login_allow = 1,//�������
		e_pr_hcid_login_unallow = 2,//���������
	};

	//��ҵ�¼��Ϸ�������������Ϣ��
	struct struct_data_pr_userinfo_login_res_ok
	{
		UserInfoInHallStruct data;
		UINT contestid;
		UINT uIndex;//��ҵ�ʱ������Ϸ������������׽���id
		struct_data_pr_userinfo_login_res_ok()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_login_res_ok));
			uIndex = ERROR_SOCKETID;
		}
	};

	//��ҵ�¼��Ϸ���������������Ϣ��
	struct struct_data_pr_userinfo_login_res_err
	{
		UINT userid;//�û�id
		UINT uIndex;//��ҵ�ʱ������Ϸ������������׽���id
		struct_data_pr_userinfo_login_res_err()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_login_res_err));
			uIndex = ERROR_SOCKETID;
		}
	};

	///////////////////////////////////////////////////////////////////////
	//��Ϸ��������֪��½������������Ҽ���÷���
	struct struct_data_pr_userinfo_allot_room
	{
		UINT userid;//�û�id
		UINT contestid;//����id
		UINT roomid;//����id
		UINT uIndex;//��ҵ�ʱ������Ϸ������������׽���id
		struct_data_pr_userinfo_allot_room()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_allot_room));
			uIndex = ERROR_SOCKETID;
		}
	};

	//�˷ѳɹ����
	enum E_hcid_enum_pr_userinfo_apply_restitution
	{
		e_pr_hcid_apply_restitution_suc = 1,//�˷ѳɹ�
		e_pr_hcid_apply_restitution_err = 2,//�˷�ʧ��
	};

	//�������Ϸ�����������˷� �� ��Ϸ�������ظ���½�������������Ϣ��
	struct struct_data_pr_userinfo_apply_restitution
	{
		UINT userid;//�û�id

		struct_data_pr_userinfo_apply_restitution()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_apply_restitution));
		}
	};

	//���û���ڷ��������棬����������Ч
	typedef struct_data_pr_userinfo_apply_restitution struct_data_pr_userinfo_unplay;
	//��Ϸ������֪ͨ����������Ѿ�������Ϸ
	typedef struct_data_pr_userinfo_apply_restitution struct_data_pr_userinfo_gameover;

	//��������Ŷ����
	enum E_hcid_enum_pr_userinfo_team_start
	{
		e_pr_hcid_team_start_suc = 1,//��������Ŷӳɹ�
		e_pr_hcid_team_start_err = 2,//��������Ŷ�ʧ��
	};

	//�����֪�������Ŷ�һ����Ϸ����
	struct struct_data_pr_userinfo_team_vs_finish
	{
		TeamStruct_VS _data;
		struct_data_pr_userinfo_team_vs_finish()
		{
			std::memset(this, 0, sizeof(struct_data_pr_userinfo_team_vs_finish));
		}
	};
}

#endif//_WJ_WJNetPtoR_userinfo_H_