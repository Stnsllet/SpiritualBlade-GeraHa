// TobiBlueWave.cpp: TobiBlueWave クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiBlueWave.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiBlueWave::TobiBlueWave()
{
	bAllHit = false;

	DmgWait = 1000.0f;
}

bool TobiBlueWave::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("bshock"));
	SetPolygon( MyPolyID );

	return true;
}

TobiBlueWave::~TobiBlueWave()
{

}
