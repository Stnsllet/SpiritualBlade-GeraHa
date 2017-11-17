// TobiAttackSpreadBase.cpp: TobiAttackSpreadBase �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "TobiAttackSpreadBase.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiAttackSpreadBase::TobiAttackSpreadBase()
{
	DmgWait = 800;
}

TobiAttackSpreadBase::~TobiAttackSpreadBase()
{
}

// �_���[�W
void TobiAttackSpreadBase::Damege( BattleSystem& System )
{
	typedef BattleTagetSystem::CharaItr CharaItr;

	if( pMember == NULL )return;

	BattleTagetSystem::CharaList TagetList;
	long Count = System.GetCharaList( TagetList, Pos, SpreadRange, pMember->GetArmyID(), true, bAllHit );
	if( Count == 0 )return;

	CharaItr itr( TagetList );
	const float NewWait = GetWait();
	while( !itr.IsEnd() )
	{
		if( IsNewHit(itr)  )
		{
			DamegeSystem DSys( *pMember, *itr );
			DSys.AttackDamege( AttackPow, GetKnockBack(Pos, *itr), NewWait );
		}
		itr.Next();
	}
}
