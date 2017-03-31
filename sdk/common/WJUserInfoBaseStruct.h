#ifndef __WJ_UserInfoBaseStruct_H__
#define __WJ_UserInfoBaseStruct_H__

#include "WJBaseType.h"
namespace WJ
{
	enum ENUM_UserStation
	{//�û�״̬
		US_P_Wait = 0,	//��������״̬
		US_P_Applying,	//������״̬
		US_P_unApplying,	//ȡ��������״̬
		US_P_CreateTeaming,//�����Ŷ���״̬
		US_P_AddTeaming,//�����Ŷ���״̬
		US_P_DissolveTeaming,//��ɢ�Ŷ���״̬
		US_P_LeaveTeaming,//�뿪�Ŷ���״̬
		US_P_TeamVSing,//�Ŷ�VS��״̬


		US_R_Wait,	//�������״̬
		US_R_GetStation,//�����λ״̬
		US_R_Agress,  //����״̬
		US_R_Playing, //��Ϸ��״̬
		US_R_NetCut,  //����״̬
	};

	///�û���Ϣ�ṹ
	struct UserInfoBaseStruct
	{
	public:
		char				szName[NAME_LENGTH];			//��¼��
		char				nickName[NAME_LENGTH];			//�û��ǳ�
		char				password[PASSWORD_MD5_LENGTH];	//����
		UINT				dwUserID;						//ID
		UINT				dwPoint;						//����
		LLONG				i64Money;						//���
		ENUM_UserStation	bUserState;						//�û�״̬
		UINT				teamid;							//�Ŷ�id
		bool				bRobot;							//������
		char				icon[USER_ICON_LENGTH];			//����ͷ����Ϣ
		UserInfoBaseStruct()
		{
			reset();
		}

		void reset()
		{
			std::memset(this, 0, sizeof(UserInfoBaseStruct));
			teamid = ERROR_TEAMID;
		}
	};
	//�����û��ڵ�
	struct UserInfoInHallStruct
	{
		UserInfoBaseStruct basedata;//��������
		char				telephone[PHONE_LENGTH];		//�绰����
		char				IDCard[IDCARD_LENGTH];			//���֤����
		
		UserInfoInHallStruct()
		{
			reset();
		}

		void reset()
		{
			basedata.reset();
			basedata.bUserState = US_P_Wait;
		}
	};
	//�����û��ڵ�
	struct UserInfoInRoomStruct
	{
		UserInfoBaseStruct basedata;//��������
		UINT                uRoomid;                        //����id
		UINT				uDeskid;						//����id
		UINT				uDeskStation;					//��λid
		UINT				uContestid;						//����id

		UserInfoInRoomStruct()
		{
			reset();
		}

		void reset()
		{
			basedata.reset();
			uRoomid = ERROR_ROOMID;
			uDeskid = ERROR_DESKID;
			uDeskStation = ERROR_DESKSTATION;
			basedata.bUserState = US_R_Wait;
			uContestid = ERROR_CONTESTID;
		}
	};
}


#endif	//__WJ_UserInfoBaseStruct_H__