// TobiHopper.cpp: TobiHopper クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"
#include "TobiHopper.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiHopper::TobiHopper()
{
	Speed = Speed * 0.25f;
}

TobiHopper::~TobiHopper()
{

}

void TobiHopper::Damege( BattleSystem& System )
{
	static const float NextHitRange = 64.0f;

	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, NextHitRange, pMember->GetArmyID() );

	const float NewWait = GetWait();
	bool bHit = false;
	BattleSystem::CharaList::iterator itr = List.begin();
	while( itr != List.end() )
	{
		RbActStatus& Ref = **itr;
		if( !Ref.InRange( Pos, SmashRange ) )
		{
			++itr;
			continue;
		}
	//	if( !IsNewHit(&Ref) )continue;
		DamegeSystem DSys( *pMember, Ref );
		DSys.AttackDamege( AttackPow, KnockBack, NewWait );
		itr = List.erase( itr );
		bHit = true;
	}
	if( !bHit )return;
	long Count = List.size();
	if( Count <= 0 )return;
	itr = List.begin();
	long RndPos = Rnd( Count );
	while( (--RndPos)>0 )++itr;

	SkillEffect Effect;
	Effect.Pow = AttackPow;
	Effect.Range = NextHitRange;
	TobiRefPtr<TobiHopper> pNext( Effect );
	if( !pNext->SetTagetPos( pMember, Pos, (*itr)->GetPosition() ) )return;
	System.TobiSys.Add( pNext );
	return;
}
