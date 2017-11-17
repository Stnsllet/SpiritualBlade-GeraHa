// TobiMagicSword.cpp: TobiMagicSword �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiMagicSword.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiMagicSword::TobiMagicSword()
{
	KnockPow = 6;
	DmgWait = 750;
}

bool TobiMagicSword::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("suka"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("knife"));
	SetPolygon( MyPolyID );

	return true;
}

TobiMagicSword::~TobiMagicSword()
{

}
