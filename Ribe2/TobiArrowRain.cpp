// TobiArrowRain.cpp: TobiArrowRain クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiHightArrow.h"
#include "BattleSystem.h"
#include "TobiArrowRain.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiArrowRain::TobiArrowRain()
: CreateDips(44.0f)
{
}

TobiArrowRain::~TobiArrowRain()
{
}

// 仕事
bool TobiArrowRain::Job( BattleSystem& System )
{
	CreateRain( MyEffect.Range );
	return false;
}

bool TobiArrowRain::CreateRain( float Range )
{
	if( pMember == NULL )return false;
	for( float Pos = CreateDips; Pos <= Range; Pos+=CreateDips )
	{
		CreateCircleRain( Pos, CreateDips );
	}
	return true;
}

long TobiArrowRain::CreateCircleRain( float Range, float Dips )
{
	if( Dips < 1 )return 0;
	long Count = (Range*2*D3DX_PI)/Dips;
	if( Count <= 0 )return 0;
	Dips = 2.0f/Count;

	RbPosition BasePos = pMember->GetPosition();
	BasePos.z += pMember->Hight()/2;
	RbPosition TagetPos( 0, -Range, 0);
	for( int i = 0; i < Count; i++ )
	{
		CreateArrow( BasePos, BasePos+TagetPos );
		TagetPos.Rotate( Dips );
	}
	return Count;
}

bool TobiArrowRain::CreateArrow( const RbPosition& Base, const RbPosition& Taget ) const
{
	RbPosition TagetPos = Taget;
	TagetPos.z = System.Land.GetHight( Taget.x, Taget.y );
	TagetPos.z += pMember->Hight()/2;

	RefPtr<TobiObjectBase> pArrow = CreateObject();
	if( pArrow == NULL )return false;

	pArrow->SetResource();
	pArrow->SetEffect( MyEffect );
	if( !pArrow->SetTagetPos( pMember, Base, TagetPos ) )return false;
	System.TobiSys.Add( pArrow );
	return true;
}

TobiObjectBase* TobiArrowRain::CreateObject() const
{
	return new TobiHightArrow;
}
