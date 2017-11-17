// TobiShockWave.cpp: TobiShockWave クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiShockWave.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiShockWave::TobiShockWave()
{
	bAllHit = false;
	SpreadKnockBack = 0;
	DmgWait = 3000;
}

bool TobiShockWave::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("shock"));
	SetPolygon( MyPolyID );

	return true;
}

TobiShockWave::~TobiShockWave()
{
}
