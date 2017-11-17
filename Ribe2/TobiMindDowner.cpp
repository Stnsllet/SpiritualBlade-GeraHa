// TobiMindDowner.cpp: TobiMindDowner クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "RbSound.h"
#include "TobiMindDowner.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiMindDowner::TobiMindDowner()
{
}

TobiMindDowner::~TobiMindDowner()
{
}

void TobiMindDowner::Damege( BattleSystem& System )
{
	static long WaveNum = 0;
	if( WaveNum == 0 )WaveNum = GetWaveID(_T("down"));
	GetSoundSystem().Play( WaveNum, Pos);

	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, MyEffect.Range, pMember->GetArmyID() );

	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
	//	if( !IsNewHit(&Ref) )continue;
		Heal( *(*itr++) );
	}
}

bool TobiMindDowner::Heal( RbActStatus& Taget)
{
	DamegeSystem Dmg( Taget, Taget );
	Dmg.MpDamege( Taget, MyEffect.Pow );
	return true;
}
