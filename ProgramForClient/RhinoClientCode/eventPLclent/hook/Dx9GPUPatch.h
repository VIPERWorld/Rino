// Dx9GPUPatch.h
/************************************************************************************
**�� �� ����Dx9GPUPatch.h
**��    �ߣ�����
**����ʱ�䣺2015-09-24
**�� �� ���
**��    ����
************************************************************************************/
#ifndef ZXD_DX9GPUPATCH_H_20150924_
#define ZXD_DX9GPUPATCH_H_20150924_
#include <windows.h>
 
#include <Objbase.h>

#include <process.h>

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <Shlobj.h>
#pragma comment(lib, "Shell32.lib")

//#define Z_CAPTURE_LIB_ 

#include "detver.h"
#include "detours.h"
#pragma comment(lib, "detours.lib")

#include <iostream>
#include <string.h>
using namespace std;


#ifdef _WIN64
typedef unsigned __int64 UPARAM;
#else
typedef unsigned long UPARAM;
#endif
 

int GetD3D9PatchType(); // 0 ��Ч
//��ѯ����D3Doffsetλ��
void FindD3D9ExOffsets(IDirect3D9Ex *d3d9ex, IDirect3DDevice9Ex *dev);

class PatchCallback{
public:
	virtual void CallbackPatch(void* _data) {};

};
class D3D9Patch
{
public:
	D3D9Patch();
	~D3D9Patch();

public:
	BOOL Patch(IDirect3DDevice9 *device, PatchCallback* _callback);
protected:

private:
	static BOOL m_bIsPatched;
	DWORD m_dwOldProtect;
	BYTE* m_pSavePatch;
};

#endif
