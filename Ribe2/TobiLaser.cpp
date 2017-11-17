// TobiLaser.cpp: TobiLaser �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiLaser.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiLaser::TobiLaser()
{
	DmgWait = 900;
}

bool TobiLaser::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("ray"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("ray"));
	SetPolygon( MyPolyID );

	return true;
}

TobiLaser::~TobiLaser()
{
}
