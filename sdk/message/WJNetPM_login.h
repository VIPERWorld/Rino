#ifndef _WJ_NetPM_login_H_
#define _WJ_NetPM_login_H_

#include "common/WJBaseType.h"
#include "../common/WJUserInfoBaseStruct.h"

namespace WJ
{
	//e_hs_mess_login = 100,//��½
	enum E_HALL_ASSID_enum_hs_mess_login
	{
		e_hs_ass_login = 1,//��ͨ��½
		e_hs_ass_login_c_finish = 2,//�ͻ��˸��߷�������ҵ�½���
		e_gs_ass_login_c_force_out = 3,//ǿ��
		e_hs_ass_login_WX = 4,//΢�ŵ�½
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_login
	{
		e_hs_hcid_login_suc = 1,//��½�ɹ�
		e_hs_hcid_login_err_unexist = 2,//��½ʧ��_�����ڸ��û�
		e_hs_hcid_login_err_password = 3,//��½ʧ��_�������
		e_hs_hcid_login_err_online = 4,//��½ʧ��_������
	};

	//�ͻ��˵�½����ṹ��
	struct struct_data_hs_mess_login
	{
		char username[NAME_LENGTH];//�û���
		char password[PASSWORD_LENGTH];//����

		struct_data_hs_mess_login()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_login));
		}
	};

	//�ͻ���΢�ŵ�½����ṹ��
	struct struct_data_hs_mess_login_WX
	{
		char username[NAME_LENGTH];//�û���
		UINT userid;//�û�id
		char wx_key[200];//΢�ŷ��ص�key
		struct_data_hs_mess_login_WX()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_login_WX));
		}
	};

	//��½�ɹ��ṹ��
	typedef UserInfoInHallStruct struct_data_hs_hcid_login_suc;

}

#endif//_WJ_NetPM_login_H_