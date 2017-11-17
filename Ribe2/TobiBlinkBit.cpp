// TobiBlinkBit.cpp: TobiBlinkBit クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbDisplay.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "TobiBlinkBit.h"
#include "DispStocker.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

const Material TobiBlinkBit::BlinkMate = Material( ARGBF( 0.75f, 0.25f, 0.25f, 1.0f ) );

TobiBlinkBit::TobiBlinkBit()
{
	StartTime = 0;
	DelayTime = 0;
	MaxActTime = 0;
	ActID = 0;
}

TobiBlinkBit::~TobiBlinkBit()
{
}

// びょうしゃ
void TobiBlinkBit::Draw( Display& Disp )
{
	float Passed = System.GetBattleTime() - StartTime - DelayTime;
	if( Passed < 0 )return;
	if( Passed >= MaxActTime )return;

	if( pMember == NULL )return;
	if( pMember->IsDead() )return;

	RefPtr<RbTexture> pTexture = pMember->RefTexture();
	if( pTexture == NULL )return;

	DispStocker::AllTrans Trans( Disp );
	DispStocker::Render StockBias( Disp, D3DRS_ZBIAS );
	DispStocker::Material MaterialStock( Disp );
	MaterialStock.Set( BlinkMate );

	const RbTexture::ActTime& Image = pTexture->GetAction( ActID, MyAngle, Passed );
	pMember->DrawPos( Disp, Image, Pos, Trans.All );
}

bool TobiBlinkBit::Job( BattleSystem& System )
{
	float Passed = System.GetBattleTime() - StartTime - DelayTime;

	if( Passed < 0 )
	{
		if( pTaget != NULL )
		{
			const RbPosition& NewPos = pTaget->GetPosition();
			Pos = Pos - TagetPos + NewPos;
			TagetPos = NewPos;
		}
		return true;
	}
	Pos += Vector;
	if( Passed >= MaxActTime )return false;
	if( pTaget == NULL )return true;
	if( Passed < (MaxActTime/2.0f) )return true;

	Damege( System );
	pTaget.Release();

	return true;
}

// データセット
bool TobiBlinkBit::SetTaget( RbActStatus* pSource, RbActStatus* pEnemy )
{
	if( pEnemy == NULL )return false;
	if( pSource == NULL )return false;
	pTaget = pEnemy;
	pMember = pSource;
	RefPtr<RbTexture> pTexture = pMember->RefTexture();
	if( pTexture == NULL )return false;

	MyAngle = Rndf(1.999f);
	Pos = TagetPos = pEnemy->GetPosition();
	DelayTime = Pos.Length( pMember->GetPosition() ) / (200/1000.0f);
	DelayTime -= 350;
	if( DelayTime < 0 )DelayTime = 0;

	StartTime = System.GetBattleTime();

	ActID = pTexture->GetActID( eActAttack );
	MaxActTime = pTexture->GetActionInfo( ActID, MyAngle ).MaxTime;
	if( MaxActTime <= 0 )return false;

	Vector = RbPosition( 0, pTexture->GetSize(), 0 );
	Vector.Rotate( MyAngle );
	Pos += Vector;
	Vector *= -(System.JobTime()/MaxActTime)*2.0f;

	return true;
}

void TobiBlinkBit::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	if( pTaget == NULL )return;

	KnockBack = Vector;
	KnockBack.Normalize( 3 );
	DamegeSystem DSys( *pMember, *pTaget );
	DSys.AttackDamege( AttackPow, KnockBack, DmgWait );
}
