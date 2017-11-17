// TobiMeteorGene.cpp: TobiMeteorGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiComet.h"
#include "BattleSystem.h"
#include "TobiMeteorGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiMeteorGene::TobiMeteorGene()
{
	GenerateTime = GenerateInterval = 200;
}

TobiMeteorGene::~TobiMeteorGene()
{
}

bool TobiMeteorGene::Generate()
{
	TobiRefPtr<TobiComet> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
