// RbTransform.cpp: RbTransform クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTransform.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbTransform::RbTransform()
{
	D3DXMatrixIdentity( this );
}

RbTransform::~RbTransform()
{

}

// 変換する
RbPosition RbTransform::Trans( const D3DVECTOR& TransSource ) const
{
	D3DXVECTOR3 Output;
	D3DXVec3TransformCoord( &Output, (D3DXVECTOR3*)&TransSource, this );
	return Output;
};

// スクロールを加える
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

// Y 軸から見て姿勢を変更するマトリクス作成	
RbTransform& RbTransform::FromPosture( const D3DVECTOR& NewPosture )
{
	D3DXMatrixIdentity( this );
	return Posture( NewPosture );
}

// Y 軸から見て姿勢を変更するマトリクス
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

// 行列を回転させる
RbTransform& RbTransform::RotateX( float angle )
{
	if( angle == 0 )return *this;
	D3DXMATRIX Rotate;
	D3DXMatrixRotationX( &Rotate, angle*D3DX_PI );
	(*this) *= Rotate;
	return *this;
}

// 行列を回転させる
RbTransform& RbTransform::RotateY( float angle )
{
	if( angle == 0 )return *this;
	D3DXMATRIX Rotate;
	D3DXMatrixRotationY( &Rotate, angle*D3DX_PI );
	(*this) *= Rotate;
	return *this;
}

// 行列を回転させる
RbTransform& RbTransform::RotateZ( float angle )
{
	if( angle == 0 )return *this;
	D3DXMATRIX Rotate;
	D3DXMatrixRotationZ( &Rotate, angle*D3DX_PI );
	(*this) *= Rotate;
	return *this;
}

// スクロール位置取得
RbPosition RbTransform::GetScrollPos() const
{
	return RbPosition(_41, _42, _43);
}

// 行列にミラーリングを加える
RbTransform& RbTransform::Mirror( bool x, bool y, bool z )
{
	if( !x && !y && !z )return *this;
	D3DXMATRIX MirrorMat;
	D3DXMatrixScaling( &MirrorMat, (x)?(-1):(1), (y)?(-1):(1), (z)?(-1):(1) );
	(*this) *= MirrorMat;
	return *this;
}

// 行列をスケーリングする
RbTransform& RbTransform::Scale( float AddScale )
{
	if( AddScale == 1 )return *this;
	return Scale( AddScale, AddScale, AddScale );
}

// 行列をスケーリングする
RbTransform& RbTransform::Scale( float x, float y, float z )
{
	D3DXMATRIX ScaleMt;
	D3DXMatrixScaling( &ScaleMt, x, y, z );
	(*this) *= ScaleMt;
	return *this;
}

// 逆行列取得
RbTransform RbTransform::GetInverse() const
{
	RbTransform InverseMt;
	D3DXMatrixInverse( &InverseMt, NULL, this );
	return InverseMt;
}

// 逆行列にする
RbTransform& RbTransform::Inverse()
{
	*this = GetInverse();
	return *this;
}
