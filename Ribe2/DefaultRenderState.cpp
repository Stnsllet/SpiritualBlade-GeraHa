// DefaultRenderState.cpp: DefaultRenderState クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DefaultRenderState.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DefaultRenderState::DefaultRenderState( DISPDEVICE * pDev )
: pDevice( pDev )
{

}

DefaultRenderState::~DefaultRenderState()
{

}

HRESULT DefaultRenderState::SetRenderState()
{
	if( pDevice == NULL )return E_PENDING;
	// レンダリング・ステートを設定
	// Z バッファ有効化
//	pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	// 裏も書く
	pDevice->SetRenderState( D3DRS_CULLMODE ,D3DCULL_NONE );

	// アルファブレンディング方法を設定
	SetAlphaType( D3DTOP_MODULATE );
	// テクスチャの色を使用
	pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	// 頂点の色を使用
	pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

	// レンダリング時の色の計算方法の設定
	pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	// テクスチャの色を使用
	pDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	// 頂点の色を使用
	pDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_CURRENT);
//	pDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

	// フィルタ設定
	pDevice->SetTextureStageState(0,D3DTSS_MIPFILTER,D3DTEXF_LINEAR);
	pDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	pDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);

	// アルファが一定以下ならZバッファを更新しない
	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	pDevice->SetRenderState( D3DRS_ALPHAREF,		0x10 );
	pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	return S_OK;
}

DWORD DefaultRenderState::SetAlphaType( DWORD Type, long Stage )
{
	if( pDevice == NULL )return D3DTOP_MODULATE;
	// アルファブレンディング方法を設定
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// レンダリング時のアルファ値の計算方法の設定

	DWORD OldType = D3DTOP_MODULATE;

	pDevice->GetTextureStageState( Stage, D3DTSS_ALPHAOP, &OldType );
	pDevice->SetTextureStageState( Stage, D3DTSS_ALPHAOP, Type );
	return OldType;
}

bool DefaultRenderState::SetFog( float Min, float Max, D3DCOLOR BackColor )
{
//	bool bTest = 0!=(DeviceHalCaps.RasterCaps & D3DPRASTERCAPS_FOGRANGE);
//	bool bTest1 = 0!=(DeviceHalCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE);
//	bool bTest2 = 0!=(DeviceHalCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX);
//	bool bTest3 = 0!=(DeviceHalCaps.RasterCaps & D3DPRASTERCAPS_ZFOG);
//	bool bTest4 = 0!=(DeviceHalCaps.RasterCaps & D3DPRASTERCAPS_WFOG);
	if( pDevice == NULL )return false;
	pDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
	pDevice->SetRenderState( D3DRS_FOGCOLOR, BackColor );

	pDevice->SetRenderState( D3DRS_FOGSTART, *((DWORD *)(&Min)) );
	pDevice->SetRenderState( D3DRS_FOGEND, *((DWORD *)(&Max)) );
//	static float FogDensity = 1.0f;
//	pDevice->SetRenderState( D3DRS_FOGDENSITY, *((DWORD *)(&FogDensity)) );

	pDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE );
	pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR );
	pDevice->SetRenderState( D3DRS_RANGEFOGENABLE, FALSE );
	return true;
}
