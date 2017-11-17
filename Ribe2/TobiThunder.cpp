// TobiThunder.cpp: TobiThunder クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbDisplay.h"
#include "RbSound.h"
#include "BattleSystem.h"
#include "TobiThunder.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiThunder::TobiThunder()
{
	bAllHit = false;

	if( pPoly != NULL )PolyTop = pPoly->Bound().top;
	else PolyTop = 0;

	MaxLife = 300.0f/System.JobTime();
	DmgWait = 1200;
}

bool TobiThunder::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("thunder"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("thunder"));
	SetPolygon( MyPolyID );

	return true;
}

TobiThunder::~TobiThunder()
{

}

void TobiThunder::Draw( Display& Disp )
{
	if( MaxLife <= 0 )return;

	float Scale = (MaxLife-Life)/MaxLife;
	if( Scale <= 0 )return;

	RbTransform Trans;
	Trans.AddScroll( 0, (1-Scale)*PolyTop/Scale, 0 );
	Trans *= Disp.UnViewMat();
	Trans.Scale( Scale );
	Trans.AddScroll( Pos );

	DrawPolygon( Disp, Trans );
}

// データセット
bool TobiThunder::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;
	Pos = TagetPos = NewTagetPos;
	Life = MaxLife;

	bTransSetOK = false;
	GetSoundSystem().Play( StartSoundNum, Pos);

	return true;
}
