// D3DObject.cpp: D3DObject クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D3DObject.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

D3DObject::D3DObject()
{
	AllZero( NowDispMode );
	SoftDraw = SoftVertex = false;
}

D3DObject::~D3DObject()
{
}

void D3DObject::SetEmurationMode( bool Draw, bool Vertex )
{
	SoftDraw = Draw;
	SoftVertex = Vertex;
}

USEDIRECT3D* D3DObject::GetD3D() const
{
	return pDirect3D;
}

// セット
bool D3DObject::SetObject( USEDIRECT3D* pNewObject )
{
	if( pNewObject == NULL )return false;
	pDirect3D = pNewObject;
	pDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT,&NowDispMode );
	return true;
}

// デバイスの作成
HRESULT D3DObject::CreateDevice( DISPDEVICE** ppDevice, HWND hWnd, bool FullScreen)
{
	if( pDirect3D == NULL )return E_PENDING;
	if( ppDevice == NULL )return E_POINTER;

	AllZero( InitParams );

	InitParams.BackBufferFormat = DisplayMode().Format;
//	InitParams.SwapEffect = D3DSWAPEFFECT_FLIP;
	InitParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	InitParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	InitParams.hDeviceWindow = hWnd;

	if( FullScreen )
	{
		InitParams.Windowed = 0;
		InitParams.BackBufferWidth = NowDispMode.Width;
		InitParams.BackBufferHeight = NowDispMode.Height;
	}
	else InitParams.Windowed = 1;

	InitParams.BackBufferCount = 1;
	// Z バッファの自動作成
	InitParams.EnableAutoDepthStencil = 1;
	InitParams.AutoDepthStencilFormat = GetDepthStencilFormat();
//	InitParams.AutoDepthStencilFormat = D3DFMT_D16;

	return CreateDevice( hWnd, InitParams, ppDevice );
}

// デバイスの作成
HRESULT D3DObject::CreateDevice( HWND hWnd, D3DPRESENT_PARAMETERS& InitParams, DISPDEVICE** ppDevice )
{
	if( pDirect3D == NULL )return E_PENDING;
	if( ppDevice == NULL )return E_POINTER;

	struct TryType { D3DDEVTYPE DevType; DWORD VerType; };
	TryType TryTypeList[] = {
		{D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING},
		{D3DDEVTYPE_HAL, D3DCREATE_MIXED_VERTEXPROCESSING},
		{D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING},
		{D3DDEVTYPE_REF, D3DCREATE_HARDWARE_VERTEXPROCESSING},
		{D3DDEVTYPE_REF, D3DCREATE_MIXED_VERTEXPROCESSING},
		{D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING},
	//	{D3DDEVTYPE_SW, D3DCREATE_SOFTWARE_VERTEXPROCESSING},
	};
	int TryCount = sizeof( TryTypeList ) / sizeof( TryType );

	for( int i = 0 ; i < TryCount; i++ )
	{
		const TryType& Ref = TryTypeList[i];
		if( SoftDraw && Ref.DevType != D3DDEVTYPE_REF )continue;
		if( SoftVertex && Ref.VerType != D3DCREATE_SOFTWARE_VERTEXPROCESSING )continue;
		// デバイスの作成
		HRESULT ret = pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
			Ref.DevType,
			hWnd,
		//	Ref.VerType,
			Ref.VerType | D3DCREATE_MULTITHREADED,
			&InitParams, ppDevice);
		if( SUCCEEDED(ret) )break;
	}
	return (*ppDevice != NULL)?(S_OK):(E_FAIL);
}

// 初期化する
HRESULT D3DObject::Init()
{
	if( pDirect3D != NULL )return S_OK;

	CComPtr<USEDIRECT3D> pNewObject;
	// Direct3D オブジェクトを作成
	pNewObject = ::Direct3DCreate8(D3D_SDK_VERSION);
	if ( pNewObject == NULL )
	{
		// オブジェクト作成失敗
		::MessageBox(NULL,"Direct3D の作成に失敗しました。\nDirectX 8以上が必要です","Base",MB_OK | MB_ICONSTOP);
		// 終了する
		return E_FAIL;
	}
	SetObject( pNewObject );
	return S_OK;
}

// フォーマットが使えるか判断
bool D3DObject::CheckRenderFormat( DISPDEVICE* pDevice, D3DFORMAT CheckFormat ) const
{
	if( pDevice == NULL )return false;
	D3DDEVICE_CREATION_PARAMETERS Params;
	if( FAILED(pDevice->GetCreationParameters(&Params)) )return false;

	HRESULT Check = pDirect3D->CheckDeviceFormat(
		Params.AdapterOrdinal, Params.DeviceType, 
		NowDispMode.Format, 
		D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE,
		CheckFormat );
	return SUCCEEDED(Check);
}

// フォーマットが使えるか判断
bool D3DObject::CheckTextureFormat( DISPDEVICE* pDevice, D3DFORMAT CheckFormat ) const
{
	if( pDevice == NULL )return false;
	D3DDEVICE_CREATION_PARAMETERS Params;
	if( FAILED(pDevice->GetCreationParameters(&Params)) )return false;

	HRESULT Check = pDirect3D->CheckDeviceFormat(
		Params.AdapterOrdinal, Params.DeviceType, 
		NowDispMode.Format, 
		NULL, D3DRTYPE_TEXTURE,
		CheckFormat );
	return SUCCEEDED(Check);
}

// 半透明フォーマット
D3DFORMAT D3DObject::AlphaFormats[] = 
{
	D3DFMT_A8R8G8B8,
	D3DFMT_A4R4G4B4,
	D3DFMT_A1R5G5B5,
	D3DFMT_A8R3G3B2,
};

// 使えるフォーマット取得
D3DFORMAT D3DObject::GetRenderFormat( DISPDEVICE* pDevice ) const
{
	for( int i = 0; i < count_of(AlphaFormats); i++ )
	{
		if( CheckRenderFormat(pDevice,AlphaFormats[i]) )return AlphaFormats[i];
	}
	return NowDispMode.Format;
}

// 使えるフォーマット取得
D3DFORMAT D3DObject::GetTextureFormat( DISPDEVICE* pDevice ) const
{
	for( int i = 0; i < count_of(AlphaFormats); i++ )
	{
		if( CheckTextureFormat(pDevice,AlphaFormats[i]) )return AlphaFormats[i];
	}
	return NowDispMode.Format;
}

// フォーマットが使えるか判断
bool D3DObject::CheckDepthStencilFormat( D3DFORMAT CheckFormat ) const
{
	HRESULT Check = pDirect3D->CheckDeviceFormat(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		NowDispMode.Format, 
		D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE,
		CheckFormat );
	return SUCCEEDED(Check);
}

// 使えるフォーマット取得
D3DFORMAT D3DObject::GetDepthStencilFormat() const
{
	static D3DFORMAT ChekList[] = 
	{
		D3DFMT_D16,
		D3DFMT_D15S1,
		D3DFMT_D32,
		D3DFMT_D24S8,
		D3DFMT_D24X4S4,
		D3DFMT_D24X8,
	};
	for( int i = 0; i < count_of(ChekList); i++ )
	{
		if( CheckDepthStencilFormat(ChekList[i]) )return ChekList[i];
	}
	return D3DFMT_UNKNOWN;
}

// デバイスの能力取得
D3DCAPS8 D3DObject::GetDeviceCaps( D3DDEVTYPE Type ) const
{
	D3DCAPS8 Caps;
	AllZero( Caps );
	if( pDirect3D == NULL )return Caps;
	pDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, Type, &Caps );
	return Caps;
}

// 半透明フォーマットかチェック
bool D3DObject::IsAlphaFormat( D3DFORMAT CheckFormat ) const
{
	for( int i = 0; i < count_of(AlphaFormats); i++ )
	{
		if( AlphaFormats[i] == CheckFormat )return true;
	}
	return false;
}

