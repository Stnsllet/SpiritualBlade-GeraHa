// JobCache.h: JobCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOBCACHE_H__C5D84CEF_CE64_49FF_9472_A2DE9E8BE9F3__INCLUDED_)
#define AFX_JOBCACHE_H__C5D84CEF_CE64_49FF_9472_A2DE9E8BE9F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "Statusstruct.h"
#include "LearningSkill.h"

class XMLPaser;

class JobCache  
{
public:
	struct JobST : public JobStatus
	{
		LearningSkill	SkillList;
	};
	JobCache();
	virtual ~JobCache();

	HRESULT ReadJobData();
	HRESULT ReadJobData( LPCTSTR FileName );
	const JobST& Get( const String& JobName ) const;

private:
	HRESULT ReadJobData( const XMLPaser& JobData );
	BattleAlgoType GetBatType( const String& Name ) const;

	typedef std::map<String,JobST> JobMap;
	JobMap JobList;
	JobST DefaultData;
};

#endif // !defined(AFX_JOBCACHE_H__C5D84CEF_CE64_49FF_9472_A2DE9E8BE9F3__INCLUDED_)
