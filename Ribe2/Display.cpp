// Display.cpp: Display �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D3DObject.h"
#include "RbTransform.h"
#include "Display.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

Display::Display()
{
	DispInfo.UpView.Length = 300;
	DispInfo.UpView.Angle = 45.0f;
	DispInfo.FishEye = 60.0f;

	AllZero( DispLight );
	DispLight.Type	   = D3DLIGHT_DIRECTIONAL;
	DispLight.Diffuse.r  = 1.0f;
	DispLight.Diffuse.g  = 1.0f;
	DispLight.Diffuse.b  = 1.0f;

	AllZero( DispMaterial );
	DispMaterial.Diffuse.r = DispMaterial.Ambient.r = 1.0f;
	DispMaterial.Diffuse.g = DispMaterial.Ambient.g = 1.0f;
	DispMaterial.Diffuse.b = DispMaterial.Ambient.b = 1.0f;
	DispMaterial.Diffuse.a = DispMaterial.Ambient.a = 1.0f;
	PlaneMaterial = DispMaterial;

	bZbias = true;
	ZPosRate = 0.01f;
}

Display::~Display()
{
}

HRESULT Display::SetRenderState( const D3DCAPS8& HalCaps )
{
	HRESULT ret = Device::SetRenderState( HalCaps );
	if( FAILED(ret) )return ret;
	bZbias = ( 0!=(HalCaps.RasterCaps | D3DPRASTERCAPS_ZBIAS ) );

	// ���C�g
	SetLight( 0.25f, -1, -.70f );

	// �r���[���W�n
	UpView( 450.0f, 30.0f );

	SetProjection();

	return S_OK;
}

// ����p�ݒ�
bool Display::SetFishEye( float angle )
{
	if( angle < 10 )angle = 10;
	DispInfo.FishEye = angle;
	return true;
}

// �v���W�F�N�V�����ݒ�
HRESULT Display::SetProjection( float aspect )
{
	if( pDevice == NULL )return E_PENDING;

	const float& Length = DispInfo.UpView.Length;
	float Width = (DispInfo.UpView.Angle<30)
		?(::sinf(30 *D3DX_PI /180.0f))
		:(::sinf(DispInfo.UpView.Angle *D3DX_PI /180.0f));
	if( Width < 0 )Width = -Width;
//	Width += 1.0f;
	D3DXMATRIX mProj;

	D3DXMatrixPerspectiveFovLH(&mProj
		,DispInfo.FishEye*D3DX_PI /180.0f	// ����p
		,aspect								// �A�X�y�N�g��
		,Length*(1-Width)		// �ŋߐڋ���
		,Length*(1+Width)		// �ŉ�������
		);
	SetFog( Length, Length*(1+Width ));
	DispInfo.ProjectionMat = mProj;
	ZPosRate = Length*(1+2*Width );
	if( ZPosRate > 0 )ZPosRate = 1/ZPosRate;
	else ZPosRate = 0.01f;

	return ResetAllMat( pDevice->SetTransform(D3DTS_PROJECTION, &mProj) );
}

// �f�o�C�X��ԕ���
HRESULT Display::_RestoreDevice()
{
	if( pDevice == NULL )return E_PENDING;

	HRESULT ret = Device::_RestoreDevice();
	if( FAILED(ret) )return ret;

	ret = SetLight( DispInfo.LightVector.x, DispInfo.LightVector.y, DispInfo.LightVector.z );
	if( FAILED(ret) )return ret;

	pDevice->SetTransform(D3DTS_PROJECTION, &DispInfo.ProjectionMat);
	pDevice->SetTransform(D3DTS_VIEW, &DispInfo.ViewMat);
	pDevice->SetTransform(D3DTS_WORLD, &DispInfo.WorldMat);

//	ret = UpView( DispInfo.UpView.Length, DispInfo.UpView.Angle );
//	if( FAILED(ret) )return ret;

//	ret = SetViewPos( DispInfo.Scroll.x, DispInfo.Scroll.y, DispInfo.Scroll.z );
//	if( FAILED(ret) )return ret;

	return S_OK;
}

// �����̐ݒ�
HRESULT Display::SetLight( float x, float y, float z )
{
	if( pDevice == NULL )return E_PENDING;

	pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	pDevice->SetRenderState( D3DRS_AMBIENT, AbientColor );

	D3DXVECTOR3 vecDir = D3DXVECTOR3( x, y, z );
	D3DXVec3Normalize( (D3DXVECTOR3*)&DispLight.Direction, &vecDir );

	pDevice->SetLight( 0, &DispLight );
	pDevice->SetMaterial( &DispMaterial );

	pDevice->LightEnable( 0, TRUE );
	DispInfo.LightVector = D3DXVECTOR3( x, y, z);

	return S_OK;
}

// �\���ʒu�ݒ�
HRESULT Display::SetViewPos( float x, float y, float z )
{
	if( pDevice == NULL )return E_PENDING;

	DispInfo.Scroll.x = x; DispInfo.Scroll.y = y; DispInfo.Scroll.z = z;
	// ���[���h���W�n
	D3DXMATRIX mWorld;
	D3DXMatrixTranslation( &mWorld, -x, -y, -z );
	DispInfo.WorldMat = mWorld;

	return ResetAllMat(pDevice->SetTransform(D3DTS_WORLD, &mWorld));
}

// �\���X�N���[���ݒ�
HRESULT Display::ScrollView( float x, float y, float z )
{
	if( pDevice == NULL )return E_PENDING;

	DispInfo.Scroll.x += x; DispInfo.Scroll.y += y; DispInfo.Scroll.z += z;
	// ���[���h���W�n
	D3DXMATRIX mWorld;
	D3DXMatrixTranslation( &mWorld, -DispInfo.Scroll.x, -DispInfo.Scroll.y, -DispInfo.Scroll.z );
	DispInfo.WorldMat = mWorld;

	return ResetAllMat(pDevice->SetTransform(D3DTS_WORLD, &mWorld));
}

// ���Ă�������Ƌ�����ݒ�
HRESULT Display::UpView( float Length, float Angle )
{
	if( pDevice == NULL )return E_PENDING;

	DispInfo.UpView.Length = Length;
	DispInfo.UpView.Angle = Angle;

	D3DXMATRIX mView;

	Angle = Angle * D3DX_PI / 180.0f;
	float UpZ = ::cosf( Angle ) * Length;
	float UpY = ::sinf( Angle ) * Length;
	DispInfo.UpView.Pos = RbPosition( 0,UpY,UpZ );

	D3DXMatrixLookAtLH(&mView
					,&DispInfo.UpView.Pos	// �J�����ʒu
					,&D3DXVECTOR3(0,0,0)	// �J�����̒��ړ_
					,&D3DXVECTOR3(0,-1,0)	// ��̌���
					);

	DispInfo.ViewMat = mView;

	return ResetAllMat(pDevice->SetTransform(D3DTS_VIEW,  &mView));
}

RbTransform Display::GetAllMat() const
{
	if( pDevice == NULL )return RbTransform();

	RbTransform GetMat;
	pDevice->GetTransform(D3DTS_WORLD,  &GetMat);
	RbTransform AllMat( GetMat );
	pDevice->GetTransform(D3DTS_VIEW,  &GetMat);
	AllMat *= GetMat;
	pDevice->GetTransform(D3DTS_PROJECTION,  &GetMat);
	AllMat *= GetMat;
	RbTransform ViewPortMat;
	D3DXMatrixOrthoOffCenterLH( &ViewPortMat,
		0, NowViewPort.Width,
		NowViewPort.Height, 0,
		NowViewPort.MinZ, NowViewPort.MaxZ);
	ViewPortMat.Inverse();

//	ViewPortMat.AddScroll( 1, -1 );
//	ViewPortMat.Scale( NowViewPort.Width/2.0f, NowViewPort.Height/2.0f, 1 );
//	ViewPortMat.Scale( 1, -1, 1 );
	AllMat *= ViewPortMat;

	return AllMat;
}

HRESULT Display::ResetAllMat( HRESULT hr )
{
	if( pDevice == NULL )return hr;

	DispInfo.AllMat = GetAllMat();
	DispInfo.AllReverseMat = DispInfo.AllMat.GetInverse();
	DispInfo.UnViewMat = RbTransform().RotateX( -DispInfo.UpView.Angle/180.0f );

	return hr;
}

// �}�E�X�ʒu�� Z=DestZ(0) �� 3D ���W�ɕϊ�
RbPosition Display::MouseToView( int x, int y, float DestZ ) const
{
	RbPosition Pos = DispToView( D3DXVECTOR3( (float)x, (float)y, 0 ) );
	RbPosition Pos2 = DispToView( D3DXVECTOR3( (float)x, (float)y, NowViewPort.MaxZ ) );

	Pos2 -= Pos;
	if( Pos2.z == 0 )return Pos;
	float n = ( DestZ - Pos.z ) / Pos2.z;
	if( n <= 0 ) n = -n;
	Pos2 *= n;
	Pos += Pos2;

	return Pos;
}

// 3D ���W���f�B�X�v���C���W��
RbPosition Display::ViewToDisp( const D3DXVECTOR3& Pos ) const
{
//	D3DXVec3Project( &NewPos, &Pos, &NowViewPort,
//		&DispInfo.ProjectionMat, &DispInfo.ViewMat, &DispInfo.WorldMat );

	return DispInfo.AllMat.Trans( Pos );
}

// �f�B�X�v���C���W�� 3D ���W��
RbPosition Display::DispToView( const D3DXVECTOR3& DispPos ) const
{
//	D3DXVec3Unproject( &NewPos, &DispPos, &NowViewPort,
//		&DispInfo.ProjectionMat, &DispInfo.ViewMat, &DispInfo.WorldMat );

	return DispInfo.AllReverseMat.Trans( DispPos );
}

// �r���{�[�h��̈ʒu�擾
RbPosition Display::BillBordPos( const D3DVECTOR& BordPos, const D3DVECTOR& BasePos ) const
{
	RbPosition Pos = UnViewMat().Trans( BordPos );
	Pos += BasePos;
	return RefAllMat().Trans( Pos );
}


// �f�B�X�v���C��̃X�P�[�����O�擾
float Display::GetDisplayScale( const D3DXVECTOR3& Pos ) const
{
	RbPosition Scale1 = Pos; Scale1.x -=.5f;
	RbPosition Scale2 = Pos; Scale2.x +=.5f;
	Scale1 = ViewToDisp( Scale1 );
	Scale2 = ViewToDisp( Scale2 );
	return Scale2.x - Scale1.x;
}
