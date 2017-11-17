// DamegeSystem.h: DamegeSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAMEGESYSTEM_H__943A1597_DE69_43DD_B5F9_3CC75C74746A__INCLUDED_)
#define AFX_DAMEGESYSTEM_H__943A1597_DE69_43DD_B5F9_3CC75C74746A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RbActStatus;

class DamegeSystem  
{
public:
	DamegeSystem(RbActStatus& NewSource, RbActStatus& NewTaget)
		:Source(NewSource),Taget(NewTaget){}
	~DamegeSystem();

	void DeadDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait );
	void DirectDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait );
	void AttackDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait );
	void MagicDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait );
	void DeathDamege( float Pow, const D3DVECTOR& KnockBack, float DmgWait );
	void Heal( float Pow );
	void HealDamege( long Damege );
	void AllRecover( float Pow );
	void HpRecover( float Pow );
	void MpRecover( float Pow );
	void MpRecover( RbActStatus& Taget, long Damege );
	void MpDamege( RbActStatus& Taget, float DamegePow );
	void MpDmg( RbActStatus& Taget, long Dmg, long AddHight=0 );

	static void SetBraveRate( float NewUp, float NewDown );
private:
	void Damege( float Dmg, float DamegeWait=500 );
	void Damege( float Dmg, float DamegeWait, const D3DVECTOR& KnockBack );
	float GetBravePoint() const;
	void  BraveChange();
	void SoundHeal() const;

	void EscapeDamege();
	RbActStatus& Source;
	RbActStatus& Taget;
	static float BraveUp, BraveDown;
};

#endif // !defined(AFX_DAMEGESYSTEM_H__943A1597_DE69_43DD_B5F9_3CC75C74746A__INCLUDED_)
