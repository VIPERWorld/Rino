#ifndef _WJ_WJNetPtoR_roominfo_H_
#define _WJ_WJNetPtoR_roominfo_H_

#include "common/WJBaseType.h"
#include "WJNetPM_gamelist.h"
namespace WJ
{
	//e_pr_mess_roominfo = 500,//������Ϣ
	enum E_PtoR_ASSID_enum_roominfo
	{
		e_pr_ass_roominfo_update = 1,//���·�����Ϣ
	};

	//������Ϣ�ṹ��
	typedef ComRoomInfo struct_data_pr_roominfo_update;
}

#endif//_WJ_WJNetPtoR_roominfo_H_