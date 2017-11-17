// TobiWaterBitGene.cpp: TobiWaterBitGene �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiWaterBit.h"
#include "BattleSystem.h"
#include "TobiWaterBitGene.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiWaterBitGene::TobiWaterBitGene()
{
	GeneCount = 3;
	GenerateInterval = 1000.0/GeneCount;
	GenerateTime = GenerateInterval;
}

TobiWaterBitGene::~TobiWaterBitGene()
{
}

// �X�L�����ʐݒ�
bool TobiWaterBitGene::SetEffect( const SkillEffect& Effect )
{
	bool bRet = TobiGeneratorBase::SetEffect( Effect );
	bActLock = true;
	return bRet;
}

bool TobiWaterBitGene::Generate()
{
	TobiRefPtr<TobiWaterBit> pBit( MyEffect );
	if( !pBit->SetSource( *pMember ) )return false;

	System.TobiSys.Add( pBit );
	return ((--GeneCount)>0);
}
