
#ifndef WJ_gamelistmanage_H
#define WJ_gamelistmanage_H
#include <vector>
#include <map>
#include <mutex>
#include "common/WJBaseType.h"
#include "message/WJNetPM_gamelist.h"
#include "Redis\WJRedis.h"

//��Ϸ�б��ģ���� ��Ϸ�ڵ������Ǳ����ڵ㣬�����ڵ������Ƿ���ڵ�

namespace WJ
{
	struct ComRoomInfo_node
	{
		ComRoomInfo room_node;//����
		UINT usocketindex;//�׽���id
		UINT uTeamCount;//�÷��䵱ǰ�Ŷ���

		ComRoomInfo_node()
		{
			usocketindex = ERROR_SOCKETID;
			uTeamCount = 0;
		}
	};
	class CGameList
	{
	
	public:
		CGameList();
		~CGameList();

		void InitLoadFromDB(CWJRedis *pRedis);
		//������м�¼
		void clear();
		//�����Ϸ�ڵ�
		void addgame_node(ComGameinfo &node);
		//��������ڵ�
		void adderea_node(ComEreainfo &node);
		//��ӱ����ڵ�
		void addcontest_node(ComContestInfo &node);
		//��ӷ���ڵ�
		void addroom_node(ComRoomInfo_node &node);
		//��ȡ��Ϸ�ڵ���Ŀ
		UINT Getgame_nodeCount();
		//��ȡ�����ڵ���Ŀ
		UINT Geterea_nodeCount();
		//��ȡ�����ڵ���Ŀ
		UINT Getcontest_nodeCount();
		//��ȡ����ڵ���Ŀ
		UINT Getroom_nodeCount();
		//��ȡ��Ϸ
		UINT FillGameName(char * pBuffer, UINT uBufferSize, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);	
		//��ȡ����
		UINT FillErea(char * pBuffer, UINT uBufferSize, UINT uGameID, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);
		//��ȡ����
		UINT FillGameContest(char * pBuffer, UINT uBufferSize, UINT uEreaID, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);
		//��ȡ����
		UINT FillGameRoom(char * pBuffer, UINT uBufferSize, UINT uContestID, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);

		//�ñ����Ƿ��з���
		bool ExistRoomForContest(UINT uContestid);
		//��ñ�������
		const ComContestInfo* GetComContestInfo(UINT uContestid);
		//��÷�������
		const ComRoomInfo* GetComRoomInfo(UINT uroomid);
		//��÷�������
		const ComRoomInfo* GetComRoomInfoEx(UINT uroomid);

		//��÷�����׽���id
		bool GetRoomSocketID(UINT roomid, UINT &usocketindex);
		//��÷����б�
		void GetComRoomList(std::vector<ComRoomInfo_node> &room_node, UINT uContestid);

		//�÷��䵱ǰ�����ж��ٱ�������
		UINT getNowTeamCountForRoom(UINT uroomid);
		//���ӷ������
		void AddTeamCount(UINT uroomid, UINT AddCount);
		//���ٷ������
		void ReduceTeamCount(UINT uroomid, UINT ReduceCount);
	private:
		std::map<long, ComGameinfo> m_game_node;				//��Ϸ�ڵ�
		std::map<long, ComEreainfo> m_erea_node;				//�����ڵ�
		std::map<long, ComContestInfo> m_contest_node;			//�����ڵ�
		std::map<long, ComRoomInfo_node> m_room_node;			//����ڵ�
	};
}

#endif//WJ_gamelistmanage_H
