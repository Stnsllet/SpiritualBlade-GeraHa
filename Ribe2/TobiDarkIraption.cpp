// TobiDarkIraption.cpp: TobiDarkIraption �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDark.h"
#include "TobiDarkIraption.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiDarkIraption::TobiDarkIraption()
{
	GenerateTime = GenerateInterval = 100;
}

TobiDarkIraption::~TobiDarkIraption()
{

}

bool TobiDarkIraption::Generate()
{
	TobiRefPtr<TobiDark> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
