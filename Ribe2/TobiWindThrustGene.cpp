// TobiWindThrustGene.cpp: TobiWindThrustGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiWindBit.h"
#include "TobiWindThrustGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiWindThrustGene::TobiWindThrustGene()
{
	GenerateInterval = 1000.0/3.0f;
	GenerateTime = GenerateInterval;
}

TobiWindThrustGene::~TobiWindThrustGene()
{

}

bool TobiWindThrustGene::Generate()
{
	TobiRefPtr<TobiWindBit> pBit( MyEffect );
	if( !pBit->SetSource( *pMember ) )return false;

	System.TobiSys.Add( pBit );
	return ((--MyEffect.Count)>0);
}
