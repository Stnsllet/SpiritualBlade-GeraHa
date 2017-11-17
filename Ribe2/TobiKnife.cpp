// TobiKnife.cpp: TobiKnife クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiKnife.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiKnife::TobiKnife()
{
	DmgWait = 750;
}

bool TobiKnife::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("suka"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("knife"));
	SetPolygon( MyPolyID );

	return true;
}

TobiKnife::~TobiKnife()
{
}
