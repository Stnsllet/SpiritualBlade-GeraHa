// TobiDeadlySlash.cpp: TobiDeadlySlash クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "TobiDeadlySlash.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiDeadlySlash::TobiDeadlySlash()
{
	bAllHit = false;
	SpreadSpeed *= 1.5f;
}

bool TobiDeadlySlash::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("kain"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("windbit"));
	SetPolygon( MyPolyID );

	return true;
}

TobiDeadlySlash::~TobiDeadlySlash()
{
}

// ダメージ
void TobiDeadlySlash::Damege( BattleSystem& System )
{
	typedef BattleTagetSystem::CharaItr CharaItr;

	if( pMember == NULL )return;

	BattleTagetSystem::CharaList TagetList;
	long Count = System.GetCharaList( TagetList, Pos, SpreadRange, pMember->GetArmyID(), true, bAllHit );
	if( Count == 0 )return;

	const float NewWait = GetWait();
	CharaItr itr( TagetList );
	while( !itr.IsEnd() )
	{
		if( IsNewHit(itr)  )
		{
			DamegeSystem DSys( *pMember, *itr );
			DSys.DeadDamege( AttackPow, GetKnockBack(Pos, *itr), NewWait );
		}
		itr.Next();
	}
}
