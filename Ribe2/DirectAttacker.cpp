// DirectAttacker.cpp: DirectAttacker クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "RbActStatus.h"
#include "BattleSystem.h"
#include "DirectAttacker.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DirectAttacker::DirectAttacker( BattleSystem& RefSystem, RbActStatus& RefSource )
: System(RefSystem), Source(RefSource)
{
	TimeScale = 1;
	bAttacking = false;
}

DirectAttacker::~DirectAttacker()
{
}

bool DirectAttacker::AttackStart( long ActID, float Angle )
{
	RefPtr<RbTexture> pTex = Source.RefTexture();
	if( pTex == NULL )return false;
	const RbTexture::AttackInfo& Info = pTex->GetActionInfo( ActID, Angle );
	StartAngle = Info.AttackStart;
	EndAngle = Info.AttackEnd;
	bSlash = Info.bSlash;
	MaxTime = Info.MaxTime;
	AttackRange = Source.AttackAlgo.NowWepon().AttackRange;
	if( MaxTime <= 0 )return false;
	TimeScale = 1.0f/MaxTime;

	HitList.clear();
	bAttacking = true;
	return bAttacking;
}

bool DirectAttacker::AttackEnd()
{
	return AttackJob( MaxTime );
}

bool DirectAttacker::AttackJob( int ActTime )
{
	if( !bAttacking )return false;
	float Time = 1.0f;
	if( ActTime < MaxTime )Time = TimeScale*ActTime;
	else bAttacking = false;

	if( bSlash )AttackW( Time );
	else AttackR( Time );

	return bAttacking;
}

bool DirectAttacker::AttackR( float Time )
{
	float Range = AttackRange * Time;
	RbPosition Pos = Source.GetPosition();
	Pos.z += Source.Hight()/2;

	CharaList List;
	System.GetCharaList( List, Pos,
		Range,
		Source.GetArmyID() );

	return Attack( List, StartAngle, EndAngle );
}

bool DirectAttacker::AttackW( float Time )
{
	RbPosition Pos = Source.GetPosition();
	Pos.z += Source.Hight()/2;

	CharaList List;
	System.GetCharaList( List, Pos,
		AttackRange,
		Source.GetArmyID() );

	float End = StartAngle + (EndAngle-StartAngle)*Time;
	return Attack( List, StartAngle, End );
}

bool DirectAttacker::IsNewHit( const RbActStatus& Ref )
{
	const long& ID = Ref.ID();
	if( HitList.find(ID) != HitList.end() )return false;
	HitList.insert(ID );
	return true;
}

bool DirectAttacker::Attack( const CharaList& List, float Start, float End )
{
	if( Start > End )std::swap( End, Start );
	CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		RbActStatus& Taget = *(*itr++);
		if( Taget.IsDead() )continue;
		if( !InAngle( Taget, Start, End ) )continue;
		if( !IsNewHit( Taget ) )continue;
		DamegeSystem Dmg( Source, Taget );
		RbPosition Kock = Taget.GetPosition();
		Kock -= Source.GetPosition();
		Kock.Normalize( 2.5 );
		Dmg.DirectDamege( 1.0f, Kock, 500 );
	}
	return true;
}

bool DirectAttacker::InAngle( const RbActStatus& Taget, const float& Start, const float& End ) const
{
	float Angle = Source.GetPosition().GetDirection( Taget.GetPosition() );
	if( Start <= Angle && Angle <= End )return true;
	Angle -= 2;
	if( Start <= Angle && Angle <= End )return true;
	return false;
}
