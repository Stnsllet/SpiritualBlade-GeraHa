// TobiDarkBlazeBit.cpp: TobiDarkBlazeBit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDarkBlazeBit.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiDarkBlazeBit::TobiDarkBlazeBit()
{
	bAllHit = false;
}

TobiDarkBlazeBit::~TobiDarkBlazeBit()
{

}

bool TobiDarkBlazeBit::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("dblaze"));
	SetPolygon( MyPolyID );

	return true;
}
