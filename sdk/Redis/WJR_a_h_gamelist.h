#ifndef _WJ_R_a_h_gamelist_H_
#define _WJ_R_a_h_gamelist_H_

#include "common/WJBaseType.h"
#include "server/WJServerManagelib.h"
#include "server/DataLine.h"
namespace WJ
{
#define Redis_Order_h_messid_gamelist 1//��ȡ��Ϸ�б������

	//������Ϸ�б��redis�ṹ��
	struct RRLine_c_gamelist : public RedisDataBaseLine
	{
		RRLine_c_gamelist()
		{
			std::memset(this, 0, sizeof(RRLine_c_gamelist));
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};

	//������Ϸ�б�������
	struct RRLine_s_gamelist : public RedisResultLine
	{
		enum E_RES
		{
			e_gamelist_game_node,//��Ϸ�ڵ�
			e_gamelist_erea_node,//�����ڵ�
			e_gamelist_contest_node,//�����ڵ�
			e_gamelist_room_node,//����ڵ�
		};
		RRLine_s_gamelist()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist));
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
	//������Ϸ�б�������(��Ϸ�ڵ�)
	struct RRLine_s_gamelist_Gameinfo : public RRLine_s_gamelist
	{
		ComGameinfo _data;
		RRLine_s_gamelist_Gameinfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist_Gameinfo));
			uHandleRusult = e_gamelist_game_node;
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
	//������Ϸ�б�������(�����ڵ�)
	struct RRLine_s_gamelist_Ereainfo : public RRLine_s_gamelist
	{
		ComEreainfo _data;
		RRLine_s_gamelist_Ereainfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist_Ereainfo));
			uHandleRusult = e_gamelist_erea_node;
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
	//������Ϸ�б�������(�����ڵ�)
	struct RRLine_s_gamelist_ContestInfo : public RRLine_s_gamelist
	{
		ComContestInfo _data;
		RRLine_s_gamelist_ContestInfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist_ContestInfo));
			uHandleRusult = e_gamelist_contest_node;
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
	//������Ϸ�б�������(����ڵ�)
	struct RRLine_s_gamelist_RoomInfo : public RRLine_s_gamelist
	{
		ComRoomInfo _data;
		RRLine_s_gamelist_RoomInfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_gamelist_RoomInfo));
			uHandleRusult = e_gamelist_room_node;
			uRediscommandid = Redis_Order_h_messid_gamelist;
		}
	};
}

#endif//_WJ_R_a_h_gamelist_H_