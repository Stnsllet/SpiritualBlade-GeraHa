// TobiSpreadBase.cpp: TobiSpreadBase クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTransform.h"
#include "Display.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "TobiSpreadBase.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TobiSpreadBase::TobiSpreadBase()
{
	DmgWait = 300;

	SpreadMaxRange = 48;
	SpreadKnockBack = 2;
	SpreadSpeed = 300 * System.JobSec();

	StartSoundNum = 0;
	bAllHit = true;

	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetWaveID(_T("fire"));
	StartSoundNum = WaveID;
}

TobiSpreadBase::~TobiSpreadBase()
{

}

// 情報セット
void TobiSpreadBase::SetBaseInfo( float NewSpreadRange, float NewSpreadKnockBack )
{
	SpreadMaxRange = NewSpreadRange;
	SpreadKnockBack = NewSpreadKnockBack;
}

// スキル効果設定
bool TobiSpreadBase::SetEffect( const SkillEffect& Effect )
{
	if( Effect.Pow > 0 )AttackPow = Effect.Pow;
	if( Effect.Range > 0 )SpreadMaxRange = Effect.Range;
	return true;
}

// データセット
bool TobiSpreadBase::SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos )
{
	if( pSource == NULL )return false;

	pMember = pSource;
	Pos = BasePos;
	TagetPos = NewTagetPos;

	SpreadRange = 0;
	Life = (int)(SpreadMaxRange/SpreadSpeed);
	Life += 1;
	PlayWave( StartSoundNum, Pos);

	return true;
}

// 仕事
bool TobiSpreadBase::Job( BattleSystem& System )
{
	if( Life > 1 ) SpreadRange += SpreadSpeed;
	else SpreadRange = SpreadMaxRange;
	if( SpreadRange > SpreadMaxRange)SpreadRange = SpreadMaxRange;

	Damege( System );
	if( (--Life) > 0 )return true;

	return false;
}

// 描写
void TobiSpreadBase::Draw( Display& Disp )
{
	RbTransform Trans;
	Trans.Scale( SpreadRange );
	Trans.AddScroll( Pos );

	DrawPolygon( Disp, Trans );
}

// ノックバック
RbPosition TobiSpreadBase::GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const
{
	RbPosition KnockBack = Taget.GetPosition();
	KnockBack -= Base;
	return KnockBack.Normalize( SpreadKnockBack );
}

// ダメージ
void TobiSpreadBase::Damege( BattleSystem& System )
{
	typedef BattleTagetSystem::CharaItr CharaItr;

	if( pMember == NULL )return;

	BattleTagetSystem::CharaList TagetList;
	long Count = System.GetCharaList( TagetList, Pos, SpreadRange, pMember->GetArmyID(), true, bAllHit );
	if( Count == 0 )return;

	CharaItr itr( TagetList );
	while( !itr.IsEnd() )
	{
		if( IsNewHit(itr)  )
		{
			DamegeSystem DSys( *pMember, *itr );
			DSys.MagicDamege( AttackPow, GetKnockBack(Pos, *itr), DmgWait );
		}
		itr.Next();
	}
}
