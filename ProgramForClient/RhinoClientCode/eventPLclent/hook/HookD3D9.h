/**
* D3D9 hookde�Ĺ�����
* ��Ȩ���� (C) 2014-2015�Ͼ���������Ƽ����޹�˾
* �������� 2015/10/25
* �޶���¼ 2015/10/25 �´�������Ӷ���D3D9��ָ��API�󶨣����İ�present �� resset����.
* �޶���¼ 2015/10/25 ��
* @author��Steven Zhang
* @version V1.0.0.2
* @usage
*/


#ifndef _HOOK_D3D9_H_
#define _HOOK_D3D9_H_

 
#include "ntstatus.h"
 
#include "EventSignal.h"
#include "macrodefinition.h"
//#pragma comment(lib,"EasyHook32.lib")
#include "ntstatus.h"
#include <d3d9.h>

#include "DirectX_Jun2010/d3dx9tex.h"
#include "macrodefinition.h"
// Windows ͷ�ļ�: 
#include <windows.h>
#include <Objbase.h>

#include <process.h>

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <Shlobj.h>
#pragma comment(lib, "Shell32.lib")

//#define Z_CAPTURE_LIB_


#include <tchar.h>

#include "detver.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib, "dxguid.lib")

#include "Dx9GPUPatch.h"

#include "ICapture.h"

#define WINDOW_MSG_USER_HANDLE  WM_APP + 1024




union MyUnion
{
	struct{
		short mWidth;
		short mHeight;
	}mSParam;
	int mParam;
};

typedef VOID(_stdcall *ExitProcessFunPtr)(_In_ UINT uExitCode);

typedef HRESULT(_stdcall *PresentFunPtr)(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);

typedef HRESULT(_stdcall *ResetFunPtr)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

/**
* @date: 2015/10/25
* @author: Steven
* @version: V1.0.0.1
* @return: 0���ɹ�  1��ʧ��
* @usage: HookPresent
* ��Ҫ��������ú�����ָ��λ��������ԭ��ַ,�µ�HookPresent��ִ�е�ַ�������ͱ�׼����һ��
**/
HRESULT _stdcall HookPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);


/**
* @date: 2015/10/25
* @author: Steven
* @version: V1.0.0.1
* @return: 0���ɹ�  1��ʧ��
* @usage: HookPresent
* ��Ҫ��������ú�����ָ��λ��������ԭ��ַ,�µ�HookReset��ִ�е�ַ�����豸��ʧ֮������������ݲ����ͱ�׼����һ��
**/
HRESULT _stdcall HookReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

void _stdcall HookExitProcessFun(UINT uExitCode);

class CHookD3D9 :public PatchCallback,public IDataRecive
{
protected:
	CHookD3D9();
	~CHookD3D9();
	public:
		static CHookD3D9* getInstance() { return ms_pHookD3D9; }
public:
	/**
	* @date: 2015/10/25
	* @author: Steven
	* @version: V1.0.0.1
	* @return: 0���ɹ�  1��ʧ��
	* @usage: ��Ҫ��������ú�����ָ��λ��������ԭ��ַ
	**/

	static BOOL PrepareRealApiEntry();

 
	/**
	* @date: 2015/10/25
	* @author: Steven
	* @version: V1.0.0.1
	* @return: 0���ɹ�  1��ʧ��
	* @usage: HookPresent
	* ��Ҫ��������ú�����ָ��λ��������ԭ��ַ,�µ�HookReset��ִ�е�ַ�����豸��ʧ֮������������ݲ����ͱ�׼����һ��
	**/
	static BOOL DoHook();


	/**
	* @date: 2015/10/25
	* @author: Steven
	* @version: V1.0.0.1
	* @return:  void
	* @usage: HookPresent
	*	//dll �˳�֮�󣬵��øú���������Դ�ͷ�
	**/

	static void DoneHook();

	static void OnTimer(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime);

	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   DoData  �ص�����Ľӿڣ������ڴ��ж�ȡ���ݽ��д���ͻص��Ľӿں���
	* @version: V1.0.0.1
	* @param:
	* @return:  bool
	**/
	virtual void DoData(void* pData, size_t size);
	/**
	* @date: 2016/8/2
	* @author: Steven
	* @brief:   SendData   Ĭ��ִ�к����ĺ���Լ��
	* @version: V1.0.0.1
	* @param:
	* @return:  bool
	**/
	virtual void SendData(void* pData, size_t maxSize);

	

	BOOL InitD3D9(unsigned int deviceID = 0);


	BOOL CreateShareTexture();

	void Clear(); 


	CaptureInfo& getD3D9Param(){ return m_captureInfo; } 

	IDirect3DDevice9* getD3D9Device(){ return m_pD3D9Device; }
	IDirect3DSurface9* getD3D9Surface9(){ return m_pD3D9Surface9Src; }
	IDirect3DTexture9* getD3D9Tex9(){ return pD3DTex9; }
	
	IDirect3DSurface9* getD3D9OffSurface9(){ return m_pD3DOffScreenSurface9; }



public:

	//�洢ԭ�ȵĵ�ַ
	static PresentFunPtr ms_pD3D9OldPresentFun;
	static ResetFunPtr ms_pD3D9OldResetFun;
	static ExitProcessFunPtr ms_exitProcessFun;

	static CHookD3D9*  ms_pHookD3D9;


protected:
	 
	//��ǰ��ַ
	//static NTSTATUS				ms_statue;
	//
	static ULONG                   ms_HookPresent_ACLEntries[1];
	static ULONG                   ms_HookReset_ACLEntries[1];

	//��������̣������Դ�����ڣ��򴴽�������������ݿ���
	void MainHook(LPDIRECT3DDEVICE9 pDevice);
	//���Ժ���
	void Test();


	virtual void CallbackPatch(void* _data);

public:
	HANDLE m_eventId_HookEnd;


	//��������backsurface��surface��
	//�����ݿ��ܻᱻ���ú���Ҫ���³�ʼ��
	IDirect3DSurface9 *		m_pSharedSurface9;
	IDirect3DTexture9*		m_pD3D9ShareTex;

	D3DSURFACE_DESC			m_d3dsurface_desc; 

	IDirect3DSurface9*	m_pD3D9Surface9Src = NULL;
	IDirect3DSurface9*	m_pD3DOffScreenSurface9 = NULL;
	IDirect3DTexture9*  pD3DTex9 = NULL;

	IDirect3D9Ex           *m_pD3DEx;
	IDirect3DDevice9       *m_pD3D9Device;

	//ϵͳ֪ͨ�¼�,�ͻ���
	//EventSignal m_client_signal;

	CaptureInfo m_captureInfo;


	ECLIENTSTATUS m_clientStatus;

	bool m_captureInit = false;

	friend HRESULT _stdcall HookPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	friend HRESULT _stdcall HookReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
};

#endif /*_HOOK_D3D9_H_*/