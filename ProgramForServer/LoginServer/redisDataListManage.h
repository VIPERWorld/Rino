#ifndef WJ_RedisDataListManage_H
#define WJ_RedisDataListManage_H

#include "Redis/WJRedis.h"

namespace WJ
{
	//redis ��������ע�⣺��Щ��̬����������dbworker �߳���
	//redis���� �û���¼����
	void onRedis_Order_h_messid_load_hall(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis���� ��ұ���
	void onRedis_Order_h_messid_apply(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis���� ����˷�
	void onRedis_Order_h_messid_apply_restitution(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis���� ��Ҵ����Ŷ�
	void onRedis_Order_h_messid_team_create(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis���� ��Ҽ����Ŷ�
	void onRedis_Order_h_messid_team_join(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis���� ����˳��Ŷ�
	void onRedis_Order_h_messid_team_leave(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis���� ��ҽ�ɢ�Ŷ�
	void onRedis_Order_h_messid_team_dissolve(CRedisWorkerThread *pDbWorker, void *pArg);
	//redis���� �����û�����(��ҡ�����)
	void onRedis_Order_h_messid_update_userinfo(CRedisWorkerThread *pDbWorker, void *pArg);
}

#endif//WJ_RedisDataListManage_H
