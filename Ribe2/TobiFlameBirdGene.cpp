// TobiFlameBirdGene.cpp: TobiFlameBirdGene �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiFenix.h"
#include "BattleSystem.h"
#include "TobiFlameBirdGene.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiFlameBirdGene::TobiFlameBirdGene()
{
	GenerateTime = GenerateInterval = 200;
}

TobiFlameBirdGene::~TobiFlameBirdGene()
{
}

bool TobiFlameBirdGene::Generate()
{
	TobiRefPtr<TobiFenix> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
