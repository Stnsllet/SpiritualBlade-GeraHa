// TobiExLiner.cpp: TobiExLiner クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiExLiner.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiExLiner::TobiExLiner()
{

}

TobiExLiner::~TobiExLiner()
{

}

bool TobiExLiner::SetEffect( const SkillEffect& Effect )
{
	if( !TobiSingleBase::SetEffect( Effect ) )return false;
	ShotRange = Effect.Range;
	if( SmashRange <= 8.0f )SmashRange = ShotRange/4.0f;
	return true;
}

bool TobiExLiner::SetExDefine( ExCreator::BaseDef* pNewDef )
{
	if( pNewDef == NULL )return false;
	SetPolygon( pNewDef->PolyID );
	StartSoundNum = pNewDef->WaveID;
	Speed = pNewDef->Speed;
	SmashRange = pNewDef->Range;
	bMagic = pNewDef->bMagic;
	bAllHit = pNewDef->bAllHit;
	KnockPow = pNewDef->KnockPow;
	DmgWait = pNewDef->Shake;
	if( pNewDef->pChiled != NULL )
	{
		ChiledName = pNewDef->pChiled->Name;
		ChiledRange = pNewDef->pChiled->Range;
	}
	return true;
}

bool TobiExLiner::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;
	Pos = BasePos;
	TagetPos = NewTagetPos;
	Vector = NewTagetPos;
	Vector -= BasePos;

	float Length = ShotRange;
	Life = (int)(Length/Speed);
	Life += 1;

	Vector.Normalize() *= Speed;
	bTransSetOK = false;

	pMember->SetAngle( BasePos.GetDirection( NewTagetPos ) );

	PlayWave( StartSoundNum, Pos);

	return true;
}
