#ifndef _WJ_NetPM_apply_H_
#define _WJ_NetPM_apply_H_

#include "common/WJBaseType.h"
#include "server/comstruct.h"
namespace WJ
{
	//e_hs_mess_apply = 102,//����
	enum E_HALL_ASSID_enum_hs_mess_apply
	{
		e_hs_ass_apply = 1,//������
		e_hs_ass_apply_login_recome_apply = 2,//��Ҷ��ߺ�������ֻ�δ���뷿�䣬���͸���Ϣ���ͻ���
		e_hs_ass_apply_login_recome_playing = 3,//��Ҷ��ߺ���������Ѿ����뷿�䣬���͸���Ϣ���ͻ���
		e_hs_ass_apply_restitution = 4,//�˷�
		e_hs_ass_apply_unplay = 5,//���û���ڷ��������棬����������Ч
		e_hs_ass_unapply = 6,//��Ҳ�û�б����μ��κα���
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_apply
	{
		e_hs_hcid_apply_suc = 1,//�����ɹ�
		e_hs_hcid_apply_err_contest_inexistence = 2,//����ʧ��_����������
		e_hs_hcid_apply_err_room_inexistence = 3,//����ʧ��_���䲻����
		e_hs_hcid_apply_err_nomoney = 4,//����ʧ��_�����Ѳ���
		e_hs_hcid_apply_err_applyed = 5,//����ʧ��_�ѱ�������δ����
		e_hs_hcid_apply_err_try_again = 6,//����ʧ��_��һ�ָս�������һ���ٱ���
		e_hs_hcid_apply_err_unknown = 9,//����ʧ��_δ֪����
	};

	//�ͻ��˱�������ṹ��
	struct struct_data_hs_mess_apply
	{
		UINT ucontestid;//����id

		struct_data_hs_mess_apply()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_apply));
		}
	};

	//����ʧ��_��һ�ָս�������һ���ٱ���
	struct struct_data_hs_mess_apply_try_again
	{
		LLONG lltimeout;//ʣ��ȴ�ʱ��(��)

		struct_data_hs_mess_apply_try_again()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_apply_try_again));
		}
	};
	
	//��½���������������ǰ����ı��������з���(��δ��ʼ��Ϸ)
	enum E_HALL_HandleCodeID_enum_hs_mess_login_recome_apply
	{
		e_hs_hcid_login_recome_apply_roomlist_part = 1,//һ���֣���û�з���
		e_hs_hcid_login_recome_apply_roomlist_finish = 2,//�������б�����
	};
	struct  ComRoomInfo;//����ڵ�
	//��½���������������ǰ����ı���id(�Ѿ���ʼ��Ϸ)
	struct struct_data_hs_mess_apply_login_recome_playing
	{
		ENUM_ContestKind uContestKind;//������������
		UINT gameid;//��Ϸid
		UINT ereaid;//��Ϸ����id
		UINT ucontestid;//����id
		UINT roomid;//����id
		char strIP[30];//����ip��ַ
		UINT uport;//����˿ں�
		struct_data_hs_mess_apply_login_recome_playing()
		{
			std::memset(this, 0, sizeof(struct_data_hs_mess_apply_login_recome_playing));
		}
	};

	enum E_HALL_HandleCodeID_enum_hs_mess_apply_restitution
	{
		e_hs_hcid_apply_restitution_suc = 1,//�˷ѳɹ�
		e_hs_hcid_apply_restitution_err = 2,//�˷�ʧ��
	};

	//�˷�ʧ�ܣ��϶�����Ϊ�Ѿ���Ϸ��ʼ��
	typedef struct_data_hs_mess_apply_login_recome_playing struct_data_hs_mess_apply_restitution_recome;

	//֪ͨ�ͻ��������Ϊָ������Ѱ�ҷ���
	typedef struct_data_hs_mess_apply struct_data_hs_mess_apply_unplay;
}

#endif//_WJ_NetPM_apply_H_