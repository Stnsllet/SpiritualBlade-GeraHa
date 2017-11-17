// TobiDeathSickle.cpp: TobiDeathSickle クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"
#include "RbSound.h"
#include "AngleChecker.h"
#include "TobiDeathSickle.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiDeathSickle::TobiDeathSickle()
{
	bAllHit = true;
	DmgWait = 1000;


	StartAngle = 0;
	NowAngle = 0.0f;
}

bool TobiDeathSickle::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("kain"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("sickle"));
	SetPolygon( MyPolyID );

	return true;
}

TobiDeathSickle::~TobiDeathSickle()
{

}

// 描写
void TobiDeathSickle::Draw( Display& Disp )
{
	RbTransform Trans;
	Trans.Scale( SpreadMaxRange );
	Trans.RotateZ( StartAngle+NowAngle );
	Trans.AddScroll( Pos );

	DrawPolygon( Disp, Trans );
}

// データセット
bool TobiDeathSickle::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( !TobiSpreadBase::SetTagetPos(pSource,BasePos,NewTagetPos) )return false;

	Locker.Lock( pSource->ActLock );
	StartAngle = pSource->GetAngle()-1.0f;
	IsNewHit( pSource );
	return true;
}

// 仕事
bool TobiDeathSickle::Job( BattleSystem& System )
{
	if( pMember == NULL || pMember->IsDead() )return false;
	NowAngle += 2.0f*System.JobSec();
	Pos = pMember->GetPosition();

	Damege( System );
	if( NowAngle >= 2.0f )return false;

	return true;
}

// ダメージ
void TobiDeathSickle::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;

	BattleTagetSystem::CharaList TagetList;
	long Count = System.GetCharaList( TagetList, Pos, SpreadMaxRange, pMember->GetArmyID(), true, bAllHit );
	if( Count == 0 )return;

	AngleChecker::Checker CheckAngle;
	CheckAngle.Set( StartAngle, StartAngle+NowAngle );
	BattleTagetSystem::CharaList::const_iterator itr = TagetList.begin();
	while( itr != TagetList.end() )
	{
		RbActStatus& Ref = *(*itr++);

		const float Angle = Pos.GetDirection( Ref.GetPosition() );
		if( !CheckAngle.Check( Angle ) )continue;
		if( !IsNewHit(&Ref) )continue;
		DamegeSystem DSys( *pMember, Ref );
		DSys.DeathDamege( AttackPow, GetKnockBack(Pos,Ref), DmgWait );
	}
}
