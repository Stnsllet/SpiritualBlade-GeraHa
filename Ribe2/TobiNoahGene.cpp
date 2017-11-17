// TobiNoahGene.cpp: TobiNoahGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiNoahBit.h"
#include "BattleSystem.h"
#include "TobiNoahGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiNoahGene::TobiNoahGene()
{
	GenerateTime = GenerateInterval = 200;
}

TobiNoahGene::~TobiNoahGene()
{
}

bool TobiNoahGene::Generate()
{
	TobiRefPtr<TobiNoahBit> pBit(MyEffect);
	if( !pBit->SetSource(*pMember) )return false;
	System.TobiSys.Add( pBit );
	return TobiGeneratorBase::Generate();
}
