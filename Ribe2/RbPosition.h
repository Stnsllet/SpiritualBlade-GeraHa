// RbPosition.h: RbPosition クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBPOSITION_H__CA634238_DF6F_4190_B2EE_74B5165DEBC6__INCLUDED_)
#define AFX_RBPOSITION_H__CA634238_DF6F_4190_B2EE_74B5165DEBC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbPoint.h"

class RbPosition : public D3DXVECTOR3 
{
public:
	RbPosition();
	RbPosition(const RbPoint& Pos){x=Pos.x;y=Pos.y;z=0;}
	RbPosition(const RbPointF& Pos){x=Pos.x;y=Pos.y;z=0;}
	RbPosition(const D3DVECTOR& Pos):D3DXVECTOR3(Pos){}
	RbPosition(float x, float y, float z=0):D3DXVECTOR3(x,y,z){}

	float GetDirection () const;
	float GetDirection ( const D3DVECTOR& Pos ) const;
	float GetDirection ( float dx, float dy ) const;
	float Length2( const D3DVECTOR& Pos ) const;
	float Length2() const;
	float Length( const D3DVECTOR& Pos ) const;
	float Length() const;
	float LenXY2( const D3DVECTOR& Pos ) const;
	float LenXY( const D3DVECTOR& Pos ) const;
	RbPosition& Normalize();
	RbPosition& Normalize( float Length );
	RbPosition& Rotate( float Angle );
	RbPosition& HalfPos( const D3DVECTOR& Pos );
	bool IsZero() const;
	bool Niar( const D3DVECTOR& Pos, float Size=3.0f ) const;
	bool NiarXY( const D3DVECTOR& Pos, float Size=3.0f ) const;
	bool Niar2( const D3DVECTOR& Pos, float Size=9.0f ) const;
	bool NiarXY2( const D3DVECTOR& Pos, float Size=9.0f ) const;

};

#endif // !defined(AFX_RBPOSITION_H__CA634238_DF6F_4190_B2EE_74B5165DEBC6__INCLUDED_)
