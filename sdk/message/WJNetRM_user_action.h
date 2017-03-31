#ifndef _WJ_WJNetRM_user_action_H_
#define _WJ_WJNetRM_user_action_H_

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"

namespace WJ
{
	//e_gs_mess_user_action = 101,//�û�����
	enum E_ROOM_ASSID_enum_gs_mess_user_action
	{
		e_gs_ass_user_action_login = 1,//��½��Ϸ������
		e_gs_ass_user_action_login_cancel = 2,//ȡ����¼
		e_gs_ass_user_action_team_out = 3,//�Ŷ�����ʱ������뿪��
		e_gs_ass_user_action_team_come = 4,//�Ŷ�����ʱ����һ�����
	};

	//�ͻ��˷����½��������
	struct RoomMessDate_user_action_login
	{
		UINT uuserid;//�û�id
		UINT contestid;

		RoomMessDate_user_action_login()
		{
			std::memset(this, 0, sizeof(RoomMessDate_user_action_login));
		}
	};

	//����˻ظ���½���
	enum E_ROOM_HandleCodeID_enum_hs_mess_login
	{
		e_gs_hcid_login_suc_ = 1,//��½�ɹ�
		e_gs_hcid_login_suc_recome = 2,//��½�ɹ�_�����ػ�
		e_gs_hcid_login_err_ = 3,//��½ʧ��
	};

	//��½�ɹ������������ػأ��������½ṹ��
	typedef UserInfoInRoomStruct struct_data_gs_mess_login_suc;

	//�ͻ���ȡ����¼����
	struct RoomMessDate_user_action_login_cancel
	{
		UINT uuserid;//�û�id
		RoomMessDate_user_action_login_cancel()
		{
			std::memset(this, 0, sizeof(RoomMessDate_user_action_login_cancel));
		}
	};

	//����˻ظ�ȡ����½���
	enum E_ROOM_HandleCodeID_enum_hs_mess_login_cancel
	{
		e_gs_hcid_login_cancel_suc_ = 1,//ȡ����½�ɹ�
		e_gs_hcid_login_cancel_err_ = 2,//ȡ����½ʧ��
	};
}

#endif//_WJ_WJNetRM_user_action_H_