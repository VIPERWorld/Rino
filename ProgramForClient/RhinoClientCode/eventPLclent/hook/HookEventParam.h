#ifndef _HookEventParam_H_
#define _HookEventParam_H_

#include <Windows.h>

enum EnEventID
{
	E_EVENT_ID_HS_HOOK_SUCCESS = 1,
	E_EVENT_ID_HS_HOOK_HEARTBEAT,
	
	E_EVENT_ID_HS_HOOK_AM_ReqAddFriend,
	E_EVENT_ID_HS_HOOK_AM_AcceptAddFriend,
	E_EVENT_ID_HS_HOOK_AM_ReqChallenge,
	E_EVENT_ID_HS_HOOK_AM_AcceptChallenge
};


#include "ICapture.h"

struct ParamHSCapture
{
	long long tick;				// ����ʱ���
	bool isHooked;				// �Ƿ�HOOK
	HANDLE sharedHandle;		// ������
	UINT captureId;				// ϵͳΨһID��
	UINT offsetX;				// x offset
	UINT offsetY;				// y offset
	UINT width;					// ������
	UINT height;				// ����߶�
	EPIXFORMAT format;			// �������ظ�ʽ,ϵͳ��Ҫ�������ͽ���һ�δ���
	int HUDLocation;			// HUD ��ʾ��λ��
	DWORD processId;			// ����ID
	char data[256];
};


struct EventParamHS
{
	EnEventID id;				// ��ϢID
	
	union 
	{
		ParamHSCapture captInfo;
	}data;
};

struct ShareParamHS
{
	int wsPort_Srv;
	int wsPort_Hook;

	char urlMatch[256];
	char urlCover[256];

	ShareParamHS()
	{
		wsPort_Srv = 0;
		wsPort_Hook = 0;

		memset(urlMatch, 0, 256);
		memset(urlCover, 0, 256);
	}
};


#endif // !_HOOKEVENTPARAM_H_
