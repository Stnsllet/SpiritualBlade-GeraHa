// LevelUpSystem.h: LevelUpSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEVELUPSYSTEM_H__27CC3905_3EB5_4249_B600_80678DE0C641__INCLUDED_)
#define AFX_LEVELUPSYSTEM_H__27CC3905_3EB5_4249_B600_80678DE0C641__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PasonCache.h"
#include "JobCache.h"
struct LearningSkill;

class LevelUpSystem  
{
public:
	typedef JobCache::JobST JobST;
	LevelUpSystem( MemberStatus& ref );
	~LevelUpSystem();
	bool AddExp( const MemberStatus& Enemy, float UpScale, bool bDead );

	bool LevelUp( int Up=1 );
	void CreateMember( long PasonID, LPCTSTR JobName, int Level=1 );
	static bool SetMaxLevel( long NewMax );

	bool ExecPow( const ItemID& Item );
	bool ExecPow( const ItemStatus& Item );
	bool ClassChange( const String& NextJob );
	bool AddSkill( LPCTSTR SkillName, long Level=1);
private:
	bool AddSkill( const LearningSkill& Learning, long OldAllLevel );
	bool PowItem( const String& Name, long Pow );
	bool ClassChange( long Level, const String& NextJob );
	void StausUp( const PasonSt& PasonData, const JobST& JobData, long UpCount=1 );
	bool LevelUp( const PasonSt& PasonData, const JobST& JobData, long UpCount=1 );
	float LevRnd() const;
	MemberStatus& Status;
	static long WavNo;
	static long MaxLevel;
};

#endif // !defined(AFX_LEVELUPSYSTEM_H__27CC3905_3EB5_4249_B600_80678DE0C641__INCLUDED_)
