#ifndef _WJ_NetPM_team_H_
#define _WJ_NetPM_team_H_

#include "common/WJBaseType.h"
namespace WJ
{
	//e_hs_mess_team = 103,//�Ŷ�
	enum E_HALL_ASSID_enum_hs_mess_team
	{
		e_hs_ass_team_create = 1,//�����Ŷ�
		e_hs_ass_team_join = 2,//�����Ŷ�
		e_hs_ass_team_leave = 3,//�˳��Ŷ�
		e_hs_ass_team_dissolve = 4,//��ɢ�Ŷ�
		e_hs_ass_team_start = 5,//�ӳ�����ʼ����
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_team_create
	{
		e_hs_hcid_team_create_suc = 1,//�����Ŷӳɹ�
		e_hs_hcid_team_create_err_contest_inexistence = 2,//�����Ŷ�ʧ��_����������
		e_hs_hcid_team_create_err_room_inexistence = 3,//�����Ŷ�ʧ��_���䲻����
		e_hs_hcid_team_create_err_nomoney = 4,//�����Ŷ�ʧ��_�����Ѳ���
		e_hs_hcid_team_create_err_exist = 5,//�����Ŷ�ʧ��_�����Ŷ��У���δ��ɢ
		e_hs_hcid_team_create_err_applyed = 6,//�����Ŷ�ʧ��_�ѱ�����������δ����
		e_hs_hcid_team_create_err_contest_rule = 7,//�����Ŷ�ʧ��_����ģʽ�����Ŷ���
		e_hs_hcid_team_create_err_PayType = 8,//�����Ŷ�ʧ��_֧������δ֪
		e_hs_hcid_team_create_err_unknown = 9,//�����Ŷ�ʧ��_δ֪����
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_team_join
	{
		e_hs_hcid_team_join_suc = 1,//�����Ŷӳɹ�
		e_hs_hcid_team_join_err_contest_inexistence = 2,//�����Ŷ�ʧ��_����������
		e_hs_hcid_team_join_err_nomoney = 3,//�����Ŷ�ʧ��_�����Ѳ���
		e_hs_hcid_team_join_err_exist = 4,//�����Ŷ�ʧ��_�����Ŷ��У���δ��ɢ
		e_hs_hcid_team_join_err_applyed = 5,//�����Ŷ�ʧ��_�ѱ�����������δ����
		e_hs_hcid_team_join_err_contest_rule = 6,//�����Ŷ�ʧ��_����ģʽ�����Ŷ���
		e_hs_hcid_team_join_err_password = 7,//�����Ŷ�ʧ��_�ŶӰ��Ŵ���
		e_hs_hcid_team_join_err_full = 8,//�����Ŷ�ʧ��_��������
		e_hs_hcid_team_join_err_unknown = 9,//�����Ŷ�ʧ��_δ֪����
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_team_leave
	{
		e_hs_hcid_team_leave_suc = 1,//�˳��Ŷӳɹ�
		e_hs_hcid_team_leave_err_contest_inexistence = 2,//�˳��Ŷ�ʧ��_����������
		e_hs_hcid_team_leave_err_created = 3,//�˳��Ŷ�ʧ��_�Լ��������Լ��������Ŷ�
		e_hs_hcid_team_leave_err_playing = 4,//�˳��Ŷ�ʧ��_��Ϸ������
		e_hs_hcid_team_leave_err_contest_rule = 5,//�˳��Ŷ�ʧ��_����ģʽ�����Ŷ���
		e_hs_hcid_team_leave_err_applyed = 6,//�˳��Ŷ�ʧ��_�ѱ�����������δ����
		e_hs_hcid_team_leave_err_password = 7,//�˳��Ŷ�ʧ��_�ŶӰ��Ŵ���
		e_hs_hcid_team_leave_err_money = 8,//�˳��Ŷ�ʧ��_�˲�����ʧ��
		e_hs_hcid_team_leave_err_unknown = 9,//�˳��Ŷ�ʧ��_δ֪����
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_team_dissolve
	{
		e_hs_hcid_team_dissolve_suc = 1,//��ɢ�Ŷӳɹ�
		e_hs_hcid_team_dissolve_err_contest_inexistence = 2,//��ɢ�Ŷ�ʧ��_����������
		e_hs_hcid_team_dissolve_err_contest_rule = 3,//��ɢ�Ŷ�ʧ��_����ģʽ�����Ŷ���
		e_hs_hcid_team_dissolve_err_uncreated = 4,//��ɢ�Ŷ�ʧ��_�Լ���û�д����Ŷ�
		e_hs_hcid_team_dissolve_err_playing = 5,//��ɢ�Ŷ�ʧ��_��Ϸ������
		e_hs_hcid_team_dissolve_err_applyed = 6,//��ɢ�Ŷ�ʧ��_�ѱ�����������δ����
		e_hs_hcid_team_dissolve_err_password = 7,//��ɢ�Ŷ�ʧ��_�ŶӰ��Ŵ���
		e_hs_hcid_team_dissolve_err_money = 8,//��ɢ�Ŷ�ʧ��_�˲�����ʧ��
		e_hs_hcid_team_dissolve_err_unknown = 9,//��ɢ�Ŷ�ʧ��_δ֪����
	};

	//�ͻ��˴����Ŷ�����ṹ��
	struct struct_data_hs_mess_team_create
	{
		UINT ucontestid;//����id
		ENUM_TeamPayType m_TeamPayType;//֧����ʽ

		struct_data_hs_mess_team_create()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_team_create));
		}
	};

	//�����Ŷӳɹ��󷵻�����
	typedef TeamStruct struct_data_hs_mess_team_create_res;
	
	//�ͻ��˼����Ŷ�����ṹ��
	struct struct_data_hs_mess_team_join
	{
		UINT ucontestid;//����id
		char m_Team_password[TEAM_PASS_LENGTH];//�ŶӰ���
		struct_data_hs_mess_team_join()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_team_join));
		}
	};

	//�����Ŷӳɹ��󷵻�����
	typedef TeamStruct struct_data_hs_mess_team_join_res;

	//�˳��Ŷ�����
	typedef struct_data_hs_mess_team_join struct_data_hs_mess_team_leave;

	//�˳��Ŷӳɹ��󷵻����ݸ��Ŷ����������
	struct struct_data_hs_mess_team_leave_res
	{
		UINT userid;//�˳����˵�userid

		struct_data_hs_mess_team_leave_res()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_team_leave_res));
		}
	};

	//��ɢ�Ŷ�����
	typedef struct_data_hs_mess_team_join struct_data_hs_mess_team_dissolve;

	//�ӳ�����ʼƥ�����
	typedef struct_data_hs_mess_team_join struct_data_hs_mess_team_start;

	enum E_HALL_HandleCodeID_enum_hs_mess_team_start
	{
		e_hs_hcid_team_start_suc = 1,//����ƥ��ɹ�
		//e_hs_hcid_team_start_err_no_free_room = 2,//����ƥ��ʧ��_��û�п��з���
	};

	//����ƥ��ɹ�, �ظ����ͻ���
	struct struct_data_hs_hcid_team_start_suc
	{
		char strIP[30];//����ip��ַ
		UINT uport;//����˿ں�
		TeamStruct_VS data;

		struct_data_hs_hcid_team_start_suc()
		{
			std::memset(this, 0, sizeof(struct_data_hs_hcid_team_start_suc));
		}
	};

}

#endif//_WJ_NetPM_team_H_