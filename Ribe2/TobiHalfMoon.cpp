// TobiHalfMoon.cpp: TobiHalfMoon クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "TobiHalfMoon.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiHalfMoon::TobiHalfMoon()
{
	bAllHit = false;
	DmgWait = 1000;

	SpreadSpeed *= 3.0f;
	MyAngle = 0;
}

bool TobiHalfMoon::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("moon"));
	SetPolygon( MyPolyID );

	return true;
}

TobiHalfMoon::~TobiHalfMoon()
{

}

// データセット
bool TobiHalfMoon::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( !TobiSpreadBase::SetTagetPos( pSource, BasePos, NewTagetPos ) )return false;
	if( pMember == NULL )return false;

	MyAngle = pMember->GetLinerAngle( SpreadMaxRange, 2/12.0f, 0.75f );
	pMember->SetAngle( MyAngle );

	CheckAngle.Set( MyAngle-0.5f, MyAngle+0.5f );
	return true;
}

// 描写
void TobiHalfMoon::Draw( Display& Disp )
{
	RbTransform Trans;
	Trans.Scale( SpreadRange );
	Trans.RotateZ( MyAngle );
	Trans.AddScroll( Pos );

	DrawPolygon( Disp, Trans );
}

// ダメージ
void TobiHalfMoon::Damege( BattleSystem& System )
{
	typedef BattleTagetSystem::CharaItr CharaItr;

	if( pMember == NULL )return;

	BattleTagetSystem::CharaList TagetList;
	long Count = System.GetCharaList( TagetList, Pos, SpreadRange, pMember->GetArmyID(), true, bAllHit );
	if( Count == 0 )return;

	const float NewWait = GetWait();
	CharaItr itr( TagetList );
	while( !itr.IsEnd() )
	{
		RbActStatus& Ref = *itr;
		const float Angle = Pos.GetDirection( Ref.GetPosition() );
		if( CheckAngle.Check( Angle ) )
			if( IsNewHit(itr)  )
			{
				DamegeSystem DSys( *pMember, *itr );
				DSys.AttackDamege( AttackPow, GetKnockBack(Pos, Ref), NewWait );
			}
		itr.Next();
	}
}
