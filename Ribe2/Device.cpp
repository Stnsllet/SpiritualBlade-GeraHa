// Device.cpp: Device �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D3DObject.h"
#include "Device.h"
#include "DefaultRenderState.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Device::Device()
{
	AllZero( PresentParams );
	AllZero( NowViewPort );
	AllZero( DeviceHalCaps );

	BackColor = 0xffffffff;
	AbientColor = 0xff404040;
}

Device::~Device()
{

}

HRESULT Device::FreeMemory()
{
	if( pDevice == NULL )return E_PENDING;
	return pDevice->ResourceManagerDiscardBytes( 0 );
}

// �f�o�C�X�̍쐬
HRESULT Device::Create( D3DObject& D3D, HWND hWnd )
{
	pDevice.Release();

	// �f�o�C�X�̍쐬
	HRESULT ret = D3D.CreateDevice( &pDevice, hWnd );
	if( FAILED(ret) )
	{
		PopupMsg(_T("Direct3D �̏������Ɏ��s���܂���") );
		return ret;
	}
	PresentParams = D3D;

	pDevice->GetViewport( &NowViewPort );

	return ret;
}

HRESULT Device::SetRenderState( const D3DCAPS8& HalCaps )
{
	if( pDevice == NULL )return E_PENDING;
	DeviceHalCaps = HalCaps;
	// �����_�����O�E�X�e�[�g��ݒ�
	DefaultRenderState Stater( pDevice );
	Stater.SetRenderState();

	// Z �o�b�t�@�L����
	pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);

	return S_OK;
}

DWORD Device::SetAlphaType( DWORD Type, long Stage )
{
	if( pDevice == NULL )return D3DTOP_MODULATE;
	// �A���t�@�u�����f�B���O���@��ݒ�
	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// �����_�����O���̃A���t�@�l�̌v�Z���@�̐ݒ�

	DWORD OldType = D3DTOP_MODULATE;

	pDevice->GetTextureStageState( Stage, D3DTSS_ALPHAOP, &OldType );
	pDevice->SetTextureStageState( Stage, D3DTSS_ALPHAOP, Type );
	return OldType;
}

bool Device::SetFog( float Min, float Max )
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

bool Device::IsLostDevice() const
{
	if( pDevice == NULL )return true;

	return FAILED(pDevice->TestCooperativeLevel());
}

// �f�o�C�X��ԕ���
HRESULT Device::RestoreDevice( bool bForce )
{
	if( pDevice == NULL )return E_PENDING;

	HRESULT ret = pDevice->TestCooperativeLevel();
	if( !bForce && ret != D3DERR_DEVICENOTRESET )return ret;

	FreeMemory();

	return _RestoreDevice();
}

// �f�o�C�X��ԕ���2
HRESULT Device::_RestoreDevice()
{
	D3DDISPLAYMODE NowDispMode;
	HRESULT ret = pDevice->GetDisplayMode( &NowDispMode );
	if( FAILED(ret) )return ret;

	PresentParams.BackBufferFormat = NowDispMode.Format;

	ret = pDevice->Reset( &PresentParams );
	if( FAILED(ret) )return ret;

	ret = SetRenderState( DeviceHalCaps );
	if( FAILED(ret) )return ret;

	return S_OK;
}


// �`�ʂ̊J�n��錾
HRESULT Device::BeginScene( bool bClear )
{
	if( pDevice == NULL )return E_PENDING;
	// �o�b�N�o�b�t�@�� Z �o�b�t�@���N���A
	if( bClear )pDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, BackColor, 1.0f,0);

	return pDevice->BeginScene();
}

// �`�ʂ̏I����錾
HRESULT Device::EndScene()
{
	if( pDevice == NULL )return E_PENDING;
	return pDevice->EndScene();
}

// ��ʂɕ\��
HRESULT Device::Flip()
{
	if( pDevice == NULL )return E_PENDING;
	return pDevice->Present(NULL,NULL,NULL,NULL);
}

// �E�C���h�E�T�C�Y��ύX
HRESULT Device::WindowSizeChange( UINT Width, UINT Height )
{
	PresentParams.BackBufferWidth = Width;
	PresentParams.BackBufferHeight = Height;
	PresentParams.Windowed = TRUE;
	HRESULT ret = RestoreDevice(true);
	if( FAILED(ret) )return ret;

	NowViewPort.Width = Width;
	NowViewPort.Height = Height;
	ret = pDevice->SetViewport( &NowViewPort );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// �t���X�N���[���ɕύX
HRESULT Device::FullScreenWindow( const D3DDISPLAYMODE& DisplayMode, UINT Width, UINT Height )
{
	PresentParams.BackBufferWidth = DisplayMode.Width;
	PresentParams.BackBufferHeight = DisplayMode.Height;
	PresentParams.Windowed = FALSE;
	PresentParams.BackBufferFormat = DisplayMode.Format;
	HRESULT ret = RestoreDevice(true);
	if( FAILED(ret) )return ret;

	NowViewPort.Width = DisplayMode.Width;
	NowViewPort.Height = DisplayMode.Height;
	ret = pDevice->SetViewport( &NowViewPort );
	if( FAILED(ret) )return ret;

	return S_OK;
}
