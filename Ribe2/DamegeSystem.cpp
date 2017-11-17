// DamegeSystem.cpp: DamegeSystem ÉNÉâÉXÇÃÉCÉìÉvÉäÉÅÉìÉeÅ[ÉVÉáÉì
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbSound.h"
#include "RbActStatus.h"
#include "BattleSystem.h"
#include "LevelUpSystem.h"
#include "DamegeSystem.h"

//////////////////////////////////////////////////////////////////////
// ç\íz/è¡ñ≈
//////////////////////////////////////////////////////////////////////

float DamegeSystem::BraveUp = 2.0f;
float DamegeSystem::BraveDown = -5.0f;

void DamegeSystem::SetBraveRate( float NewUp, float NewDown )
{
	if( NewUp >= 0 ) BraveUp = NewUp;
	if( NewDown <= 0 ) BraveDown = NewDown;
}

DamegeSystem::~DamegeSystem()
{

}

float DamegeSystem::GetBravePoint() const
{
	RefPtr<RbBattlePaty> pPaty = Source.GetPaty();
	if( pPaty == NULL )return 0.01f;
	return pPaty->GetBrave()/200.0f;
}

void  DamegeSystem::BraveChange()
{
	RefPtr<RbBattlePaty> pPaty = Source.GetPaty();
	if( pPaty != NULL )
		pPaty->BraveUp( BraveUp );
//		pPaty->ChangeBrave( 0.2f*(1+Source.LeaderEffect.BraveUp) );

	pPaty = Taget.GetPaty();
	if( pPaty != NULL )
		pPaty->BraveUp( BraveDown );
//		pPaty->ChangeBrave( -0.25f*max(0.0f,(2-Taget.LeaderEffect.BraveUp)) );
}

void DamegeSystem::SoundHeal() const
{
	static long WaveID = 0;
	if( WaveID == 0 )WaveID = GetSoundSystem().GetID(_T("heal"));
	GetSoundSystem().Play( WaveID );
}

void DamegeSystem::AllRecover( float Pow )
{
	if( Taget.IsDead() )return;
	long HpDmg = Source.AllState().Hp*Pow;
	long MpDmg = Source.AllState().Mp*Pow;
	if( HpDmg <= 0 && MpDmg <= 0 )return;
	Taget.HpJob( HpDmg );
	Taget.MpJob( MpDmg );

	SoundHeal();
	TobiSystem& Tobi = GetBattleSystem().TobiSys;
	Tobi.Damege.AddDamege( MpDmg, Taget.GetPosition(), TobiDamege::eDmgColPurple, 0 );
	Tobi.Damege.AddDamege( HpDmg, Taget.GetPosition(), TobiDamege::eDmgColGreen, 1 );
	Tobi.AddHealLight( Taget );
}

void DamegeSystem::HpRecover( float Pow )
{
	if( Taget.IsDead() )return;
	long HpDmg = Source.AllState().Hp*Pow;
	if( HpDmg <= 0 )return;
	Taget.HpJob( HpDmg );

	SoundHeal();
	TobiSystem& Tobi = GetBattleSystem().TobiSys;
	Tobi.Damege.AddDamege( HpDmg, Taget.GetPosition(), TobiDamege::eDmgColGreen );
	Tobi.AddHealLight( Taget );
}

void DamegeSystem::MpRecover( float Pow )
{
	if( Taget.IsDead() )return;
	long MpDmg = Source.AllState().Mp*Pow;
	if( MpDmg <= 0 )return;
	Taget.MpJob( MpDmg );

	SoundHeal();
	TobiSystem& Tobi = GetBattleSystem().TobiSys;
	Tobi.Damege.AddDamege( MpDmg, Taget.GetPosition(), TobiDamege::eDmgColPurple );
	Tobi.AddHealLight( Taget );
}

void DamegeSystem::Heal( float Pow )
{
	HealDamege( Source.NowState().Magic * Pow * Rndf2(0.9f,1) );

}

void DamegeSystem::HealDamege( long Damege )
{
	if( Taget.IsDead() )return;
	if( Damege <= 0 )return;
	SoundHeal();

	Taget.HpJob( Damege );
	TobiSystem& Tobi = GetBattleSystem().TobiSys;
	Tobi.Damege.AddDamege( Damege, Taget.GetPosition(), TobiDamege::eDmgColGreen );
	Tobi.AddHealLight( Taget );
}

void DamegeSystem::MpRecover( RbActStatus& Taget, long Damege )
{
	if( Taget.IsDead() )return;
	if( Damege <= 0 )return;
	SoundHeal();

	Taget.MpJob( Damege );
	TobiSystem& Tobi = GetBattleSystem().TobiSys;
	Tobi.Damege.AddDamege( Damege, Taget.GetPosition(), TobiDamege::eDmgColPurple );
	Tobi.AddHealLight( Taget );
}

void DamegeSystem::MpDamege( RbActStatus& Taget, float DamegePow )
{
	if( Taget.IsDead() )return;
	if( DamegePow <= 0 )return;

	long Damege = Taget.AllState().Mp * DamegePow;
	MpDmg( Taget, Damege );
}

void DamegeSystem::MpDmg( RbActStatus& Taget, long Dmg, long AddHight )
{
	if( Dmg <= 0 )return;

	Taget.MpJob( -Dmg );
	TobiSystem& Tobi = GetBattleSystem().TobiSys;
	Tobi.Damege.AddDamege( Dmg, Taget.GetPosition(), TobiDamege::eDmgColRed, AddHight );
}

void DamegeSystem::Damege( float Dmg, float DamegeWait )
{
	Damege( Dmg, DamegeWait, RbPosition(0,0,0) );
}

void DamegeSystem::Damege( float Dmg, float DamegeWait, const D3DVECTOR& KnockBack )
{
	const float& HradHit = Source.PasonalEffect.HardHit;
	if( Source.IsOverDrive() )
		Dmg *= ((1.0f+Source.PasonalEffect.OverDrive)*2);
	if( Taget.IsOverDrive() )
		Dmg *= (1.0f-Taget.PasonalEffect.OverDrive);
	Dmg *= Taget.LeaderEffect.GurdForm * Taget.AlgoMove.RefLandDmg();

	if( Dmg < 1 )Dmg = 1;
	bool bDead = Taget.Damege( Dmg, DamegeWait*HradHit,
		RbPosition(KnockBack)*=(HradHit*Taget.LeaderEffect.SolidForm) );

	if( bDead )
	{
		BraveChange();
		Source.KillUp();
		if( (Source.PasonalEffect.MindEater > 0) && (Source.AllState().Mp>Source.NowState().Mp) )
		{
			long Damege  = Taget.AllState().Mp * Source.PasonalEffect.MindEater;
			MpRecover( Source, Damege );
		}
	}
	TobiSystem& TobiSys = GetBattleSystem().TobiSys;
	if( Source.IsUserData() && !Source.IsDead() )
	{
		LevelUpSystem LevUp( Source );
		if( LevUp.AddExp( Taget, Source.LeaderEffect.Growth, bDead ) )
		{
			Source.ResetData();
			Source.Update();
			TobiSys.Planes.AddPlane(_T("levup"),Source.GetPosition() );
		}
	}

	TobiSys.Damege.AddDamege( Dmg+.5f, Taget.GetPosition() );
}

void DamegeSystem::DirectDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait )
{
	if( Taget.IsDead() )return;
	if( Rnd100(Taget.PasonalEffect.Counter) )
	{
		Taget.CounterAction();
		EscapeDamege();
		return;
	}
	AttackDamege( Pow, KnockBack, DmgWait );
}

void DamegeSystem::AttackDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait )
{
	if( Taget.IsDead() )return;
	if( Rnd100(Taget.PasonalEffect.Avoid) )
	{
		EscapeDamege();
		return;
	}
	Pow *= Taget.GetComboRate();

	float Dmg = Source.NowState().Attack*Pow - Taget.NowState().Defence;
	Dmg *= GetBravePoint()*Rndf2(0.9f,1.0f);	// 0 Å` -10 %

	Damege( Dmg, DmgWait, KnockBack );
}

void DamegeSystem::DeadDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait )
{
	if( Taget.IsDead() )return;

	float Dmg = Taget.AllState().Hp * Pow;
	Damege( Dmg, DmgWait, KnockBack );
}

void DamegeSystem::MagicDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait )
{
	if( Taget.IsDead() )return;
	if( Rnd100(Taget.PasonalEffect.AntiMagic+Taget.LeaderEffect.GutsMagic) )
	{
		EscapeDamege();
		return;
	}
	Pow *= Taget.GetComboRate();

	float Dmg = (Source.NowState().Magic*Pow) - Taget.NowState().Magic;
	Dmg *= GetBravePoint()*Rndf2(0.9f,1.0f);	// 0 Å` -10 %

	Damege( Dmg, DmgWait, KnockBack );
}

void DamegeSystem::DeathDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait )
{
	if( Source.IsDead() )return;
	if( Taget.IsDead() )return;

	if( Rnd100(Taget.PasonalEffect.AntiMagic+Taget.LeaderEffect.GutsMagic) ) EscapeDamege();
	else
	{
		float Dmg = (Source.NowState().Magic*Pow*Taget.GetComboRate()) - Taget.NowState().Magic;
		Dmg *= GetBravePoint()*Rndf2(0.9f,1.0f);	// 0 Å` -10 %
		float RecoverMp = Taget.NowState().Mp;
		MpDmg( Taget, Dmg, -1 );
		RecoverMp -= Taget.NowState().Mp;
		Source.MpJob( RecoverMp );
	}

	float RecoverHp = Taget.NowState().Hp;
	AttackDamege( Pow, KnockBack, DmgWait );
	RecoverHp = Taget.NowState().Hp;
	Source.HpJob( RecoverHp );
}

void DamegeSystem::EscapeDamege()
{
	GetBattleSystem().TobiSys.AddErase( Taget );
}
