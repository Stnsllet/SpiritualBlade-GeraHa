// TobiIwasi.cpp: TobiIwasi �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"
#include "TobiIwasi.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

TobiIwasi::TobiIwasi()
{

}

TobiIwasi::~TobiIwasi()
{

}

bool TobiIwasi::SetResource()
{
	static long WaveNum = 0;
	if( WaveNum == 0 )WaveNum = GetWaveID(_T("gao"));
	StartSoundNum = WaveNum;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("iwasi"));
	SetPolygon( MyPolyID );

	return true;
}

void TobiIwasi::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, SmashRange, pMember->GetArmyID() );

	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		RbActStatus& Ref = *(*itr++);
		if( !IsNewHit(&Ref) )continue;
		DamegeSystem DSys( *pMember, Ref );
		DSys.MagicDamege( AttackPow, GetKnockBack(Pos,Ref), DmgWait );
	}
}
