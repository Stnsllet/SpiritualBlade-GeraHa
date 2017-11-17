// TrapSystem.cpp: TrapSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringSplitter.h"
#include "RbActStatus.h"
#include "RbBattlePaty.h"
#include "GameSystem.h"
#include "TrapSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

float TrapSystem::DmgRate = 0.75f;
float TrapSystem::MindRate = 1.00f;
float TrapSystem::BraveRate = 0.75f;

TrapSystem::TrapSystem( GameSystem& RefSystem )
: System( RefSystem )
{
}

TrapSystem::~TrapSystem()
{
}

bool TrapSystem::Trap( RbActStatus* pStatus, const String& Traps, const String& MonsterPath )
{
	if( pStatus == NULL )return false;
	return Trap(pStatus->GetPaty(), Traps, MonsterPath);
}

bool TrapSystem::Trap( RbBattlePaty* pPaty, const String& Traps, const String& MonsterPath )
{
	if( pPaty == NULL )return false;
	if( AntiTrap( pPaty ) )return false;

	StringSplitter<> Split( Traps );
	long Count = Split.Count();
	if( Count <= 0 )return false;
	long Num = Rnd( Count-1 );
	const String& Trap = Split[Num];

	if( Trap==_T("dmg") )return DamegeTrap( pPaty );
	if( Trap==_T("mind") )return MindTrap( pPaty );
	if( Trap==_T("down") )return DownTrap( pPaty );
	if( Trap==_T("mons") )return MonsterTrap( MonsterPath );

	return false;
}

bool TrapSystem::DamegeTrap( RbBattlePaty* pPaty )
{
	TobiDamege& TobiSys = System.BattleSys.TobiSys.Damege;
	long Count = pPaty->MemberCount();
	for( int i = 0; i < Count; i++ )
	{
		RefPtr<RbActStatus> pMember = pPaty->GetMember(i);
		if( pMember->IsDead() )continue;
		float Damege = DmgRate * pMember->AllState().Hp;
		pMember->Damege( Damege, 500, RbPosition(0,0,0) );
		TobiSys.AddDamege( Damege+.5f, pMember->GetPosition() );
	}
	LPCTSTR Name = _T("trap_dmg");
	System.SoundSystem.Play( Name );
	System.WindowSys.PopupMsg( NULL, System.Cache.GetMessage(Name) );
	return true;
}

bool TrapSystem::MindTrap( RbBattlePaty* pPaty )
{
	TobiDamege& TobiSys = System.BattleSys.TobiSys.Damege;
	long Count = pPaty->MemberCount();
	for( int i = 0; i < Count; i++ )
	{
		RefPtr<RbActStatus> pMember = pPaty->GetMember(i);
		if( pMember->IsDead() )continue;
		float Damege = MindRate * pMember->AllState().Mp;
		pMember->MpJob( -Damege );
		TobiSys.AddDamege( Damege+.5f, pMember->GetPosition(), TobiDamege::eDmgColRed );
	}
	LPCTSTR Name = _T("trap_mind");
	System.SoundSystem.Play( Name );
	System.WindowSys.PopupMsg( NULL, System.Cache.GetMessage(Name) );
	return true;
}

bool TrapSystem::DownTrap( RbBattlePaty* pPaty )
{
	float Damege = BraveRate * pPaty->GetBrave();
	pPaty->BraveUp( -Damege );
	LPCTSTR Name = _T("trap_down");
	System.SoundSystem.Play( Name );
	System.WindowSys.PopupMsg( NULL, System.Cache.GetMessage(Name) );
	return true;
}

bool TrapSystem::MonsterTrap( const String& MonsterPath )
{
	LPCTSTR Name = _T("trap_mons");
	System.SoundSystem.Play( Name );
	System.WindowSys.PopupMsg( NULL, System.Cache.GetMessage(Name) );
	return !FAILED(System.EventSystem.LoadEvent( MonsterPath ));
}

bool TrapSystem::AntiTrap( RbBattlePaty* pPaty )
{
	long Count = pPaty->MemberCount();
	for( int i = 0; i < Count; i++ )
	{
		RefPtr<RbActStatus> pMember = pPaty->GetMember(i);
		if( pMember->IsDead() )continue;
		if( !Rnd100(pMember->PasonalEffect.AntiTrap) )continue;

		System.SoundSystem.Play(_T("trap_cancel"));
		String Msg = pMember->GetPasonName();
		Msg += System.Cache.GetMessage(_T("trap_cancel"));
		System.WindowSys.PopupMsg( NULL, Msg );
		return true;
	}
	return false;
}
