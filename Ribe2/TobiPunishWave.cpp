// TobiPunishWave.cpp: TobiPunishWave クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiBlueWave.h"
#include "TobiPunishWave.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiPunishWave::TobiPunishWave()
{
	GenerateTime = GenerateInterval = 100;
}

TobiPunishWave::~TobiPunishWave()
{

}

bool TobiPunishWave::Generate()
{
	TobiRefPtr<TobiBlueWave> pBit(MyEffect);
	if( !pBit->SetSource(*pMember) )return false;
	System.TobiSys.Add( pBit );
	return TobiGeneratorBase::Generate();
}
