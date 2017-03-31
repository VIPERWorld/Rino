
#include "LUserInfoManage.h"
#include "common/WJCommonMarco.h"
namespace WJ
{
	//���캯��
	CLUserInfoManage::CLUserInfoManage(void)
	{
		m_puserinfo_online_data = nullptr;
		m_UserInfo.clear();
		return;
	}

	//��������
	CLUserInfoManage::~CLUserInfoManage(void)
	{
		UnInit_CLUserInfoManage();
		return;
	}

	//��ʼ������
	bool CLUserInfoManage::Init_CLUserInfoManage(UINT uMaxOnLineCount)
	{
		UnInit_CLUserInfoManage();
		m_nMaxCount = uMaxOnLineCount;
		/*try
		{
			m_puserinfo_online_data = new LUserStruct[uMaxOnLineCount];
			for (int i = 0; i < uMaxOnLineCount; ++i)
			{
				m_UserInfo.push_back(&m_puserinfo_online_data[i]);
			}
		}
		catch (...)
		{
			return false;
		}*/

		
		return true;
	}

	//ȡ����ʼ��
	bool CLUserInfoManage::UnInit_CLUserInfoManage()
	{
		WJ_SAFE_DELETE_ARRAY(m_puserinfo_online_data);
		m_UserInfo.clear();
		m_mapUserNameID.clear();
		return true;
	}

	//�����������
	void CLUserInfoManage::clear()
	{
		//����ֻ�ǰ����е����ݳ�ʼ��
		m_UserInfo.clear();
	}

	//�����û�
	CLoginUserInfo * CLUserInfoManage::GetOnLineUserInfo(UINT dwUserID)
	{
		LUserStruct* p = GetLUserStruct(dwUserID, usertype_online);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	///��ȡ�����û�
	CLoginUserInfo * CLUserInfoManage::GetOnLineUserInfo(char username[])
	{
		LUserStruct* p = GetLUserStruct(username, usertype_online);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	//��ȡ�û�
	CLoginUserInfo * CLUserInfoManage::GetOnLineUserInfoEx(UINT uSocketIndex)
	{
		LUserStruct* p = GetLUserStructEx(uSocketIndex, usertype_online);
		if (nullptr == p)
		{
			return nullptr;
		}
		return &(p->userinfo);
	}

	///��ȡ�û���Ϣ
	LUserStruct* CLUserInfoManage::GetLUserStruct(UINT dwUserID, ENUM_usertypeL usertype)
	{
		long nID = -1;
		{
			auto iterName = m_mapUserIDSocketID.find(dwUserID);
			if (iterName != m_mapUserIDSocketID.end())
			{
				nID = iterName->second;
			}
		}
		if (nID >= 0)
		{
			auto iter = m_UserInfo.find(nID);
			if (iter != m_UserInfo.end())
			{
				if ((usertype == (*iter).second->usertype))
				{
					return (*iter).second;
				}
			}
		}
		
		return nullptr;
	}

	///��ȡ�û���Ϣ
	LUserStruct* CLUserInfoManage::GetLUserStruct(char username[], ENUM_usertypeL usertype)
	{
		if (nullptr == username)
		{
			return nullptr;
		}
		long nID = -1;
		{
			std::string strName = username;
			auto iterName = m_mapUserNameID.find(strName);
			if (iterName != m_mapUserNameID.end())
			{
				nID = iterName->second;
			}
		}
		if (nID >= 0)
		{
			auto iter = m_UserInfo.find(nID);
			if( iter != m_UserInfo.end())
			{
				if ((usertype == (*iter).second->usertype) && 0 == strcmp(username, (*iter).second->userinfo.GetUserData()->basedata.szName))
				{
					return (*iter).second;
				}
			}
		}
		return nullptr;
	}

	///��ȡ�û���Ϣ
	LUserStruct* CLUserInfoManage::GetLUserStructEx(UINT uSocketIndex, ENUM_usertypeL usertype)
	{
		auto iter = m_UserInfo.find(uSocketIndex);
		if(iter != m_UserInfo.end())
		{
			if ((usertype == (*iter).second->usertype) && (uSocketIndex == (*iter).second->userinfo.GetSocketIndex()))
			{
				return (*iter).second;
			}
		}

		return nullptr;
	}

	///��ȡ�û���Ϣtest
	LUserStruct* CLUserInfoManage::GetLUserStructEx_TEST(UINT uSocketIndex)
	{
		auto iter = m_UserInfo.find(uSocketIndex);
		if (iter != m_UserInfo.end())
		{
			if (uSocketIndex == (*iter).second->userinfo.GetSocketIndex())
			{
				return (*iter).second;
			}
		}
		
			
		
		return nullptr;
	}

	//�����û�
	CLoginUserInfo * CLUserInfoManage::ActiveUser(UserInfoInHallStruct * pUserInfo, UINT uSocketIndex)
	{
		if (nullptr == pUserInfo)
		{
			return nullptr;
		}

		if (m_UserInfo.size() > m_nMaxCount)
		{
			return nullptr;
		}
		auto iter = m_UserInfo.find(uSocketIndex);
		if (iter != m_UserInfo.end())
		{
			if (iter->second->usertype == usertype_def)
			{
				iter->second->userinfo.SetUserData(*pUserInfo, uSocketIndex);
				iter->second->usertype = usertype_online;
			}
			return &(iter->second->userinfo);
		}
		else
		{
			LUserStruct*pData = new LUserStruct();
			pData->userinfo.SetUserData(*pUserInfo, uSocketIndex);
			pData->usertype = usertype_online;
			m_UserInfo.insert(std::map<long, LUserStruct*>::value_type(uSocketIndex, pData));
			std::string strUserName = pUserInfo->basedata.szName;
			if (strUserName.length() > 0)
			{
				m_mapUserNameID.insert(std::map<std::string, long>::value_type(strUserName, uSocketIndex));
			}
			else
			{
				// ��ʾ����������
			}

			m_mapUserIDSocketID.insert(std::map<long, long>::value_type(pUserInfo->basedata.dwUserID, uSocketIndex));
			return &(pData->userinfo);
			
		}
	}

	//�ͷ��û�
	bool CLUserInfoManage::FreeUser(UINT uSocketIndex)
	{
		LUserStruct* p = GetLUserStructEx(uSocketIndex, usertype_online);
		if (nullptr == p)
		{
			return false;
		}

		m_mapUserNameID.erase(p->userinfo.GetUserData()->basedata.szName);
		m_mapUserIDSocketID.erase(p->userinfo.GetUserData()->basedata.dwUserID);
		m_UserInfo.erase(uSocketIndex);
		p->userinfo.CleanData();
		p->usertype = usertype_def;	
		WJ_SAFE_DELETE(p);
		return true;
	}

	UINT CLUserInfoManage::GetOnLineUserCount()
	{
		UINT icount = 0;
		for (auto iter = m_UserInfo.begin(); iter != m_UserInfo.end(); ++iter)
		{
			if (usertype_online == (*iter).second->usertype)
			{
				++icount;
			}
		}

		return icount;
	}

	///��ȡȫ������
	UINT CLUserInfoManage::GetLoginCount()
	{
		return m_UserInfo.size();
	}
}
