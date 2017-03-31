#ifndef _WJ_WJNetRM_game_frame_H_
#define _WJ_WJNetRM_game_frame_H_

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"
namespace WJ
{
	//e_gs_mess_game_frame = 104,//��Ϸ�����Ϣ
	enum E_ROOM_ASSID_enum_gs_mess_game_frame
	{
		e_gs_ass_game_frame_info = 1,//������Ϸ����������Ϣ
		e_gs_ass_game_frame_station = 2,//������Ϸ״̬��Ϣ
		e_gs_ass_game_frame_agree = 3,//��Ҿ���ͬ����Ϣ
		e_gs_ass_game_frame_begin = 4,//��Ϸ��ʽ��ʼ
		e_gs_ass_game_frame_end_c = 5,//��Ҹ��߷�������Ϸ����,���ҷ�������ת��
		e_gs_ass_game_frame_end_s = 6,//��Ϸ��ʽ����
		e_gs_ass_game_frame_user_leave = 7,//����ڷ���Ϸ�������뿪����
	};

	///��Ϸ��Ϣ
	struct RoomMessDate_deskinfo
	{
		UINT								bGameStation;						///��Ϸ״̬
		UINT								bReserve;							///�����ֶ�

		RoomMessDate_deskinfo()
		{
			std::memset(this, 0, sizeof(RoomMessDate_deskinfo));
		}
	};

	//��Ҿ���׼��
	struct RoomMessDate_user_frame_agree
	{
		UINT udeskid;//����id
		UINT udeskstation;//��λ
		UserInfoInRoomStruct userdata;//�������
		RoomMessDate_user_frame_agree()
		{
			std::memset(this, 0, sizeof(RoomMessDate_user_frame_agree));
		}
	};

	//��������������Ľ��
	enum E_hcid_enum_game_frame_third_party_room_create
	{
		e_gs_hcid_game_frame_third_party_room_create_suc = 1,//�����ɹ�
		e_gs_hcid_game_frame_third_party_room_create_fail = 2,//����ʧ��
	};
}

#endif//_WJ_WJNetRM_game_frame_H_