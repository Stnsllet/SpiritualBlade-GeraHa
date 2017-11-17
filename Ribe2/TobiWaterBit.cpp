// TobiWaterBit.cpp: TobiWaterBit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiWaterBit.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiWaterBit::TobiWaterBit()
{
	NowAngle = 0;
	bAllHit = false;

	Time = 0;
}

bool TobiWaterBit::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("water"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("waterbit"));
	SetPolygon( MyPolyID );

	return true;
}

TobiWaterBit::~TobiWaterBit()
{
}

bool TobiWaterBit::SetEffect( const SkillEffect& Effect )
{
	MyEffect = Effect;
	SmashRange = Effect.Range/1.5f;
	return TobiSingleBase::SetEffect( Effect );
}

bool TobiWaterBit::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;
	Vector = RbPosition( 1, 0, 0 );
	Pos = BasePos;
	TagetPos = BasePos;

	return true;
}

RbPosition TobiWaterBit::GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const
{
	RbPosition Knock = Taget.GetPosition();
	Knock -= pMember->GetPosition();

	return Knock.Normalize(10);
}

bool TobiWaterBit::ResetTrans()
{
	if( bTransSetOK )return false;
	Trans.FromPosture( Vector );
	Trans.Scale( SmashRange );
	Trans.AddScroll( Pos );
	bTransSetOK = true;
	return true;
}

bool TobiWaterBit::Job( BattleSystem& System )
{
	if( pMember->IsDead() )return false;

	const float CheckTime = 1000;
	Time += System.JobTime();
	if( Time > CheckTime )
	{
		Time -= CheckTime;
		HitList.clear();
		MyEffect.Count -= 1;
	}
	bTransSetOK = false;
	if( MyEffect.Count <= 0 )return false;

	TagetPos = RbPosition( 0, -MyEffect.Range, 0 );
	TagetPos.Rotate( Time / (CheckTime/2) );
	TagetPos += pMember->GetPosition();

	Vector = TagetPos-Pos;
	Pos = TagetPos;

	Damege( System );
	return true;
}
