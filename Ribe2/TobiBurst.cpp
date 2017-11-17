// TobiBurst.cpp: TobiBurst �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiBurst.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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
