// TobiStarLightBlade.cpp: TobiStarLightBlade クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiStar.h"
#include "TobiStarLightBlade.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiStarLightBlade::TobiStarLightBlade()
{
	GenerateTime = GenerateInterval = 100;
}

TobiStarLightBlade::~TobiStarLightBlade()
{
}

bool TobiStarLightBlade::Generate()
{
	TobiRefPtr<TobiStar> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
