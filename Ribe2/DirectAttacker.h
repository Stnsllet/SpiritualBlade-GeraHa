// DirectAttacker.h: DirectAttacker クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTATTACKER_H__45ABE0C3_7AC4_4AB7_A8A9_6BBCAAD9627C__INCLUDED_)
#define AFX_DIRECTATTACKER_H__45ABE0C3_7AC4_4AB7_A8A9_6BBCAAD9627C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>

class BattleSystem;
class RbActStatus;

class DirectAttacker  
{
public:
	DirectAttacker( BattleSystem& RefSystem, RbActStatus& RefSource );
	virtual ~DirectAttacker();

	bool AttackStart( long ActID, float Angle );
	bool AttackJob( int ActTime );
	bool AttackEnd();
protected:
	typedef std::list<RefPtr<RbActStatus> > CharaList;
	bool Attack( const CharaList& List, float Start, float End );
	bool AttackW( float Time );
	bool AttackR( float Time );
	bool IsNewHit( const RbActStatus& Ref );
	bool InAngle( const RbActStatus& Taget, const float& Start, const float& End ) const;

	BattleSystem& System;
	RbActStatus& Source;
	bool bAttacking;
	float StartAngle;
	float EndAngle;
	float AttackRange;
	int MaxTime;
	bool bSlash;

	std::set<long> HitList;
	float TimeScale;
};

#endif // !defined(AFX_DIRECTATTACKER_H__45ABE0C3_7AC4_4AB7_A8A9_6BBCAAD9627C__INCLUDED_)
