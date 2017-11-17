// TexRender.cpp: TexRender クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TexRender.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TexRender::TexRender()
{
	AllZero(SurfaceDesc);
	bChangeScene = false;
}

TexRender::~TexRender()
{
	Restore();
}

HRESULT TexRender::Restore()
{
	if( pRender == NULL )return S_FALSE;
	pRender->EndScene();
	if( bChangeScene )
	{
		CComPtr<DISPDEVICE> pDevice;
		pRender->GetDevice( &pDevice );
		if( pDevice != NULL )pDevice->BeginScene();
		bChangeScene = false;
	}
	AllZero(SurfaceDesc);
	pRender.Release();
	return S_OK;
}

HRESULT TexRender::Swap( DISPDEVICE* pDevice, MYSURFACE* pTaget, D3DFORMAT DepthStencilFormat )
{
	if( pDevice == NULL )return E_POINTER;
	if( pTaget == NULL )return E_POINTER;
	Restore();

	pTaget->GetDesc( &SurfaceDesc );

	BOOL UseStencil = (DepthStencilFormat==D3DFMT_UNKNOWN)?(FALSE):(TRUE);

	HRESULT ret = D3DXCreateRenderToSurface( pDevice,
		SurfaceDesc.Width, SurfaceDesc.Height, SurfaceDesc.Format,
		UseStencil, DepthStencilFormat, &pRender );
	if( FAILED(ret) )if( FAILED(ret) )return ret;
	if( pRender == NULL )return E_FAIL;

	bChangeScene = SUCCEEDED( pDevice->EndScene() );

	ret = pRender->BeginScene( pTaget, NULL );
	if( FAILED(ret) )
	{
		pDevice->ResourceManagerDiscardBytes( 0 );
		ret = pRender->BeginScene( pTaget, NULL );
		if( FAILED(ret) )return ret;
	}

	return S_OK;
}
