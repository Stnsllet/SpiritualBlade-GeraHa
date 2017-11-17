// TobiFullRecover.cpp: TobiFullRecover クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "TobiFullRecover.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
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
