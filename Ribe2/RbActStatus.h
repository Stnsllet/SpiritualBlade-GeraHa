// RbActStatus.h: RbActStatus クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBACTSTATUS_H__BF5D9FE2_98F5_45E7_BAEC_4AF3CDF40AFF__INCLUDED_)
#define AFX_RBACTSTATUS_H__BF5D9FE2_98F5_45E7_BAEC_4AF3CDF40AFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ActLocker.h"
#include "MoveList.h"
#include "MoveAlgo.h"
#include "TacticsAlgo.h"
#include "TagetAlgo.h"
#include "RbAttackAlgo.h"
#include "RbBattleStatus.h"

class RbActStatus : public RbBattleStatus  
{
friend class MoveAlgo;
public:
	enum PatyState
	{
		ePatyNoState = -1,
		ePatyWait = 0,
		ePatyMove,
		ePatyEscape,
		ePatyBattle ,
		ePatyAmbush ,
	};
	RbActStatus();
	virtual ~RbActStatus();
	virtual HRESULT SetData( RbBattlePaty* pPatyData, const MemberStatus& Member );
	virtual const ActTime* GetNowImage() const;
	virtual long GetWeponID() const;
	virtual void GetWeponImage( BillbordPlane& Output ) const;
	virtual RefBord* GetWeponImage2() const;
	bool IsAction() const;
	bool IsActLock() const;
	bool IsPatyMoving() const;
	bool IsPatyBattle() const;
	const float WaitTime() const{return 80000.0f;} 
	bool CounterAction();
	bool IsActOK() const{return (Status.Wait >= WaitTime())?(true):(false);};

	virtual bool DropJob();
	void Job( BattleSystem& System );
	bool Damege( float Damege, float DamegeWait, const D3DVECTOR& KockBack );
	bool RecoverLife( float Hp, float Mp=0 );

	void ChangeState( PatyState NewState );
	bool PatyChangeState( PatyState NewState );
	const PatyState& GetPatyState()const{return MyPatyStatus;}
	RbActStatus* GetLeader()const;

	bool SpecialGo( const String& SkillName );
	bool SpecialGo( long SkillID );
	float TagetAngle( RbActStatus* pTaget );
	bool MoveCancel();

	TagetAlgo TagetSys;
	RbAttackAlgo AttackAlgo;
	MoveAlgo AlgoMove;
	TacticsAlgo AlgoTactics;
	ActLocker::Obj ActLock;
	ActLocker SpecialLock;
	ActLocker AttackLock;
protected:
	void Regenation();
	void Waiting();

	void Wait();
	void Cancel();
	void Stop();
	void Recover();
	void Attack();
	void Dead();
	bool DeadCheck();
	void Escape( RbActStatus* pTaget );
	void EscapeMove( RbActStatus* pTaget );

	void ActStart( BattleAction Action, bool bActTimeReset=true );

	void ActMove( BattleSystem& System );
	void ActWait( BattleSystem& System );
	void ActStop( BattleSystem& System );
	void ActAttack( BattleSystem& System );
	void ActDamege( BattleSystem& System );
	void ActDead( BattleSystem& System );
	void ActRecover( BattleSystem& System );
	void ActSpecial( BattleSystem& System );

	void _Job( BattleSystem& System );
	void AlgoBattle( BattleSystem& System );

	bool AddActTime( BattleSystem& System );
	bool BattleCheck( BattleSystem& System );

	void ResetCombo();
	bool ReCheckSpecial();
private:
	PatyState MyPatyStatus;
	Move::List MovingList;
	const NamedID& MyName;
};

#endif // !defined(AFX_RBACTSTATUS_H__BF5D9FE2_98F5_45E7_BAEC_4AF3CDF40AFF__INCLUDED_)
