// DefaultRenderState.cpp: DefaultRenderState �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DefaultRenderState.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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
	// �����_�����O�E�X�e�[�g��ݒ�
	// Z �o�b�t�@�L����
//	pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	// ��������
	pDevice->SetRenderState( D3DRS_CULLMODE ,D3DCULL_NONE );

	// �A���t�@�u�����f�B���O���@��ݒ�
	SetAlphaType( D3DTOP_MODULATE );
	// �e�N�X�`���̐F���g�p
	pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	// ���_�̐F���g�p
	pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

	// �����_�����O���̐F�̌v�Z���@�̐ݒ�
	pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	// �e�N�X�`���̐F���g�p
	pDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	// ���_�̐F���g�p
	pDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_CURRENT);
//	pDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

	// �t�B���^�ݒ�
	pDevice->SetTextureStageState(0,D3DTSS_MIPFILTER,D3DTEXF_LINEAR);
	pDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	pDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);

	// �A���t�@�����ȉ��Ȃ�Z�o�b�t�@���X�V���Ȃ�
	pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	pDevice->SetRenderState( D3DRS_ALPHAREF,		0x10 );
	pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	return S_OK;
}

DWORD DefaultRenderState::SetAlphaType( DWORD Type, long Stage )
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
