// TobiHpRecover.cpp: TobiHpRecover �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "TobiHpRecover.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiHpRecover::TobiHpRecover()
{
}

TobiHpRecover::~TobiHpRecover()
{
}

bool TobiHpRecover::Heal( RbActStatus& Taget)
{
	DamegeSystem Dmg(Taget,Taget);
	Dmg.HpRecover( MyEffect.Pow );
	return true;
}
