// TobiGenocide.cpp: TobiGenocide �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiGenocide.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiGenocide::TobiGenocide()
{
	DmgWait = 1500;
}

TobiGenocide::~TobiGenocide()
{
}

// �d��
bool TobiGenocide::Job( BattleSystem& System )
{
	Damege( System );

	return false;
}

// �f�[�^�Z�b�g
bool TobiGenocide::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	pMember = pSource;
	return true;
}

// �_���[�W
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
