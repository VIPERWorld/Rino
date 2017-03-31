#ifndef WJ_GameDesk_H
#define WJ_GameDesk_H
#include <mutex>
#include "common/WJBaseType.h"
#include "UserInfo.h"
#include "message/WJNetMessageHead.h"
#include "server/WJTimerMgr.h"


namespace WJ
{
	//��Ϸ��ʱ��id ��������ϵͳ��ʱ��һ�ɰ��� TIMER_GAME_TIMERID_MASK_SYS ��ʽ���
#define GAME_TIME_GAMEFINISH_WAIT		0x00000010 //���յ�һ����ҷ��ͽ�����Ϣ��ʱ����������ֹ��Щ��Ҷ��߲���
#define GAME_TIME_AGREE_OUTTIME			0x00000020 //��Ϸ�����ɹ��������ü�ʱ��ֱ����Ϸ��ʼ��ʱ�������ʱ���������ˣ���ֹ�����ղ����ͻ���֪ͨ��ͬ����Ϣ���������ӿ�ס
#define GAME_TIME_GAMEFINISH_OUTTIME	0x00000030 //��Ϸһ����ʼ�������ü�ʱ������ֹ�����ղ����ͻ���֪ͨ�Ľ����Ӷ��޷�������Ϸ
	class CGameRoom;
	struct Room_ManageInfoStruct;
	class ISysDeskInterface
	{
	public://����ӿ��Ǹ�������õģ���Ϸ�˲��ܼ̳У�����Ҫ�õ��ؼ��� final
		//��������,ǿ������(�Ƿ��������·Ż��Ŷӻ���)
		virtual void ClearDesk(bool bInqueue = false) = 0;
		///�ж���Ϸ���ϵ�ĳ������Ƿ�ʼ��Ϸ��
		virtual bool IsPlayGame() = 0;
		///�û�ͬ�⿪ʼ
		virtual bool UserAgreeGame(UINT uDeskStation) = 0;
		///�����û����ͽ�����Ϣ
		virtual bool Sys_UserEndGame(UINT uDeskStation, void* pData, UINT DataSize) = 0;
		/// ��������е�һ��û���˵���λ����
		virtual bool GetDeskNoPlayerStation(UINT &uDeskStation) = 0;
		/// ����������ж�����
		virtual UINT GetDeskPlayerNum() = 0;
		///�û�����״̬�뿪��Ϸ
		virtual bool UserOnlineStationNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) = 0;
		///�û�����״̬�ػ���Ϸ
		virtual bool UserOnlineStationReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) = 0;
		///�Ƿ�����
		virtual bool IsLock() = 0;
		///����
		virtual void LockDesk() = 0;
		///����
		virtual void UnlockDesk() = 0;
		//�Ƿ���ڸ����
		virtual bool IsExistPlayer(CGameUserInfo * pUserInfo) = 0;
		///��ʼ������
		virtual bool Init(UINT uDeskStation, UINT bMaxPeople, void* pDataManage) = 0;
		//�ж������λ�Ƿ�����������
		virtual bool bExistPlayerForDeskStation(UINT uDeskStation) = 0;
		/// ���������е������Ϣ
		virtual void SetDeskUserInfo(CGameUserInfo * pUserInfo, UINT uDeskStation) = 0;
		//��ȡ�û���Ϣָ��
		virtual /*const*/ CGameUserInfo* GetUserInfo(UINT uDeskStation) = 0;
		//��ȡ�������
		virtual const UINT GetMaxPeople() = 0;
		//��ȡ����id
		virtual const UINT GetDeskIndex() = 0;
		//������������
		virtual bool SendDeskData(void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode) = 0;
		virtual bool SendDeskData(UINT uMainID, UINT uAssID, UINT uHandleCode) = 0;
		//��ȡͬ����� 
		virtual UINT FillDeskUserInfo(char * pBuffer, UINT uBufferSize, UINT& uindex, bool & bFinish) = 0;
		///��Ϸ����
		/// @param bDeskStation �쳣����ʱ���������������������е�λ�úţ���������ʱ������
		/// @param bCloseFlag  �쳣������Ϸʱ�ı�־����������ʱΪ0
		virtual bool GameFinish(UINT uCloseFlag, UINT uDeskStation = 0) = 0;
		///�û������뿪
		virtual bool Sys_UserNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) = 0;
		///�û���������
		virtual bool Sys_UserReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) = 0;
		///��Ϸ��ʼ
		virtual bool Sys_GameBegin(UINT uBeginFlag) = 0;
		///�û��뿪��Ϸ��
		virtual UINT Sys_UserLeftDesk(UINT uDeskStation, CGameUserInfo * pUserInfo, UINT uLeftCause = 0) = 0;
		///�����Ϣ������
		virtual bool Sys_HandleFrameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) = 0;
		///��Ϸ���ݰ�������
		virtual bool Sys_HandleGameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) = 0;
		///��ʱ����Ϣ
		virtual bool Sys_OnTimer(UINT uTimerID) = 0;
		///��ȡ��Ϸ״̬��Ϣ
		virtual bool Sys_OnGetGameStation(UINT uDeskStation, UINT uSocketID) = 0;
		//���÷���
		virtual void SetdwOwnerUserID(UINT uuserid) = 0;
		//��ȡ����
		virtual UINT GetdwOwnerUserID() = 0;
		//�����������
		virtual void QueueOver() = 0;
		//����Ƿ��κ�ʱ�򶼿����뿪����ζ��һ�뿪��Ϸ�ͽ����ˣ�����Ϸ״ֻ̬��һ����
		virtual bool SysCanLeaveEveryTime() = 0;
	};

	class /*WJSERVERGAMELIB_API*/ CGameDesk : public ISysDeskInterface
	{
	private://��������һ��ȷ���������
		UINT							m_uDeskIndex;							///< ���Ӻ���
		UINT							m_uMaxPeople;							///< ��Ϸ����
		CGameRoom						* m_pGameRoom;							///< ���ݹ���ָ��
	private:
		long int						m_dwBeginTime;							///< ��ʼʱ��
		bool							m_bLock;								///< �Ƿ�����
		char							m_szLockPass[61];						///< ��������
		UINT							m_dwOwnerUserID;						///< ���� ID
		bool							m_bPlayGame;							///< ��Ϸ�Ƿ�ʼ��־
		CGameUserInfo					* m_pUserInfo[MAX_PEOPLE];				///< �û���Ϣָ�룬����������һ������ҵ���λ������
		bool							m_UserEndGame[MAX_PEOPLE];				///< �ͻ���֪ͨ����������Ϸ���Խ�����

	public://������Ϸ�޸ĵ�����
		UINT							m_uGameStation;							///< ��Ϸ״̬

	public:
		CGameDesk();
		~CGameDesk();

	public:///��Ϸ����ʵ�ֵĺ���/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///�Ƿ���Կ�ʼ��Ϸ
		virtual bool CanBeginGame() = 0;
		//��Ϸ�ڲ���ʼ��
		virtual bool InitDeskGameStation() = 0;
		///��ȡ��Ϸ״̬��Ϣ
		virtual bool OnGetGameStation(UINT uDeskStation, UINT uSocketID) = 0;
		///������Ϸ״̬
		virtual bool ReSetGameState(UINT uLastStation) = 0;
		///�û������뿪
		virtual bool UserNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) = 0;
		///�û���������
		virtual bool UserReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) = 0;
		///��Ϸ��ʼ
		virtual bool GameBegin(UINT uBeginFlag) = 0;
		///�û��뿪��Ϸ��
		virtual UINT UserLeftDesk(UINT uDeskStation, CGameUserInfo * pUserInfo, UINT uLeftCause = 0) = 0;
		///�����Ϣ������
		virtual bool HandleFrameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) = 0;
		///��Ϸ���ݰ�������
		virtual bool HandleGameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) = 0;
		///��ʱ����Ϣ
		virtual bool OnTimer(UINT uTimerID) = 0;
		//��Ϸ������ʱ������Ϸ�ڲ����ݣ�����
		//void* pData:���ؽ�������Ϣ������
		//UINT &DataSize:���ؽ�������Ϣ����С
		virtual bool GameOverSettlement(UINT uCloseFlag, std::string &pData) = 0;
		///�����û����ͽ�����Ϣ
		virtual bool UserEndGame(UINT uDeskStation, void* pData, UINT DataSize) = 0;
		//����Ƿ��κ�ʱ�򶼿����뿪����ζ��һ�뿪��Ϸ�ͽ����ˣ�����Ϸ״ֻ̬��һ����
		virtual bool CanLeaveEveryTime() = 0;
	public://��Ϸ�ͷ���˶���ʹ�õĽӿ�///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///������Ϸ״̬
		bool SendGameStation(UINT uDeskStation, UINT uSocketID, void* pStationData, UINT uSize);
		///���Ϳ�ʼ��Ϣ
		bool SendBeginData(void * pData, UINT uSize);
		///���ͽ�����Ϣ
		bool SendFinishData(void * pData, UINT uSize);
		///�������ݺ���
		bool SendGameData(UINT uDeskStation, UINT uMainID, UINT uAssID, UINT uHandleCode);
		///�������ݺ���
		bool SendGameData(UINT uDeskStation, void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode);
		//�������ͺ���
		void SendDataBatch(void * pData, UINT uSize, UINT uMainID, UINT uAssistantID, UINT uHandleCode);
		///���ö�ʱ��
		bool SetTimer(UINT uTimerID, int uElapse);
		///ɾ����ʱ��
		bool KillTimer(UINT uTimerID);
		//��ȡ����ָ��
		const Room_ManageInfoStruct* GetRoom_ManageInfoStruct();
		//����������
		void GetRandText(std::string &str, int strleng);

		//��ȡ�û���Ϣָ��
		virtual CGameUserInfo* GetUserInfo(UINT uDeskStation) override final;
		//��ȡ�������
		virtual const UINT GetMaxPeople() override final;
		//��ȡ����id
		virtual const UINT GetDeskIndex() override final;
		///�ж���Ϸ���Ƿ�ʼ��Ϸ��
		virtual bool IsPlayGame() override final;
		/// ����������ж�����
		virtual UINT GetDeskPlayerNum() override final;
		//������������
		virtual bool SendDeskData(void * pData, UINT uSize, UINT uMainID, UINT uAssID, UINT uHandleCode) override final;
		virtual bool SendDeskData(UINT uMainID, UINT uAssID, UINT uHandleCode) override final;
		///��Ϸ����
		/// @param bCloseFlag  �쳣������Ϸʱ�ı�־����������ʱΪ0
		virtual bool GameFinish(UINT uCloseFlag, UINT uDeskStation = 0) override final;
	private://�������ʹ�ýӿ�///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//��������,ǿ������(�Ƿ��������·Ż��Ŷӻ���)
		virtual void ClearDesk(bool bInqueue = false) override final;
		///�û�ͬ�⿪ʼ
		virtual bool UserAgreeGame(UINT uDeskStation) override final;
		///�����û����ͽ�����Ϣ
		virtual bool Sys_UserEndGame(UINT uDeskStation, void* pData, UINT DataSize) override final;
		/// ��������е�һ��û���˵���λ����
		virtual bool GetDeskNoPlayerStation(UINT &uDeskStation) override final;
		///�û�����״̬�뿪��Ϸ
		virtual bool UserOnlineStationNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) override final;
		///�û�����״̬�ػ���Ϸ
		virtual bool UserOnlineStationReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) override final;
		///�Ƿ�����
		virtual bool IsLock() override final;
		///����
		virtual void LockDesk() override final;
		///����
		virtual void UnlockDesk() override final;
		//�Ƿ���ڸ����
		virtual bool IsExistPlayer(CGameUserInfo * pUserInfo) override final;
		///��ʼ������
		virtual bool Init(UINT uDeskStation, UINT bMaxPeople, void* pDataManage) override final;
		//�ж������λ�Ƿ�����������
		virtual bool bExistPlayerForDeskStation(UINT uDeskStation) override final;
		/// ���������е������Ϣ
		virtual void SetDeskUserInfo(CGameUserInfo * pUserInfo, UINT uDeskStation) override final;
		//��ȡͬ����� 
		virtual UINT FillDeskUserInfo(char * pBuffer, UINT uBufferSize, UINT& uindex, bool & bFinish) override final;
		///�û������뿪
		virtual bool Sys_UserNetCut(UINT uDeskStation, CGameUserInfo * pLostUserInfo) override final;
		///�û���������
		virtual bool Sys_UserReCome(UINT uDeskStation, CGameUserInfo * pNewUserInfo) override final;
		///��Ϸ��ʼ
		virtual bool Sys_GameBegin(UINT bBeginFlag) override final;
		///�û��뿪��Ϸ��
		virtual UINT Sys_UserLeftDesk(UINT uDeskStation, CGameUserInfo * pUserInfo, UINT uLeftCause = 0) override final;
		///�����Ϣ������
		virtual bool Sys_HandleFrameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) override final;
		///��Ϸ���ݰ�������
		virtual bool Sys_HandleGameMessage(UINT uDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID) override final;
		///��ʱ����Ϣ
		virtual bool Sys_OnTimer(UINT uTimerID) override final;
		///��ȡ��Ϸ״̬��Ϣ
		virtual bool Sys_OnGetGameStation(UINT uDeskStation, UINT uSocketID) override final;
		//���÷���
		virtual void SetdwOwnerUserID(UINT uuserid) override final;
		//��ȡ����
		virtual UINT GetdwOwnerUserID() override final;
		//�����������
		virtual void QueueOver() override final;
		//����Ƿ��κ�ʱ�򶼿����뿪����ζ��һ�뿪��Ϸ�ͽ����ˣ�����Ϸ״ֻ̬��һ����
		virtual bool SysCanLeaveEveryTime() override final;
	private://�����Լ�ʹ�õķ���/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///�Ƿ���Կ�ʼ��Ϸ
		bool Sys_CanBeginGame();
		///�������ж������
		bool CleanCutGameInfoAll();
		///����ָ���������
		bool CleanCutGameInfo(UINT bDeskStation);
		//���������
		int MyRand();

		//��ȡһ����������������б�,���Լ���Ϣ��
		UINT FillDeskUsers(char * pBuffer, UINT uBufferSize, UINT deskid, UINT myuserid, UINT uBeginPos, bool & bFinish, UINT &uCopyCount);

		//��ָ����Ҹ��¸�����������������г��Լ���������û��б�
		void SendUserListForDesk(UINT deskid, UINT socketindex, UINT myuserid);
	};
}

#endif//WJ_GameDesk_H