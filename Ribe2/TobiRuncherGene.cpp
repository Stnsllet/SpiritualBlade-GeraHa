// TobiRuncherGene.cpp: TobiRuncherGene �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiRoket.h"
#include "BattleSystem.h"
#include "TobiRuncherGene.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiRuncherGene::TobiRuncherGene()
{
	GenerateTime = GenerateInterval = 300;
}

TobiRuncherGene::~TobiRuncherGene()
{
}

bool TobiRuncherGene::Generate()
{
	TobiRefPtr<TobiRoket> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
