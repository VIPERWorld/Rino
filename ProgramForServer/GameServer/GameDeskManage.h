#ifndef WJ_GAMEDESKMANAGE_H
#define WJ_GAMEDESKMANAGE_H

#include <mutex>
#include <vector>
#include "common/WJBaseType.h"
#include "server/comstruct.h"
#include "GameDesk.h"
namespace WJ
{
	class CGameDeskManage
	{
	private:
		std::vector<ISysDeskInterface*>			m_pDeskArray;		//��Ϸ�����б�
		HANDLE								m_hDllInstance;		//������
		Room_ManageInfoStruct*				m_gameinfo;			//��Ϸ��Ϣ
	public:
		CGameDeskManage();
		~CGameDeskManage();

		///��ʼ����Ϸ��
		//uDeskCount �����������
		//uDeskPeople ÿһ����������
		bool InitGameDesk(UINT uDeskCount, UINT uDeskPeople, Room_ManageInfoStruct* pData, void* pDataManage);
		///��ȡ��Ϸ������Ϣ
		void CreateDeskObject(UINT uInitDeskCount);
		//������
		UINT GetCount();
		//���ؾ���һ������
		ISysDeskInterface* At(UINT index);
	};
}

#endif//WJ_GAMEDESKMANAGE_H