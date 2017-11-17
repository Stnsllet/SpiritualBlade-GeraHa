// TobiHpRecover.cpp: TobiHpRecover クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "TobiHpRecover.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
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
