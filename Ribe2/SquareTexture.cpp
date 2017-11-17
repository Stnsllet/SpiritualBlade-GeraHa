// SquareTexture.cpp: SquareTexture クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbRect.h"
#include "SquareTexture.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SquareTexture::SquareTexture()
{

}

SquareTexture::~SquareTexture()
{

}

HRESULT SquareTexture::LoadSquare( DISPDEVICE* pDevice, LPCTSTR pFileName )
{
	if( !bMaxAlgin )return LoadTexture( pDevice, pFileName );
	Texture PreRead;
	HRESULT hr = PreRead.LoadTexture( pDevice, pFileName );
	if( FAILED( hr ) )return hr;
	UINT Width=PreRead.ImageInfo().Width, Height = PreRead.ImageInfo().Height;

	Algin( pDevice, &Width, &Height, NULL, NULL, D3DPOOL_MANAGED );

	pTexture.Release();
	hr = D3DXCreateTextureFromFileEx( pDevice,
		pFileName,
		Width, Height, 1,
		NULL, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE , D3DX_FILTER_NONE ,
		0x00000000, &TexInfo, NULL,
		&pTexture );
	if( FAILED( hr ) )return hr;
	GetDesc( &DescInfo );
	TexInfo.Width = DescInfo.Width;
	TexInfo.Height = DescInfo.Height;

	return S_OK;
}
