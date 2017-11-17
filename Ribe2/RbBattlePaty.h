// RbBattlePaty.h: RbBattlePaty2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBBATTLEPATY_H__853459E0_642C_40BB_BED3_2A84B8AD5E5D__INCLUDED_)
#define AFX_RBBATTLEPATY_H__853459E0_642C_40BB_BED3_2A84B8AD5E5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbActStatus.h"

class XMLPaser;
class BattleSystem;

class RbBattlePaty : public RefAuto
{
public:
	struct PatyMemberData
	{
		float formX, formY;
	};
	typedef RbActStatus::PatyState PatyState;

	RbBattlePaty();
	~RbBattlePaty();
	bool DropJob();
	bool EscapePaty();

	bool SetState( const XMLPaser& PatyXML );
	HRESULT SetPaty( long PatyNo, float x, float y, float angle );
	HRESULT SetPaty( const PatyStatus& Data, float x, float y, float angle );
	HRESULT SetPaty( const XMLPaser& PatyXML );
	bool ChangeState( PatyState NewState );

	float GetBrave() const{return Brave;}
	float BraveUp( float Change );
	long ID() const{return BattleID;}
	long GetArmyID() const{return ArmyID;}
	bool IsAlivePaty() const{return (LiveCount>0);}
	bool IsUserData() const{return (Army==_T("user"));}
	float GetEscapeLife() const{return EscapeLife;}
	float GetFormationRange() const{return FormationRange;}
	bool InFormation() const;
	bool IsDeleteOK() const;

	bool ChangeTactics( TacticsAlgoType NewTactics );
	PatyState GetPatyStatus() const{return State;}
	RbActStatus* GetLeader() const{return pLeader;}
	PatyMemberData GetMemberData( long MemberID ) const;
	void DeadCheck( long MemberID );
	long MemberCount() const{return MemberList.size();}
	RbActStatus* GetMember( long Index ) const;
	const TacticsAlgoType& RefTacticsType() const{return AlgoType;}
	static RbPosition GetEventPos( const BattleSystem& System, const XMLPaser& PatyXML, long ArmyID );
protected:
	float ChangeBrave( float Change );
	void LiveCheck();
	void ResetPosition( float x, float y, float Angle );
	RbActStatus* AddMember( const MemberStatus& Status, float formX, float formY );

	static long IDManage;
	long BattleID;
	PatyState State;
	TacticsAlgoType AlgoType;
	float Brave;
	float EscapeLife;
	float MaxSize;
	float FormationRange;
	long LiveCount;

	String Army;
	long ArmyID;
	RefPtr<RbActStatus> pLeader;
	std::vector<RefPtr<RbActStatus> > MemberList;
	std::map<long,PatyMemberData> MemberDataList;

	BattleSystem& System;
};

#endif // !defined(AFX_RBBATTLEPATY_H__853459E0_642C_40BB_BED3_2A84B8AD5E5D__INCLUDED_)
