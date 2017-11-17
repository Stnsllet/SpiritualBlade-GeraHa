// TobiSelfBomb.cpp: TobiSelfBomb �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiSelfBomb.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiSelfBomb::TobiSelfBomb()
{
	bAllHit = true;
}

bool TobiSelfBomb::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("selfbomb"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("flare"));
	SetPolygon( MyPolyID );

	return true;
}

TobiSelfBomb::~TobiSelfBomb()
{

}
