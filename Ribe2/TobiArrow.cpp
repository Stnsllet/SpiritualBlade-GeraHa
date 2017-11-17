// TobiArrow.cpp: TobiArrow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"
#include "TobiArrow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiArrow::TobiArrow()
{
	DmgWait = 800;
}

bool TobiArrow::SetResource()
{
	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("arrow"));
	SetPolygon( MyPolyID );

	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("suka"));
	StartSoundNum = WaveID;

	return true;
}

TobiArrow::~TobiArrow()
{

}

bool TobiArrow::Job( BattleSystem& System )
{
	Vector.z -= 1.0f;
	bTransSetOK = false;
	return TobiSingleBase::Job( System );
}

bool TobiArrow::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( !TobiSingleBase::SetTagetPos( pSource, BasePos, NewTagetPos ) )return false;

	Vector.z += Life/2.0f;
	return true;
}

void TobiArrow::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, SmashRange, pMember->GetArmyID() );

	const float NewWait = GetWait();
	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		RbActStatus& Ref = *(*itr++);
	//	if( !IsNewHit(&Ref) )continue;
		DamegeSystem DSys( *pMember, Ref );
		DSys.AttackDamege( AttackPow, KnockBack, NewWait );
	}
}
