#ifndef _WJ_NetPM_gamelist_H_
#define _WJ_NetPM_gamelist_H_

#include "common/WJBaseType.h"
#include "server/comstruct.h"
namespace WJ
{
	//e_hs_mess_gamelist = 101,//��Ϸ�б�
	enum E_HALL_ASSID_enum_hs_mess_gamelist
	{
		e_hs_ass_gamelist_game = 1,//��ȡ��Ϸ�ڵ�
		e_hs_ass_gamelist_erea = 2,//��ȡ�����ڵ�
		e_hs_ass_gamelist_contest = 3,//��ȡ�����ڵ�
		e_hs_ass_gamelist_room = 4,//��ȡ����ڵ�
		e_hs_ass_gamelist_contest_online = 5,//��ȡ�����ڵ�(���ڵ�����)
		e_hs_ass_gamelist_onlien_count = 6,//��������
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_gamelist
	{
		e_hs_hcid_gamelist_part = 1,//һ���֣���û�з���
		e_hs_hcid_gamelist_finish = 2,//�������б�����
	};

	//�ͻ��������ȡ�����ڵ�ṹ��
	struct struct_data_hs_ass_gamelist_erea
	{
		UINT gameid;//��Ϸid

		struct_data_hs_ass_gamelist_erea()
		{
			std::memset(this, 0, sizeof(struct_data_hs_ass_gamelist_erea));
		}
	};

	//�ͻ��������ȡ�����ڵ�ṹ��
	struct struct_data_hs_ass_gamelist_contest
	{
		UINT ereaid;//����id

		struct_data_hs_ass_gamelist_contest()
		{
			std::memset(this, 0, sizeof(struct_data_hs_ass_gamelist_contest));
		}
	};

	//�ͻ��������ȡ�����ڵ�ṹ��
	struct struct_data_hs_ass_gamelist_contest_online
	{
		UINT contestid;//����id

		struct_data_hs_ass_gamelist_contest_online()
		{
			std::memset(this, 0, sizeof(struct_data_hs_ass_gamelist_contest_online));
		}
	};

	//�ͻ��������ȡ����ڵ�ṹ��
	struct struct_data_hs_ass_gamelist_room
	{
		UINT contestid;//����id

		struct_data_hs_ass_gamelist_room()
		{
			std::memset(this, 0, sizeof(struct_data_hs_ass_gamelist_room));
		}
	};

	///��Ϸ��������
	struct ComGameinfo
	{
		UINT gameid;//��Ϸid
		char gamename[GAMENAME_LENGTH];//��Ϸ����

		ComGameinfo()
		{
			reset();
		}

		void reset()
		{
			std::memset(this, 0, sizeof(ComGameinfo));
		}
	};

	///��������
	struct ComEreainfo
	{
		UINT ereaid;//��Ϸ����id
		char ereaname[EREANAME_LENGTH];//��������
		UINT gameid;//��Ϸid
		ComEreainfo()
		{
			reset();
		}

		void reset()
		{
			std::memset(this, 0, sizeof(ComEreainfo));
		}
	};

	///������������
	struct ComContestInfo
	{
		UINT contestid;//����id
		char contestname[CONTESTNAME_LENGTH];//��������
		UINT gameid;//��Ϸid
		UINT ereaid;//��Ϸ����id
		char cContestDescribe[200];//��������
		ENUM_ContestRule uContestRule;//����ģʽ
		ENUM_ContestKind uContestKind;//������������
		LLONG _i64cost;//��������
		ENUM_AwardType awardType;//��������
		UINT award;//����
		ENUM_RoomRule uRoomRule;//�������
		//��������
		ComContestInfo()
		{
			reset();
		}

		void reset()
		{
			std::memset(this, 0, sizeof(ComContestInfo));
			uContestRule = US_ContestRule_solo;
			awardType = ENUM_AwardType_point;
			uRoomRule = US_RoomRule_1v1;

		}
	};

	///������������
	struct ComRoomInfo
	{
		UINT roomid;//����id
		char roomname[ROOMNAME_LENGTH];//��������
		UINT gameid;//��Ϸid
		UINT ereaid;//��Ϸ����id
		UINT contestid;//����id
		char strIP[30];//����ip��ַ
		UINT uport;//����˿ں�
		UINT uOlineCount;//��������
		ComRoomInfo()
		{
			reset();
		}
		void reset()
		{
			std::memset(this, 0, sizeof(ComRoomInfo));
		}
	};
}

#endif//_WJ_NetPM_gamelist_H_