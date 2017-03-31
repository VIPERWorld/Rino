#ifndef _WJ_R_a_h_load_hall_H_
#define _WJ_R_a_h_load_hall_H_

#include "common/WJBaseType.h"


namespace WJ
{
#define Redis_Order_h_messid_load_hall 2//�û���¼����
#define Redis_Order_h_messid_update_userinfo 9//�����û�����
	//�����¼������redis�ṹ��
	struct RRLine_c_load_hall : public RedisDataBaseLine
	{
		struct_data_hs_mess_login data;
		struct_data_hs_mess_login_WX data2;
		bool bWX;//�Ƿ�Ϊ΢�ŵ�½
		RRLine_c_load_hall()
		{
			std::memset(this, 0, sizeof(RRLine_c_load_hall));
			//uRediscommandid = Redis_Order_h_messid_load_hall;
		}
	};


	//�����¼�����Ľ������
	struct RRLine_s_load_hall : public RedisResultLine
	{
		enum E_RES
		{
			e_login_err_unexist,//��Ҳ�����
			e_login_err_password,//�������
			e_login_err_online,//����Ѿ�����
			e_login_suc,//��½�ɹ�
		};

		struct_data_hs_hcid_login_suc _data;
		RRLine_s_load_hall()
		{
			std::memset(this, 0, sizeof(RRLine_s_load_hall));
			//uRediscommandid = Redis_Order_h_messid_load_hall;
		}
	};

	struct CTransaction_load_hall
	{
		RRLine_c_load_hall m_in;
		RRLine_s_load_hall m_out;
	};

	//ˢ���û����ݵĽ������
	struct RRLine_s_Update_Userinfo : public RedisResultLine
	{
		UINT				iuserid;
		UINT				itype;							//1���֣�2���
		UINT				dwPoint;						//����
		LLONG				i64Money;						//���
		RRLine_s_Update_Userinfo()
		{
			std::memset(this, 0, sizeof(RRLine_s_Update_Userinfo));
			uRediscommandid = Redis_Order_h_messid_update_userinfo;
		}
	};

	struct CTransaction_Update_Userinfo
	{
		RedisDataBaseLine m_in;
		std::vector<RRLine_s_Update_Userinfo> m_out_vector;
	};
}

#endif//_WJ_R_a_h_load_hall_H_