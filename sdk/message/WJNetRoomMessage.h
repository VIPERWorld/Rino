#ifndef _WJ_NetRoomMessage_H_
#define _WJ_NetRoomMessage_H_

#include "message/WJNetRM_game_frame.h"
#include "message/WJNetRM_queue.h"
#include "message/WJNetRM_room.h"
#include "message/WJNetRM_user_action.h"
#include "message/WJNetRM_user_list.h"

namespace WJ
{
	//////////////////////////////////////////////////////////////////////////
	//�������ϷͨѶָ��궨��

	enum E_ROOM_MAINID
	{
		e_gs_mess_user_list = 100,//�û��б�
		e_gs_mess_user_action = 101,//�û�����
		e_gs_mess_room = 102,//������Ϣ
		e_gs_mess_queue = 103,//�Ŷ���Ϣ
		e_gs_mess_game_frame = 104,//��Ϸ�����Ϣ
		e_gs_mess_game_notify = 105,//��Ϸ��ͨ��Ϣ
	};

	//��Ϸ����ͨ������
#define GAME_FINISH_TYPE_NORMAL 1	//��������
#define GAME_FINISH_TYPE_FLEE 2		//���ܽ���
#define GAME_FINISH_TYPE_FORCE 3	//ϵͳǿ�ƽ���
}

#endif // _WJ_NetRoomMessage_H_