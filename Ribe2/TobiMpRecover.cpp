// TobiMpRecover.cpp: TobiMpRecover �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "TobiMpRecover.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiMpRecover::TobiMpRecover()
{
}

TobiMpRecover::~TobiMpRecover()
{
}

bool TobiMpRecover::Heal( RbActStatus& Taget)
{
	DamegeSystem Dmg(Taget,Taget);
	Dmg.MpRecover( MyEffect.Pow );
	return true;
}
