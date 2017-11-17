// TobiWhiteBlowGene.cpp: TobiWhiteBlowGene クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "AngleChecker.h"
#include "TobiWhiteBlowBit.h"
#include "TobiWhiteBlowGene.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiWhiteBlowGene::TobiWhiteBlowGene()
{
	StartAngle = 0;
	NowAngle = 0.0f;
}

TobiWhiteBlowGene::~TobiWhiteBlowGene()
{

}

// データセット
bool TobiWhiteBlowGene::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	bActLock = true;
	if( !TobiGeneratorBase::SetTagetPos( pSource, BasePos, NewTagetPos) )return false;
	StartAngle = pSource->GetAngle()-1.0f;
	return true;
}

// 仕事
bool TobiWhiteBlowGene::Job( BattleSystem& System )
{
	if( pMember->IsDead() )return false;

	NowAngle += 1.5f*2.0f*System.JobSec();
	Pos = pMember->GetPosition();

	BattleTagetSystem::CharaList TagetList;
	long Count = System.GetCharaList( TagetList, Pos, MyEffect.Range, pMember->GetArmyID(), true, false );
	if( Count == 0 )
		return ( NowAngle < 2.0f );

	AngleChecker::Checker CheckAngle;
	CheckAngle.Set( StartAngle, StartAngle+NowAngle );
	BattleTagetSystem::CharaList::const_iterator itr = TagetList.begin();
	while( itr != TagetList.end() )
	{
		RbActStatus& Ref = *(*itr++);

		const float Angle = Pos.GetDirection( Ref.GetPosition() );
		if( !CheckAngle.Check( Angle ) )continue;
		if( !IsNewHit(&Ref) )continue;
		AddBlow( Ref );
	}

	return ( NowAngle < 2.0f );
}

bool TobiWhiteBlowGene::AddBlow( RbActStatus& Taget )
{
	TobiRefPtr<TobiWhiteBlowBit> pBit(MyEffect);
	if( !pBit->SetTaget( pMember, &Taget ) )return false;
	System.TobiSys.Add( pBit );
	return TobiGeneratorBase::Generate();
}
