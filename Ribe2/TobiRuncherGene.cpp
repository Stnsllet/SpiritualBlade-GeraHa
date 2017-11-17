// TobiRuncherGene.cpp: TobiRuncherGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiRoket.h"
#include "BattleSystem.h"
#include "TobiRuncherGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiRuncherGene::TobiRuncherGene()
{
	GenerateTime = GenerateInterval = 300;
}

TobiRuncherGene::~TobiRuncherGene()
{
}

bool TobiRuncherGene::Generate()
{
	TobiRefPtr<TobiRoket> pBit(MyEffect);
	return AddRandomTaget( pBit, MyEffect.Range );
}
