
#ifndef WJ_USERINFO_H
#define WJ_USERINFO_H

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"
using namespace std;

namespace WJ
{
	///�û�������
	class CLoginUserInfo
	{
		///��������
	private:
		UserInfoInHallStruct				m_UserData;							///�û�����
		UINT								m_uSocketIndex;						///��������
		///�������� 
	public:
		///���캯�� 
		CLoginUserInfo(void);
		///��������
		~CLoginUserInfo(void);

		///���ܺ���
	public:
		///������Ϣ
		bool SetUserData(UserInfoInHallStruct & UserInfo, UINT uSocketIndex);
		///����״̬
		bool SetUserState(ENUM_UserStation bUserState);
		///�����û�����
		bool ChangePoint(__int64 dwPoint, __int64 dwTaxCom, bool bWin, bool bLost, bool bMid, bool bCut, long int dwPlayCount, __int64 &dwMoney);
		///�����û���Ϣ
		bool CleanData();
		///��ȡ�û���Ϣ
		UserInfoInHallStruct * GetUserData();

		///��ȡ��������
		UINT GetSocketIndex();

		//�Ƿ�����л��û�״̬�����籨���в��ܽ����˷ѣ�Ӧ��Ҫ�ȱ���״̬��Ϊ����״̬��
		bool bCanChangeUserStation(ENUM_UserStation	bUserState);
	};

	/*******************************************************************************************************/



	/*******************************************************************************************************/
}

#endif//WJ_USERINFO_H