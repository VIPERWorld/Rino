#ifndef _WJ_WJNetRM_room_H_
#define _WJ_WJNetRM_room_H_

#include "common/WJBaseType.h"

namespace WJ
{
	//e_gs_mess_room = 102,//������Ϣ
	enum E_ROOM_ASSID_enum_gs_mess_room
	{
		e_gs_ass_room_dissolve_desk = 1,//��ɢ����
		e_gs_ass_room_chat = 2,//����
		e_gs_ass_room_info = 3,//���͸��ͻ��˵ķ�������
	};

	enum E_hcid_enum_gs_room_chat
	{
		e_gs_hcid_gs_room_chat_for_room = 1,//�㲥��������
		e_gs_hcid_gs_room_chat_for_desk = 2,//�鲥������
		e_gs_hcid_gs_room_chat_for_player = 3,//����������
	};

	//�����
	struct RoomMessDate_room_chat
	{
		char from_username[NAME_LENGTH];//�������û���
		char to_username[NAME_LENGTH];//�������û���

		char mess[CHAT_LENDTH];//����
		UINT strleng;		//���ݳ���
		RoomMessDate_room_chat()
		{
			std::memset(this, 0, sizeof(RoomMessDate_room_chat));
		}
	};
}

#endif//_WJ_WJNetRM_room_H_