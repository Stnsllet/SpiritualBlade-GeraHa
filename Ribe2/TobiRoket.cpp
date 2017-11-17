// TobiRoket.cpp: TobiRoket �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiSelfBomb.h"
#include "BattleSystem.h"
#include "TobiRoket.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiRoket::TobiRoket()
{
	MaxSpeed = Speed;
	DownSpeed = MaxSpeed/100.0f;
	TurnSpeed = MaxSpeed/10.0f;
}

bool TobiRoket::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("missile"));
	SetPolygon( MyPolyID );

	return true;
}

TobiRoket::~TobiRoket()
{
}

// �_���[�W����Ȃ��ăI�u�W�F�N�g�����
void TobiRoket::Damege( BattleSystem& System )
{
	SkillEffect Effect;
	Effect.Pow = AttackPow;
	Effect.Range = 80;
	TobiRefPtr<TobiSelfBomb> pBomb( Effect );
	if( !pBomb->SetTagetPos( pMember, Pos, Pos ) )return;
	System.TobiSys.Add( pBomb );
	return;
}
