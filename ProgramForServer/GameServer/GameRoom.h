#ifndef WJ_GameRoom_H
#define WJ_GameRoom_H

#include <unordered_map>
#include "UserInfoManage.h"
#include "GameDeskManage.h"
#include "QueueManage.h"
#include "server/WJSSocket.h"

namespace WJ
{
	class CGameRoom
	{
	private:
		CGameDeskManage						m_GameDeskManage;	//���ӹ���
		Room_ManageInfoStruct				m_initRoomData;		//��������

	public:
		CGameRoom();
		~CGameRoom();

		const Room_ManageInfoStruct* GetRoom_ManageInfoStruct();
		CGameDeskManage* GetCGameDeskManage();
		
		///��ʼ����������
		void Init_Room(Room_ManageInfoStruct &initRoomData);
		///��ʼ������
		virtual bool Init_subclass();
		///ȡ����ʼ������
		virtual bool UnInit_subclass();
		///��������
		virtual bool Start_subclass();
		///ֹͣ����
		virtual bool Stop_subclass();

		void UpdateInfoToLS();

		//�������ͺ���
		void SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode);
	public:
		//�û��б���Ϣ
		bool OnSocketRead(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);

		bool OnSocketClose(CWJSSocket *pSocket);
		bool OnTimerMessage(UINT uTimerID);
	private:
		//�û��б���Ϣ
		bool OnSocketRead_user_list(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//�û�������Ϣ
		bool OnSocketRead_user_action(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//������Ϣ
		bool OnSocketRead_room(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//�Ŷ���Ϣ
		bool OnSocketRead_queue(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//��Ϸ�����Ϣ
		bool OnSocketRead_game_frame(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
		//��Ϸ��ͨ��Ϣ
		bool OnSocketRead_game_notify(CWJSSocket *pSocket, NetMessageHead * pNetHead, void * pData, UINT uSize);
	};
}

#endif//WJ_GameRoom_H