#ifndef _WJ_R_a_h_apply_H_
#define _WJ_R_a_h_apply_H_

#include "common/WJBaseType.h"
namespace WJ
{
#define Redis_Order_h_messid_apply 3//��ұ���
#define Redis_Order_h_messid_apply_restitution 4//����˷�


	//��������redis�ṹ��
	struct RRLine_c_apply : public RedisDataBaseLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		RRLine_c_apply()
		{
			std::memset(this, 0, sizeof(RRLine_c_apply));
			uRediscommandid = Redis_Order_h_messid_apply;
		}
	};

	//�������Ľ������
	struct RRLine_s_apply : public RedisResultLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		bool bSuc;//�Ƿ�ɹ�
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		RRLine_s_apply()
		{
			std::memset(this, 0, sizeof(RRLine_s_apply));
			uRediscommandid = Redis_Order_h_messid_apply;
		}
	};

	//�����������
	struct CTransaction_apply
	{
		RRLine_c_apply m_in;
		RRLine_s_apply m_out;
	};

//Redis_Order_h_messid_apply_restitution 4//����˷�
	//�����˷ѵ�redis�ṹ��
	struct RRLine_c_apply_restitution : public RedisDataBaseLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		RRLine_c_apply_restitution()
		{
			std::memset(this, 0, sizeof(RRLine_c_apply_restitution));
			uRediscommandid = Redis_Order_h_messid_apply_restitution;
		}
	};

	//�����˷ѵĽ������
	struct RRLine_s_apply_restitution : public RedisResultLine
	{
		LLONG changemoney;//���۵����
		UINT changepoint;//���۵Ļ���
		bool bSuc;//�Ƿ�ɹ�
		ENUM_ContestKind uContestKind;//������������
		UINT userid;//�û�id
		UINT ucontestid;//����id
		RRLine_s_apply_restitution()
		{
			std::memset(this, 0, sizeof(RRLine_s_apply_restitution));
			uRediscommandid = Redis_Order_h_messid_apply_restitution;
		}
	};

	//�����˷��������
	struct CTransaction_apply_restitution
	{
		RRLine_c_apply_restitution m_in;
		RRLine_s_apply_restitution m_out;
	};
}

#endif//_WJ_R_a_h_apply_H_