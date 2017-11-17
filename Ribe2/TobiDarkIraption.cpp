// TobiDarkIraption.cpp: TobiDarkIraption クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDark.h"
#include "TobiDarkIraption.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
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
