// TobiHado.cpp: TobiHado クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiHado.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiHado::TobiHado()
{
	DmgWait = 1000;
	Speed *= 0.8f;
}

bool TobiHado::SetResource()
{
	static long WaveNum = 0;
	if( WaveNum == 0 )WaveNum = GetWaveID(_T("gao"));
	StartSoundNum = WaveNum;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("spirit"));
	SetPolygon( MyPolyID );

	return true;
}

TobiHado::~TobiHado()
{

}

bool TobiHado::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( !TobiLinerBase::SetTagetPos( pSource, BasePos, NewTagetPos ) )return false;

	KnockBack = RbPosition( 0, 10, 0 );
	KnockBack.Rotate( pMember->GetAngle() );

	pMember->AlgoMove.AddKnockBack( KnockBack );
	KnockBack.Rotate( -1 );
	KnockBack *= 2;
	EchoImage.SetCount( 150, 3 );

	return true;
}

bool TobiHado::SetEffect( const SkillEffect& Effect )
{
	if( !TobiLinerBase::SetEffect( Effect ) )return false;
	SmashRange = ShotRange/2.0f;
	return true;
}
