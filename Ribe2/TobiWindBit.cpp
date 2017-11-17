// TobiWindBit.cpp: TobiWindBit �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiWindBit.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiWindBit::TobiWindBit()
{
}

bool TobiWindBit::SetResource()
{
	static long WaveNum = 0;
	if( WaveNum == 0 )
		WaveNum = GetWaveID(_T("fire"));
	StartSoundNum = WaveNum;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("windbit"));
	SetPolygon( MyPolyID );

	return true;
}

TobiWindBit::~TobiWindBit()
{

}

bool TobiWindBit::SetEffect( const SkillEffect& Effect )
{
	if( !TobiLinerBase::SetEffect( Effect ) )return false;
	SmashRange = ShotRange/6.0f;
	return true;
}
