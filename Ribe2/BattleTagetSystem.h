// BattleTagetSystem.h: BattleTagetSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLETAGETSYSTEM_H__A77FEF4F_718B_418A_BEC7_9780991CA018__INCLUDED_)
#define AFX_BATTLETAGETSYSTEM_H__A77FEF4F_718B_418A_BEC7_9780991CA018__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbPosition.h"
#include "BattleDataManager.h"

class BattleTagetSystem : public BattleDataManager  
{
public:
	BattleTagetSystem();
	virtual ~BattleTagetSystem();
	typedef RbActStatus::PatyState PatyState;

	RbActStatus* GetEnemy( const RbBattleStatus& Source, float* pRange=NULL ) const;
	RbActStatus* GetEnemy( const RbPosition& Pos, long ArmyID, float* pRange=NULL ) const;
	RbActStatus* FindEnemy( const RbBattleStatus& Source, float* pRange=NULL ) const;
	RbActStatus* FindEnemy( const RbPosition& Pos, long ArmyID, float* pRange=NULL ) const;
	RbActStatus* GetRndEnemy( const RbActStatus& Source ) const;
	RbActStatus* GetRndEnemy( const RbPosition& Pos, long ArmyID, float Range ) const;
	RbActStatus* GetUnHelpFriendPaty( const RbBattleStatus& Source ) const;
	RbActStatus* GetFriendPaty( const RbBattleStatus& Source, PatyState CheckState=RbActStatus::ePatyNoState, float* pRange=NULL ) const;
	RbActStatus* GetFriendPaty( const RbPosition& Pos, long ArmyID, PatyState CheckState=RbActStatus::ePatyNoState, float* pRange=NULL, RbBattlePaty* pCheckPaty=NULL, bool bUnHelpPaty=false ) const;

	float GetDirection( const CharaList& List, const RbPosition& BasePos, float Dips, float Width=0, float StartAngle=0 ) const;
	long GetCharaList( CharaList& Output, const RbPosition& Pos, float Range, long ArmyID, bool bEnemy=true, bool bAll=false ) const;
	long ReCallTaget( MemCallBack* pCallBack, const RbPosition& Pos, float Range, long ArmyID, bool bEnemy=true, bool bAll=false ) const;
	long GetCharaCount( const RbPosition& Pos, float Range, long ArmyID, bool bEnemy=true, bool bAll=false ) const;
	long LeaderCount( RbActStatus* pBase, float Range, bool bEnemy=true ) const;
	long LeaderCount( const RbPosition& BasePos, long ArmyID, float Range, bool bEnemy=true ) const;
	long LeaderCount( long ArmyID ) const;
	long Count( const CharaList& List, long ArmyID, long* pFriend=NULL ) const;
	long Count( const D3DVECTOR& Pos, float Range, long ArmyID, long* pFriend=NULL ) const;
	float GetArmyPos( const RbPosition& BasePos, const RbPosition& TagetPos, long ArmyID) const;

	RbPosition CircleTaget( const D3DVECTOR& Pos, float BurstRange, float CheckRange, long ArmyID, float FirstAngle ) const;
};

#endif // !defined(AFX_BATTLETAGETSYSTEM_H__A77FEF4F_718B_418A_BEC7_9780991CA018__INCLUDED_)
