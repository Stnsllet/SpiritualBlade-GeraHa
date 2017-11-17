// BattleEffect.h: BattleEffect クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLEEFFECT_H__23AA2C2A_DA13_433B_ADA2_A1351A4B38B7__INCLUDED_)
#define AFX_BATTLEEFFECT_H__23AA2C2A_DA13_433B_ADA2_A1351A4B38B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class EquipSkill;

class BattleEffect  
{
public:
	BattleEffect();
	~BattleEffect();

	bool SetSkillEffect( const EquipSkill& SkillList );
	bool SetLeaderEffect( const EquipSkill& SkillList );

	bool PotionOK();
	bool ReviveOK();
	bool RushOK();

	void Clear();

public:
	struct _EffectSet
	{
		long Counter;
		long AntiMagic;
		long Avoid;
		long AntiTrap;
		float Regenation;
		DefaultStatus Pow;
		float OverDrive;
		float OverDriveHp;
		long Rush;
		float MindEater;
		float HardHit;
		float UnShake;
		float SoulStrength;
		float FlyHight;
		SkillEffect SelfBomb;
		float PotionPow;
		long PotionCount;
	};

	struct _Leader
	{
		float MedicalPow;
		float MoveSpeed;
		float SolidForm;
		float GurdForm;
		float Growth;
		float BraveUp;
		float Activate;
		long GutsMagic;
		long GutsLife;
		float MagicForce;
		float RevivePow;
		long ReviveCount;
		float RangeUp;
		float LifeTrans;
	};
	struct _Counts
	{
		long Rush;
		long Revive;
		long Potion;
	};

	float Per( const float& Value ){return (1.0f+Value);}
	const _EffectSet& RefEffect() const{return Effect;}
	const _Leader& RefLeader() const{return Leader;}
private:
	_EffectSet Effect;
	_Leader Leader;
	_Counts Counts;
};

#endif // !defined(AFX_BATTLEEFFECT_H__23AA2C2A_DA13_433B_ADA2_A1351A4B38B7__INCLUDED_)
