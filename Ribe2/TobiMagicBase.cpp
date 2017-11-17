// TobiMagicBase.cpp: TobiMagicBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"
#include "TobiMagicBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiMagicBase::TobiMagicBase()
{
	DmgWait = 200;
	bAllHit = true;
}

TobiMagicBase::~TobiMagicBase()
{

}

void TobiMagicBase::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, SmashRange, pMember->GetArmyID(), true, bAllHit );

	const float NewWait = GetWait();
	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( !IsNewHit(&Ref) )continue;
		DamegeSystem DSys( *pMember, Ref );
		DSys.MagicDamege( AttackPow, GetKnockBack(Pos,Ref), NewWait );
	}
}
