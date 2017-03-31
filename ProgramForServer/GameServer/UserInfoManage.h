#pragma once

#include <vector>
#include <mutex>
#include <unordered_map>
#include "common/WJBaseType.h"
#include "UserInfo.h"
namespace WJ
{
	//�û����й���ʱ���õ�--------------------------------------------
	enum ENUM_usertype
	{
		usertype_def = 0,//Ĭ��״̬
		usertype_online,//����
		usertype_netcut,//����
	};

	struct GameUserStruct
	{
		ENUM_usertype usertype;//�û���������
		CGameUserInfo userinfo;//�û�������Ϣ

		GameUserStruct()
		{
			usertype = usertype_def;
		}
		~GameUserStruct()
		{
			int x;
			x = 1;
		}
	};
	//----------------------------------------------------------------

	///�û�������
	class CGameUserInfoManage
	{
		///��������
	private:
		std::unordered_map<UINT, GameUserStruct*>	m_userid2userinfo;	//�û���Ϣ
		std::unordered_map<std::string, UINT>       m_username2userid;  //�û������û�idӳ��,Ŀǰû��,�Ժ����Ҳ��Ҫ��,gameserver�Ͱ�userid��
		std::unordered_map<UINT, UINT>	m_socketid2userid;	//socketid���û�idӳ��
	public:
		///���캯��
		CGameUserInfoManage(void);
		///��������
		~CGameUserInfoManage(void);

		//�½��û�����
		CGameUserInfo* ActiveUser(UserInfoInRoomStruct * pUserInfo, UINT uSocketIndex);

		///ͨ���׽���id�ͷ��û����������ڶ����û���
		bool FreeUser(UINT uSocketIndex);
		///�ͷ��û������߷����߶����ã�
		bool FreeUserEx(UINT userid);
		///ע���û�����
		CGameUserInfo* RegNetCutUser(UINT socketId);
		///��ȡ�����û�
		CGameUserInfo* GetOnLineUserInfo(UINT dwUserID);
		///��ȡ�����û�
		CGameUserInfo* GetOnLineUserInfo(char* username);
		///��ȡ�����û�
		CGameUserInfo* GetOnLineUserInfoEx(UINT socketId);


		///�����û�
		CGameUserInfo* GetNetCutUser(UINT dwUserID);
		///��ȡ�����û�
		CGameUserInfo* GetNetCutUser(char* username);

		CGameUserInfo* GetUserInfo(UINT dwUserID);
		CGameUserInfo* GetUserInfo(char* username);
		CGameUserInfo* GetUserInfoEx(UINT socketid);

		std::unordered_map<UINT, GameUserStruct*>* GetGameUserStruct_map(){ return &m_userid2userinfo; }
		///��ȡ��������
		UINT GetOnLineUserCount();
		///��ȡ��������
		UINT GetNetCutUserCount();
		///��ȡȫ������
		UINT GetGameRoomCount(UINT uRoomid);
	private:
		///��ȡ�û���Ϣ
		GameUserStruct* GetGameUserStruct(UINT dwUserID, ENUM_usertype usertype);
		GameUserStruct* GetGameUserStructEx(UINT socketid, ENUM_usertype usertype);
		GameUserStruct* GetGameUserStruct(char* username, ENUM_usertype usertype);
		GameUserStruct* GetGameUserStruct(UINT dwUserID);
		GameUserStruct* GetGameUserStruct(char* username);
		GameUserStruct* GetGameUserStructEx(UINT socketid);
	};
}
