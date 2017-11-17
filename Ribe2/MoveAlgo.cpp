// MoveAlgo.cpp: MoveAlgo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "MoveLiner.h"
#include "BattleStatus.h"
#include "RbBattlePaty.h"
#include "RbMember.h"
#include "RbTimer.h"
#include "RbCache.h"
#include "MoveAlgo.h"
#include "TacticsAlgo.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MoveAlgo::MoveAlgo( RbActStatus& RefState, BattleStatus& RefBattle, BattleSystem& RefSystem )
: Ref(RefState), NowPos( RefBattle.Pos ), Angle( RefBattle.Direction ), bLeader( RefBattle.bLeader )
, FlyHight(RefBattle.FlyHight)
, System( RefSystem )
{
	AllZero( KnockBack );
	KnockBackLength = 0;
	Running = 0;
	LastMoveTime = 0;
	MoveMainSpeed = MoveSpeed = 100 * System.JobSec();
	MoveTypeSpeed = 1.0f;
	AllZero( Form );
	pMoveDef = NULL;
	bFromUser = bForceMove = false;
	bDontMove = false;
	AllZero(ArmyPos);
}

MoveAlgo::~MoveAlgo()
{
}

bool MoveAlgo::IsUnRadar() const
{
	if( !LandData.bUnRadar )return false;
	return (Ref.GetArmyID()!=0);
}

bool MoveAlgo::IsUnbush() const
{
	return LandData.bUnRadar;
}

bool MoveAlgo::KnockBackJob()
{
	if( KnockBackLength < 1 )return false;
	if( bDontMove )return false;
	RbPosition NewPos = NowPos;
	NewPos += KnockBack;

	const float Down = max(1.0f,KnockBackLength/10.0f);
	KnockBack *= (KnockBackLength-Down)/KnockBackLength;
	KnockBackLength -= Down;

	if( !System.Land.MoveOK(NewPos,Ref.Size()*1.2f, 48.0f+Ref.Size()) )return false;

	NowPos = NewPos;
	ResetLand();
	return true;
}

bool MoveAlgo::AddKnockBack( const D3DVECTOR& Add )
{
	KnockBack += Add;
	KnockBackLength = KnockBack.Length();

	return true;
}

bool MoveAlgo::NoKnockBack()
{
	KnockBack = RbPosition(0,0,0);
	KnockBackLength = 0;

	return true;
}

bool MoveAlgo::MoveInterval( double Check ) const
{
	return (System.Passed(LastMoveTime)>Check);
}

const RbPosition& MoveAlgo::RefLeaderPos() const
{
	RefPtr<RbActStatus> pLeader = Ref.GetLeader();
	if( pLeader == NULL )return NowPos;
	return pLeader->GetPosition();
}

bool MoveAlgo::SetMoveSpeed( float NewSpeed )
{
	MoveMainSpeed = NewSpeed * 100 * System.JobSec();
	MoveSpeed = MoveTypeSpeed * MoveMainSpeed;

	return true;
}

bool MoveAlgo::SetMoveType( const String& Type )
{
	pMoveDef = &( GetCache().Land.MoveDat( Type ) );
	MoveTypeSpeed = pMoveDef->RefSpeed();
	MoveSpeed = MoveTypeSpeed * MoveMainSpeed;

	return true;
}

float MoveAlgo::ResetAngle()
{
	return SetAngle( MoveTo );
}

float MoveAlgo::SetAngle( const RbPointF& DestPos )
{
	Angle = AClip( NowPos.GetDirection( DestPos.x, DestPos.y ) );
	return Angle;
}

bool MoveAlgo::SetPosition( const RbPosition& Pos )
{
	if( !System.Land.MoveOK(Pos,Ref.Size()) )return false;
	NowPos = Pos;
	ResetLand();

	return true;
}

bool MoveAlgo::InMovePos() const
{
	float Range = Ref.Size()/2.0f;
	Range *= Range;
	return ( Range <= MoveTo.Length2(RbPointF(NowPos.x,NowPos.y)) );
}

bool MoveAlgo::SetPosition( const RbPosition& LeaderPos, const RbPointF& FormPos, float NewAngle )
{
	NewAngle = AClip( NewAngle );
	RbMember Mem( Ref.MemberState() );

	if( Ref.IsFly() )SetMoveType( _T("fly") );
	else SetMoveType( Mem.GetMoveType() );
	const BattleStatus& RefBattle = Ref.RefBattleStatus();
	bDontMove = (RefBattle.BattleType == eBatDontMove)||(RefBattle.BattleType == eBatNotBat);

	NowPos = LeaderPos;
	Form.BasePos = FormPos;
	Form.Pos = Form.BasePos * System.FormSize;

	Angle = NewAngle;
	RbPointF Vector = Form.Pos;
	Vector.Rotate( NewAngle );
	NowPos.x += Vector.x;
	NowPos.y += Vector.y;
	NowPos = System.Land.MovingPos( NowPos, LeaderPos, Ref.Size() );
	ResetLand();

	MoveTo.x = NowPos.x;
	MoveTo.y= NowPos.y;

	AllZero( Form.TagetMovePos );
//	float TagetShift = Trim( FormPos.x, 2.0f, -2.0f )/2.0f;
//	Form.TagetMovePos.x = Ref.Size()*TagetShift;
	if( FormPos.x > 0 )Form.TagetMovePos.x = Ref.Size();
	else if( FormPos.x < 0 )Form.TagetMovePos.x = -Ref.Size();

	Form.CheckSize2 = Ref.Size()/4;
	Form.CheckSize2 = Form.CheckSize2*Form.CheckSize2;
	return true;
}

void MoveAlgo::ResetLand()
{
	int FacePos = System.Land.Map.GetFacePosition( NowPos.x, NowPos.y );
	NowPos.z = LandData.LandHight = System.Land.GetHight( NowPos.x, NowPos.y, FacePos );
	NowPos.z += FlyHight;
	((LandMoveType::MoveData&)LandData) = pMoveDef->RefData( System.Land.Map.GetMapID(FacePos).Move );
}

bool MoveAlgo::SetMovePos( RbActStatus* pTaget, bool bEscapeOK )
{
	if( pTaget == NULL )return false;
	const RbPosition& Pos = pTaget->GetPosition();

	if( bLeader )
	{
		if( Ref.IsPatyMoving() )return false;
		const RbPointF& ShiftPos = GetArmyPos(Pos.x,Pos.y);
		return SetMovePos( Pos.x + ShiftPos.x, Pos.y + ShiftPos.y, bEscapeOK );
	}

	RbPointF TagetPos = Form.TagetMovePos;
	if( TagetPos.x == 0 )TagetPos.x = -Ref.Size();
//	if( TagetPos.x == 0 )TagetPos.x = GetArmyPos() * Ref.Size();
	TagetPos.Rotate( Ref.GetPosition().GetDirection( Pos ) );

	RefPtr<RbBattlePaty> pPaty = Ref.GetPaty();

	long Count = pPaty->MemberCount();
	for( int i = 0; i < Count; i++ )
	{
		RefPtr<RbActStatus> pCheck = pPaty->GetMember(i);
		if( pCheck->IsDead() )continue;
		if( pCheck == &Ref )continue;
		if( Form.BasePos.x <= 0 )
		{
			if( Form.BasePos.x > pCheck->AlgoMove.Form.BasePos.x )continue;
			else if( Form.BasePos.x == pCheck->AlgoMove.Form.BasePos.x )
				if( Form.BasePos.y <= pCheck->AlgoMove.Form.BasePos.y )continue;
		}
		else if( Form.BasePos.x > 0 )
		{
			if( Form.BasePos.x < pCheck->AlgoMove.Form.BasePos.x )continue;
			else if( Form.BasePos.x == pCheck->AlgoMove.Form.BasePos.x )
				if( Form.BasePos.y <= pCheck->AlgoMove.Form.BasePos.y )continue;
		}
		if( !pCheck->InRange( Ref.GetPosition(), Ref.Size()/4 ) )continue;

		RbPosition NewPos = pCheck->GetPosition();
		NewPos.x += TagetPos.x;
		NewPos.y += TagetPos.y;
		while( pCheck->InRange( NewPos, Ref.Size()/3 ) )
		{
			if( TagetPos.x == 0 && TagetPos.y==0 )break;
			NewPos.x += TagetPos.x;TagetPos.y;
		}

		if( !System.Land.MoveOK(NewPos, Ref.Size()) )break;
		return SetMovePos( NewPos.x, NewPos.y, bEscapeOK );
	}

	RbPosition SetPos( Pos );
	SetPos.x += TagetPos.x;
	SetPos.y += TagetPos.y;
	if( System.Land.MoveOK( SetPos, Ref.Size() ) )
		return SetMovePos( SetPos.x, SetPos.y, bEscapeOK );
	return SetMovePos( Pos.x, Pos.y, bEscapeOK );
}

bool MoveAlgo::SetMovePos( float x, float y, bool bEscapeOK )
{
	if( Ref.IsDead() )return false;

	if( NowPos.NiarXY2( RbPosition(x,y) ) )return Stop();

	Move::List GetList;
	MoveLiner Mover(System.Land, Ref.Size());
	Mover.GetMoveList( GetList, RbPosition(x,y,0), NowPos );
	return SetMoveList( GetList, bEscapeOK );
}

bool MoveAlgo::SetMoveList( const Move::List NewList, bool bEscapeOK )
{
	if( Ref.IsDead() )return false;
	if( bDontMove )return false;
	MovingList = NewList;
	if( !MovingList )
	{
		Stop();
		return false;
	}
	RbPosition Pos = MovingList.pick();

	if( bLeader && (!Ref.IsPatyBattle() || System.BStatus.IsPatyMove()) )
	{
		RbActStatus::PatyState NewState = RbActStatus::ePatyMove;
		if( Ref.IsPatyBattle() && bEscapeOK )NewState = RbActStatus::ePatyEscape;

		Ref.PatyChangeState( NewState );
	}
	Move( Pos.x, Pos.y );
	bFromUser = bForceMove = bEscapeOK && Ref.IsPatyBattle();
	return true;
}

bool MoveAlgo::Move( float x, float y )
{
	if( Ref.IsDead() )return false;

	RbPosition DestPos( x, y, NowPos.z );
	RecheckMoveList( DestPos, Ref.Size() );

	if( !MovingList )return false;
	DestPos = MovingList.pick();

	MoveTo.x = DestPos.x;
	MoveTo.y = DestPos.y;
	if( Ref.IsActLock() )return false;

	Ref.ActStart( eActMove, false );
	SetAngle( MoveTo );

	LastMoveTime = System.GetBattleTime();
	TurnCheck();
	return true;
}

bool MoveAlgo::RecheckMoveList( const RbPosition& DestPos, float Size )
{
	MoveLiner Mover(System.Land, Size);
	if( !MovingList )
	{
		Mover.GetMoveList( MovingList, DestPos, NowPos );
	}
	else
	{
		Move::List GetList;
		Mover.GetMoveList( GetList, DestPos, NowPos );
		MovingList.Insert( GetList );
	}
	return !!MovingList;
}

bool MoveAlgo::CheckLeaderLength()
{
	if( bLeader )return false;
	RefPtr<RbActStatus> pLeader = Ref.GetLeader();
	if( pLeader == NULL )return false;

	const RbPosition& LeaderPos = pLeader->GetPosition();
	float Length = NowPos.Length(LeaderPos);
	if( Length <= System.BatIn )return false;
	if( pLeader->IsDead() )return false;
	RbPosition Pos( NowPos );
	Pos += LeaderPos;
	Pos += LeaderPos;
	Pos /= 3.0f;
	Pos = System.Land.MovingPos( Pos, LeaderPos, Ref.Size() );

	MovingList.Clear();
	bForceMove = Move( Pos.x, Pos.y );
	return bForceMove;
}

bool MoveAlgo::MoveToFormation()
{
	if( bLeader )return false;

	RbPosition NewForm = GetFormPos();

//	if( NowPos.x == NewForm.x
//		&& NowPos.y == NewForm.y )return true;
	if( NowPos.NiarXY2( NewForm, 1 ) )return true;

	MovingList.Clear();
	return Move( NewForm.x, NewForm.y );
}

bool MoveAlgo::ActMove()
{
	Ref.AddActTime(System);
	if( ActMove( MoveTo.x, MoveTo.y ) )
	{
		if( bLeader )Ref.BattleCheck( System );
		return true;
	}

	if( !MovingList )Stop();
	else
	{
		Move::MovePointer Pos = MovingList.pick();
		Move( Pos.x, Pos.y );
	}
	return false;
}

bool MoveAlgo::ActMove( float x, float y )
{
	if( TurnWait() )return true;

	RbPosition LengthPos( x, y, 0 );
	LengthPos -= NowPos;
	LengthPos.z = 0;

	float Speed = MoveSpeed * LandData.Move;
	float Length = LengthPos.Length();
	if( Length < Speed )
	{
		NowPos.x = x;
		NowPos.y = y;
		ResetLand();
		Running += Speed;
		return false;
	}
	Running += Speed;
	float Moveing = Speed / Length;
	NowPos.x += LengthPos.x * Moveing;
	NowPos.y += LengthPos.y * Moveing;
	ResetLand();

	return true;
}

bool MoveAlgo::Stop()
{
	Ref.Stop();
	bFromUser = bForceMove = false;
	return true;
}

bool MoveAlgo::Cancel()
{
	if( Ref.GetPatyState() == RbActStatus::ePatyEscape )return false;
	if( Ref.IsPatyBattle() && IsForce() )return false;
	MoveTo.x = NowPos.x;
	MoveTo.y = NowPos.y;
	bFromUser = bForceMove = false;
	return true;
}

bool MoveAlgo::TurnCheck()
{
	if( !bLeader )return false;
	if( !Ref.IsPatyMoving() )return false;

	if( PatyInFormation() )return false;
	Form.bTurnWait = true;
	return true;
}

bool MoveAlgo::TurnWait()
{
	if( !Form.bTurnWait )return false;
	if( !Ref.IsPatyMoving() )
	{
		Form.bTurnWait = false;
		return false;
	}

	if( !PatyInFormation() )return true;
	Form.bTurnWait = false;
	return false;
}

RbPosition MoveAlgo::GetFormPos() const
{
	if( bLeader )return NowPos;
	RefPtr<RbBattleStatus> pLeader = Ref.GetLeader();
	if( pLeader == NULL )return NowPos;
	const RbPosition& LeaderPos = pLeader->GetPosition();

	RbPosition NewForm = Form.Pos;
	NewForm.Rotate( pLeader->GetAngle() );

	NewForm += LeaderPos;
	NewForm = System.Land.MovingPos( NewForm, LeaderPos, Ref.Size()*1.2f );

	return NewForm;
}

bool MoveAlgo::InFormation() const
{
	if( bLeader )return true;
	if( Ref.IsDead() )return true;
	Ref.MoveCancel();
	return GetFormPos().NiarXY2(NowPos, Form.CheckSize2);
}

bool MoveAlgo::PatyInFormation() const
{
	RefPtr<RbBattlePaty> pPaty = Ref.GetPaty();
	if( pPaty == NULL )return true;
	if( MoveInterval( 5000 ) )return true;
	return pPaty->InFormation();
}

RbPointF MoveAlgo::GetArmyPos( float TagetX, float TagetY ) const
{
	if( System.GetBattleTime() - ArmyPos.CheckTime  < 1000 )return ArmyPos.Pos;

	ArmyPos.CheckTime = System.GetBattleTime();

	float Range = 0;
	RefPtr<RbActStatus> pFriend = System.GetFriendPaty( Ref, RbActStatus::ePatyBattle, &Range );
	if( pFriend == NULL )return AllZero(ArmyPos.Pos);
	if( Range > TacticsAlgo::TacticsRange )return AllZero(ArmyPos.Pos);

	ArmyPos.Pos.x = TagetX-NowPos.x;
	ArmyPos.Pos.y = TagetY-NowPos.y;
	const RbPosition& FriendPos = pFriend->GetPosition();
	const bool bCheck = (FriendPos.x-NowPos.x)*(ArmyPos.Pos.y)>(FriendPos.y-NowPos.y)*(ArmyPos.Pos.x);

	ArmyPos.Pos.Normalize( Ref.Size() );
	if( bCheck )ArmyPos.Pos.Rotate( 0.5f );
	else ArmyPos.Pos.Rotate( -0.5f );
	return ArmyPos.Pos;
}
