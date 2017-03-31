#ifndef WJ_USERINFOMANAGE_H
#define WJ_USERINFOMANAGE_H

#include <vector>
#include <map>
#include <mutex>
#include "common/WJBaseType.h"
#include "LUserInfo.h"
namespace WJ
{
	//�û����й���ʱ���õ�--------------------------------------------
	enum ENUM_usertypeL
	{
		usertype_def = 0,//Ĭ��״̬
		usertype_online,//����
	};

	struct LUserStruct
	{
		ENUM_usertypeL usertype;//�û���������
		CLoginUserInfo userinfo;//�û�������Ϣ

		LUserStruct()
		{
			usertype = usertype_def;
		}
	};
	//----------------------------------------------------------------

	///�û�������
	class CLUserInfoManage
	{
		///��������
	private:
		map<long ,LUserStruct*>					m_UserInfo;				///�û���Ϣ
		map<std::string, long>                  m_mapUserNameID;        // �û���-��ID ӳ�� �ӿ�����ٶ�
		map<long, long>                  m_mapUserIDSocketID;        // �û���-��ID ӳ�� �ӿ�����ٶ�
		LUserStruct *m_puserinfo_online_data;//�ڲ�������ڴ�ռ䣬��ֱ��ʹ��
		long m_nMaxCount;
		///��������
	public:
		///���캯��
		CLUserInfoManage(void);
		///��������
		~CLUserInfoManage(void);

		///��ʼ������
		bool Init_CLUserInfoManage(UINT uMaxOnLineCount);
		///ȡ����ʼ��
		bool UnInit_CLUserInfoManage();
		//�����������
		void clear();
		///�����û�
		CLoginUserInfo* ActiveUser(UserInfoInHallStruct * pUserInfo, UINT uSocketIndex);
		///�ͷ��û�
		bool FreeUser(UINT uSocketIndex);
		///��ȡ�����û�
		CLoginUserInfo* GetOnLineUserInfo(UINT dwUserID);
		///��ȡ�����û�
		CLoginUserInfo* GetOnLineUserInfo(char username[]);
		///��ȡ�����û�
		CLoginUserInfo* GetOnLineUserInfoEx(UINT uSocketIndex);
	

		///��ȡ��������
		UINT GetOnLineUserCount();
		///��ȡȫ������
		UINT GetLoginCount();
		
		///��ȡ�û���Ϣtest
		LUserStruct* GetLUserStructEx_TEST(UINT uSocketIndex);

		map<long, LUserStruct*>* GetLUserStruct_map(){ return &m_UserInfo; }
	private:
		///��ȡ�û���Ϣ
		LUserStruct* GetLUserStruct(UINT dwUserID, ENUM_usertypeL usertype);
		///��ȡ�û���Ϣ
		LUserStruct* GetLUserStruct(char username[], ENUM_usertypeL usertype);
		///��ȡ�û���Ϣ
		LUserStruct* GetLUserStructEx(UINT uSocketIndex, ENUM_usertypeL usertype);
	};
}

#endif //WJ_USERINFOMANAGE_H