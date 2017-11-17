// TobiSplash.cpp: TobiSplash クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiSplash.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiSplash::TobiSplash()
{
	bAllHit = false;
	SpreadKnockBack = 20;
	DmgWait = 1000;
}

bool TobiSplash::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("water"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("splash"));
	SetPolygon( MyPolyID );

	return true;
}

TobiSplash::~TobiSplash()
{
}
