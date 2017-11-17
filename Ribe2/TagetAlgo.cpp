// TagetAlgo.cpp: TagetAlgo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "RbActStatus.h"
#include "TagetAlgo.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
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

// 近い敵取得
RbActStatus* TagetAlgo::GetMinEnemy()
{
	if( MinTaget.CheckTime(System) )return MinTaget;
	MinTaget.LastTime = System.GetBattleTime();
//	MinRange = (1<<30);
	MinTaget.pTaget = System.GetEnemy( Source, &MinRange );
	return MinTaget;
}

// 攻撃する敵取得
RbActStatus* TagetAlgo::GetAttackTaget()
{
	if( AttackTaget.CheckTime(System) )return MinTaget;
	AttackTaget.LastTime = System.GetBattleTime();
	AttackTaget.pTaget = System.GetRndEnemy( Source );
	return AttackTaget;
}
