// TobiFenix.cpp: TobiFenix クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"
#include "TobiFenix.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiFenix::TobiFenix()
{
	SmashRange = 24;
	MaxSpeed = Speed;
	DownSpeed = MaxSpeed/150.0f;
	TurnSpeed = MaxSpeed/8.0f;

	NextHitTime = 0;
}

bool TobiFenix::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("suka"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("fenix"));
	SetPolygon( MyPolyID );

	return true;
}

TobiFenix::~TobiFenix()
{
}

bool TobiFenix::ResetTrans()
{
	if( bTransSetOK )return false;
	Trans.FromPosture( Vector );
	Trans.Scale( SmashRange );
	Trans.AddScroll( Pos );
	bTransSetOK = true;
	return true;
}

bool TobiFenix::Job( BattleSystem& System )
{
	KnockBack = Vector;
	KnockBack.Normalize(2);
	if( System.GetBattleTime() > NextHitTime )
	{
		HitList.clear();
		NextHitTime = System.GetBattleTime()+1000;
	}
	Damege( System );
	return TobiMissleBase::Job( System );
}

bool TobiFenix::TagetHitJob( BattleSystem& System )
{
	pTaget = System.GetRndEnemy( TagetPos, pMember->GetArmyID(), System.EscapeRange );
	return true;
}

void TobiFenix::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, SmashRange, pMember->GetArmyID(), true, bAllHit );

	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( !IsNewHit(&Ref) )continue;
		DamegeSystem DSys( *pMember, Ref );
		DSys.AttackDamege( AttackPow, GetKnockBack(Pos,Ref), DmgWait );
	}
}
