#ifndef WJ_RedisBaseManage_H
#define WJ_RedisBaseManage_H

#include "Redis/WJRedis.h"
#include "Redis/WJR_agreement_hall.h"

namespace WJ
{
	class InterfaceForWJRedisCallBack
	{//ʹ��CWJRedis��ʱ�Ļص��������ڴ˴�
	public:
		///redis�������
		virtual bool OnRedisResultEvent(RedisResultLine* pResultData, UINT uResultSize) = 0;
	};

	class WJSERVERMANAGELIB_API CRedisBaseManage
	{
	private:
		const ManageInfoStruct* m_pInitData;//����������
		InterfaceForWJRedisCallBack* m_pCallBack;//�ϲ�ص��ӿ�
	public:
		explicit CRedisBaseManage(const ManageInfoStruct *pInitData, InterfaceForWJRedisCallBack* pCallBack);
		~CRedisBaseManage();

		const ManageInfoStruct* getInitData(){ return m_pInitData; }
		InterfaceForWJRedisCallBack* getCallBack(){ return m_pCallBack; }

		///Redis�����ӿ�
		virtual void RedisDataBase(CWJRedis* pCWJRedis, RedisDataBaseLine * pSourceData) = 0;

		///���봦�����
		bool PushLine(void* pData, UINT uSize, UINT uIndex);
		///redis�������
		bool OnRedisResultEvent(RedisResultLine* pResultData, UINT uResultSize);
	};
}

#endif//WJ_RedisBaseManage_H
