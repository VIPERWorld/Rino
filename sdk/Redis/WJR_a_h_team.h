#ifndef _WJ_R_a_h_team_H_
#define _WJ_R_a_h_team_H_

#include "common/WJBaseType.h"
namespace WJ
{
#define Redis_Order_h_messid_team_create 5//��Ҵ����Ŷ�
#define Redis_Order_h_messid_team_join 6//��Ҽ����Ŷ�
#define Redis_Order_h_messid_team_leave 7//����˳��Ŷ�
#define Redis_Order_h_messid_team_dissolve 8//��ҽ�ɢ�Ŷ�
	//���󴴽��Ŷӵ�redis�ṹ��
	struct RRLine_c_team_create : public RedisDataBaseLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		ENUM_TeamPayType m_TeamPayType;//֧����ʽ
		ENUM_RoomRule uRoomRule;//�������
		RRLine_c_team_create()
		{
			std::memset(this, 0, sizeof(RRLine_c_team_create));
			uRediscommandid = Redis_Order_h_messid_team_create;
		}
	};

	//���󴴽��ŶӵĽ������
	struct RRLine_s_team_create : public RedisResultLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		bool bSuc;//�Ƿ�ɹ�
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		ENUM_TeamPayType m_TeamPayType;//֧����ʽ
		ENUM_RoomRule uRoomRule;//�������
		RRLine_s_team_create()
		{
			std::memset(this, 0, sizeof(RRLine_s_team_create));
			uRediscommandid = Redis_Order_h_messid_team_create;
		}
	};

	struct CTransaction_team_create
	{
		RRLine_c_team_create m_in;
		RRLine_s_team_create m_out;
	};

	//��������Ŷӵ�redis�ṹ��
	struct RRLine_c_team_join : public RedisDataBaseLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		char m_Team_password[TEAM_PASS_LENGTH];//�ŶӰ���
		RRLine_c_team_join()
		{
			std::memset(this, 0, sizeof(RRLine_c_team_join));
			uRediscommandid = Redis_Order_h_messid_team_join;
		}
	};

	//��������ŶӵĽ������
	struct RRLine_s_team_join : public RedisResultLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		bool bSuc;//�Ƿ�ɹ�
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		char m_Team_password[TEAM_PASS_LENGTH];//�ŶӰ���
		RRLine_s_team_join()
		{
			std::memset(this, 0, sizeof(RRLine_s_team_join));
			uRediscommandid = Redis_Order_h_messid_team_join;
		}
	};

	struct CTransaction_team_join
	{
		RRLine_c_team_join m_in;
		RRLine_s_team_join m_out;
	};

	//�����˳��Ŷӵ�redis�ṹ��
	struct RRLine_c_team_leave : public RedisDataBaseLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		char m_Team_password[TEAM_PASS_LENGTH];//�ŶӰ���
		RRLine_c_team_leave()
		{
			std::memset(this, 0, sizeof(RRLine_c_team_leave));
			uRediscommandid = Redis_Order_h_messid_team_leave;
		}
	};

	//�����˳��ŶӵĽ������
	struct RRLine_s_team_leave : public RedisResultLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		bool bSuc;//�Ƿ�ɹ�
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		char m_Team_password[TEAM_PASS_LENGTH];//�ŶӰ���
		RRLine_s_team_leave()
		{
			std::memset(this, 0, sizeof(RRLine_s_team_leave));
			uRediscommandid = Redis_Order_h_messid_team_leave;
		}
	};

	struct CTransaction_team_leave
	{
		RRLine_c_team_leave m_in;
		RRLine_s_team_leave m_out;
	};

	//�����˳��Ŷӵ�redis�ṹ��
	struct RRLine_c_team_dissolve : public RedisDataBaseLine
	{
		LLONG changemoney[TEAM_USER_NUM];//���۵����
		UINT changepoint[TEAM_USER_NUM];//���۵Ļ���
		ENUM_ContestKind uContestKind;//������������
		UINT userid[TEAM_USER_NUM];//�û�id
		UINT ucontestid;//����id
		char m_Team_password[TEAM_PASS_LENGTH];//�ŶӰ���
		UINT m_uTemaPlayerNum;//��������
		RRLine_c_team_dissolve()
		{
			std::memset(this, 0, sizeof(RRLine_c_team_dissolve));
			uRediscommandid = Redis_Order_h_messid_team_dissolve;
		}
	};

	//�����˳��ŶӵĽ������
	struct RRLine_s_team_dissolve : public RedisResultLine
	{
		LLONG changemoney[TEAM_USER_NUM];//���۵����
		UINT changepoint[TEAM_USER_NUM];//���۵Ļ���
		bool bSuc;//�Ƿ�ɹ�
		ENUM_ContestKind uContestKind;//������������
		UINT userid[TEAM_USER_NUM];//�û�id
		UINT ucontestid;//����id
		char m_Team_password[TEAM_PASS_LENGTH];//�ŶӰ���
		UINT m_uTemaPlayerNum;//��������
		RRLine_s_team_dissolve()
		{
			std::memset(this, 0, sizeof(RRLine_s_team_dissolve));
			uRediscommandid = Redis_Order_h_messid_team_dissolve;
		}
	};

	struct CTransaction_team_dissolve
	{
		RRLine_c_team_dissolve m_in;
		RRLine_s_team_dissolve m_out;
	};
}

#endif//_WJ_R_a_h_team_H_