// TobiDarkRainBit.cpp: TobiDarkRainBit �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDarkRainBit.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiDarkRainBit::TobiDarkRainBit()
{

}

TobiDarkRainBit::~TobiDarkRainBit()
{

}

bool TobiDarkRainBit::SetResource()
{
	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("drain"));
	SetPolygon( MyPolyID );

	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("suka"));
	StartSoundNum = WaveID;

	return true;
}
