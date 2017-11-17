// PolyStation.cpp: PolyStation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "DispStocker.h"
#include "LandScape.h"
#include "PolyStation.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

PolyStation::PolyStation()
{
	bRevDraw = false;
	Scale = 1;
	CheckRange = Top = 0;
	AllZero( BoxAngle );
	AllZero( HightGurid );
}

PolyStation::~PolyStation()
{
}

bool PolyStation::SetPolygon( const String& Name )
{
	pSource = GetCache().Poly.Get( Name );
	if( pSource == NULL )return false;

	CheckArea = pSource->Bound();
	Top = pSource->Top();
	CheckRange = pSource->Range();

	RbPointF Center = CheckArea.Center();
	BoxAngle.LT = Center.GetDirection( CheckArea.LT() );
	BoxAngle.LB = Center.GetDirection( CheckArea.LB() );
	BoxAngle.RT = Center.GetDirection( CheckArea.RT() );
	BoxAngle.RB = Center.GetDirection( CheckArea.RB() );
	return true;
}

bool PolyStation::SetXML( const XMLPaser& PolyXML, const LandScape& Land )
{
	String PolyName = PolyXML.AttributeString(TAG::NAME);
	RbPosition NewPos;
	NewPos.x = PolyXML.AttributeFloat(TAG::X);
	NewPos.y = PolyXML.AttributeFloat(TAG::Y);
	NewPos.z = PolyXML.AttributeFloat(TAG::Z);

	return SetStation( PolyName, Land, NewPos,
		PolyXML.AttributeFloat(_T("dir")),
		PolyXML.AttributeFloat(_T("scale")),
		PolyXML.AttributeFloat(_T("angle"))/180,
		PolyXML.AttributeBool(_T("rev")) );
}

bool PolyStation::SetStation( const String& PolyName, const LandScape& Land,
		const RbPosition& NewPos, float NewDirection, float NewScale, float NewAngle, bool bNewRevDraw )
{
	if( !SetPolygon( PolyName ) )return false;

	Pos = NewPos;
	Scale = NewScale;
	if( Scale <= 0 )Scale = 1;

	bRevDraw = bNewRevDraw;

	Pos.z += Land.GetHight( Pos.x, Pos.y );

	Trans.Scale( Scale );
	Trans.RotateX( NewAngle );
	Trans.RotateZ( NewDirection );
	Trans.AddScroll( Pos );

	InvTrans = Trans.GetInverse();

	// グリッド
	RbPosition Pos1( CheckArea.left, CheckArea.bottom, Top );
	Pos1 = Trans.Trans(Pos1);
	RbPosition Pos2( CheckArea.left, CheckArea.top, Top );
	Pos2 = Trans.Trans(Pos2);
	RbPosition Pos3( CheckArea.right, CheckArea.top, Top );
	Pos3 = Trans.Trans(Pos3);
	D3DXPlaneFromPoints(&HightGurid, &Pos1, &Pos2, &Pos3 );

	return true;
}

HRESULT PolyStation::Draw( Display& Disp )
{
	return DrawTrans( Disp, Trans );
}

HRESULT PolyStation::DrawTrans( Display& Disp, const RbTransform& Trans )
{
	if( pSource == NULL )return E_PENDING;
	if( bRevDraw )
	{
		// Z バッファ無効化
		DispStocker::Render StockZ( Disp, D3DRS_ZENABLE );
		StockZ.Set( FALSE );
		// ライト無効化
		DispStocker::Render StockLight( Disp, D3DRS_LIGHTING );
		StockLight.Set( FALSE );
		// 裏も書く
		DispStocker::Render StockCull( Disp, D3DRS_CULLMODE );
		StockCull.Set( D3DCULL_NONE );
		return pSource->DrawTrans( Disp, Trans );
	}
	if( Scale != 1 )
	{
		DispStocker::Render Normal( Disp, D3DRS_NORMALIZENORMALS );
		Normal.Set( TRUE );
		return pSource->DrawTrans( Disp, Trans );
	}

	return pSource->DrawTrans( Disp, Trans );
}

// 高さ
float PolyStation::Hight( float x, float y ) const
{
	float z = -HightGurid.d - HightGurid.a*x - HightGurid.b*y;
	if( HightGurid.c != 0 )z /= HightGurid.c;

	return z;
}

bool PolyStation::Pin( const RbPosition& CheckPos, float Size ) const
{
	float Range = Size + (CheckRange*Scale);
	Range *= Range;
	if( Pos.Length2(CheckPos) > Range )return false;
	RbPosition InvPos = InvTrans.Trans( CheckPos );
	return CheckArea.PinSize( InvPos.x, InvPos.y, Size/Scale );
}

bool PolyStation::Pout( const RbPosition& CheckPos, float Size ) const
{
	float Range = Size + (CheckRange*Scale);
	Range *= Range;
	if( Pos.Length2(CheckPos) > Range )return false;
	RbPosition InvPos = InvTrans.Trans( CheckPos );
	return CheckArea.PoutSize( InvPos.x, InvPos.y, Size/Scale );
}

RbPosition PolyStation::UnBlockVector( const RbPosition& Pos ) const
{
	RbPosition InvPos = InvTrans.Trans( Pos );
	float Angle = CheckArea.Center().GetDirection(RbPointF(InvPos.x,InvPos.y));
	RbPosition Vector( 0,-1,0 );
	if( Angle>=BoxAngle.RT && Angle<BoxAngle.RB )Vector.Rotate(0.5f);
	else if( Angle>=BoxAngle.RB && Angle<BoxAngle.LB )Vector.Rotate(1.0f);
	else if( Angle>=BoxAngle.LB && Angle<BoxAngle.LT )Vector.Rotate(1.5f);

	InvPos += Vector;
	InvPos = Trans.Trans( InvPos );

	InvPos -= Pos;
	return InvPos;
}

RbPosition PolyStation::UnBlockPos( const RbPosition& Pos, float Size ) const
{
	Size /= Scale;
	RbPosition InvPos = InvTrans.Trans( Pos );
	float Angle = CheckArea.Center().GetDirection(RbPointF(InvPos.x,InvPos.y));

	if( Angle>=BoxAngle.RT && Angle<BoxAngle.RB )InvPos.x = CheckArea.right + Size;
	else if( Angle>=BoxAngle.RB && Angle<BoxAngle.LB )InvPos.y = CheckArea.bottom + Size;
	else if( Angle>=BoxAngle.LB && Angle<BoxAngle.LT )InvPos.x = CheckArea.left - Size;
	else InvPos.y = CheckArea.top - Size;

	InvPos = Trans.Trans( InvPos );
	return InvPos;
}
