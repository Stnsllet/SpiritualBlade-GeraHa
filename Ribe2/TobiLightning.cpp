// TobiLightning.cpp: TobiLightning �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiThunder.h"
#include "TobiLightning.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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

// �d��
bool TobiLightning::Job( BattleSystem& System )
{
	return TobiGeneratorBase::Job( System );
}

TobiObjectBase* TobiLightning::CreateObject() const
{
	return new TobiThunder;
}

// �X�L�����ʐݒ�
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
