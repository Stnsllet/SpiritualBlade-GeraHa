// DDObject.cpp: DDObject クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <ddraw.h>
#include "DDObject.h"

#pragma comment(lib,"ddraw.lib")

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DDObject::DDObject()
{

}

DDObject::~DDObject()
{

}

HRESULT DDObject::CreateDirectDraw()
{
	CComPtr<IDirectDraw> pDDraw1;
	HRESULT ret = DirectDrawCreate(NULL,&pDDraw1,NULL);
	if( FAILED(ret) )return ret;

	pDDraw.Release();
	return pDDraw1->QueryInterface(IID_IDirectDraw2,(LPVOID *)&pDDraw);
}

HRESULT DDObject::GetVideoMemory( DWORD& dwFree, DWORD& dwAll )
{
	if( pDDraw == NULL )if( FAILED(CreateDirectDraw()) )return E_FAIL;
	_DDSCAPS ddsCaps; 
	AllZero( ddsCaps );
	ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN; 
	return pDDraw->GetAvailableVidMem(&ddsCaps,&dwAll,&dwFree);
}
