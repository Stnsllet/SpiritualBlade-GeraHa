// TobiLightning.cpp: TobiLightning クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiThunder.h"
#include "TobiLightning.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
const float TobiLightning::LightDips = 44.0f;

TobiLightning::TobiLightning()
{
	GenerateTime = GenerateInterval = 200;
	MyRange = 0;
}

TobiLightning::~TobiLightning()
{
}

// 仕事
bool TobiLightning::Job( BattleSystem& System )
{
	return TobiGeneratorBase::Job( System );
}

TobiObjectBase* TobiLightning::CreateObject() const
{
	return new TobiThunder;
}

// スキル効果設定
bool TobiLightning::SetEffect( const SkillEffect& Effect )
{
	bool bRet = TobiArrowRain::SetEffect( Effect );
	MyEffect.Count = Effect.Range / LightDips;
	bActLock = true;
	return bRet;
}

bool TobiLightning::Generate()
{
	MyRange += LightDips;
	CreateCircleRain( MyRange, LightDips );
	return TobiGeneratorBase::Generate();
}
