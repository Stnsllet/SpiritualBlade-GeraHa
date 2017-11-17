// MoveAlgo.h: MoveAlgo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEALGO_H__E37BAA41_AC81_4C41_97C9_A8A253C9190C__INCLUDED_)
#define AFX_MOVEALGO_H__E37BAA41_AC81_4C41_97C9_A8A253C9190C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbPoint.h"
#include "RbPosition.h"
#include "MoveList.h"
#include "LandMoveType.h"
struct BattleStatus;
class RbActStatus;
class BattleSystem;
class LandMoveType;

class MoveAlgo  
{
public:
	MoveAlgo( RbActStatus& RefState, BattleStatus& RefBattle, BattleSystem& RefSystem );
	~MoveAlgo();
	bool SetMoveSpeed( float NewSpeed );
	bool SetPosition( const RbPosition& Pos );
	bool SetPosition( const RbPosition& LeaderPos, const RbPointF& FormPos, float NewAngle );
	bool SetMoveList( const Move::List NewList, bool bEscapeOK=false );
	bool SetMovePos( float x, float y, bool bEscapeOK=false );
	bool SetMovePos( RbActStatus* pTaget, bool bEscapeOK=false );
	bool ActMove();
	bool ActMove( float x, float y );
	bool Cancel();
	bool Stop();
	bool MoveToFormation();
	bool InFormation() const;
	bool KnockBackJob();
	bool NoKnockBack();
	bool AddKnockBack( const D3DVECTOR& Add );
	bool CheckLeaderLength();
	bool IsUnRadar() const;
	bool IsUnbush() const;
	bool MoveInterval( double Check=500 ) const;
	bool InMovePos() const;
	bool IsForce() const{return bForceMove;}
	bool IsFromUser() const{return bFromUser;}

	const float& MyLandHight() const{return LandData.LandHight;}
	const float& RefLandSpeed() const{return LandData.Move;}
	const float& RefLandDmg() const{return LandData.Dmg;}
	float& Run(){return Running;}
	float ResetAngle();
private:
	bool Move( float x, float y );
	void ResetLand();
	float SetAngle( const RbPointF& DestPos );
	bool SetMoveType( const String& Type );
	RbPosition GetFormPos() const;
	bool TurnCheck();
	bool TurnWait();
	const RbPosition& RefLeaderPos() const;
	bool PatyInFormation() const;
	bool RecheckMoveList( const RbPosition& DestPos, float Size );
	RbPointF GetArmyPos( float TagetX, float TagetY ) const;

	Move::List MovingList;
	BattleSystem& System;
	RbPosition& NowPos;
	RbActStatus& Ref;
	const LandMoveType* pMoveDef;
	float& Angle;
	const bool& bLeader;
	float& FlyHight;
	RbPosition KnockBack;
	float KnockBackLength;
	double LastMoveTime;
	bool bFromUser;
	bool bForceMove;
	bool bDontMove;

	mutable struct _ArmyPos
	{
		RbPointF Pos;
		double CheckTime;
	}ArmyPos;
	float MoveSpeed;
	float MoveMainSpeed;
	float MoveTypeSpeed;
	float Running;
	RbPointF MoveTo;
	struct Formation
	{
		float CheckSize2;
		RbPointF BasePos;
		RbPointF Pos;
		bool bTurnWait;
		RbPointF TagetMovePos;
	}Form;

	struct _Land : public LandMoveType::MoveData
	{
		float LandHight;
	}LandData;
};

#endif // !defined(AFX_MOVEALGO_H__E37BAA41_AC81_4C41_97C9_A8A253C9190C__INCLUDED_)
