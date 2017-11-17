// TobiSplash.cpp: TobiSplash �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiSplash.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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
