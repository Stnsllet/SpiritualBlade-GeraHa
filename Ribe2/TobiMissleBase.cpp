// TobiMissleBase.cpp: TobiMissleBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbSound.h"
#include "BattleSystem.h"
#include "TobiMissleBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiMissleBase::TobiMissleBase()
{
	MaxSpeed = Speed;
	DownSpeed = MaxSpeed/75.0f;
	TurnSpeed = MaxSpeed/10.0f;
	SmashRange = 16;
	bAllHit = false;
	KnockPow = 2;
}

TobiMissleBase::~TobiMissleBase()
{

}

// データセット
bool TobiMissleBase::SetTaget( RbActStatus* pSource, RbActStatus* pNewTaget )
{
	pTaget = pNewTaget;
	return TobiMagicBase::SetTaget( pSource, pNewTaget );
}

// データセット
bool TobiMissleBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;

	Pos = BasePos;
	TagetPos = NewTagetPos;
	Vector = RbPosition( 0, -Speed, 0 );
	Vector.Rotate( Rndf(2.0f) );

	bTransSetOK = false;
	GetSoundSystem().Play( StartSoundNum, Pos);
	return true;
}

bool TobiMissleBase::Job( BattleSystem& System )
{
	Speed -= DownSpeed;
	if( Speed <= 0 )
	{
		Damege( System );
		return false;
	}

	if( pMember == NULL )return false;
	if( pTaget == NULL || pTaget->IsDead() )
	{
		pTaget = System.GetEnemy( TagetPos, pMember->GetArmyID() );
		if( pTaget == NULL )return false;
	}
	TagetPos = pTaget->GetPosition();
	TagetPos.z += pTaget->Hight()/2.0f;

	RbPosition AddVector = TagetPos;
	AddVector -= Pos;
	AddVector.Normalize( TurnSpeed );
	Vector += AddVector;
	float NowSpeed = Vector.Length();
	if( NowSpeed > Speed )Vector *= Speed/NowSpeed;

	Pos += Vector;
	bTransSetOK = false;

//	if( TagetPos.Length2( Pos ) > SmashRange*SmashRange )return true;
	if( !pTaget->InRange(Pos,SmashRange/2.0f) )return true;

	return TagetHitJob( System );
}

bool TobiMissleBase::TagetHitJob( BattleSystem& System )
{
	KnockBack = Vector;
	KnockBack.Normalize(KnockPow);
	Damege( System );

	return false;
}
