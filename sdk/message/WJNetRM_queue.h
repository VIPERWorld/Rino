#ifndef _WJ_WJNetRM_queue_H_
#define _WJ_WJNetRM_queue_H_

#include "common/WJBaseType.h"

namespace WJ
{
	//enum_gs_mess_queue = 103,//�Ŷ���Ϣ
	enum E_ROOM_ASSID_enum_gs_mess_queue
	{
		e_gs_ass_queue_allot = 1,//�������ȥ����
	};

	//����������λ����ң����������ø����Ϊ����
	struct RoomMessDate_queue_allot
	{
		UINT uuserid;//�û�id
		UINT udeskid;//����id
		UINT udeskstation;//��λ
		bool bOwerUserid;//�Ƿ�Ϊ����

		RoomMessDate_queue_allot()
		{
			std::memset(this, 0, sizeof(RoomMessDate_queue_allot));
		}
	};
}

#endif//_WJ_WJNetRM_queue_H_