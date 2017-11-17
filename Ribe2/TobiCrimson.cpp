// TobiCrimson.cpp: TobiCrimson クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "DispStocker.h"
#include "TobiCrimson.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiCrimson::TobiCrimson()
:EchoImage( 100, 5 )
{
	bAllHit = false;
	EffectCount = 1;
	EffectInterval = 500;
	NowEffectTime = 0;

	SpreadKnockBack = 0;
	DmgWait = 500;

}

bool TobiCrimson::SetResource()
{
	if( !TobiSpreadBase::SetResource() )return false;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("triangle"));
	SetPolygon( MyPolyID );

	return true;
}

TobiCrimson::~TobiCrimson()
{
}

// スキル効果設定
bool TobiCrimson::SetEffect( const SkillEffect& Effect )
{
	if( !TobiSpreadBase::SetEffect( Effect ) )return false;
	EffectCount = Effect.Count;
	return true;
}

// データセット
bool TobiCrimson::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( !TobiSpreadBase::SetTagetPos(pSource,BasePos,NewTagetPos) )return false;

	Locker.Lock( pSource->ActLock );
	Life = EffectCount*EffectInterval/System.JobTime();
	StartTime = System.GetBattleTime();
	return true;
}

// 仕事
bool TobiCrimson::Job( BattleSystem& System )
{
	if( pMember == NULL || pMember->IsDead() )return false;
	NowEffectTime += System.JobTime();
	Pos = pMember->GetPosition();

	if( Life > 1 ) SpreadRange += SpreadSpeed;
	else SpreadRange = SpreadMaxRange;
	if( SpreadRange > SpreadMaxRange)SpreadRange = SpreadMaxRange;

	if( NowEffectTime >= EffectInterval )
	{
		HitList.clear();
		NowEffectTime = 0;
	}
	Damege( System );
	if( (--Life) > 0 )return true;

	return false;
}

// 描写
void TobiCrimson::Draw( Display& Disp )
{
	float PassTime = (System.GetBattleTime()-StartTime)/1000;
	RbTransform Trans;
	Trans.Scale( SpreadRange );
	Trans.RotateY( PassTime*2 );
	Trans.RotateX( PassTime );
	Trans.AddScroll( Pos );

	DrawPolygon( Disp, Trans );
/*
	DispStocker::Material Mate( Disp );
	Draws::Echo<RbTransform>::citr itr = EchoImage.rbegin();
	while( itr.ok() )
	{
		float val = itr.val();
		const RbTransform& Ref = *itr++;
		Mate.SetAlpha( val );
		DrawPolygon( Disp, Ref );
	}
	EchoImage.Add( Trans, System.GetBattleTime() );
*/
}
