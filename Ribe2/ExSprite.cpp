// ExSprite.cpp: ExSprite クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include "RbCache.h"
#include "ExSprite.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

const ARGBF ExSprite::BaseColor = ARGBF(1,0.9375f,0.9375f,0.9375f);
bool ExSprite::bUseExMode = false;
D3DFORMAT ExSprite::RenderFormat = D3DFMT_UNKNOWN;

ExSprite::ExSprite()
{

}

ExSprite::~ExSprite()
{
	Restore();
}

// 解除
bool ExSprite::Restore()
{
	RbSprite::Restore();
	Flip();
	pRenderTaget.Release();
	return true;
}

// Exモードで実行
bool ExSprite::SetEx( D3DFORMAT Format )
{
//	if( !Is3D() )return false;
	bUseExMode = true;
	RenderFormat = Format;
	return bUseExMode;
}

// ちょっと暗くする
ARGBF ExSprite::SetDrawColor( const D3DCOLORVALUE& NewColor )
{
	if( !bUseExMode )return RbSprite::SetDrawColor( NewColor );
	ARGBF Ret = RbSprite::SetDrawColor( NewColor );
	RbSprite::SetDrawColor( ARGBF(NewColor)*=BaseColor );

	return Ret;
}

// クリアする
HRESULT ExSprite::Clear()
{
	if( !bUseExMode )return RbSprite::Clear();
	if( pRenderTaget == NULL )return RbSprite::Clear();

	ClearFromTex( pBaseSurface );
/*	CComPtr<DISPDEVICE> pDevice;
	GetDevice(&pDevice);
	if( pDevice != NULL )
	{
		TexRender Swapper;
		if( !FAILED(Swapper.Swap( pDevice, pBaseSurface )) )
		{
			ColorClear( D3DCOLOR_RGBA(0,0,0,0) );
		}
	}
*/
	return ColorClear( D3DCOLOR_RGBA(255,255,255,255) );
}

HRESULT ExSprite::_InitSprite( DISPDEVICE* pTagetDevice, MYSURFACE* pBackBuffer )
{
	if( !bUseExMode )return RbSprite::_InitSprite( pTagetDevice, pBackBuffer );

	pBaseSurface = pBackBuffer;
	HRESULT hr = CreateRenderTaget( pTagetDevice, pBackBuffer );
	if( FAILED(hr) )return hr;
	if( pRenderTaget==NULL )return E_FAIL;

	CComPtr<MYSURFACE> pSurface;
	pRenderTaget->GetSurfaceLevel( 0, &pSurface );

	hr = RbSprite::_InitSprite( pTagetDevice, pSurface );
	if( FAILED(hr) )
	{
		pRenderTaget.Release();
		return hr;
	}

	SetDrawColor( ARGBF(1,1,1,1) );
	return hr;
}

HRESULT ExSprite::CreateRenderTaget( DISPDEVICE* pDevice, MYSURFACE* pBaseSurface )
{
	if( pDevice == NULL )return E_POINTER;
	if( pBaseSurface == NULL )return S_FALSE;
	D3DSURFACE_DESC Desc;
	pBaseSurface->GetDesc( &Desc );

	Texture NewTexture;
	HRESULT hr = NewTexture.CreateDefault( pDevice, Desc.Width, Desc.Height );
	if( FAILED(hr) )
	{
		pDevice->ResourceManagerDiscardBytes( 0 );
		hr = NewTexture.CreateDefault( pDevice, Desc.Width, Desc.Height );
		if( FAILED(hr) )return hr;
	}
	pRenderTaget = NewTexture;
	return S_OK;
}

HRESULT ExSprite::Flip()
{
	if( pRenderTaget == NULL )return S_FALSE;
	CComPtr<MYSURFACE> pSurface;
	HRESULT hr =  pRenderTaget->GetSurfaceLevel( 0, &pSurface );
	if( FAILED(hr) )return hr;

	hr = D3DXLoadSurfaceFromSurface( pBaseSurface, NULL, NULL,
		pSurface, NULL, NULL,
		D3DX_FILTER_NONE, D3DCOLOR_RGBA(255,255,255,255) );
	FAILEDTRACE(hr);
	return hr;
}

HRESULT ExSprite::ClearFromTex( MYSURFACE* pTagetSurface ) const
{
	Plane<> pClear = GetCache().PathTex.Get( _T("Tex/System/Clear.png") );
	if( pClear.pTexture == NULL )return E_FAIL;

	CComPtr<MYSURFACE> pSurface;
	HRESULT hr =  pClear.pTexture->GetSurfaceLevel( 0, &pSurface );
	if( FAILED(hr) )return hr;

	hr = D3DXLoadSurfaceFromSurface( pTagetSurface, NULL, NULL,
		pSurface, NULL, NULL,
		D3DX_FILTER_NONE, 0 );
	FAILEDTRACE(hr);
	return hr;
}
