/**
* ICapture.h ץȡ�ӿ��࣬������������Ļ�����ڣ����������ݵ�ץȡ�ӿ���
* ��Ȩ���� (C) 2014-2015�Ͼ���������Ƽ����޹�˾
* �������� 2015/10/25
* �޶���¼ 2015/10/25  
* �޶���¼ 2015/10/25 ��
* @author��Steven Zhang
* @version V1.0.0.2
* @usage
*/


#ifndef _ICAPTURE_icapture_H_
#define _ICAPTURE_icapture_H_
#include <windows.h>

#define MAX_SHORT 1<<14

enum EPIXFORMAT
{
	EPIXFORMAT_R8,
	EPIXFORMAT_A8,
	EPIXFORMAT_A8R8G8B8,
	EPIXFORMAT_A8B8G8R8,
	EPIXFORMAT_X8R8G8B8,
	EPIXFORMAT_X8B8G8R8,
	EPIXFORMAT_R8G8B8,
	EPIXFORMAT_B8G8R8,
	EPIXFORMAT_R8G8B8A8,
	EPIXFORMAT_B8G8R8A8,
	EPIXFORMAT_RGBA16F,
	EPIXFORMAT_R32F,
	EPIXFORMAT_R64F,
	EPIXFORMAT_R32G32B32A32F,
	EPIXFORMAT_A32B32G32R32F,
	EPIXFORMAT_A32R32G32B32F, 
	EPIXFORMAT_RGBA64F
};


//HUD ��λ��
enum EHUDLocation
{
	EHUDLocation_TOP_LEFT = 0
	, EHUDLocation_TOP_RIGHT = 1
	, EHUDLocation_BOTTOM_LEFT = 2
	, EHUDLocation_BOTTOM_RIGHT = 3

};
typedef long long TIMERTICKS;
//��Ƶ�������������ÿ�η���֮��ϵͳ����ݵ�ǰϵͳ�����������м�⣬�����������ִ�г���3��δ�������ԭʼ���̵��ж�
//ץȡ���洰�ڴ�С�仯ʱ���������һ���豸�豸���ã��豸������Ҫ֪ͨ�ͻ��˽��о�����ã����½��д���
enum ECaptureType
{
	CAPTURE_HOOK_INVALID = -2,		//δ��ʼ��
	CAPTURE_HOOK_FAIL = -1,			//ʧ�ܣ��������β���
	CAPTURE_HOOK_SUCCESS,			//�ɹ������ݹ��������ڴ�С�����ظ�ʽ����Ϣ
	CAPTURE_RESET,					//��������������ݴ��ݣ�ͬ���������εĴ��ڴ�С�;����Ϣ---��Ҫ����
	CAPTURE_HEARTBEAT,				//����������������δ������ж�Ŀ������Ѿ�����
	CAPTURE_START_RECORD,			//��������Ϊ�ֶ�¼�Ƶ�ʱ��Ŀ�����ֻ���г������ݷַ����ͻ��˲�������Ƶ¼�ƣ���ʱ����ͻ��˷��Ϳ�ʼ¼�ƣ����ؽ����жϽ�����ز�����������������������-- 
	CAPTURE_STOP_RECORD,			//ֹͣ����
	CAPTURE_PROCESS_EXIT,			//Ŀ������˳�֪ͨ���������exit�����ش���
	TYPE_Test = 100,
	TYPE_Unknown
};

enum ECLIENTSTATUS {
	CLIENT_INVALID = -1,
	CLIENT_HOOK_FALSE,
	CLIENT_HOOK_SUCCESS,
	CLIENT_HOOK_CAPTURE_ING,
	CLIENT_HOOK_CAPTURE_PAUSE,
	CLIENT_HOOK_CAPTURE_STOP
};

enum EVideoServerStatus {
	VIDEO_SERVER_INVALID = -1,	//δ��ʼ��״̬
	VIDEO_READY,				//ϵͳ׼�����״̬
	VIDEO_Client_READY,				//�ͻ���׼�����״̬
	VIDEO_RECORDING,			//ϵͳ¼��״̬
	VIDEO_PAUSE,				//ϵͳ��ͣ
	VIDEO_STOP					//ϵͳֹͣ״̬
};


typedef struct _CaptureInfo
{
	ECaptureType type;
	TIMERTICKS mHeartBeat;			//��ǰ�������ģ�������ʱ����Ĵ���
	BOOL mIsHOOKed;					//�Ƿ�HOOK
	HANDLE mSharedHandle;			//������
	UINT mCaptureId;				//ϵͳΨһID��
	UINT mOffsetX;						//x offset
	UINT mOffsetY;						//y offset
	UINT mWidth;					//������
	UINT mHeight;					//����߶�
	EPIXFORMAT mFormat;					//�������ظ�ʽ,ϵͳ��Ҫ�������ͽ���һ�δ���
	EHUDLocation mHUDLocation;		//HUD ��ʾ��λ��
	DWORD processId;				//����ID
	char pData[256];
	_CaptureInfo()
	{
		type = CAPTURE_HOOK_INVALID;
		mIsHOOKed = FALSE;
		mSharedHandle = NULL;
		mCaptureId = 0;
		mOffsetX = 0;
		mOffsetY = 0;
		mWidth = 800;
		mHeight = 600;
		mFormat = EPIXFORMAT_B8G8R8A8;
		mHUDLocation = EHUDLocation_TOP_LEFT;
		processId = GetCurrentProcessId();
		memset(pData, 0, 256);
	}
}CaptureInfo;



/************************************************************************/
/*
���ػص�������ʵ��ʵ������Ҫ���м̳л��ߵ����Ĺ�������Ҫ�̳иú�����Ϊ�ص�ʵ��
*/
/************************************************************************/
class CaptureCallback {
public:
	virtual void* _callback(void* _data) = 0;
};

/************************************************************************/
/* 
�����ص��ӿ��࣬ͨ���������ʵ��
*/
/************************************************************************/
class ICapture
{
public:
	virtual bool init(CaptureInfo& info) = 0;

	virtual void clear() = 0;

	virtual bool lock(CaptureCallback* _capture_callback = nullptr) = 0;

	virtual void unlock() = 0;

	virtual void save_tex() = 0;

	virtual void onHeartStick(CaptureInfo* info) {};
}; 

struct LOCALSETTING
{
	UINT frameRate = 40;
};



//����win���ھ��
HWND CreateMyWnd();

EPIXFORMAT convert_format_dx9(DWORD format);
DWORD convert_dx9format_sys(EPIXFORMAT format);


EPIXFORMAT convert_format_dxgi(DWORD format);
DWORD convert_dxgiformat_sys(EPIXFORMAT format);

#endif /*_ICAPTURE_icapture_H_*/
