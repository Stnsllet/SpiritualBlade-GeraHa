// TobiMpRecover.cpp: TobiMpRecover クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "TobiMpRecover.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
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
