// TobiFullRecover.cpp: TobiFullRecover �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "TobiFullRecover.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiFullRecover::TobiFullRecover()
{
}

TobiFullRecover::~TobiFullRecover()
{
}

bool TobiFullRecover::Heal( RbActStatus& Taget)
{
	DamegeSystem Dmg(Taget,Taget);
	Dmg.AllRecover( MyEffect.Pow );
	return true;
}
