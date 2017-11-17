// EquipSkill.h: EquipSkill クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQUIPSKILL_H__267CE337_D959_43E9_B0CF_A0CB3B5A2D6D__INCLUDED_)
#define AFX_EQUIPSKILL_H__267CE337_D959_43E9_B0CF_A0CB3B5A2D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "StatusStruct.h"
#include "BattleEffect.h"
class SkillCache;

class EquipSkill  
{
public:
	struct SkillDef
	{
		SkillDef();
		bool AddLevel( long Lev );
		bool AddLevel( long Lev, long SetID );
		long ID;
		long Level;
	};
	class SkillLists : protected std::map<long, SkillDef>
	{
		friend class EquipSkill;
	public:
		SkillDef Get( long Index ) const;
		SkillDef GetSkill( long ID ) const;
		SkillEffect GetEffect( long SkillID ) const;
		long GetCount() const{return size();}
	};
	EquipSkill();
	~EquipSkill();

	void Clear();

	bool SetMemberSkill( const MemberStatus& MemStatus );

	long AllSkillCount() const;
	SkillDef Get( long Index ) const;

	SkillLists Action;
	SkillLists Pasonal;
	SkillLists Leader;
	BattleEffect Effect;
private:
	bool AddSkill( long SkillID, long Level );
	bool AddSkill( LPCTSTR NameID, long Level );
	bool AddItemSkill( short ItemNo );
	bool AddActionSkill( long ID, long Level );
	bool AddPasonalSkill( long ID, long Level );
	bool AddLeaderSkill( long ID, long Level );

	const SkillCache& Cache;
};

#endif // !defined(AFX_EQUIPSKILL_H__267CE337_D959_43E9_B0CF_A0CB3B5A2D6D__INCLUDED_)
