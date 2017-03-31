#include "LUserInfo.h"
#include "common/commonfunction.h"
#include "common\QkLogger.h"

namespace WJ
{
	//���캯��
	CLoginUserInfo::CLoginUserInfo(void)
	{
		m_uSocketIndex = ERROR_SOCKETID;
		memset(&m_UserData, 0, sizeof(m_UserData));
	}

	//��������
	CLoginUserInfo::~CLoginUserInfo(void)
	{
	}

	UINT CLoginUserInfo::GetSocketIndex()
	{
		return m_uSocketIndex; 
	}

	bool CLoginUserInfo::SetUserData(UserInfoInHallStruct & UserInfo, UINT uSocketIndex)
	{
		if (uSocketIndex == ERROR_SOCKETID)
		{
			QK_InfoLog("sdp_testaaa 22222 m_uSocketIndex=%d", m_uSocketIndex);
		}

		memcpy(&m_UserData, &UserInfo, sizeof(UserInfoInHallStruct));
		m_uSocketIndex = uSocketIndex;
		return true;
	}

	//�����û�״̬
	bool CLoginUserInfo::SetUserState(ENUM_UserStation bUserState)
	{
		m_UserData.basedata.bUserState = bUserState;
		QK_InfoLog("sdp_test hall �������״̬(%d)", bUserState);
		return true;
	}

	//�����û�����
	bool CLoginUserInfo::ChangePoint(__int64 dwPoint, __int64 dwTaxCom, bool bWin, bool bLost, bool bMid, bool bCut, long int dwPlayCount, __int64 &dwMoney)
	{
		return false;
	}

	//�����û���Ϣ
	bool CLoginUserInfo::CleanData()
	{
		m_uSocketIndex = ERROR_SOCKETID;
		memset(&m_UserData, 0, sizeof(m_UserData));
		return true;
	}

	///��ȡ�û���Ϣ
	UserInfoInHallStruct * CLoginUserInfo::GetUserData()
	{
		return &m_UserData;
	}

	//�Ƿ�����л��û�״̬�����籨���в��ܽ����˷ѣ�Ӧ��Ҫ�ȱ���״̬��Ϊ����״̬��
	bool CLoginUserInfo::bCanChangeUserStation(ENUM_UserStation	bUserState)
	{
		QK_InfoLog("sdp_test hall �ж����״̬(%d, %d)", m_UserData.basedata.bUserState, bUserState);
		switch (m_UserData.basedata.bUserState)
		{
		case US_P_Wait:	//��������״̬
		{
							return true;
		}break;
		case US_P_Applying:	//������״̬
		case US_P_unApplying:	//ȡ��������״̬
		case US_P_CreateTeaming://�����Ŷ���״̬
		case US_P_AddTeaming://�����Ŷ���״̬
		case US_P_DissolveTeaming://��ɢ�Ŷ���״̬
		case US_P_LeaveTeaming://�뿪�Ŷ���״̬
		case US_P_TeamVSing://�Ŷ�VS��״̬
		{
								return US_P_Wait == bUserState;
		}break;
		default:
		{
				   return false;
		}
		}

		return false;
	}
}

