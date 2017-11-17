// TobiWhiteBlowBit.cpp: TobiWhiteBlowBit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TobiWhiteBlowBit.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiWhiteBlowBit::TobiWhiteBlowBit()
{
	DmgWait = 750;
}

bool TobiWhiteBlowBit::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("wblow"));
	SetPolygon( MyPolyID );

	return true;
}

TobiWhiteBlowBit::~TobiWhiteBlowBit()
{

}

bool TobiWhiteBlowBit::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( !TobiSingleBase::SetTagetPos( pSource, BasePos, NewTagetPos ) )return false;
	KnockBack = Vector;
	KnockBack.Normalize( 8 );
	return true;
}
