// TobiGenocide.cpp: TobiGenocide クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiGenocide.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiGenocide::TobiGenocide()
{
	DmgWait = 1500;
}

TobiGenocide::~TobiGenocide()
{
}

// 仕事
bool TobiGenocide::Job( BattleSystem& System )
{
	Damege( System );

	return false;
}

// データセット
bool TobiGenocide::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	pMember = pSource;
	return true;
}

// ダメージ
void TobiGenocide::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	RbActStatus& Source = *pMember;
	const RbPosition KnockBack( 0,0,0 );
	long ArmyID = Source.GetArmyID();

	const float NewWait = GetWait();
	const BattleSystem::CharaList& List = System.RefAllCharactor();
	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( ArmyID == Ref.GetArmyID() )continue;
		DamegeSystem Dmg( Source, Ref );
		Dmg.AttackDamege( AttackPow, KnockBack, NewWait );
	}
}
