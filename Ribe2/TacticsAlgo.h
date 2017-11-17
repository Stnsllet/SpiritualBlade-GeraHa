// TacticsAlgo.h: TacticsAlgo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACTICSALGO_H__5EA60840_0624_4768_A31D_927E5495B000__INCLUDED_)
#define AFX_TACTICSALGO_H__5EA60840_0624_4768_A31D_927E5495B000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbPosition.h"
#include "StatusStruct.h"

class RbActStatus;
class BattleSystem;

class TacticsAlgo
{
public:
	TacticsAlgo( RbActStatus& Ref, BattleSystem& RefSystem );
	virtual ~TacticsAlgo();
	bool CheckTactics();
	bool ForceTactics() const;

	void SetTactics( TacticsAlgoType NewType ){Type=NewType;}
	const TacticsAlgoType& RefTactics() const{return Type;}
	static float TacticsRange;
	static TacticsAlgoType GetTactics( const String& Name );
protected:
	bool RndMoveAlgo();
	bool AttackAlgo();
	bool AttackAlgo( RbActStatus* pEnemy );
	bool HelpAlgo();
	bool SyncAttackAlgo();
	bool HelpAlgo( RbActStatus* pFriend );
	bool HelpAttack( RbActStatus* pFriend, RbActStatus* pEnemy );
	RbPosition _GetMovePosition( const RbPosition& BasePos, float BaseAngle ) const;
	RbPosition GetMovePosition( const RbPosition& BasePos, float BaseAngle ) const;
	bool MovePosCheck( const RbPosition& Pos ) const;
	RbPosition GetHalfPos( const RbPosition& Taget, const RbPosition& Base ) const;
	RbPosition ArmyTacticsPos( const RbPosition& Taget ) const;

	RbActStatus& Source;
	BattleSystem& System;
	TacticsAlgoType Type;
	double LastCheckTime;
	static double CheckInterval;

};

#endif // !defined(AFX_TACTICSALGO_H__5EA60840_0624_4768_A31D_927E5495B000__INCLUDED_)
