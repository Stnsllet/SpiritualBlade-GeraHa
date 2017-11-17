// TobiBomb.cpp: TobiBomb クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTransform.h"
#include "BillbordPlane.h"
#include "TobiHeatWave.h"
#include "BattleSystem.h"
#include "Display.h"
#include "TobiBomb.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiBomb::TobiBomb()
{
	SpreadRange = 64;
//	SetPlane(_T("bomb"));
	Speed /= 2;

}

bool TobiBomb::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("bomb"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("bomb"));
	SetPolygon( MyPolyID );

	return true;
}

TobiBomb::~TobiBomb()
{
}

RefPtr<TobiObjectBase> TobiBomb::GetBurstObject()
{
	TobiRefPtr<TobiHeatWave> pBurst;
	pBurst->SetInfo( SpreadRange, 2 );
	if( !pBurst->SetTagetPos( pMember, Pos, TagetPos ) )return NULL;
	return pBurst;
}

bool TobiBomb::ResetTrans()
{
	if( bTransSetOK )return false;
	float Angle = (float)System.GetBattleTime();
	Angle /= 100;

	Trans = RbTransform();
	Trans.RotateZ( Angle );
//	Trans.Posture( Vector );
	Trans.AddScroll( Pos );

	bTransSetOK = true;
	return true;
}
