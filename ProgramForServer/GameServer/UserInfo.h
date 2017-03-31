
#ifndef WJ_USERINFO_H
#define WJ_USERINFO_H

#include "common/WJBaseType.h"
#include "common/WJUserInfoBaseStruct.h"
using namespace std;

namespace WJ
{
	///�û�������
	class CGameUserInfo
	{
		///��������
	private:
		UserInfoInRoomStruct				m_UserData;							///�û�����
		UINT								m_uSockId;						    ///��������
		///�������� 
	public:
		///���캯�� 
		CGameUserInfo(void);
		///��������
		~CGameUserInfo(void);

		///���ܺ���
	public:
		///������Ϣ
		bool SetUserData(UserInfoInRoomStruct & UserInfo, UINT uSocketId);
		///����״̬
		bool SetUserState(ENUM_UserStation bUserState, UINT bDeskNO, UINT uDeskStation, bool bSetDeskStation);
		///�����û�����
		bool ChangePoint(__int64 dwPoint, __int64 dwTaxCom, bool bWin, bool bLost, bool bMid, bool bCut, long int dwPlayCount, __int64 &dwMoney);
		///�����û���Ϣ
		bool CleanData();
		///��ȡ�û���Ϣ
		UserInfoInRoomStruct * GetUserData();

		///��ȡ��������
		inline UINT GetSocketId() { return m_uSockId; }
	};

	/*******************************************************************************************************/



	/*******************************************************************************************************/
}

#endif//WJ_USERINFO_H