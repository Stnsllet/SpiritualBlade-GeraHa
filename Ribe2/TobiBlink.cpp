// TobiBlink.cpp: TobiBlink クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiBlinkBit.h"
#include "TobiBlink.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiBlink::TobiBlink()
{
	GenerateTime = GenerateInterval = 300;
}

TobiBlink::~TobiBlink()
{
}

bool TobiBlink::Generate()
{
	if( pMember == NULL )return TobiGeneratorBase::Generate();

	BattleTagetSystem::CharaList TagetList;
	long Count = System.GetCharaList( TagetList, Pos, MyEffect.Range, pMember->GetArmyID() );
	if( Count == 0 )return TobiGeneratorBase::Generate();

	BattleTagetSystem::CharaList::const_iterator itr = TagetList.begin();
	while( itr != TagetList.end() )
	{
		TobiRefPtr<TobiBlinkBit> pBit(MyEffect);
		if( pBit->SetTaget( pMember, (*itr++) ) )
			System.TobiSys.Add( pBit );
	}
	return TobiGeneratorBase::Generate();
}
