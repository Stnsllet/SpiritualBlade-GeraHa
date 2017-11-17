// TagetAlgo.h: TagetAlgo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAGETALGO_H__DB0D1094_45AA_4020_89FF_2429A647D844__INCLUDED_)
#define AFX_TAGETALGO_H__DB0D1094_45AA_4020_89FF_2429A647D844__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WeponSystem.h"

class BattleSystem;
class RbActStatus;

class TagetAlgo  
{
public:
	TagetAlgo( BattleSystem& RefSystem, RbActStatus& RefSource );
	virtual ~TagetAlgo();
	void ClearTaget();

	RbActStatus* GetNowTaget() const{return AttackTaget;}
	RbActStatus* GetMinEnemy();
	RbActStatus* GetAttackTaget();
	const float& GetMinRange() const{return MinRange;}
protected:
	struct TimeTaget
	{
		TimeTaget();
		bool CheckTime( BattleSystem& System ) const;
		double LastTime;
		RefPtr<RbActStatus> pTaget;
		operator RbActStatus*() const{return pTaget;}
	};
	float MinRange;
	TimeTaget MinTaget;
	TimeTaget AttackTaget;

	BattleSystem& System;
	RbActStatus& Source;
};

#endif // !defined(AFX_TAGETALGO_H__DB0D1094_45AA_4020_89FF_2429A647D844__INCLUDED_)
