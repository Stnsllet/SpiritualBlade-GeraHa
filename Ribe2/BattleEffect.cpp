// BattleEffect.cpp: BattleEffect クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "StringIDMap.h"
#include "EquipSkill.h"
#include "BattleEffect.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BattleEffect::BattleEffect()
{
	AllZero( Effect );
	AllZero( Leader );
	AllZero( Counts );
}

BattleEffect::~BattleEffect()
{
}

bool BattleEffect::PotionOK()
{
	if( Counts.Potion >= Effect.PotionCount )return false;
	++Counts.Potion;
	return true;
}

bool BattleEffect::ReviveOK()
{
	if( Counts.Revive >= Leader.ReviveCount )return false;
	++Counts.Revive;
	return true;
}

bool BattleEffect::RushOK()
{
	if( Effect.Rush <= 0 )return false;
	if( Counts.Rush >= Effect.Rush )
	{
		Counts.Rush = 0;
		return false;
	}
	++Counts.Rush;
	return true;
}

void BattleEffect::Clear()
{
	AllZero( Effect );
	AllZero( Leader );
}

// パーソナルスキル
bool BattleEffect::SetSkillEffect( const EquipSkill& SkillList )
{
	const EquipSkill::SkillLists& Ref =  SkillList.Pasonal;
	const IDManager& RefM = GetCache().Skill.RefIDManager();
	static StockID Regene(_T("regene"));
	static StockID Avoid(_T("avoid"));
	static StockID Counter(_T("counter"));
	static StockID AntiMagic(_T("antimagic"));
	static StockID FlyHight(_T("fly"));
	static StockID OverDrive(_T("overdrive"));
	static StockID HardHit(_T("hardhit"));
	static StockID Rush(_T("rush"));
	static StockID UnShake(_T("unshake"));
	static StockID MindEater(_T("m_eater"));
	static StockID SoulStrength(_T("soul_str"));
	static StockID AntiTrap(_T("antitrap"));
	static StockID SelfBomb(_T("selfbomb"));
	static StockID Potion(_T("potion"));

	static StockID Hp_up(_T("hp_up"));
	static StockID Mp_up(_T("mp_up"));
	static StockID Atc_up(_T("atc_up"));
	static StockID Def_up(_T("def_up"));
	static StockID Mag_up(_T("mag_up"));
	static StockID Spd_up(_T("spd_up"));
	static StockID Hp_dwn(_T("hp_dwn"));
	static StockID Mp_dwn(_T("mp_dwn"));
	static StockID Atc_dwn(_T("atc_dwn"));
	static StockID Def_dwn(_T("def_dwn"));
	static StockID Mag_dwn(_T("mag_dwn"));
	static StockID Spd_dwn(_T("spd_dwn"));

	AllZero( Effect );

	Effect.Regenation = Ref.GetEffect( Regene.ID(RefM) );
	Effect.Avoid = Ref.GetEffect( Avoid.ID(RefM) ).Pow;
	Effect.Counter = Ref.GetEffect( Counter.ID(RefM) ).Pow;
	Effect.AntiMagic = Ref.GetEffect( AntiMagic.ID(RefM) ).Pow;
	Effect.FlyHight = Ref.GetEffect( FlyHight.ID(RefM) ).Pow;
	Effect.AntiTrap =  Ref.GetEffect( AntiTrap.ID(RefM) ).Pow;
	Effect.SelfBomb =  Ref.GetEffect( SelfBomb.ID(RefM) );

	const SkillEffect& RefDrive = Ref.GetEffect( OverDrive.ID(RefM) );
	Effect.OverDrive = RefDrive.Pow;
	Effect.OverDriveHp = RefDrive.Range;
	Effect.UnShake = Ref.GetEffect(UnShake.ID(RefM));
	Effect.Rush = Ref.GetEffect(Rush.ID(RefM)).Count;
	Effect.HardHit = Per(Ref.GetEffect(HardHit.ID(RefM)) );
	Effect.MindEater = Ref.GetEffect(MindEater.ID(RefM));
	Effect.SoulStrength = Ref.GetEffect(SoulStrength.ID(RefM));
	const SkillEffect& RefPotion = Ref.GetEffect( Potion.ID(RefM) );
	Effect.PotionPow = RefPotion.Pow;
	Effect.PotionCount = RefPotion.Count;

	Effect.Pow.Hp = Per(Ref.GetEffect(Hp_up.ID(RefM)) );
	Effect.Pow.Hp -= Ref.GetEffect(Hp_dwn.ID(RefM)).Pow;
	Effect.Pow.Mp = Per(Ref.GetEffect(Mp_up.ID(RefM)) );
	Effect.Pow.Mp -= Ref.GetEffect(Mp_dwn.ID(RefM)).Pow;
	Effect.Pow.Attack =	Per(Ref.GetEffect(Atc_up.ID(RefM)) );
	Effect.Pow.Attack -= Ref.GetEffect(Atc_dwn.ID(RefM)).Pow;
	Effect.Pow.Defence = Per(Ref.GetEffect(Def_up.ID(RefM)) );
	Effect.Pow.Defence -= Ref.GetEffect(Def_dwn.ID(RefM)).Pow;
	Effect.Pow.Magic =	Per(Ref.GetEffect(Mag_up.ID(RefM)) );
	Effect.Pow.Magic -= Ref.GetEffect(Mag_dwn.ID(RefM)).Pow;
	Effect.Pow.Speed =	Per(Ref.GetEffect(Spd_up.ID(RefM)) );
	Effect.Pow.Speed -= Ref.GetEffect(Spd_dwn.ID(RefM)).Pow;
	Effect.Pow.Move = 1.0f;	// 常に 1 倍

	return true;
}

// リーダーの設定
bool BattleEffect::SetLeaderEffect( const EquipSkill& SkillList )
{
	const EquipSkill::SkillLists& Ref =  SkillList.Leader;
	const IDManager& RefM = GetCache().Skill.RefIDManager();
	static StockID Medical(_T("medical"));
	static StockID Spd_Form(_T("spd_form"));
	static StockID GutsMagic(_T("guts_magic"));
	static StockID GutsLife(_T("guts_life"));
	static StockID SolidForm(_T("solid_form"));
	static StockID GurdForm(_T("gurd_form"));
	static StockID MagicForce(_T("mag_force"));
	static StockID RangeUp(_T("rangeup"));
	static StockID BraveUp(_T("braveup"));
	static StockID Growth(_T("grow"));
	static StockID Revive(_T("revive"));
	static StockID LifeTrans(_T("lifetrans"));
	static StockID Activate(_T("active"));

	AllZero( Leader );

	Leader.MedicalPow = Per(Ref.GetEffect(Medical.ID(RefM)) );
	Leader.MoveSpeed = Per(Ref.GetEffect(Spd_Form.ID(RefM)) );
	Leader.GutsMagic = Ref.GetEffect(GutsMagic.ID(RefM)).Pow;
	Leader.GutsLife = Ref.GetEffect(GutsLife.ID(RefM)).Pow;
	Leader.SolidForm = Per(Ref.GetEffect(SolidForm.ID(RefM)) );
	Leader.GurdForm = Per(Ref.GetEffect(GurdForm.ID(RefM)) );
	Leader.Growth = Per(Ref.GetEffect(Growth.ID(RefM)) );
	Leader.BraveUp = Ref.GetEffect(BraveUp.ID(RefM));
	Leader.MagicForce = Per(Ref.GetEffect(MagicForce.ID(RefM)) );
	Leader.RangeUp = Per(Ref.GetEffect(RangeUp.ID(RefM)) );
	Leader.Activate = Per(Ref.GetEffect(Activate.ID(RefM)) );
	Leader.LifeTrans = Ref.GetEffect(LifeTrans.ID(RefM)).Pow;

	const SkillEffect& ReviveEff = Ref.GetEffect(Revive.ID(RefM));
	Leader.RevivePow = ReviveEff.Pow;
	Leader.ReviveCount = ReviveEff.Count;

	return true;
}
