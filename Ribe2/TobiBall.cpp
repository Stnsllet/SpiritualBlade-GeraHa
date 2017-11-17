// TobiBall.cpp: TobiBall クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiBurnFlare.h"
#include "TobiBall.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiBall::TobiBall()
{
	SpreadRange = 80;
}

TobiBall::~TobiBall()
{

}

RefPtr<TobiObjectBase> TobiBall::GetBurstObject()
{
	SkillEffect Effect;
	Effect.Pow = AttackPow;
	Effect.Range = SpreadRange;

	TobiRefPtr<TobiBurnFlare> pBurst( Effect );
	if( !pBurst->SetTagetPos( pMember, Pos, TagetPos ) )return NULL;
	return pBurst;
}

bool TobiBall::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	return TobiMagicBase::SetTagetPos( pSource, BasePos, NewTagetPos );
}
