// TobiWhiteNoise.cpp: TobiWhiteNoise クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiWhiteNoise.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiWhiteNoise::TobiWhiteNoise()
{
	bAllHit = false;

	SpreadKnockBack = 2;
	DmgWait = 2000;
}

bool TobiWhiteNoise::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("wflare"));
	SetPolygon( MyPolyID );

	return true;
}

TobiWhiteNoise::~TobiWhiteNoise()
{

}
