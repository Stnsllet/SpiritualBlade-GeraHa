// TobiFrameCircleGene.cpp: TobiFrameCircleGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiFrameCircleBit.h"
#include "TobiFrameCircleGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiFrameCircleGene::TobiFrameCircleGene()
{

}

TobiFrameCircleGene::~TobiFrameCircleGene()
{

}

// スキル効果設定
bool TobiFrameCircleGene::SetEffect( const SkillEffect& Effect )
{
	bool bRet = TobiGeneratorBase::SetEffect( Effect );
	bActLock = false;
	return bRet;
}

// 仕事
bool TobiFrameCircleGene::Job( BattleSystem& System )
{
	CreateCircle( MyEffect );
	return false;
}

bool TobiFrameCircleGene::CreateCircle( const SkillEffect& Effect )
{
	if( Effect.Count <= 0 )return false;
	float Angle = 2.0f / Effect.Count;

	for( int i = 0; i < Effect.Count; i++ )
	{
		CreateFlameBit( MyEffect, Angle*i );
	}
	return true;
}

bool TobiFrameCircleGene::CreateFlameBit( const SkillEffect& Effect, float StartAngle )
{
	if( pMember == NULL )return false;
	RefPtr<TobiFrameCircleBit> pBit = CreateFlameObject();
	if( pBit == NULL )return false;

	pBit->SetStartAngle( StartAngle );
	pBit->SetResource();
	pBit->SetEffect( MyEffect );
	if( !pBit->SetSource( *pMember ) )return false;
	System.TobiSys.Add( pBit );
	return true;
}

TobiFrameCircleBit* TobiFrameCircleGene::CreateFlameObject() const
{
	return new TobiFrameCircleBit;
}
