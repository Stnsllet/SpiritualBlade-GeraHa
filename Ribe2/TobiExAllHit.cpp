// TobiExAllHit.cpp: TobiExAllHit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "AngleChecker.h"
#include "TobiExAllHit.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiExAllHit::TobiExAllHit()
{
}

TobiExAllHit::~TobiExAllHit()
{

}

// 仕事
bool TobiExAllHit::Job( BattleSystem& System )
{
	if( pMember->IsDead() )return false;

	NowAngle += 1.5f*2.0f*System.JobSec();
//	Pos = pMember->GetPosition();

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

bool TobiExAllHit::SetExDefine( ExCreator::BaseDef* pNewDef )
{
	if( pNewDef == NULL )return false;

	if( pNewDef->pChiled == NULL )return false;
	pBitCreator = System.TobiSys.GetCreator( pNewDef->pChiled->Name );
	ChiledRange = pNewDef->pChiled->Range;

	return true;
}

bool TobiExAllHit::AddBlow( RbActStatus& Taget )
{
	if( pBitCreator == NULL )return false;
	RefPtr<TobiObjectBase> pBit = pBitCreator->Create();
	if( pBit == NULL )return false;
	SkillEffect Effect = MyEffect;
	pBit->SetEffect( Effect );

	RbPosition TagetPos = Taget.GetPosition();
	TagetPos.z += Taget.Hight()/2.0f;
	if( !pBit->SetTagetPos( pMember, Pos, TagetPos ) )return false;
	System.TobiSys.Add( pBit );
	return TobiGeneratorBase::Generate();
}
