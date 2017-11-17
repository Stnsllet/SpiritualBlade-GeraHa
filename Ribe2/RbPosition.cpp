// RbPosition.cpp: RbPosition クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbPosition.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbPosition::RbPosition()
{
	AllZero(*this);
}

float RbPosition::GetDirection ( const D3DVECTOR& Pos ) const
{
	return RbPointF(x,y).GetDirection(RbPointF(Pos.x,Pos.y));
}

float RbPosition::GetDirection ( float dx, float dy ) const
{
	return RbPointF(x,y).GetDirection(RbPointF(dx,dy));
}

float RbPosition::GetDirection () const
{
	return RbPointF(x,y).GetDirection();
}

float RbPosition::Length2( const D3DVECTOR& Pos ) const
{
	RbPosition New( *this );
	New -= Pos;

	return New.Length2();
}

float RbPosition::Length2() const
{
	return (x*x) + (y*y) + (z*z);
}

float RbPosition::Length( const D3DVECTOR& Pos ) const
{
	return ::sqrtf( Length2( Pos ) );
}

float RbPosition::Length() const
{
	return ::sqrtf( Length2() );
}

float RbPosition::LenXY2( const D3DVECTOR& Pos ) const
{
	float dx = x-Pos.x;
	float dy = y-Pos.y;
	return (dx*dx)+(dy*dy);
}

float RbPosition::LenXY( const D3DVECTOR& Pos ) const
{
	return ::sqrtf( LenXY2(Pos) );
}

bool RbPosition::Niar( const D3DVECTOR& Pos, float Size ) const
{
	return Length2( Pos ) <= (Size*Size);
}

bool RbPosition::NiarXY( const D3DVECTOR& Pos, float Size ) const
{
	return LenXY2( Pos ) <= (Size*Size);
}

bool RbPosition::Niar2( const D3DVECTOR& Pos, float Size2 ) const
{
	return Length2( Pos ) <= (Size2);
}

bool RbPosition::NiarXY2( const D3DVECTOR& Pos, float Size2 ) const
{
	return LenXY2( Pos ) <= (Size2);
}

RbPosition& RbPosition::Normalize()
{
	float Len = Length();
	if( Len <= 0 )return *this;
	x /= Len;
	y /= Len;
	z /= Len;

	return *this;
}

RbPosition& RbPosition::Normalize( float Length )
{
	Normalize()*=Length;
	return *this;
}

RbPosition& RbPosition::Rotate( float Angle )
{
	Angle *= D3DX_PI;
	float Sin = ::sinf( Angle );
	float Cos = ::cosf( Angle );
	float sx = x;
	x = sx*Cos - y*Sin;
	y = sx*Sin + y*Cos;

	return *this;
}

RbPosition& RbPosition::HalfPos( const D3DVECTOR& Pos )
{
	*this += Pos;
	*this *= 0.5f;
	return *this;
}

bool RbPosition::IsZero() const
{
	if( x != 0 )return false;
	if( y != 0 )return false;
	if( z != 0 )return false;
	return true;
}
