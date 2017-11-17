// TobiGeneratorBase.cpp: TobiGeneratorBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiGeneratorBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiGeneratorBase::TobiGeneratorBase()
{
	GenerateTime = 0;
	GenerateInterval = 0;
	bActLock = true;
}

TobiGeneratorBase::~TobiGeneratorBase()
{
}

// スキル効果設定
bool TobiGeneratorBase::SetEffect( const SkillEffect& Effect )
{
	MyEffect = Effect;
	if( MyEffect.Count <= 1 )bActLock = false;
	return TobiObjectBase::SetEffect( Effect );
}

// ランダムターゲットを指定して追加
bool TobiGeneratorBase::AddRandomTaget( TobiObjectBase* pBit, float Range )
{
	if( pBit == NULL )return false;
	if( pMember == NULL )return false;
	RefPtr<RbActStatus> pTaget
		= System.GetRndEnemy( Pos, pMember->GetArmyID(), Range );
	if( pTaget == NULL )return TobiGeneratorBase::Generate();
	if( !pBit->SetTagetPos(pMember, Pos, pTaget->GetPosition()) )return false;

	System.TobiSys.Add( pBit );
	return TobiGeneratorBase::Generate();
}

// 仕事
bool TobiGeneratorBase::Job( BattleSystem& System )
{
	if( pMember->IsDead() )return false;

	GenerateTime += System.JobTime();
	if( GenerateTime >= GenerateInterval )
	{
		GenerateTime -= GenerateInterval;
		return Generate();
	}
	return true;
}

// データセット
bool TobiGeneratorBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;
	Pos = BasePos;
	TagetPos = NewTagetPos;
	if( bActLock )Locker.Lock( pSource->ActLock );
	return true;
}

bool TobiGeneratorBase::Generate()
{
	return ((--MyEffect.Count)>0);
}