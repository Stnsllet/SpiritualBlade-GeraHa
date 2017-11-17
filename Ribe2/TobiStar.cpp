// TobiStar.cpp: TobiStar クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiStar.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiStar::TobiStar()
{
	Speed *= 2;
	MaxSpeed = Speed;
	DownSpeed = MaxSpeed/75.0f;
	TurnSpeed = MaxSpeed/10.0f;
}

bool TobiStar::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("suka"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("lightbit"));
	SetPolygon( MyPolyID );

	return true;
}

TobiStar::~TobiStar()
{
}
