#ifndef _WJ_WJNetRM_user_list_H_
#define _WJ_WJNetRM_user_list_H_

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"

namespace WJ
{
	//e_gs_mess_user_list=100,//�û��б�
	enum E_ROOM_ASSID_enum_gs_mess_user_list
	{
		e_gs_ass_user_list_update_userinfo = 1,//���������Ϣ
		e_gs_ass_user_list_get_desk_userinfo = 2,//��ȡ��������������ҵ�����
	};

	////���������Ϣ
	//struct RoomMessDate_update_userinfo
	//{
	//	UINT udeskid;//����id
	//	UINT udeskstation;//��λ
	//	bool bOwerUserid;//�Ƿ�Ϊ����
	//	ENUM_UserStation bUserState;						//�û�״̬
	//	RoomMessDate_update_userinfo()
	//	{
	//		std::memset(this, 0, sizeof(RoomMessDate_update_userinfo));
	//	}
	//};

	enum E_HALL_HandleCodeID_enum_gs_mess_get_desk_userinfo
	{
		e_gs_hcid_userlist_part = 1,//һ���֣���û�з���
		e_gs_hcid_userlist_finish = 2,//�������б�����
	};

	//��ȡ��������������ҵ�������ÿһ�������Ϣ�ṹ��
	struct WJ::UserInfoInRoomStruct;
}


#endif//_WJ_WJNetRM_user_list_H_