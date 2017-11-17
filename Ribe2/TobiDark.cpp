// TobiDark.cpp: TobiDark クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "TobiDarkBit.h"
#include "TobiDark.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiDark::TobiDark()
{
	SpreadRange = 48;
	Speed *= 0.4f;
}

TobiDark::~TobiDark()
{

}

bool TobiDark::SetResource()
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("bomb"));
	StartSoundNum = WaveID;

	static long MyPolyID = 0;
	if( MyPolyID == 0 )MyPolyID = GetPolyID(_T("dark"));
	SetPolygon( MyPolyID );

	return true;
}

// ダメージとオブジェクトを作る
void TobiDark::Damege( BattleSystem& System )
{
	TobiMagicBase::Damege( System );
	float Range = 44.0f;
	RbPosition AddPos( 0, -Range, 0 );
	AddPos.Rotate( 2.0f/12.0f );

	SkillEffect Effect;
	Effect.Pow = AttackPow;
	for( int i = 0; i < 6; i++ )
	{
		RbPosition Taget( Pos );
		Taget += AddPos;
		Taget.z = System.Land.GetHight( Taget.x, Taget.y ) + pMember->Hight()/2.0f;

		TobiRefPtr<TobiDarkBit> pBit( Effect );
		pBit->SetTagetPos( pMember, Pos, Taget );
		System.TobiSys.Add( pBit );
		AddPos.Rotate( 2.0f/6.0f );
	}
	return;
}
