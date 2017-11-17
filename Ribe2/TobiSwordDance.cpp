// TobiSwordDance.cpp: TobiSwordDance �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiMagicSword.h"
#include "TobiSwordDance.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiSwordDance::TobiSwordDance()
{
	GenerateTime = GenerateInterval = 300;
}

TobiSwordDance::~TobiSwordDance()
{
}

bool TobiSwordDance::Generate()
{
	TobiRefPtr<TobiMagicSword> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
