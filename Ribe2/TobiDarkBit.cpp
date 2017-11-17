// TobiDrakBit.cpp: TobiDrakBit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"
#include "TobiDarkBit.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiDarkBit::TobiDarkBit()
{
	DmgWait = 200;
	Speed *= 0.5f;
}

TobiDarkBit::~TobiDarkBit()
{

}

void TobiDarkBit::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, SmashRange, pMember->GetArmyID(), true, true );

	const float NewWait = GetWait();
	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		RbActStatus& Ref = *(*itr++);
	//	if( !IsNewHit(&Ref) )continue;
		DamegeSystem DSys( *pMember, Ref );
		DSys.MagicDamege( AttackPow, KnockBack, NewWait );
	}
}
