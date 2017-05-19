// MemeryShare.h

#ifndef _EVENTSIGNAL_eventsignal_H_
#define _EVENTSIGNAL_eventsignal_H_
#include <Windows.h>
#include "macrodefinition.h"
#include <vector>
#include "ShareMemory.h"
#define MEMERY_DEFAULT_SIZE		(0xA00000)

#define EVENT_SERVER_DEFAULT_NAME	TEXT("ZSEvent_Signal_Server_Name")
#define EVENT_CLIENT_DEFAULT_NAME	TEXT("ZSEvent_Signal_Client_Name")
#define EVENT_EXIT_DEFAULT_NAME		TEXT("ZSEvent_Signal_Exit_Name")

#define WANGJING_PLATFORM_SERVER_SHARE_MEMORY_NAME		TEXT("WANGJING_PLATFORM_SERVER_SHARE_MEMORY_NAME")
#define WANGJING_PLATFORM_CLIENT_SHARE_MEMORY_NAME		TEXT("WANGJING_PLATFORM_CLIENT_SHARE_MEMORY_NAME")

#define EVENT_EXIT_INDEX 0
#define EVENT_SERVER_INDEX 1
#define EVENT_CLIENT_INDEX 2

#include "common/XLog.h"

class EventSignal {
public:
	EventSignal();
	~EventSignal();

public:
	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   initEventSignal   ��ʼ���źŹ�����
	* @version: V1.0.0.1
	* @param:	LPCTSTR serverEventName	���������
	* @param:	LPCTSTR clientEventName	�ͻ�������
	* @param:	LPCTSTR exitEventName	�˳��¼������¼���ȥ��������λ�ùرձ������/�ͻ��˵�����
	* @param:	IDataRecive* pDataRecive �¼����մ�����
	* @param:	bool isServer = true	�Ƿ�Ϊ����ˣ�Ĭ��Ϊ�����
	* @return:  BOOL
	**/

	BOOL initEventSignal(LPCTSTR serverEventName, LPCTSTR clientEventName, LPCTSTR exitEventName,
		IDataRecive* pDataRecive, bool isServer = true);
	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   Start   �����̣߳���������
	* @version: V1.0.0.1
 	* @return:  BOOL
	**/
	BOOL StartThread();

	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   SendEvent   ����֪ͨ�¼���ͬʱ������Ҫ���͵��������С
	* @version: V1.0.0.1
	* @return:  BOOL
	**/
	BOOL SendEvent(LPVOID pData, size_t size);


	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   CallExit   �˳�����
	* @version: V1.0.0.1
	* @return:  BOOL
	**/

	BOOL CallExit();

	void SetAutoSendMode(bool _flag) { m_isAutoSend = _flag; };

	XLog& getLog() { return m_log; };

	static UINT __stdcall  mainLoop(void* r);
protected:

	//�¼�����
	std::vector<HANDLE> m_eventArray; 
	//�߳� ���
	HANDLE m_threadHandle = NULL;

	//��Ҫ���ڿͻ��������˷��͵Ĺ�������
	CShareMemory* m_pServerSM;
	//��Ҫ���ڷ������ͻ��˷������ݵĹ�������
	CShareMemory* m_pClientSM;
	//�Ƿ�Ϊ����ˣ�
	bool m_isServer;

	bool m_isAutoSend;

	XLog m_log;
};



#endif  /*_EVENTSIGNAL_eventsignal_H_*/
