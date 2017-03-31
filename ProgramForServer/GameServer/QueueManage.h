#ifndef WJ_QueueManage_H
#define WJ_QueueManage_H

#include <list>
#include <map>
#include <mutex>
#include <unordered_map>
#include "common/WJBaseType.h"
#include "UserInfo.h"
#include "server/comstruct.h"

namespace WJ
{
	enum E_R_TeamStruct_VS_station
	{
		E_R_TeamStruct_VS_station_wait=0,//�ȴ�������״̬
		E_R_TeamStruct_VS_station_playing,//�Ѿ���ʼ���䷿������
		E_R_TeamStruct_VS_station_end,//�Ѿ�������
	};
	struct R_TeamStruct_VS//�������Ŷ�VS����
	{
		TeamStruct_VS _data;//����
		E_R_TeamStruct_VS_station _E_R_TeamStruct_VS_station;//״̬

		R_TeamStruct_VS()
		{
			_E_R_TeamStruct_VS_station = E_R_TeamStruct_VS_station_wait;
		}
	};

	//�ýṹ���¼ƥ����Ҽ�¼
	struct RememberVS_struct
	{
		UINT useridA;//solo�����
		UINT useridB;//��֮ƥ��������

		long int ltime;//
	};

	//����б�����Ŷ�
	class QueueInfo_t
	{
	public:
		QueueInfo_t()
		{
			m_dwUserID = 0;
			m_EnterTime = 0;
		}

		bool operator < (const QueueInfo_t& ti) const
		{//���������ʱ�䳤������
			return m_EnterTime < ti.m_EnterTime;
		}


		UINT m_dwUserID;//ID
		UINT m_uContestid;//����id
		__time64_t m_EnterTime;//������е�ʱ���
	};

	class CQueueManage
	{
	public:
		UINT m_ucontestid;//�ö�������ĳ����id��
		ENUM_ContestRule m_uContestRule;//����ģʽ
	private:
		//���ڶ��������Ӧ�ô��ڶ����Ӧ���û��ŶӶ���
		std::list<QueueInfo_t>	m_QueueUserList;	//�ȴ������������Ŷӵ���Ҷ���
        std::unordered_map<UINT, std::list<QueueInfo_t>::iterator >          m_userid2iter;     //�ȴ����������ID�������еĴ��λ�õ�ӳ��
		//ƥ�����
		std::map<UINT, RememberVS_struct> m_RememberVS_map;
	public:
		CQueueManage();
		~CQueueManage();

		void run();
		//��������Ϣ��������
		bool Add(CGameUserInfo* pGameUserInfo);
		//ɾ��������ָ�������Ϣ
		void Delete(CGameUserInfo* pGameUserInfo);
		//ɾ�����������������Ϣ
		void DeleteAll();

	private:
		//����soloģʽ���г���
		void run_RoomRule_solo();
		//���ģʽģʽ���г���
		void run_RoomRule_team();
	};
}

#endif//WJ_QueueManage_H