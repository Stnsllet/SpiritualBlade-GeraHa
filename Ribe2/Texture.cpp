// Texture.cpp: Texture クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

D3DFORMAT Texture::RenderFormat = D3DFMT_A8R8G8B8;
D3DFORMAT Texture::DefaultFormat = D3DFMT_A8R8G8B8;
bool Texture::bUseManaged = false;
bool Texture::bMaxAlgin = false;

void Texture::SetUseFormat( D3DFORMAT Default, D3DFORMAT Render )
{
	DefaultFormat = Default;
	if( Render == D3DFMT_UNKNOWN )Render = Default;
	RenderFormat = Render;
	bUseManaged = ( RenderFormat!=DefaultFormat );
}

bool Texture::SetUseManaged( bool bManaged )
{
	return (bUseManaged=bManaged);
}

bool Texture::Algin( DISPDEVICE* pDevice, UINT* pWidth, UINT* pHeight, DWORD Usage, D3DFORMAT* pFormat, D3DPOOL Pool )
{
	if( pDevice == NULL )return false;
	if( pWidth == NULL || pHeight == NULL )return false;
	ATLTRACE("Algin Before Size %d %d\n", *pWidth, *pHeight );

	UINT Width = *pWidth, Height = *pHeight;
	if( bMaxAlgin )
		*pWidth = *pHeight = max(*pWidth,*pHeight);


	HRESULT hr = D3DXCheckTextureRequirements( pDevice, pWidth, pHeight, NULL, Usage, pFormat, Pool );
	if( FAILED(hr) )return false;
	if( *pWidth < Width )(*pWidth) *= 2;
	if( *pHeight < Height )(*pHeight) *= 2;
	ATLTRACE("Algin After Size %d %d\n", *pWidth, *pHeight );

	return true;
}

bool Texture::Algin( DISPDEVICE* pDevice, UINT* pWidth, UINT* pHeight )
{
	D3DFORMAT Format = RenderFormat;
	D3DPOOL Pool = D3DPOOL_DEFAULT;
	DWORD Usage = D3DUSAGE_RENDERTARGET;
	if( bUseManaged )
	{
		Format = DefaultFormat;
		Pool = D3DPOOL_MANAGED;
		Usage = NULL;
	}
	return Algin( pDevice, pWidth, pHeight, Usage, &Format, D3DPOOL_MANAGED );
}

bool Texture::CheckAlgin( DISPDEVICE* pDevice )
{
	D3DCAPS8 Caps;
	AllZero( Caps );
	pDevice->GetDeviceCaps( &Caps );
	if( 0==(Caps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) )return false;
	return SetAlgin( true );
}

bool Texture::SetAlgin( bool bAlgin )
{
	return (bMaxAlgin = bAlgin);
}

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::Release()
{
	pTexture.Release();
}

HRESULT Texture::GetSurface( MYSURFACE** ppSurface ) const
{
	if( pTexture == NULL )return E_PENDING;
	return pTexture->GetSurfaceLevel( 0, ppSurface );
}

HRESULT Texture::GetDesc( D3DSURFACE_DESC* pDesc ) const
{
	CComPtr<MYSURFACE> pSurface;
	HRESULT hr = GetSurface( &pSurface );
	if( pSurface == NULL )return hr;
	hr = pSurface->GetDesc( pDesc );
	if( FAILED(hr) )return hr;
	return S_OK;
}

// テクスチャをファイルからロード
HRESULT Texture::LoadTexture( DISPDEVICE* pDevice, LPCTSTR pFileName )
{
	if( pDevice == NULL )return E_POINTER;
	if( pFileName == NULL )return E_POINTER;

	AllZero( TexInfo );
	pTexture.Release();

//	HRESULT ret = D3DXCreateTextureFromFile( pDevice, pFileName, &pTexture );
//	if( FAILED(ret) )return ret;

	HRESULT ret = D3DXCreateTextureFromFileEx( pDevice,
		pFileName,
		0, 0, 1,
		NULL, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT,
		0x00000000, &TexInfo, NULL,
		&pTexture );
	if( FAILED(ret) )return ret;
	GetDesc( &DescInfo );

	return S_OK;
}

// 何もなしテクスチャ生成
HRESULT Texture::Create( DISPDEVICE* pDevice, int Width, int Height )
{
	if( Texture::bUseManaged )return CreateDefault( pDevice, Width, Height );
	return  CreateRenderTaget( pDevice, Width, Height );
}

// 標準テクスチャ作成
HRESULT Texture::CreateDefault( DISPDEVICE* pDevice, int Width, int Height )
{
	return CreateTexture( pDevice, Width, Height, DefaultFormat, NULL, D3DPOOL_MANAGED );
}

// 描写先生成
HRESULT Texture::CreateRenderTaget( DISPDEVICE* pDevice, int Width, int Height )
{
	return CreateTexture( pDevice, Width, Height, RenderFormat, D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT );
}

// テクスチャを作成
HRESULT Texture::CreateTexture( DISPDEVICE* pDevice, int Width, int Height, D3DFORMAT Format, DWORD Usage, D3DPOOL Memory, long MipLevel )
{
	if( pDevice == NULL )return E_POINTER;

	AllZero( TexInfo );
	pTexture.Release();

	ATLTRACE("CreateTexture Size %d %d\n", Width, Height );
	HRESULT ret = pDevice->CreateTexture( Width, Height, MipLevel,
		Usage, Format, Memory, &pTexture );
	if( ret == D3DERR_OUTOFVIDEOMEMORY || ret == E_OUTOFMEMORY )
	{
		pDevice->ResourceManagerDiscardBytes( 0 );
		ret = pDevice->CreateTexture( Width, Height, MipLevel,
			Usage, Format, Memory, &pTexture );
	}

	if( FAILED(ret) )
	{
		pTexture.Release();
		UINT NewW = Width, NewH = Height;
		Algin( pDevice, &NewW, &NewH, Usage, &Format, Memory );
		// パラメータ修正機能付き呼び出し
		ret = D3DXCreateTexture( pDevice, NewW, NewH, MipLevel,
			Usage, Format, Memory, &pTexture );
	}
	if( FAILED(ret) )return ret;

	TexInfo.Width = Width;
	TexInfo.Height = Height;
	TexInfo.Format = Format;
	GetDesc( &DescInfo );

	return S_OK;
}

// テクスチャを設定
HRESULT Texture::SetTexture( MYTEXTURE* pNewTexture )
{
	if( pNewTexture == NULL )return E_POINTER;
	pTexture = pNewTexture;

	AllZero( TexInfo );
	GetDesc( &DescInfo );

	TexInfo.Width = DescInfo.Width;
	TexInfo.Height = DescInfo.Height;
	TexInfo.Format = DescInfo.Format;

	return S_OK;
}
