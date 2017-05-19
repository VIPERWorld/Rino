/*Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved*/
#include "stdafx.h"
#include "BKWnd.h"

//
BOOL LoadImageFromResource(CImage *pImage, UINT nResID,LPCTSTR lpTyp)
{
	if( pImage ==NULL)return false;

	pImage->Destroy();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// ������Դ
	HRSRC hRsrc =::FindResource(nullptr/*AfxGetResourceHandle()*/, MAKEINTRESOURCE(nResID), lpTyp);
	if(hRsrc ==NULL)return false;

	// ������Դ
	HGLOBAL hImgData =::LoadResource(nullptr/*AfxGetResourceHandle()*/, hRsrc);
	if(hImgData ==NULL)
	{
		::FreeResource(hImgData);
		return false;
	}

	// �����ڴ��е�ָ����Դ
	LPVOID lpVoid    =::LockResource(hImgData);

	LPSTREAM pStream =NULL;
	DWORD dwSize    =::SizeofResource(nullptr/*AfxGetResourceHandle()*/, hRsrc);
	HGLOBAL hNew    =::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte    =(LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// ����ڴ��е�ָ����Դ
	::GlobalUnlock(hNew);

	// ��ָ���ڴ洴��������
	HRESULT ht =::CreateStreamOnHGlobal(hNew,TRUE,&pStream);
	if( ht != S_OK )
	{
		GlobalFree(hNew);
	}
	else
	{
		// ����ͼƬ
		pImage->Load(pStream);

		GlobalFree(hNew);
	}
	// �ͷ���Դ
	::FreeResource(hImgData);
	return true;
}

CBKWnd::CBKWnd()
{
}

CBKWnd::~CBKWnd()
{
}

LRESULT CBKWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_EXSTYLE);
	styleValue |= WS_EX_LAYERED;
	::SetWindowLong(*this, GWL_EXSTYLE, styleValue);

	CImage img;
	bool bres = LoadImageFromResource(&img, IDB_PNG1, TEXT("PNG"));
	if (!bres)
	{
		return 0;
	}
	SIZE sz;//ͼƬ��С
	sz.cx = img.GetWidth();
	sz.cy = img.GetHeight();

	SetWindowPos(m_hWnd, NULL, 0, 0, sz.cx, sz.cy, SWP_NOREDRAW);//�����ڴ�С����ΪͼƬ��Сʹ֮�໥����

	HDC hdc = GetDC(m_hWnd);//��ȡ�����豸���
	HDC hdcMem = CreateCompatibleDC(hdc);//����һ����hdc����ݵ��ڴ��豸���
	HBITMAP    hBitmap = CreateCompatibleBitmap(hdc, sz.cx, sz.cy);
	SelectObject(hdcMem,(HGDIOBJ)hBitmap);
	img.Draw(hdcMem, 0, 0, sz.cx, sz.cy, 0, 0, sz.cx, sz.cy);

	POINT pt;
	pt.x = 0;
	pt.y = 0;

	BLENDFUNCTION  bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;

	UpdateLayeredWindow(m_hWnd, hdc, NULL, &sz, hdcMem, &pt, NULL, &bf, ULW_ALPHA);
	ReleaseDC(m_hWnd, hdc);

	return 0/*WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled)*/;
}

LRESULT CBKWnd::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = TRUE;
	//ExitProcess(0);
	return 0;
}