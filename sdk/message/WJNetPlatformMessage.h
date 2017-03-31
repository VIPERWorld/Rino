#ifndef _WJ_NetPlatformMessage_H_
#define _WJ_NetPlatformMessage_H_

#include "common/WJBaseType.h"
#include "message/WJNetPM_apply.h"
#include "message/WJNetPM_gamelist.h"
#include "message/WJNetPM_login.h"
#include "message/WJNetPM_team.h"

namespace WJ
{
	//////////////////////////////////////////////////////////////////////////
	//����ͨѶָ��궨��

	enum E_HALL_MAINID
	{
		e_hs_mess_login = 100,//��½
		e_hs_mess_gamelist = 101,//��Ϸ�б�
		e_hs_mess_apply = 102,//����
		e_hs_mess_team = 103,//�Ŷ�
	};
}

#endif // _WJ_NetPlatformMessage_H_