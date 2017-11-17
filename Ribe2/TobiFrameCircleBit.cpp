// TobiFrameCircleBit.cpp: TobiFrameCircleBit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "RbSound.h"
#include "TobiFrameCircleBit.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiFrameCircleBit::TobiFrameCircleBit()
{
	bAllHit = false;
	SmashRange = 64.0f;
	MyLength = MyAngle = 0;
	MaxRange = 100;

	DmgWait = 1000.0f;
}

bool TobiFrameCircleBit::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("blaze"));
	SetPolygon( MyPolyID );

	return true;
}

TobiFrameCircleBit::~TobiFrameCircleBit()
{

}

// スキル効果設定
bool TobiFrameCircleBit::SetEffect( const SkillEffect& Effect )
{
	MaxRange = Effect.Range;
	return TobiMagicBase::SetEffect( Effect );
}

bool TobiFrameCircleBit::Job( BattleSystem& System )
{
	float AddSpeed = Speed - MyLength * D3DX_PI;
	if( AddSpeed < Speed/3 ) AddSpeed = Speed/4;
	MyLength += AddSpeed;
	MyAngle += 2.0f*System.JobSec();
	Vector = -Pos;
	Pos = RbPosition( 0,-MyLength,0 );
	Pos.Rotate( MyAngle );
	Pos += TagetPos;
	Vector += Pos;

	bTransSetOK = false;
	Damege( System );
	if( MyLength <= MaxRange )return true;

	return false;
}

bool TobiFrameCircleBit::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;

	Pos = BasePos;
	TagetPos = BasePos;
	Vector = TagetPos;
	Vector -= Pos;

	bTransSetOK = false;
	GetSoundSystem().Play( StartSoundNum, Pos);

	return true;
}

bool TobiFrameCircleBit::ResetTrans()
{
	if( bTransSetOK )return false;
	Trans.FromPosture( Vector );
	Trans.Scale( SmashRange );
	Trans.AddScroll( Pos );
	bTransSetOK = true;
	return true;
}

RbPosition TobiFrameCircleBit::GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const
{
	RbPosition KnockBack = Base;
	KnockBack -= TagetPos;
	return KnockBack.Normalize(3);
}
