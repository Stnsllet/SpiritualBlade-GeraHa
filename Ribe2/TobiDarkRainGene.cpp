// TobiDarkRainGene.cpp: TobiDarkRainGene �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDarkRainBit.h"
#include "TobiDarkRainGene.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiDarkRainGene::TobiDarkRainGene()
{

}

TobiDarkRainGene::~TobiDarkRainGene()
{

}

TobiObjectBase* TobiDarkRainGene::CreateObject() const
{
	return new TobiDarkRainBit;
}
