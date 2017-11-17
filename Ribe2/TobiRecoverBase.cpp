// TobiRecoverBase.cpp: TobiRecoverBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiRecoverBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiRecoverBase::TobiRecoverBase()
{
}

TobiRecoverBase::~TobiRecoverBase()
{
}

// スキル効果設定
bool TobiRecoverBase::SetEffect( const SkillEffect& Effect )
{
	MyEffect = Effect;
	return true;
}

// 仕事
bool TobiRecoverBase::Job( BattleSystem& System )
{
	Damege( System );

	return false;
}

// データセット
bool TobiRecoverBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;

	Pos = BasePos;
	TagetPos = NewTagetPos;

	return true;
}

void TobiRecoverBase::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, MyEffect.Range, pMember->GetArmyID(), false );

	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
	//	if( !IsNewHit(&Ref) )continue;
		Heal( *(*itr++) );
	}
}

bool TobiRecoverBase::Heal( RbActStatus& Taget)
{
	return true;
}
