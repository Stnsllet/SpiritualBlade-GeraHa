// TacticsAlgo.cpp: TacticsAlgo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "RbActStatus.h"
#include "BattleSystem.h"
#include "TacticsAlgo.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

float TacticsAlgo::TacticsRange = 170;
double TacticsAlgo::CheckInterval = 500;

TacticsAlgoType TacticsAlgo::GetTactics( const String& Name )
{
	if( Name == _T("attack") )return eTacticsAttack;
	else if( Name == _T("flying") )return eTacticsSyncAttack;
	else if( Name == _T("help") )return eTacticsHelp;
	else if( Name == _T("walking") )return eTacticsRndMove;

	return eTacticsNone;
}

TacticsAlgo::TacticsAlgo( RbActStatus& Ref, BattleSystem& RefSystem )
: Source( Ref ), System( RefSystem )
{
	Type = eTacticsNone;
	LastCheckTime = Rnd2(0,(long)CheckInterval) + System.GetBattleTime();
}

TacticsAlgo::~TacticsAlgo()
{
}

bool TacticsAlgo::CheckTactics()
{
	if( System.Passed(LastCheckTime) < CheckInterval )return false;
	LastCheckTime = System.GetBattleTime();
	switch( Type )
	{
	case eTacticsAttack:
		return AttackAlgo();
	case eTacticsSyncAttack:
		return SyncAttackAlgo();
	case eTacticsHelp:
		return HelpAlgo();
	case eTacticsRndMove:
		return RndMoveAlgo();
	}
	return false;
}

bool TacticsAlgo::ForceTactics() const
{
	switch( Type )
	{
	case eTacticsAttack:
	case eTacticsSyncAttack:
	case eTacticsHelp:
		return true;
	}
	return false;
}

bool TacticsAlgo::MovePosCheck( const RbPosition& Pos ) const
{
	RefPtr<RbActStatus> pFriend = System.GetFriendPaty( Pos, Source.GetArmyID(), RbActStatus::ePatyNoState, NULL, Source.GetPaty() );
	if( pFriend == NULL )return true;
	if( pFriend->AlgoTactics.RefTactics() != eTacticsHelp )return true;
	const float Friendlength = Pos.LenXY2( pFriend->GetPosition() );
	if( Friendlength > TacticsRange*TacticsRange-10.0f )return true;
	return ( Friendlength > Pos.LenXY2( Source.GetPosition() ) );
}

RbPosition TacticsAlgo::GetHalfPos( const RbPosition& Taget, const RbPosition& Base ) const
{
	RbPosition Ret = Taget;
//	Ret *= 3;
	Ret += Base;
//	Ret *= 0.25f;
	Ret *= 0.5f;
	return System.Land.MovingPos( Ret, Taget, Source.Size() );
}

RbPosition TacticsAlgo::_GetMovePosition( const RbPosition& BasePos, float BaseAngle ) const
{
	static const float AngleDips = 2.0f/6.0f;
	long ArmyID = Source.GetArmyID();

	RbPosition Vector(0,-TacticsRange,0);
	Vector.Rotate( BaseAngle );

	RbPosition Pos = BasePos+Vector;
	if( MovePosCheck(Pos) )return Pos;

	for( int i = 1; i < 3; i++ )
	{
		Pos = Vector;
		Pos.Rotate( AngleDips*i );
		Pos += BasePos;
		if( MovePosCheck(Pos) )return Pos;

		Pos = Vector;
		Pos.Rotate( -AngleDips*i );
		Pos += BasePos;
		if( MovePosCheck(Pos) )return Pos;
	}
	Pos = Vector;
	Pos.Rotate( -AngleDips*i );
	Pos += BasePos;
	return Pos;
}

RbPosition TacticsAlgo::GetMovePosition( const RbPosition& BasePos, float BaseAngle ) const
{
	return System.Land.MovingPos( GetHalfPos(_GetMovePosition( BasePos, BaseAngle ),BasePos), BasePos, Source.Size() );
}

bool TacticsAlgo::AttackAlgo()
{
//	RefPtr<RbActStatus> pEnemy = Source.TagetSys.GetMinEnemy();
	RefPtr<RbActStatus> pEnemy = System.FindEnemy( Source );
	if( AttackAlgo( pEnemy ) )return true;
	return SyncAttackAlgo();
}

bool TacticsAlgo::AttackAlgo( RbActStatus* pEnemy )
{
	if( pEnemy == NULL )return false;
	const RbPosition& Pos =ArmyTacticsPos( GetHalfPos( pEnemy->GetPosition(), Source.GetPosition() ) );
	Source.AlgoMove.SetMovePos( Pos.x, Pos.y );
	return true;
}

bool TacticsAlgo::HelpAlgo()
{
	RefPtr<RbActStatus> pFriend = System.GetUnHelpFriendPaty( Source );
	return HelpAlgo( pFriend );
}

bool TacticsAlgo::SyncAttackAlgo()
{
	RefPtr<RbActStatus> pFriend = System.GetFriendPaty( Source, RbActStatus::ePatyBattle );
	if( pFriend == NULL )return false;
	RefPtr<RbActStatus> pEnemy = pFriend->TagetSys.GetMinEnemy();

	if( pEnemy == NULL )return HelpAlgo( pFriend );

	return HelpAttack( pFriend, pEnemy);
}

bool TacticsAlgo::HelpAlgo( RbActStatus* pFriend )
{
	if( pFriend == NULL )return false;
	const RbPosition& FriendPos = pFriend->GetPosition();
	const RbPosition& Pos = Source.GetPosition();
	float Length = Pos.Length( FriendPos );
	if( Length < TacticsRange*1.1f )
	{
		if( pFriend->GetPatyState() != RbActStatus::ePatyBattle )return true;
		RefPtr<RbActStatus> pEnemy = pFriend->TagetSys.GetMinEnemy();
		HelpAttack( pFriend, pEnemy );
		return true;
	}

	RbPosition MovePos = GetMovePosition( FriendPos, FriendPos.GetDirection(Pos) );
	Source.AlgoMove.SetMovePos( MovePos.x, MovePos.y );
	return true;
}

bool TacticsAlgo::HelpAttack( RbActStatus* pFriend, RbActStatus* pEnemy )
{
	if( pFriend == NULL )return false;
	if( pEnemy == NULL )return false;
	if( pEnemy->IsDead() )return false;
	if( pFriend->GetPosition().Length(pEnemy->GetPosition())
		>= System.BatKeep )return false;

	return AttackAlgo( pEnemy );
}


bool TacticsAlgo::RndMoveAlgo()
{
	const RbPosition& Pos = System.GetRndPosition( Source.Size()*2.0f );
	if( Pos.LenXY2(Source.GetPosition()) <= Source.Size() )return false;
	Source.AlgoMove.SetMovePos( Pos.x, Pos.y );
	return true;
}

//#include "RbWIndowSystem.h"

RbPosition TacticsAlgo::ArmyTacticsPos( const RbPosition& Taget ) const
{
	const RbPosition& BasePos = Source.GetPosition();
	const float ArmyPos = System.GetArmyPos( BasePos, Taget, Source.GetArmyID() );
	if( ArmyPos == 0 )return Taget;
//	if( ArmyPos > 0.5 )GetWindowSystem().PopupMsg(&Source, _T("右翼") );
//	if( ArmyPos < -0.5 )GetWindowSystem().PopupMsg(&Source, _T("左翼") );

	float Length = BasePos.LenXY( Taget );
	if( Length <= TacticsRange*3 )Length = TacticsRange;
	else Length = TacticsRange*(TacticsRange*3)/Length;
	Length *= ArmyPos*::sqrtf((ArmyPos<0)?(-ArmyPos):(ArmyPos));

	RbPosition Pos = Taget;
	Pos -= Source.GetPosition();
	Pos.Normalize( Length );
	Pos.Rotate( 0.5f );
	Pos += Taget;
	if( !System.Land.MoveOK( Pos, TacticsRange, TacticsRange/2.0f ) )return Taget;

	return Pos;
}
