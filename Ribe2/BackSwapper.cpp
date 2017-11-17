// BackSwapper.cpp: BackSwapper クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BackSwapper.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BackSwapper::BackSwapper()
{

}

BackSwapper::~BackSwapper()
{
	Restore();
}

HRESULT BackSwapper::Restore()
{
	if( pDevice == NULL )return E_PENDING;

	HRESULT ret = pDevice->SetRenderTarget( pOldBackBuffer, pOldZBuffer );
	if( FAILED(ret) )return ret;

	pDevice.Release();
	pOldBackBuffer.Release();
	pOldZBuffer.Release();
	return S_OK;
}

HRESULT BackSwapper::Swap( DISPDEVICE* pSetDevice, MYSURFACE* pNewBackBuffer, MYSURFACE* pNewZBuffer )
{
	if( pSetDevice == NULL )return E_POINTER;
	if( pNewBackBuffer == NULL )return E_POINTER;

	if( pDevice != NULL )Restore();

	pOldBackBuffer.Release();
	HRESULT ret = pSetDevice->GetRenderTarget( &pOldBackBuffer );
	if( FAILED(ret) )return ret;

	pOldZBuffer.Release();
	ret = pSetDevice->GetDepthStencilSurface( &pOldZBuffer );
//	if( FAILED(ret) )return ret;	// 設定されてないこともある

	pDevice = pSetDevice;
	ret = pSetDevice->SetRenderTarget( pNewBackBuffer, pNewZBuffer );
	if( FAILED(ret) )return ret;

	return S_OK;
}

HRESULT BackSwapper::Swap( const BackSwapper& OldSwapper )
{
	CComPtr<DISPDEVICE> pNewDevice = OldSwapper.pDevice;
	CComPtr<MYSURFACE> pNewBackBuffer = OldSwapper.pOldBackBuffer;
	CComPtr<MYSURFACE> pNewZBuffer = OldSwapper.pOldZBuffer;

	return Swap( pNewDevice, pNewBackBuffer, pNewZBuffer );
}
