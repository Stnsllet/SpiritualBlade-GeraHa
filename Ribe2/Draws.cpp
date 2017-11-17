// Draws.cpp: Draws クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DispStocker.h"
#include "BillbordPlane.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "Draws.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Draws::Liner::Liner()
{
}

bool Draws::Liner::InitLine( LPCTSTR Name )
{
	Plane<TexRect> LPlane = GetCache().SystemPlane.GetPlane(Name);
	HandTexRect Hand;
	AllZero( Hand );
	(TexRect&)Hand = LPlane;

	return InitLine( Hand, LPlane );
}

bool Draws::Liner::InitLine( const HandTexRect& Rect, MYTEXTURE* pTexture )
{
	if( Rect.h == 0 || pTexture == NULL )return false;

	LinePlane = BillbordPlane( Rect, pTexture );
	RbTransform TransLength;
	TransLength.Scale( 1, 1.0f/Rect.h, 1 );
	LinePlane.Trans( TransLength );
	return true;
}

HRESULT Draws::Liner::Draw( Display& Disp, const RbPosition& Pos1, const RbPosition& Pos2 ) const
{
	if( LinePlane.pTexture == NULL )return E_PENDING;
	BillbordPlane TransPlane = LinePlane;

	RbPosition Vector = Pos2;
	Vector -= Pos1;

	RbTransform Trans;
	Trans.Scale( 1, Vector.Length(), 1 );
	Trans.Posture( Vector );
	Trans.AddScroll( Pos1 );
	TransPlane.Trans( Trans );

	return TransPlane.Draw( Disp );
}

Draws::Poly::Poly( Display& SetDisp ) : Disp( SetDisp )
{
}

long Draws::Poly::GetCount( D3DPRIMITIVETYPE Type ) const
{
	switch( Type )
	{
//	case D3DPT_TRIANGLESTRIP:return -2;
//	case D3DPT_TRIANGLEFAN:return -2;
	case D3DPT_LINESTRIP:return -1;
	case D3DPT_TRIANGLELIST:return 0;
	}
	return -2;
}

Draws::Bord::Bord( Display& Ref )
:Disp( Ref )
{
}

HRESULT Draws::Bord::Draw( const BillbordPlane& BordPlane, const RbPosition& Pos ) const
{
	RbTransform Trans = Disp.UnViewMat();
	Trans.AddScroll( Pos );
	DispStocker::Transform WorldTrans( Disp, D3DTS_WORLD );
	WorldTrans.Set( Trans );
	return BordPlane.Draw( Disp );
}
