// TobiFlare.cpp: TobiFlare �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiFlare.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiFlare::TobiFlare()
{
	bAllHit = false;
}

bool TobiFlare::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("flare"));
	SetPolygon( MyPolyID );

	return true;
}

TobiFlare::~TobiFlare()
{

}
