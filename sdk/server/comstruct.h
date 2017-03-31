
#ifndef _S_COM_STRUCT_HEAD_FILE
#define _S_COM_STRUCT_HEAD_FILE
#include "common/WJBaseType.h"
namespace WJ
{
	enum ENUM_chip_change_type//��redisԼ������޸ĵ�ԭ��
	{
		e_chip_change_type_apply=3,//��������
		e_chip_change_type_apply_restitution=8,//�����˷�
		e_chip_change_type_contest_finish_win = 7,//������ʤ
	};

	enum ENUM_coin_change_type//��redisԼ������޸ĵ�ԭ��
	{
		e_coin_change_type_apply=2,//��������
		e_coin_change_type_apply_restitution=10,//�����˷�
		e_coin_change_type_contest_finish_win=9,//������ʤ
	};

	enum ENUM_ContestKind//�����������ͣ����⣩
	{
		US_ContestKind_free = 1,//��ѳ�
		US_ContestRule_point,//���ֳ�
		US_ContestRule_happy, //��ҳ�
	};

	static char g_char_ENUM_ContestKind[3][60] = {"��ѳ�", "���ֳ�", "��ҳ�"};

	enum ENUM_ContestRule//����ģʽ�����⣩
	{
		US_ContestRule_mission = 0,//������
		US_ContestRule_solo = 1,//������ս��
		US_ContestRule_team,//�Ŷ���ս��
	};

	enum ENUM_AwardType//�������ͣ����⣩
	{
		ENUM_AwardType_point = 1,//��������
		ENUM_AwardType_happy,//�������
	};
	
	static char g_char_ENUM_ContestRule[2][60] = { "������ս��", "�Ŷ���ս��" };

	enum ENUM_RoomRule//������򣨻��⣩
	{
		US_RoomRule_alone = 0,//����
		US_RoomRule_1v1,//1v1
		US_RoomRule_2v2,//2v2
		US_RoomRule_3v3,//3v3
		US_RoomRule_4v4,//4v4
		US_RoomRule_5v5,//5v5
		US_RoomRule_6v6,//6v6
		US_RoomRule_7v7,//7v7
		US_RoomRule_8v8,//8v8
		US_RoomRule_9v9,//9v9
		US_RoomRule_10v10,//10v10
	};

	enum ENUM_TeamPayType//�ŶӸ�������
	{
		TeamPayType_Captain = 0,//�ӳ�֧��
		TeamPayType_AA,//AA��
	};

	//�Ŷӽڵ�
	struct TeamStruct
	{
		char m_Team_password[TEAM_PASS_LENGTH];//�ŶӰ���
		UINT m_uCaptain_userid;//�ӳ�
		UINT m_uTemaPlayerNum;//��������
		UINT m_Userid[TEAM_USER_NUM];//ÿ���˵�userid
		ENUM_TeamPayType m_TeamPayType;//֧����ʽ
		UINT m_uContest;//����id
		bool bAllot;//�Ƿ񱻷��䵽����ȥ��
		UINT uroomid;//������ķ���id
		bool bVS;//�Ƿ�ƥ��
		bool bButVS;//�ӳ�������ƥ�䰴ť
		TeamStruct()
		{
			std::memset(this, 0, sizeof(TeamStruct));
		}
	};

	//�Ŷ�VS��Ϣ
	struct TeamStruct_VS
	{
		UINT teamid; //����id
		UINT uroomid;//������ķ���id
		TeamStruct _TeamStruct[2];

		TeamStruct_VS()
		{
			teamid = 0;
			uroomid = 0;
		}
	};
	//���������������������ݻ���
	struct ManageInfoStruct
	{
	public:
		bool bRun_SocketLib;//�Ƿ������׽������
		bool bRun_DataSQLLib;//�Ƿ��������ݿ����
		bool bRun_Redis;//Redis�������

		bool bAutoPort;//�Ƿ���ϵͳ�Զ�����˿ں�
		UINT uMaxCount;//���������
		UINT uListenPort;//�˿ں�

		char uRedis_ip[20];//redis��������ip
		UINT uRedis_port;//redis�������˿ں�
		char uRedis_Pass[20];//��½redis����
	public:
		ManageInfoStruct()
		{
			std::memset(this, 0, sizeof(ManageInfoStruct));
		}
	};

	//�������������ݽṹ��
	struct Room_ManageInfoStruct
	{
		UINT uRoomID;//����id
		char cRoomName[ROOMNAME_LENGTH];//������
		ENUM_RoomRule uRoomRule;//�������
		UINT uGameID;//��Ϸid
		UINT ereaid;//��Ϸ����id
		UINT uDeskCount;//��Ϸ����Ŀ
		UINT uDeskPeople;//һ�������Ϸ����
		UINT uContestid;//����id
		char cContestName[CONTESTNAME_LENGTH];//������
		ENUM_ContestRule uContestRule;//����ģʽ
		ENUM_ContestKind uContestKind;//������������
		LLONG _i64cost;//��������
		ENUM_AwardType awardType;//��������
		UINT award;//����
		HANDLE hDllInstance;//������

		Room_ManageInfoStruct()
		{
			std::memset(this, 0, sizeof(Room_ManageInfoStruct));
		}
	};

	//�������������ݽṹ��
	struct Hall_ManageInfoStruct
	{
		Hall_ManageInfoStruct()
		{
			std::memset(this, 0, sizeof(Hall_ManageInfoStruct));
		}
	};
}

#endif //_S_COM_STRUCT_HEAD_FILE


