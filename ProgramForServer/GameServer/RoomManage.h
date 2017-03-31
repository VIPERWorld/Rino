#ifndef WJ_RoomManage_H
#define WJ_RoomManage_H

#include <list>
#include "common/WJBaseType.h"
#include "server/comstruct.h"
#include "GameRoom.h"
#include "Redis/WJRedis.h"

namespace WJ
{
	class CRoomManage
	{
	private:
		std::unordered_map<UINT, CGameRoom*> m_roomid2room;
	public:
		CRoomManage();
		~CRoomManage();
		bool LoadRoomInfoFromDb();
		void UpdateRoomInfoToLoginServer();
		//ͨ������id��ȡ����ʵ��
		CGameRoom * GetRoom(UINT uRoomId);
		//ͨ������id����ȡ����ʵ�������Ҹ�ʵ�������п��ÿ�����
		CGameRoom * GetRoom_ForDeskid(UINT uContestid, ISysDeskInterface**pDesk);
	};
}

#endif//WJ_RoomManage_H