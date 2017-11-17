// TagetAlgo.cpp: TagetAlgo �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "RbActStatus.h"
#include "TagetAlgo.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TagetAlgo::TagetAlgo( BattleSystem& RefSystem, RbActStatus& RefSource )
:System( RefSystem ), Source( RefSource )
{
	MinRange = (1<<30);
}

TagetAlgo::~TagetAlgo()
{
}

TagetAlgo::TimeTaget::TimeTaget()
{
	LastTime = Rnd2(0,300);
}

bool TagetAlgo::TimeTaget::CheckTime( BattleSystem& System ) const
{
	if( pTaget == NULL )return false;
	if( pTaget->IsDead() )return false;
	return System.GetBattleTime()<(LastTime+300);
}

void TagetAlgo::ClearTaget()
{
	MinTaget.pTaget = NULL;
	AttackTaget.pTaget = NULL;
}

// �߂��G�擾
RbActStatus* TagetAlgo::GetMinEnemy()
{
	if( MinTaget.CheckTime(System) )return MinTaget;
	MinTaget.LastTime = System.GetBattleTime();
//	MinRange = (1<<30);
	MinTaget.pTaget = System.GetEnemy( Source, &MinRange );
	return MinTaget;
}

// �U������G�擾
RbActStatus* TagetAlgo::GetAttackTaget()
{
	if( AttackTaget.CheckTime(System) )return MinTaget;
	AttackTaget.LastTime = System.GetBattleTime();
	AttackTaget.pTaget = System.GetRndEnemy( Source );
	return AttackTaget;
}
