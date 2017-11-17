// TobiDarkRainBit.cpp: TobiDarkRainBit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDarkRainBit.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiDarkRainBit::TobiDarkRainBit()
{

}

TobiDarkRainBit::~TobiDarkRainBit()
{

}

bool TobiDarkRainBit::SetResource()
{
	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("drain"));
	SetPolygon( MyPolyID );

	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("suka"));
	StartSoundNum = WaveID;

	return true;
}
