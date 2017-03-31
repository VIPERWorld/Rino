#ifndef WJCLIENTSOCKETLOGIC_H
#define WJCLIENTSOCKETLOGIC_H

#include <list>

#ifdef _UNUSE_QT
#else
#include <QtCore>
#endif

#include "WJSocketMessageDelegate.h"
#include "WJSocketEventDelegate.h"
#include "WJSocketSelector.h"
#include "WJSocketThread.h"
#include "WJSocketMessage.h"

namespace WJ
{
#ifdef _UNUSE_QT
	class WJCLIENTSOCKETLOGIC_EXPORT WJClientSocketLogic : public ISocketEventDelegate
#else
	class WJCLIENTSOCKETLOGIC_EXPORT WJClientSocketLogic : public QObject, ISocketEventDelegate
#endif
	{
	public:
		WJClientSocketLogic(ISocketMessageDelegate* delegate, const std::string& tag);
		virtual ~WJClientSocketLogic();

		typedef WJSocketSelector<std::string> WJSocketMessageSelectorHandler;

		/*
		* �׽��ָ����Ѿ���ʱ��ɣ���ʱ���ָ�
		*/
		//virtual void onReadComplete() override;
	public:
		//����ĳ�ʼ��
		bool init();
		//�Ƿ����ӳɹ�
		bool connected();

	public:
		BYTE GetSocketState();

		//���ӷ�����
		bool openWithIp(const CHAR* ip, INT port);

		//���ӷ�����
		bool openWithHost(const CHAR* host, INT port);

		//�ر��׽���
		bool close();

		//��������
		INT send(UINT MainID, UINT AssistantID, void* object, INT objectSize);
		//��������
		INT send(UINT MainID, UINT AssistantID, UINT uHandleCode, void* object, INT objectSize);
		//������Ϣ�ص��ĺ�����
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		//�Ƴ���Ϣ�ص��ĺ�����
		void removeEventSelector(UINT MainID, UINT AssistantID);

		//����������Ϣ
		void heartBeat();

		//��������
		void resetData();

		int gethWindow();
		void sethWindow(int hWindow);
		//��ʱ��
		void OnTimeMesage(int itimeid);
	private:
		///WINDOW ��Ϣѭ���߳�
		void WindowMsgThread();

		///�趨��ʱ��
		bool SetTimer(UINT uTimerID, UINT uElapse);
		///�����ʱ��
		bool KillTimer(UINT uTimerID);

		//��������������
		void socketHeartBeatSend();

#ifdef _UNUSE_QT

#else
		Q_OBJECT

	private slots:
#endif
		/*
		* ������Ϣ֪ͨ������
		*/
		void socketDataDispatch();
	private:
		WJSocketThread*					_socketThread;//�׽����߳�

		WJSocketMessageSelectorHandler*	_selectorQueue;//��Ϣ�ص��ĺ����󶨶���

		ISocketMessageDelegate*			_delegate;//�ϲ����������ӿڸ���

		//�Ƿ���Դ�����Ϣ����
		//bool _bReadComplete;

		int		m_hWindow;					///���ھ��

	};
}


#endif // WJCLIENTSOCKETLOGIC_H
