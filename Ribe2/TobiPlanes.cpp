// TobiPlanes.cpp: TobiPlanes クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbSprite.h"
#include "RbDisplay.h"
#include "TobiPlanes.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiPlanes::TobiPlanes()
{
}

TobiPlanes::~TobiPlanes()
{
}

bool TobiPlanes::TobiPDef::operator ==( const TobiPDef& Ref ) const
{
	if( HightPos != Ref.HightPos )return false;
	if( BasePos != Ref.BasePos )return false;
	if( ViewPlane != Ref.ViewPlane )return false;
	return true;
}

void TobiPlanes::ClearData()
{
	TobiPlaneList.clear();
}

bool TobiPlanes::AddPlane( LPCTSTR PlaneName, const D3DVECTOR& BasePos, int Life )
{
	if( PlaneName == NULL )return false;

	TobiPDef AddData;
	AddData.Type = ePlaneDefault;
	AddData.ViewPlane = GetCache().SystemPlane.GetPlane( PlaneName );
	if( AddData.ViewPlane.pTexture == NULL )return false;
	AddData.Life = Life;
	AddData.HightPos = - AddData.ViewPlane.h;
	AddData.BasePos = BasePos;

	return AddPlaneData( AddData );
}

bool TobiPlanes::AddBord( const String& PlaneName, const D3DVECTOR& BasePos )
{
	if( PlaneName.empty() )return false;

	TobiPDef AddData;
	AddData.Type = ePlaneBord;
	AddData.ViewPlane = GetCache().SystemPlane.GetPlane( PlaneName );
	if( AddData.ViewPlane.pTexture == NULL )return false;
	AddData.Life = 1;
	AddData.HightPos = - AddData.ViewPlane.h;
	AddData.BasePos = BasePos;

	return AddPlaneData( AddData );
}

bool TobiPlanes::AddPlaneData( const TobiPDef& AddData )
{
	std::list<TobiPDef>::const_iterator itr = TobiPlaneList.begin();
	while( itr != TobiPlaneList.end() )
		if( AddData == *itr++ )return false;

	TobiPlaneList.push_front( AddData );
	return true;
}

void TobiPlanes::Job()
{
	std::list<TobiPDef>::iterator itr = TobiPlaneList.begin();
	while( itr != TobiPlaneList.end() )
	{
		(itr->HightPos)--;
		if( ((itr->Life)--)<=0 )itr = TobiPlaneList.erase(itr);
		else itr++;
	}
}

void TobiPlanes::Draw( Display& Disp ) const
{
	if( TobiPlaneList.size() <= 0 )return;
	RbSprite Surface;
	HRESULT hr = Surface.InitSprite( Disp );
	if( FAILED(hr) )return;

	Draw( Disp, Surface );
}

void TobiPlanes::Draw( Display& Disp, RbSprite& Surface ) const
{
	std::list<TobiPDef>::const_iterator itr = TobiPlaneList.begin();
	while( itr != TobiPlaneList.end() )
	{
		const TobiPDef& Ref = *(itr++);
		Ref.Draw( Disp, Surface );
	}
}

bool TobiPlanes::TobiPDef::Job()
{
	switch( Type )
	{
	case TobiPlanes::ePlaneBord:
		return false;
	}
	// default
	--HightPos;
	return ((--Life)<=0);
}


bool TobiPlanes::TobiPDef::Draw( Display& Disp, RbSprite& Surface ) const
{
	RbPosition Pos = Disp.ViewToDisp( BasePos );
	Pos.x -= (ViewPlane.w)/2.0f;
	Pos.y += HightPos;
	const POINT& DisplaySize = Surface.GetSize();

	if( Pos.y+ViewPlane.h < 0 || Pos.y > DisplaySize.y )return false;
	if( Pos.x+ViewPlane.w < 0 || Pos.x > DisplaySize.x )return false;

	Surface.DrawPlane( ViewPlane, Pos.x, Pos.y );
	return true;
}
