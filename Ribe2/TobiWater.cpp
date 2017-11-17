// TobiWater.cpp: TobiWater クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "TobiWaterSpread.h"
#include "TobiWater.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiWater::TobiWater()
{
	SpreadRange = 32;
}

bool TobiWater::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("water"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("wave"));
	SetPolygon( MyPolyID );

	return true;
}

TobiWater::~TobiWater()
{
}

RefPtr<TobiObjectBase> TobiWater::GetBurstObject()
{
	TobiRefPtr<TobiWaterSpread> pSpread;
	pSpread->SetInfo( KnockBack.Normalize(5), SpreadRange, 3 );
	if( !pSpread->SetTagetPos( pMember, Pos, TagetPos ) )return NULL;
	return pSpread;
}

bool TobiWater::Job( BattleSystem& System )
{
	if( !TobiSingleBase::Job( System ))return false;
	if( Pos.Length( TagetPos ) > SpreadRange )return true;

	Damege( System );
	return false;
}
