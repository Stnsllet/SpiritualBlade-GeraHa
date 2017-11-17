// TobiComet.cpp: TobiComet クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbSound.h"
#include "RbDisplay.h"
#include "BattleSystem.h"
#include "TobiHeatWave.h"
#include "TobiComet.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiComet::TobiComet()
{
}

bool TobiComet::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("rock"));
	SetPolygon( MyPolyID );

	return true;
}

TobiComet::~TobiComet()
{
}

void TobiComet::Draw( Display& Disp )
{
	RbTransform Trans;
	Trans *= Disp.UnViewMat();
	Trans.AddScroll( Pos );

	DrawPolygon( Disp, Trans );
}

bool TobiComet::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;

	Life = 50;
	RbPointF ZPos( 0, -400.0f );
	ZPos.Rotate( Rndf2(-0.125,0.125) );
	Vector = RbPosition( ZPos.x, 0, ZPos.y );
	Pos = TagetPos = NewTagetPos;
	Pos -= Vector;
	Vector /= Life;

	GetSoundSystem().Play( StartSoundNum, Pos);
	return true;
}

// ダメージじゃなくてオブジェクトを作る
void TobiComet::Damege( BattleSystem& System )
{
	SkillEffect Effect;
	Effect.Pow = AttackPow;
	Effect.Range = 96;
	TobiRefPtr<TobiHeatWave> pBurst( Effect );
	pBurst->SetAllHit( false );
	if( !pBurst->SetTagetPos( pMember, TagetPos, TagetPos ) )return;
	System.TobiSys.Add( pBurst );
	return;
}
