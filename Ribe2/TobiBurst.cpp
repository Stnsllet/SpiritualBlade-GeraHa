// TobiBurst.cpp: TobiBurst クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiBurst.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiBurst::TobiBurst()
{
	bAllHit = false;
}

bool TobiBurst::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("shock"));
	SetPolygon( MyPolyID );

	return true;
}

TobiBurst::~TobiBurst()
{
}
