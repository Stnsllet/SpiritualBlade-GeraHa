// TobiFeather.cpp: TobiFeather クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbSound.h"
#include "BattleSystem.h"
#include "TobiWhiteNoise.h"
#include "TobiFeather.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

const float TobiFeather::Direction = 0.25f;

TobiFeather::TobiFeather()
{
	Speed = 16.0f;
	MyAngle = 0;
}

bool TobiFeather::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("feather"));
	SetPolygon( MyPolyID );

	return true;
}

TobiFeather::~TobiFeather()
{

}

bool TobiFeather::Job( BattleSystem& System )
{
	if( Life > 1 )
	{
		Vector = -Pos;
		MyAngle += 2.0f*System.JobSec();

		float ZPos = Speed*Life;
		Pos = RbPosition( 0, -ZPos*Direction, ZPos );
		Pos.Rotate( MyAngle );
		Pos += TagetPos;
		Vector += Pos;
	}
	else Pos = TagetPos;

	bTransSetOK = false;
	if( (--Life) > 0 )return true;
	Damege( System );

	return false;
}

bool TobiFeather::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;

	Life = 25;
	MyAngle = Rndf(2.0f);
	float ZPos = Speed*Life;
	RbPosition AddPos( 0, -ZPos*Direction, ZPos );
	AddPos.Rotate( MyAngle );

	Pos = TagetPos = Vector = NewTagetPos;
	Pos += AddPos;
	Vector -= Pos;

	GetSoundSystem().Play( StartSoundNum, Pos);
	return true;
}

// ダメージじゃなくてオブジェクトを作る
void TobiFeather::Damege( BattleSystem& System )
{
	SkillEffect Effect;
	Effect.Pow = AttackPow;
	Effect.Range = 96;
	TobiRefPtr<TobiWhiteNoise> pBurst( Effect );
	if( !pBurst->SetTagetPos( pMember, TagetPos, TagetPos ) )return;
	System.TobiSys.Add( pBurst );
	return;
}
