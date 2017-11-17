// RbAttackAlgo.cpp: RbAttackAlgo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "RbCache.h"
#include "RbSound.h"
#include "RbTexture.h"
#include "DamegeSystem.h"
#include "BattleSystem.h"
#include "RbDisplay.h"
#include "RbAttackAlgo.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbAttackAlgo::RbAttackAlgo( BattleSystem& RefSystem, RbActStatus& Data, TagetAlgo& RefTagetSys )
: System( RefSystem ) , Source( Data ) , TagetSys( RefTagetSys ), Direct( RefSystem, Data ), WeponRandom(0)
{
	WepChangeTime = RbTimer();
}

RbAttackAlgo::~RbAttackAlgo()
{

}

bool RbAttackAlgo::InTagetRange( RbBattleStatus* pEnemy ) const
{
	return Wepons.Now().InTagetRange( pEnemy, Source );
}

bool RbAttackAlgo::InAttackRange( RbBattleStatus* pEnemy ) const
{
	return Wepons.Now().InAttackRange( pEnemy, Source );
}

long RbAttackAlgo::ActID() const
{
	switch( Source.GetAction() )
	{
	case eActSpecial:
		if( SPDat.ActID > 0 )return SPDat.ActID;
	case eActAttack:
		if( Wepons.Now().ActID > 0 )return Wepons.Now().ActID;
	}
	RbTexture* pTexture = Source.RefTexture();
	if( pTexture == NULL )return -1;
	return pTexture->GetActID( Source.GetAction() );
}

RefBord* RbAttackAlgo::GetWeponImage() const
{
	switch( Source.GetAction() )
	{
	case eActSpecial:
		if( SPDat.pWeponImage != NULL )return SPDat.pWeponImage;
	}
	return NowWepon().pImage;
}

// 武器の変更通知
bool RbAttackAlgo::ChangeWepon( RbActStatus* pTaget )
{
	if( Wepons.Count() <= 1 ) return true;
	if( !CheckMp() )return Wepons.ChangeWepon( pTaget,Source );

	RbTimer NowTime;
	if( NowTime.Passed(WepChangeTime) < 500 )return true;
	WepChangeTime = NowTime;

	if( Rnd100(WeponRandom) )return Wepons.RandomWepon( Source );

	return Wepons.ChangeWepon( pTaget,Source );
}

// 武器の変更通知
bool RbAttackAlgo::WideWepon()
{
	return Wepons.SetWideWepon( Source );
}

// 武器の変更通知
void RbAttackAlgo::CheckWepon()
{
	if( Wepons.Count() > 0 )return;
	const MemberStatus& State = Source.MemberState();

	Wepons.Add( State.Wepon, Source, true );
	Wepons.Add( State.Body, Source );
	Wepons.Add( State.Acs, Source );

	WeponRandom = Source.RefBattleStatus().WeponRandom;
}

// ターゲットが攻撃可能か？
bool RbAttackAlgo::TagetInRange()
{
	return Wepons.Now().IsAttackOK( TagetSys.GetNowTaget(), Source );
}

// Mp 判定
bool RbAttackAlgo::CheckMp() const
{
	return (Source.NowState().Mp>=Wepons.Now().UseMp(Source.Level()));
}

bool RbAttackAlgo::AttackStart()
{
	long AttackActID = ActID();
	Source.TexSound( AttackActID );
	RefPtr<RbActStatus> pTaget = TagetSys.GetAttackTaget();
	Source.TagetAngle( pTaget );
//	ChangeWepon( pTaget );

	if( Wepons.Now().pCreator != NULL )return true;
	return Direct.AttackStart( AttackActID, Source.GetAngle() );
}

bool RbAttackAlgo::AttackJob( const int& ActTime )
{
	if( Wepons.Now().pCreator != NULL )return true;
	return Direct.AttackJob( ActTime );
}

void RbAttackAlgo::AttackEnd()
{
	if( !TagetInRange() )return;
	if( Wepons.Now().pCreator == NULL )Direct.AttackEnd();//DirectAttack();
	else TobiAttack();
}

void RbAttackAlgo::TobiAttack()
{
	RefPtr<RbActStatus> pTaget = TagetSys.GetNowTaget();
	if( pTaget == NULL )return;
	const WeponSystem::WeponData& MainWepon = Wepons.Now();

	if( MainWepon.pCreator == NULL )return;
	if( !Source.UseMp(MainWepon.pCreator->UseMp(Source.Level())) )return; //Mp 不足

	RefPtr<TobiObjectBase> pNew = MainWepon.pCreator->Create();
	if( pNew == NULL )return;

	pNew->SetEffect( MainWepon.Effect );
	if( !pNew->SetTaget( &Source, pTaget ) )return;
	System.TobiSys.Add( pNew );
}

// 必殺技
bool RbAttackAlgo::SpecialGo( long SkillID, long Level )
{
	if( Source.IsDead() )return false;
	if( Source.GetAction() == eActSpecial )return false;

	return SPDat.Set( SkillID, Level, Source );
}

// 必殺技アクション終了
bool RbAttackAlgo::SpecialEnd()
{
	if( Source.IsDead() )return false;
	if( SPDat.pCreator == NULL )return false;
	RefPtr<TobiObjectBase> pTobi = SPDat.pCreator->Create();
	pTobi->SetEffect( SPDat.Effect );
	if( !pTobi->SetSource(Source) )return false;
	System.TobiSys.Add( pTobi );

	if( SPDat.Shake > 0 )GetRbDisplay().SetShake( SPDat.Shake );
	return true;
}

// 必殺技オブジェクト設定
bool RbAttackAlgo::SpecialAttackData::Set( long ID, long Level, const RbActStatus& Source )
{
	const TobiSystem& TobiSys = GetBattleSystem().TobiSys;
	RbCache& Cache = GetCache();
	const SkillCache& Skill = GetCache().Skill;
	const SkillCache::SkillData* pData = Skill.GetSkill( ID );
	if( pData == NULL )return false;

	pWeponImage.Release();
	AllZero( *this );
	SkillID = ID;
	Effect = pData->Level( Level );
	ActID = 0;
	pCreator = TobiSys.GetCreator( pData->EffectName );

	if( !pData->Action.empty() )
	{
		StringSplitter<> Split(pData->Action);
		ActID = Cache.ActID.GetID( Split[0] );
		if( !Source.IsTexAct(ActID) )ActID = 0;
		long ItemNum = Split[1].ToLong();
		if( ItemNum > 0 )pWeponImage = Cache.Item.GetTexture(ItemNum);
	}

	Shake = pData->Shake;

	return true;
}
