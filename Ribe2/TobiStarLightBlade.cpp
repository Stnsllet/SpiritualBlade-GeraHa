// TobiStarLightBlade.cpp: TobiStarLightBlade �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiStar.h"
#include "TobiStarLightBlade.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiStarLightBlade::TobiStarLightBlade()
{
	GenerateTime = GenerateInterval = 100;
}

TobiStarLightBlade::~TobiStarLightBlade()
{
}

bool TobiStarLightBlade::Generate()
{
	TobiRefPtr<TobiStar> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
