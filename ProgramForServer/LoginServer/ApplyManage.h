#ifndef WJ_ApplyManage_H
#define WJ_ApplyManage_H

#include <map>
#include <mutex>
#include "common/WJBaseType.h"
namespace WJ
{
	//�����ڵ�����
	struct struct_applynode
	{
		UINT userid;//�û�id
		UINT ucontestid;//����id
		bool bAllot;//�Ƿ񱻷��䵽����ȥ��
		UINT uroomid;//������ķ���id
		struct_applynode()
		{
			std::memset(this, 0, sizeof(struct_applynode));
		}
	};

	class CApplayManage
	{
	public:
		CApplayManage();
		~CApplayManage();

		//������м�¼
		void clear();
		//���
		bool add(struct_applynode &data);
		//ɾ��
		bool del(UINT userid);
		//�Ƿ����
		bool bExist(UINT userid);
		//������ұ����䵽����
		void AllotRoom(UINT userid, UINT uroomid);
		//�������δ�����䵽����
		void unAllotRoom(UINT userid);
		//��øñ����ж����˱�����û�д���
		UINT getusercount(UINT ucontestid);
		//��ø������ж����˱�����û�д���
		UINT getusercount_unplaying();
		//��ø������ж������Ѿ��ڱ�����
		UINT getusercount_playing();
		//�����ұ�������
		bool at(UINT userid, struct_applynode& data);
	private:
		std::map<long long, struct_applynode> m_datalist;
	};
}

#endif//WJ_ApplyManage_H

