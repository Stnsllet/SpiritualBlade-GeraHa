// TobiNoahBit.cpp: TobiNoahBit �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiNoahBit.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiNoahBit::TobiNoahBit()
{
	bAllHit = false;
	SpreadKnockBack = 5;
	DmgWait = 1000;
}

bool TobiNoahBit::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("water"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("splash"));
	SetPolygon( MyPolyID );

	return true;
}

TobiNoahBit::~TobiNoahBit()
{

}
