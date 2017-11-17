// TobiLinerBase.cpp: TobiLinerBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"
#include "DispStocker.h"
#include "TobiLinerBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiLinerBase::TobiLinerBase()
:EchoImage( 0, 0 )
{
}

TobiLinerBase::~TobiLinerBase()
{
}

RbPosition TobiLinerBase::GetSideKnockBack( float VecAngle, float KnockVal, const RbPosition& Base, const RbActStatus& Taget ) const
{
	float Angle = Base.GetDirection(Taget.GetPosition());
	Angle -= VecAngle;
	if( Angle > 1 )Angle -= 2;
	else if( Angle < -1 )Angle += 2;
	RbPosition Knock = Vector;
	Knock.Normalize( KnockVal );
	if( Angle > 0 )Knock.Rotate( 0.5 );
	else Knock.Rotate( -0.5 );
	return Knock;
}

bool TobiLinerBase::Job( BattleSystem& System )
{
	Damege( System );
	if( Life > 1 )Pos += Vector;
	else Pos = TagetPos;

	bTransSetOK = false;
	if( (--Life) > 0 )return true;

	return false;
}

void TobiLinerBase::Draw( Display& Disp )
{
	ResetTrans();
	DrawPolygon( Disp, Trans );

	if( !EchoImage.IsEcho() )return;

	DispStocker::Material Mate( Disp );
	Draws::Echo<RbTransform>::citr itr = EchoImage.rbegin();
	while( itr.ok() )
	{
		float val = itr.val();
		const RbTransform& Ref = *itr++;
		Mate.SetAlpha( val );
		DrawPolygon( Disp, Ref );
	}
	EchoImage.Add( Trans, System.GetBattleTime() );
}

bool TobiLinerBase::ResetTrans()
{
	if( bTransSetOK )return false;
	Trans.FromPosture( Vector );
	Trans.Scale( SmashRange );
	Trans.AddScroll( Pos );
	bTransSetOK = true;
	return true;
}

bool TobiLinerBase::SetEffect( const SkillEffect& Effect )
{
	if( !TobiSingleBase::SetEffect( Effect ) )return false;
	ShotRange = Effect.Range;
	SmashRange = ShotRange/3.0f;
	return true;
}

bool TobiLinerBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;
	Pos = BasePos;

	pMember->SetAngle( SetAutoAngleTaget( ShotRange, 2/24.0f ) );

	PlayWave( StartSoundNum, Pos);

	return true;
}

float TobiLinerBase::SetAutoAngleTaget( float Range, float Dips, float Width )
{
	if( pMember == NULL )return 0;

	const float TagetAngle = pMember->GetLinerAngle( Range, Dips, Width );

	TagetPos = RbPosition( 0, -Range, 0 );
	TagetPos.Rotate( TagetAngle );
	TagetPos += Pos;

	Vector = TagetPos;
	Vector -= Pos;

	float Length = Vector.Length();
	Life = (int)(Length/Speed);
	Life += 1;

	Vector.Normalize() *= Speed;
	bTransSetOK = false;

	return TagetAngle;
}

RbPosition TobiLinerBase::GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const
{
	return KnockBack;
}

void TobiLinerBase::Damege( BattleSystem& System )
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
		DSys.AttackDamege( AttackPow, GetKnockBack(Pos,Ref), DmgWait );
	}
}
