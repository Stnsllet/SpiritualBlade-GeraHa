// TobiBlastEdge.cpp: TobiBlastEdge �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiBlastEdge.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiBlastEdge::TobiBlastEdge()
{
	bAllHit = false;
}

bool TobiBlastEdge::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("kain"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("edge"));
	SetPolygon( MyPolyID );

	return true;
}

TobiBlastEdge::~TobiBlastEdge()
{

}
