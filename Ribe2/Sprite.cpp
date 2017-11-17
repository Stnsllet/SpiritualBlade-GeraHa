// Sprite.cpp: Sprite クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include "RbRect.h"
#include "Sprite.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

float Sprite::SetAduster( float New )
{
	return false;
}

bool Sprite::Stoker::Stock( DISPDEVICE* pStockDevice )
{
	if( pStockDevice == NULL )return false;
	pDevice = pStockDevice;

	pDevice->GetRenderState( D3DRS_FOGENABLE, &FogEnable );
	pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

	return true;
}

bool Sprite::Stoker::Restore()
{
	if( pDevice == NULL )return false;
	pDevice->SetRenderState( D3DRS_FOGENABLE, FogEnable );
	pDevice.Release();
	return true;
}

Sprite::Sprite()
{
	AllZero(SpriteSize);
	DrawColor = D3DCOLOR_XRGB(255,255,255);
}

Sprite::~Sprite()
{
	Restore();
}

// 解除
bool Sprite::Restore()
{
	if( pSprite != NULL )
	{
		pSprite->End();
	}
	MyStock.Restore();
	Render.Restore();
	pSprite.Release();
	return true;
}

// 描写色設定
ARGBF Sprite::SetDrawColor( const D3DCOLORVALUE& NewColor )
{
	D3DCOLOR OldColor = DrawColor;
	DrawColor = ARGB(NewColor);
	return ARGBF(OldColor);
}

// 真スプライト初期化
HRESULT Sprite::_InitSprite( DISPDEVICE* pDevice, MYSURFACE* pBackBuffer )
{
	if( pDevice == NULL )return E_POINTER;
	if( pBackBuffer == NULL )return E_POINTER;

	HRESULT ret = Render.Swap( pDevice, pBackBuffer );
	if( FAILED(ret) )return ret;

	MyStock.Stock( pDevice );

	const D3DSURFACE_DESC& Desc = Render.RefDesc();
	SpriteSize.x = Desc.Width;
	SpriteSize.y = Desc.Height;

	pSprite.Release();
	ret = D3DXCreateSprite( pDevice, &pSprite );
	if( FAILED(ret) )return ret;

	ret = pSprite->Begin();
	if( FAILED(ret) )return ret;

	return S_OK;
}

// スプライト初期化
HRESULT Sprite::InitSprite( DISPDEVICE* pDevice )
{
	CComPtr<IDirect3DSurface8> pSurface;
	pDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pSurface );
	return _InitSprite( pDevice, pSurface );
}

// バックバッファを指定して	スプライト初期化
HRESULT Sprite::InitSprite( DISPDEVICE* pDevice, const Texture& BackBuffer )
{
	CComPtr<MYSURFACE> pSurface;
	HRESULT ret = BackBuffer.GetSurface( &pSurface );
	if( FAILED(ret) )return ret;

	return InitSprite( pDevice, pSurface );
}

// バックバッファを指定して	スプライト初期化
HRESULT Sprite::InitSprite( DISPDEVICE* pDevice, MYSURFACE* pBackBuffer )
{
	if( pDevice == NULL )return E_POINTER;
	if( pBackBuffer == NULL )return E_POINTER;

	return _InitSprite( pDevice, pBackBuffer );
}

// デバイス取得
HRESULT Sprite::GetDevice( DISPDEVICE** ppDevice ) const
{
	if( pSprite == NULL )return E_PENDING;
	if( ppDevice == NULL )return E_POINTER;
	return pSprite->GetDevice( ppDevice );
}

// クリアする
HRESULT Sprite::Clear()
{
	return ColorClear( D3DCOLOR_RGBA(0,0,0,0) );
}

// クリアする
HRESULT Sprite::ColorClear( D3DCOLOR ClearColor )
{
	CComPtr<DISPDEVICE> pDevice;
	HRESULT ret = GetDevice( &pDevice );
	if( FAILED(ret) )return ret;

	ret = pDevice->Clear(0,NULL,D3DCLEAR_TARGET , ClearColor,1.0f,0);
	if( FAILED(ret) )return ret;

	return S_OK;
}

// 指定した点に描写
HRESULT Sprite::Draw( const Texture& Tex, float x, float y )
{
	RbRect Rect( Tex.ImageInfo().Width, Tex.ImageInfo().Height );
	return Draw( Tex, x, y, Rect );
}

// 指定した点に描写
HRESULT Sprite::Draw( MYTEXTURE* pTexture, float x, float y )
{
	if( pTexture == NULL )return E_POINTER;
	if( pSprite == NULL )return E_PENDING;

	D3DXVECTOR2 Pos( x, y );

	HRESULT ret = pSprite->Draw( pTexture, NULL, NULL, NULL, 0.0f, &Pos, DrawColor );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// 指定したRectに描写
HRESULT Sprite::Draw( MYTEXTURE* pTexture, const RECT& Dest )
{
	if( pTexture == NULL )return E_POINTER;
	if( pSprite == NULL )return E_PENDING;

	Texture Info;
	Info.SetTexture( pTexture );
	RbRect Rect( Info.ImageInfo().Width, Info.ImageInfo().Height );

	return Draw( pTexture, Rect, Dest );
}

// 指定した点に描写
HRESULT Sprite::Draw( MYTEXTURE* pTexture, float x, float y , const RECT& Rect )
{
	if( pTexture == NULL )return E_POINTER;
	if( pSprite == NULL )return E_PENDING;


	D3DXVECTOR2 Pos( x, y );

	HRESULT ret = pSprite->Draw( pTexture, &Rect, NULL, NULL, 0.0f, &Pos, DrawColor );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// 指定したRECTに描写
HRESULT Sprite::Draw( MYTEXTURE* pTexture, const RECT& Rect, const RECT& Dest)
{
	if( pTexture == NULL )return E_POINTER;
	if( pSprite == NULL )return E_PENDING;

	float GrowX=1, GrowY=1;

	int Width = Rect.right - Rect.left;
	if(Width!=0)GrowX = (float)(Dest.right - Dest.left)/Width;
	int Height = Rect.bottom - Rect.top;
	if(Height!=0)GrowY = (float)(Dest.bottom - Dest.top)/Height;


	D3DXVECTOR2 Pos( (float)Dest.left, (float)Dest.top );
	D3DXVECTOR2 Grow( GrowX, GrowY );

	HRESULT ret = pSprite->Draw( pTexture, &Rect, &Grow, NULL, 0.0f, &Pos, DrawColor );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// トランスフォーム描写
HRESULT Sprite::Draw( MYTEXTURE* pTexture, const D3DMATRIX& Trans )
{
	if( pTexture == NULL )return E_POINTER;
	if( pSprite == NULL )return E_PENDING;

	HRESULT ret = pSprite->DrawTransform( pTexture, NULL, (D3DXMATRIX*)&Trans, DrawColor );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// トランスフォーム描写
HRESULT Sprite::Draw( MYTEXTURE* pTexture, const D3DMATRIX& Trans, const RECT& Rect )
{
	if( pTexture == NULL )return E_POINTER;
	if( pSprite == NULL )return E_PENDING;

	HRESULT ret = pSprite->DrawTransform( pTexture, &Rect, (D3DXMATRIX*)&Trans, DrawColor );
	if( FAILED(ret) )return ret;

	return S_OK;
}
