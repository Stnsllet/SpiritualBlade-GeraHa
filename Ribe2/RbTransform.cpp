// RbTransform.cpp: RbTransform �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTransform.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RbTransform::RbTransform()
{
	D3DXMatrixIdentity( this );
}

RbTransform::~RbTransform()
{

}

// �ϊ�����
RbPosition RbTransform::Trans( const D3DVECTOR& TransSource ) const
{
	D3DXVECTOR3 Output;
	D3DXVec3TransformCoord( &Output, (D3DXVECTOR3*)&TransSource, this );
	return Output;
};

// �X�N���[����������
RbTransform& RbTransform::AddScroll( float x, float y, float z )
{
	D3DXMATRIX Shift;
	D3DXMatrixTranslation( &Shift, x, y, z );
	(*this) *= Shift;
//	_41 += x * _44;
//	_42 += y * _44;
//	_43 += z * _44;
	return *this;
}

// Y �����猩�Ďp����ύX����}�g���N�X�쐬	
RbTransform& RbTransform::FromPosture( const D3DVECTOR& NewPosture )
{
	D3DXMatrixIdentity( this );
	return Posture( NewPosture );
}

// Y �����猩�Ďp����ύX����}�g���N�X
RbTransform& RbTransform::Posture( const D3DVECTOR& AddPosture )
{
	RbPosition Vector = AddPosture;
	float XYAngle = Vector.GetDirection();
	Vector.Normalize();
	float ZAngle = ::asinf( -Vector.z )/D3DX_PI;

	RotateX( ZAngle );
	RotateZ( XYAngle );
	return *this;
}

// �s�����]������
RbTransform& RbTransform::RotateX( float angle )
{
	if( angle == 0 )return *this;
	D3DXMATRIX Rotate;
	D3DXMatrixRotationX( &Rotate, angle*D3DX_PI );
	(*this) *= Rotate;
	return *this;
}

// �s�����]������
RbTransform& RbTransform::RotateY( float angle )
{
	if( angle == 0 )return *this;
	D3DXMATRIX Rotate;
	D3DXMatrixRotationY( &Rotate, angle*D3DX_PI );
	(*this) *= Rotate;
	return *this;
}

// �s�����]������
RbTransform& RbTransform::RotateZ( float angle )
{
	if( angle == 0 )return *this;
	D3DXMATRIX Rotate;
	D3DXMatrixRotationZ( &Rotate, angle*D3DX_PI );
	(*this) *= Rotate;
	return *this;
}

// �X�N���[���ʒu�擾
RbPosition RbTransform::GetScrollPos() const
{
	return RbPosition(_41, _42, _43);
}

// �s��Ƀ~���[�����O��������
RbTransform& RbTransform::Mirror( bool x, bool y, bool z )
{
	if( !x && !y && !z )return *this;
	D3DXMATRIX MirrorMat;
	D3DXMatrixScaling( &MirrorMat, (x)?(-1):(1), (y)?(-1):(1), (z)?(-1):(1) );
	(*this) *= MirrorMat;
	return *this;
}

// �s����X�P�[�����O����
RbTransform& RbTransform::Scale( float AddScale )
{
	if( AddScale == 1 )return *this;
	return Scale( AddScale, AddScale, AddScale );
}

// �s����X�P�[�����O����
RbTransform& RbTransform::Scale( float x, float y, float z )
{
	D3DXMATRIX ScaleMt;
	D3DXMatrixScaling( &ScaleMt, x, y, z );
	(*this) *= ScaleMt;
	return *this;
}

// �t�s��擾
RbTransform RbTransform::GetInverse() const
{
	RbTransform InverseMt;
	D3DXMatrixInverse( &InverseMt, NULL, this );
	return InverseMt;
}

// �t�s��ɂ���
RbTransform& RbTransform::Inverse()
{
	*this = GetInverse();
	return *this;
}
