// TobiSwordDance.cpp: TobiSwordDance クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiMagicSword.h"
#include "TobiSwordDance.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
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
