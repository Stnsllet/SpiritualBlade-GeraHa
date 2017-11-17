// TobiBeast.cpp: TobiBeast クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbSound.h"
#include "BattleSystem.h"
#include "DamegeSystem.h"
#include "DispStocker.h"
#include "TobiBeast.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiBeast::TobiBeast()
{
	VectorAngle = 0;
}

bool TobiBeast::SetResource()
{
	static long WaveNum = 0;
	if( WaveNum == 0 )
		WaveNum = GetWaveID(_T("gao"));
	StartSoundNum = WaveNum;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("beast"));
	SetPolygon( MyPolyID );

	return true;
}

TobiBeast::~TobiBeast()
{
}

bool TobiBeast::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( !TobiLinerBase::SetTagetPos( pSource, BasePos, NewTagetPos ) )return false;
	EchoImage.SetCount( 100, 5 );
	VectorAngle = Vector.GetDirection();

	return true;
}

RbPosition TobiBeast::GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const
{
	return GetSideKnockBack( VectorAngle, 10, Base, Taget );
}
