// TobiFlameBirdGene.cpp: TobiFlameBirdGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiFenix.h"
#include "BattleSystem.h"
#include "TobiFlameBirdGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiFlameBirdGene::TobiFlameBirdGene()
{
	GenerateTime = GenerateInterval = 200;
}

TobiFlameBirdGene::~TobiFlameBirdGene()
{
}

bool TobiFlameBirdGene::Generate()
{
	TobiRefPtr<TobiFenix> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
