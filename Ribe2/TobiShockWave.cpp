// TobiShockWave.cpp: TobiShockWave �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiShockWave.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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
