// TobiDarkBlazeBit.cpp: TobiDarkBlazeBit �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiDarkBlazeBit.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiDarkBlazeBit::TobiDarkBlazeBit()
{
	bAllHit = false;
}

TobiDarkBlazeBit::~TobiDarkBlazeBit()
{

}

bool TobiDarkBlazeBit::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("dblaze"));
	SetPolygon( MyPolyID );

	return true;
}
