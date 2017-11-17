// TobiMeteorGene.cpp: TobiMeteorGene �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiComet.h"
#include "BattleSystem.h"
#include "TobiMeteorGene.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiMeteorGene::TobiMeteorGene()
{
	GenerateTime = GenerateInterval = 200;
}

TobiMeteorGene::~TobiMeteorGene()
{
}

bool TobiMeteorGene::Generate()
{
	TobiRefPtr<TobiComet> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
