// TobiHeatWave.cpp: TobiHeatWave クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbSound.h"
#include "TobiHeatWave.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiHeatWave::TobiHeatWave()
{
}

bool TobiHeatWave::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("heat"));
	SetPolygon( MyPolyID );

	return true;
}

TobiHeatWave::~TobiHeatWave()
{
}
