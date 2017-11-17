// Sprite2.cpp: Sprite2 クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include "RbRect.h"
#include "VertexDef.h"
#include "Material.h"
#include "RbTransform.h"
#include "DefaultRenderState.h"
#include "Sprite2.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

float Sprite2::Adjuster = -0.5f;
float Sprite2::SetAduster( float New )
{
	return (Adjuster = New);
}

Sprite2::Sprite2()
{
}

Sprite2::~Sprite2()
{
	Restore();
}

// 解除
bool Sprite2::Restore()
{
	MyStock.Restore();
	Render.Restore();
	return true;
}

// デバイス取得
HRESULT Sprite2::GetDevice( DISPDEVICE** ppDevice ) const
{
	if( pDevice == NULL )return E_PENDING;
	if( ppDevice == NULL )return E_POINTER;
	CComPtr<DISPDEVICE> pOutput = pDevice;
	*ppDevice = pOutput.Detach();
	return S_OK;
}

// クリアする
HRESULT Sprite2::Clear()
{
	return ColorClear( D3DCOLOR_RGBA(0,0,0,0) );
}

// クリアする
HRESULT Sprite2::ColorClear( D3DCOLOR ClearColor )
{
	if( pDevice == NULL )return E_PENDING;

	HRESULT ret = pDevice->Clear(0,NULL,D3DCLEAR_TARGET , ClearColor,1.0f,0);
	if( FAILED(ret) )return ret;

	return S_OK;
}

// 描写職設定
ARGBF Sprite2::SetDrawColor( const D3DCOLORVALUE& NewColor )
{
	if( pDevice == NULL )return ARGBF();

	D3DMATERIAL8 NewMate = MyMaterial;
	NewMate.Diffuse = NewColor;
	NewMate.Emissive = NewColor;
	pDevice->SetMaterial( &NewMate );

	return ARGBF(MyMaterial.Diffuse);
}

HRESULT Sprite2::InitSprite( DISPDEVICE* pDevice )
{
	CComPtr<IDirect3DSurface8> pSurface;
	pDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pSurface );
	return _InitSprite( pDevice, pSurface );
}

HRESULT Sprite2::InitSprite( DISPDEVICE* pDevice, const Texture& BackBuffer )
{
	CComPtr<MYSURFACE> pSurface;
	HRESULT ret = BackBuffer.GetSurface( &pSurface );
	if( FAILED(ret) )return ret;

	return InitSprite( pDevice, pSurface );
}

HRESULT Sprite2::InitSprite( DISPDEVICE* pDevice, MYSURFACE* pBackBuffer )
{
	return _InitSprite( pDevice, pBackBuffer );
}

// 真スプライト初期化
HRESULT Sprite2::_InitSprite( DISPDEVICE* pTagetDevice, MYSURFACE* pBackBuffer )
{
	if( pTagetDevice == NULL )return E_POINTER;
	if( pBackBuffer == NULL )return E_POINTER;
	pDevice = pTagetDevice;

	HRESULT ret = Render.Swap( pDevice, pBackBuffer );
	if( FAILED(ret) )return ret;

	const D3DSURFACE_DESC& Desc = Render.RefDesc();
	SpriteSize.x = Desc.Width;
	SpriteSize.y = Desc.Height;
	ScaleX = 1.0f/Desc.Width;
	ScaleY = 1.0f/Desc.Height;

	MyStock.Stock( pDevice );

	RbTransform ProjMat;
	ProjMat.Scale( 2, -2, 1 );
	ProjMat.AddScroll( -1, 1, 0 );
	RbTransform ViewMat;
	RbTransform WorldMat;
	WorldMat.AddScroll( Adjuster, Adjuster );
	WorldMat.Scale( ScaleX, ScaleY, 1 );

	DefaultRenderState Stater( pTagetDevice );
	Stater.SetRenderState();

	pDevice->SetTransform( D3DTS_PROJECTION, &ProjMat );
	pDevice->SetTransform( D3DTS_VIEW, &ViewMat );
	pDevice->SetTransform( D3DTS_WORLD, &WorldMat );
	MyWorld = WorldMat;

	Material NewMate;
	NewMate.Emissive = NewMate.Diffuse;
	MyMaterial = NewMate;
	pDevice->SetMaterial( &NewMate );
	return S_OK;
}

bool Sprite2::Stoker::Stock( DISPDEVICE * pStockDevice )
{
	if( pStockDevice == NULL )return false;
	pDevice = pStockDevice;
	pDevice->GetTransform( D3DTS_PROJECTION, &ProjTrans );
	pDevice->GetTransform( D3DTS_VIEW, &ViewTrans );
	pDevice->GetTransform( D3DTS_WORLD, &WorldTrans );

	pDevice->GetRenderState( D3DRS_ZENABLE, &ZEnable );
	pDevice->GetRenderState( D3DRS_STENCILENABLE, &StencilEnable );
	pDevice->GetRenderState( D3DRS_CULLMODE, &CullMode );
	pDevice->GetRenderState( D3DRS_LIGHTING, &Lighting );
	pDevice->GetRenderState( D3DRS_AMBIENT, &Ambient );
	pDevice->GetRenderState( D3DRS_FOGENABLE, &FogEnable );
	pDevice->GetTextureStageState( 0, D3DTSS_ADDRESSU, &SSU );
	pDevice->GetTextureStageState( 0, D3DTSS_ADDRESSV, &SSV );

	pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	pDevice->SetRenderState( D3DRS_STENCILENABLE, FALSE );
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	pDevice->SetRenderState( D3DRS_AMBIENT, 0 );
	pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_BORDER );
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_BORDER );

	pDevice->GetMaterial( &Mate );
	return true;
}

bool Sprite2::Stoker::Restore()
{
	if( pDevice == NULL )return false;
	pDevice->SetTransform( D3DTS_PROJECTION, &ProjTrans );
	pDevice->SetTransform( D3DTS_VIEW, &ViewTrans );
	pDevice->SetTransform( D3DTS_WORLD, &WorldTrans );

	pDevice->SetRenderState( D3DRS_ZENABLE, ZEnable );
	pDevice->SetRenderState( D3DRS_STENCILENABLE, StencilEnable );
	pDevice->SetRenderState( D3DRS_CULLMODE, CullMode );
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	pDevice->SetRenderState( D3DRS_AMBIENT, Ambient );
	pDevice->SetRenderState( D3DRS_FOGENABLE, FogEnable );
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, SSU );
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, SSV );

	pDevice->SetMaterial( &Mate );
	pDevice.Release();
	return true;
}

HRESULT Sprite2::Draw( const Texture& Tex, float x, float y )
{
	D3DXMATRIX Shift;
	D3DXMatrixTranslation( &Shift, x, y, 0 );
	RbRect Rect( Tex.ImageInfo().Width, Tex.ImageInfo().Height );
	return _Draw( Tex, Shift, Rect );
}

HRESULT Sprite2::Draw( MYTEXTURE* pTexture, float x, float y )
{
	D3DXMATRIX Shift;
	D3DXMatrixTranslation( &Shift, x, y, 0 );
	return Draw( pTexture, Shift );
}

HRESULT Sprite2::Draw( MYTEXTURE* pTexture, float x, float y , const RECT& Rect )
{
	D3DXMATRIX Shift;
	D3DXMatrixTranslation( &Shift, x, y, 0 );
	return Draw( pTexture, Shift, Rect );
}

HRESULT Sprite2::Draw( MYTEXTURE* pTexture, const RECT& Dest )
{
	Texture Tex(pTexture);
	float x = Tex.ImageInfo().Width;
	float y = Tex.ImageInfo().Height;
	if( x <=0 || y <= 0 )return S_FALSE;

	RbTransform Trans;
	Trans.Scale( (Dest.right-Dest.left)/x, (Dest.bottom-Dest.top)/y, 1 );
	Trans.AddScroll( Dest.left, Dest.top );

	return _Draw( Tex, Trans );
}

HRESULT Sprite2::Draw( MYTEXTURE* pTexture, const RECT& Rect, const RECT& Dest )
{
	float x = Rect.right-Rect.left;
	float y = Rect.bottom-Rect.top;
	if( x <=0 || y <= 0 )return S_FALSE;

	RbTransform Trans;
	Trans.Scale( (Dest.right-Dest.left)/x, (Dest.bottom-Dest.top)/y, 1 );
	Trans.AddScroll( Dest.left, Dest.top );

	return Draw( pTexture, Trans, Rect );
}

HRESULT Sprite2::Draw( MYTEXTURE* pTexture, const D3DMATRIX& Trans )
{
	return _Draw( Texture(pTexture), Trans );
}

HRESULT Sprite2::Draw( MYTEXTURE* pTexture, const D3DMATRIX& Trans, const RECT& Rect )
{
	return _Draw( Texture(pTexture), Trans, Rect );
}

HRESULT Sprite2::_Draw( const Texture& Tex, const D3DMATRIX& Trans )
{
	if( pDevice == NULL )return E_PENDING;

	pDevice->MultiplyTransform( D3DTS_WORLD, &Trans );
	HRESULT hr = _Draw( Tex );
	pDevice->SetTransform( D3DTS_WORLD, &MyWorld );
	return hr;
}

HRESULT Sprite2::_Draw( const Texture& Tex, const D3DMATRIX& Trans, const RECT& Rect )
{
	if( pDevice == NULL )return E_PENDING;

	pDevice->MultiplyTransform( D3DTS_WORLD, &Trans );
	HRESULT hr = _Draw( Tex, Rect );
	pDevice->SetTransform( D3DTS_WORLD, &MyWorld );
	return hr;
}

HRESULT Sprite2::_Draw( const Texture& Tex )
{
	float x = Tex.RefDesc().Width;
	float y = Tex.RefDesc().Height;

	TexVertex Ver[4];
	AllZero( Ver );
//	Ver[0].tu = Ver[3].tu = 0;
//	Ver[0].tv = Ver[1].tv = 0;
	Ver[1].tu = Ver[2].tu = 1;
	Ver[2].tv = Ver[3].tv = 1;

//	Ver[0].x = Ver[3].x = 0;
//	Ver[0].y = Ver[1].y = 0;
	Ver[1].x = Ver[2].x = x;
	Ver[2].y = Ver[3].y = y;

	pDevice->SetVertexShader(Ver[0].Def());
	pDevice->SetTexture( 0, Tex );
	return pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Ver, sizeof(Ver[0]) );
}

HRESULT Sprite2::_Draw( const Texture& Tex, const RECT& Rect )
{
	float x = Tex.RefDesc().Width;
	float y = Tex.RefDesc().Height;
	if( x <=0 || y <= 0 )return S_FALSE;

	TexVertex Ver[4];
	AllZero( Ver );
	Ver[0].tu = Ver[3].tu = Rect.left/x;
	Ver[0].tv = Ver[1].tv = Rect.top/y;
	Ver[1].tu = Ver[2].tu = Rect.right/x;
	Ver[2].tv = Ver[3].tv = Rect.bottom/y;

//	Ver[0].x = Ver[3].x = 0;
//	Ver[0].y = Ver[1].y = 0;
	Ver[1].x = Ver[2].x = Rect.right-Rect.left;
	Ver[2].y = Ver[3].y = Rect.bottom-Rect.top;

	pDevice->SetVertexShader(Ver[0].Def());
	pDevice->SetTexture( 0, Tex );
	return pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Ver, sizeof(Ver[0]) );
}
