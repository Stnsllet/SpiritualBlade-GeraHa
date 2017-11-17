// TobiNoahBit.cpp: TobiNoahBit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiNoahBit.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiNoahBit::TobiNoahBit()
{
	bAllHit = false;
	SpreadKnockBack = 5;
	DmgWait = 1000;
}

bool TobiNoahBit::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("water"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("splash"));
	SetPolygon( MyPolyID );

	return true;
}

TobiNoahBit::~TobiNoahBit()
{

}
