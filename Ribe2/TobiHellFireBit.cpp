// TobiHellFireBit.cpp: TobiHellFireBit �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "TobiHellFireBit.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiHellFireBit::TobiHellFireBit()
{
	bAllHit = true;
}

TobiHellFireBit::~TobiHellFireBit()
{

}

bool TobiHellFireBit::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	IsNewHit( pSource );
	return TobiFrameCircleBit::SetTagetPos( pSource, BasePos, NewTagetPos );
}

void TobiHellFireBit::Damege( BattleSystem& System )
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
		DSys.AttackDamege( AttackPow, GetKnockBack(Pos,Ref), NewWait );
	}
}
