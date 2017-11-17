// TobiSingleBase.cpp: TobiSingleBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbSound.h"
#include "BillBordPlane.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "Display.h"
#include "TobiSingleBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
const float TobiSingleBase::DefaultSpeed = 450;

TobiSingleBase::TobiSingleBase()
{
	Speed = DefaultSpeed * System.JobSec();
	bTransSetOK = false;
	StartSoundNum = 0;
	SmashRange = 8;
	KnockPow = 2.0f;
}

TobiSingleBase::~TobiSingleBase()
{
}

bool TobiSingleBase::Job( BattleSystem& System )
{
	if( Life > 1 )Pos += Vector;
	else Pos = TagetPos;

	bTransSetOK = false;
	if( (--Life) > 0 )return true;
	Damege( System );

	return false;
}

bool TobiSingleBase::ResetTrans()
{
	if( bTransSetOK )return false;
	Trans.FromPosture( Vector );
	Trans.AddScroll( Pos );
	bTransSetOK = true;
	return true;
}

void TobiSingleBase::Draw( Display& Disp )
{
	ResetTrans();
	DrawPolygon( Disp, Trans );
}

bool TobiSingleBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;
	pMember = pSource;

	Pos = BasePos;
	TagetPos = NewTagetPos;
	Vector = TagetPos;
	Vector -= Pos;

	float Length = Vector.Length();
	Life = (int)(Length/Speed);
	Life += 1;
	Vector.Normalize() *= Speed;
	bTransSetOK = false;
	GetSoundSystem().Play( StartSoundNum, Pos);

	KnockBack = Vector;
	KnockBack.Normalize( KnockPow );
	return true;
}

RbPosition TobiSingleBase::GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const
{
	return KnockBack;
}

void TobiSingleBase::Damege( BattleSystem& System )
{
	if( pMember == NULL )return;
	BattleSystem::CharaList List;
	System.GetCharaList( List, Pos, SmashRange, pMember->GetArmyID() );

	BattleSystem::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		RbActStatus& Ref = *(*itr++);
	//	if( !IsNewHit(&Ref) )continue;
		DamegeSystem DSys( *pMember, Ref );
		DSys.AttackDamege( AttackPow, KnockBack, DmgWait );
	}
}
