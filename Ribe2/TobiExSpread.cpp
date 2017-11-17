// TobiExSpread.cpp: TobiExSpread クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiExSpread.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiExSpread::TobiExSpread()
{

}

TobiExSpread::~TobiExSpread()
{

}

bool TobiExSpread::SetExDefine( ExCreator::BaseDef* pNewDef )
{
	if( pNewDef == NULL )return false;
	SetPolygon( pNewDef->PolyID );
	StartSoundNum = pNewDef->WaveID;
	SpreadSpeed = pNewDef->Speed;
	SpreadMaxRange = pNewDef->Range;
	bMagic = pNewDef->bMagic;
	bAllHit = pNewDef->bAllHit;
	SpreadKnockBack = pNewDef->KnockPow;
	DmgWait = pNewDef->Shake;
	if( pNewDef->pChiled != NULL )
	{
		ChiledName = pNewDef->pChiled->Name;
		ChiledRange = pNewDef->pChiled->Range;
	}
	return true;
}

void TobiExSpread::Damege( BattleSystem& System )
{
	typedef BattleTagetSystem::CharaItr CharaItr;

	if( pMember == NULL )return;

	BattleTagetSystem::CharaList TagetList;
	long Count = System.GetCharaList( TagetList, Pos, SpreadRange, pMember->GetArmyID(), true, bAllHit );
	if( Count == 0 )return;

	CharaItr itr( TagetList );
	while( !itr.IsEnd() )
	{
		RbActStatus& Ref = *itr;
		itr.Next();
		if( !IsNewHit(&Ref)  )continue;
		if( CreateChiled(ChiledName,Pos,Ref) )continue;

		DamegeSystem DSys( *pMember, Ref );
		if( bMagic )DSys.MagicDamege( AttackPow, GetKnockBack(Pos, Ref), DmgWait );
		else DSys.AttackDamege( AttackPow, GetKnockBack(Pos, Ref), DmgWait );
	}
}