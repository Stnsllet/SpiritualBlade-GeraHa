// LevelUpSystem.cpp: LevelUpSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbSound.h"
#include "RbMember.h"
#include "ExpSystem.h"
#include "LevelUpSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

long LevelUpSystem::WavNo = 0;
long LevelUpSystem::MaxLevel = 99;
// Rnd 9.95 ～ 1.05
float LevelUpSystem::LevRnd() const
{
	return Rndf2(0.995f,1.05f);
}

bool LevelUpSystem::SetMaxLevel( long NewMax )
{
	if( NewMax <= 0 )return false;
	MaxLevel = NewMax;
	return true;
}

LevelUpSystem::LevelUpSystem( MemberStatus& ref ) : Status(ref)
{
	if( WavNo == 0 )WavNo = GetSoundSystem().GetID(_T("levup"));
}

LevelUpSystem::~LevelUpSystem()
{
}
// 経験値追加
bool LevelUpSystem::AddExp( const MemberStatus& Enemy, float UpScale, bool bDead )
{
	ExpSystem Exp( Status, MaxLevel );
	long UpCount = Exp.AddExp( Enemy, UpScale, bDead );
	if( UpCount <= 0 )return false;
	if( !LevelUp( UpCount ) )return false;
	GetSoundSystem().Play( WavNo );
	return true;
}

// Level 1 データを作る
void LevelUpSystem::CreateMember( long PasonID, LPCTSTR JobName, int Level )
{
	const RbCache& Cache = GetCache();
	const PasonSt& PasonData = Cache.Pason.Get( PasonID );
	const JobST& JobData = Cache.Job.Get( JobName );

	AllZero( Status );

	Status.PasonName = PasonData.NameID;
	Status.JobName = JobData.NameID;

	Status.Status = PasonData.FirstStatus;

	Status.Status *= 40;
//	Status.Status.Hp *= 1.5f;
//	Status.Status.Mp *= 1.5f;
	Status.Status.Move = PasonData.FirstStatus.Move;

	StausUp( PasonData, JobData, 2 );	// 2 レベル分 追加
	LevelUp( PasonData, JobData,  Level-Status.Level );
}

// Level Up する
bool LevelUpSystem::LevelUp( int Up )
{
	const RbCache& Cache = GetCache();
	const PasonSt& PasonData = Cache.Pason.Get( Status.PasonName );
	const JobST& JobData = Cache.Job.Get( Status.JobName );

	return LevelUp( PasonData, JobData,  Up );
}

void LevelUpSystem::StausUp( const PasonSt& PasonData, const JobST& JobData, long UpCount )
{
	DefaultStatus AveGrow = PasonData.Grow.Status;
	AveGrow *= JobData.Grow.Status;
	AveGrow *= 3;	// 100 で 約 3 ポイント成長
	AveGrow.Hp *= 2.0f;
	AveGrow.Mp *= 2.0f;

	AveGrow.Hp *= LevRnd();
	AveGrow.Mp *= LevRnd();
	AveGrow.Attack *= LevRnd();
	AveGrow.Defence *= LevRnd();
	AveGrow.Magic *= LevRnd();
	AveGrow.Speed *= LevRnd();

	DefaultStatus AppendState;
	AppendState = Status.Append.Status;
	AppendState *= 0.5; // ひとつ0.5 ポイント(13%)
	AveGrow += AppendState;

	AveGrow *= (float)UpCount;

	AveGrow.Move = 0.0f;	// Move は成長しない

	Status.Status += AveGrow;
}

bool LevelUpSystem::LevelUp( const PasonSt& PasonData, const JobST& JobData, long UpCount )
{
	if( UpCount <= 0 )return true;
	if( Status.Level >= MaxLevel )return false;
	long OldAllLevel = Status.AllLevel;
	Status.Level += UpCount;
	Status.AllLevel += UpCount;
	StausUp( PasonData, JobData, UpCount );
//	RbMember Mem( Status );

	AddSkill( PasonData.SkillList, OldAllLevel );
	AddSkill( JobData.SkillList, OldAllLevel );

	return true;
}

bool LevelUpSystem::AddSkill( const LearningSkill& Learning, long OldAllLevel )
{
	int Count = Learning.Count();
	for( int i= 0; i < Count; i++ )
	{
		const LearningSkill::Data& RefSkill = Learning[i];
		if( RefSkill.Level > Status.AllLevel )continue;
		if( RefSkill.Level <= OldAllLevel )continue;	// 習得済み
		AddSkill( RefSkill.name );
	}
	return true;
}

bool LevelUpSystem::AddSkill( LPCTSTR SkillName, long Level)
{
	if( SkillName == NULL )return false;

	RbMember Mem( Status );
	long Number = Mem.CheckSkill( SkillName );
	if( Number >= 0 )
	{
		char& RefLevel = Status.Skill[Number].Level;
		if( RefLevel >= SkillCache::MaxLevel )return false;
		RefLevel = (char)min(RefLevel+Level, SkillCache::MaxLevel);
		return true;
	}
	Number = Mem.FirstEmptySkill();
	if( Number < 0 )return false;
	Status.Skill[Number].name = SkillName;
	Status.Skill[Number].Level = min(Level, SkillCache::MaxLevel);

	return true;
}

bool LevelUpSystem::ExecPow( const ItemID& Item )
{
	if( Item.EquipType != 4 )return false;
	return ExecPow(GetCache().Item.Get(Item));
}

bool LevelUpSystem::ExecPow( const ItemStatus& Item )
{
	if( Item.id.EquipType != ePowTypeItem )return false;
	StringSplitter<> Split( Item.Ex );
	const String& Com = Split[0];

	if( Com==_T("pow") )return PowItem( Split[1], Split[2].ToLong() );
	if( Com==_T("skill") )return AddSkill( Split[1], Split[2].ToLong() );
	if( Com==_T("class") )return ClassChange( Split[1].ToLong(), Split[2] );

	return false;
}


bool LevelUpSystem::PowItem( const String& Name, long Pow )
{
	if( Pow <= 0 )Pow = 1;
	if( Name==_T("lev") )
	{
		if( Status.Level <= 1 )return false;
		Status.Level = max(1,Status.Level-Pow);
		Status.Append.Level += 5;
		return true;
	}
	StatusEdit<> Editer(Status.Append.Status);
	Editer.RefState(Name) += Pow;
	return true;
}

bool LevelUpSystem::ClassChange( long Level, const String& NextJob )
{
	if( Status.Level < Level )
	{
		SystemPopup(_T("lowlevel"));
		return false;
	}
	String NewJobName = NextJob;
	if( NewJobName.empty() )
	{
		NewJobName = GetCache().Job.Get(Status.JobName).NextJob;
		if( NewJobName.empty() )return false;
	}
	return ClassChange( NewJobName );
}

bool LevelUpSystem::ClassChange( const String& NextJob )
{
	if( NextJob.empty() )return false;
	if( NextJob == (LPCTSTR)Status.JobName )return false;
	Status.JobName = NextJob;
	const JobST& JobData = GetCache().Job.Get( Status.JobName );
	AddSkill( JobData.SkillList, 0 );
	return true;
}
