// RbAttackAlgo.h: RbAttackAlgo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBATTACKALGO_H__77E4F2E9_91CF_4185_9C6E_566339717616__INCLUDED_)
#define AFX_RBATTACKALGO_H__77E4F2E9_91CF_4185_9C6E_566339717616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "WeponSystem.h"
#include "DirectAttacker.h"
#include "StatusStruct.h"

class BattleSystem;
class RbActStatus;
class RbTexture;
class TobiObjectBase;
class TobiCreator;
class TagetAlgo;

class RbAttackAlgo  
{
public:
	RbAttackAlgo( BattleSystem& RefSystem, RbActStatus& Source, TagetAlgo& RefTagetSys );
	virtual ~RbAttackAlgo();

	bool TagetInRange();
	bool AttackStart();
	bool AttackJob( const int& ActTime );
	void AttackEnd();
	bool CheckMp() const;
	RbActStatus* GetTaget() const{return TagetSys.GetNowTaget();}

	long ActID() const;
	long SpecialID()const{return SPDat.SkillID;}
	const WeponSystem::WeponData& NowWepon() const{return Wepons.Now();}

	bool ChangeWepon( RbActStatus* pTaget );
	void CheckWepon();
	bool SpecialGo( long SkillID, long Level );
	bool SpecialEnd();

	bool InTagetRange( RbBattleStatus* pEnemy ) const;
	bool InAttackRange( RbBattleStatus* pEnemy ) const;
	bool WideWepon();

	RefBord* GetWeponImage() const;
protected:
	void TobiAttack();

protected:
	RbActStatus& Source;
//	RefPtr<RbActStatus> pTaget;
	WeponSystem::WeponList Wepons;
	TagetAlgo& TagetSys;
	DirectAttacker Direct;
	BattleSystem& System;
	long WeponRandom;
	double WepChangeTime;

	struct SpecialAttackData
	{
		bool Set( long ID, long Level, const RbActStatus& Source );
		long SkillID;
		long ActID;
		float Shake;
		SkillEffect Effect; 
		RefPtr<RefBord> pWeponImage;
		TobiCreator* pCreator;
	}SPDat;
};

#endif // !defined(AFX_RBATTACKALGO_H__77E4F2E9_91CF_4185_9C6E_566339717616__INCLUDED_)
