/**
* ������Դ��ϵͳ����3D��Ӳ�����ٻ��������ͳһʹ��DX11��2D������л�ȡ��������ز�֧��Ӳ�����٣�����ö༶�������CPU����
* GDI����Դ�������ڴ湲��ķ�ʽ���༶����������ݱ����������ö༶�������CPU����
* ��Ȩ���� (C) 2014-2015�Ͼ���������Ƽ����޹�˾
* �������� 2015/10/25
* �޶���¼ 2015/10/25  
* �޶���¼ 2015/10/25 ��
* @author��Steven Zhang
* @version V1.0.0.2
* @usage
*/


#ifndef  _CAPTUREFROMSHARETEXDX9_capturefromsharetexdx9_H_
#define _CAPTUREFROMSHARETEXDX9_capturefromsharetexdx9_H_
 
#include <d3d9.h>
#include "DirectX_Jun2010/d3dx9tex.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib, "dxguid.lib")
 
#include <dxgi.h> 
#pragma comment(lib,"dxgi.lib")

#include "ShareMemory.h"
#include "macrodefinition.h"
 
#include "macrodefinition.h"
// Windows ͷ�ļ�: 
#include <windows.h>
#include <Objbase.h>
#include <process.h>

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <Shlobj.h>
#pragma comment(lib, "Shell32.lib")
 
#include "ICapture.h"

#define WINDOW_MSG_USER_HANDLE  WM_APP + 1024

class CaptureFromShareTexDx9:public ICapture,public CaptureCallback
{
public:
	CaptureFromShareTexDx9();
	~CaptureFromShareTexDx9();
public:
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	virtual bool init(CaptureInfo& info);

	virtual void clear();

	virtual bool lock(CaptureCallback* _capture_callback = nullptr);

	virtual IDirect3DSurface9* getRenderData();

	virtual void unlock();

	virtual void* _callback(void* _data);
	
	virtual void save_tex();

	virtual void onHeartStick(CaptureInfo* info);

protected:
	HRESULT DX9Init(CaptureInfo& info,HWND hWnd = NULL);
public:

	CaptureInfo m_capture_info;

	IDirect3D9Ex           *m_pD3DEx;
	IDirect3DDevice9       *m_pD3D9Device;

	

	IDirect3DSurface9*		m_pSharedSurface9;
	IDirect3DSurface9*		m_pD3DOffScreenSurface9;
	IDirect3DTexture9*		m_pD3D9ShareTex;
};

#endif /*_CAPTUREFROMSHARETEXDX9_capturefromsharetexdx9_H_*/