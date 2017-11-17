// BattleSystemStatus.h: BattleSystemStatus クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLESYSTEMSTATUS_H__99F4C71B_1535_4205_A85B_41B682BB2746__INCLUDED_)
#define AFX_BATTLESYSTEMSTATUS_H__99F4C71B_1535_4205_A85B_41B682BB2746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "MoveList.h"
#include "RbPolyPoly.h"
class BattleSystem;
class BillbordPlane;
class RbGameData;

struct BattleSystemStatus  
{
public:
	struct ResultData
	{
		void Clear();
		bool SetXML( const XMLPaser& ResultXML );
		bool SetResult( RbGameData& UserData );
		void SetMaxCombo( long Combo );
		bool bSetDone;
		float Kill;
		float Medical;
		long Bonus;
		long MaxCombo;
		long GetKill( const BattleSystem& System ) const;
		long GetMedical( const BattleSystem& System ) const;
		RbBuffer<long> ItemList;
	};
	BattleSystemStatus(BattleSystem& Ref);
	~BattleSystemStatus();
	ResultData& RefResult(){return Result;}
	const ResultData& RefResult() const {return Result;}

	bool SetDrawCursor( bool bNew );
	bool Clear();
	bool WaitChange();
	bool SpeedChange(){return Com.bSpeed=!Com.bSpeed;}
	bool MoveChange();

	bool SetBlackOut( bool bNew=true){SysState.bBlackOut=bNew;return true;}
	bool IsBlackOut() const{return SysState.bBlackOut;}

	HRESULT Draw( Display& Disp, const GameMessage& MouseState );
	HRESULT DrawCursor( Display& Disp, const GameMessage& MouseState );
	bool AddPoint( const RbPosition& Pos, bool bCheck=false );
	void ClearPoints(){LinePoints.clear();}
	bool IsMoveMode() const{return Com.bMove;}
	bool IsSpeedMode() const{return Com.bSpeed;}
	bool IsWaitMode() const{return Com.WaitLocker.IsLock();}

	bool MoveMouseJob( Display& Disp, GameMessage& State );
	bool SetPayMove( bool bNew ){return (bPatyMove = bNew);}
	bool IsPatyMove() const{return bPatyMove;}
private:
	bool GetMoveList( const RbPosition& TagetPos, Move::List& OutList ) const;
	float GetMoveSize( const RbPosition& StartPos, const RbPosition& TagetPos, bool bMin=false ) const;
	RbPosition GetStartPos() const;
	struct BComMode
	{
		BComMode();
		bool bMove;
		bool bSpeed;
		LockCounter::Locker MoveLocker;
		LockCounter::Locker WaitLocker;
	}Com;
	struct LinePos
	{
		RbPosition Pos;
	};
	ResultData Result;
	double LastCheck;
	RbPosition LastCheckPos;
	struct _SysState
	{
		bool bDrawCursor;
		bool bBlackOut;
	}SysState;
	bool bPatyMove;

	Move::List MoveList;
	std::list<LinePos> LinePoints;
	RefPtr<RbPolyPoly> pCursor;
	BattleSystem& System;

	static const float ClickNiar2;
};

#endif // !defined(AFX_BATTLESYSTEMSTATUS_H__99F4C71B_1535_4205_A85B_41B682BB2746__INCLUDED_)
