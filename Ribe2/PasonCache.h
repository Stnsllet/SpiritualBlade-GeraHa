// PasonCache.h: PasonCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PASONCACHE_H__57773E45_904E_4938_B958_3FF5BF6964A1__INCLUDED_)
#define AFX_PASONCACHE_H__57773E45_904E_4938_B958_3FF5BF6964A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "StringIDMap.h"
#include "Statusstruct.h"
#include "LearningSkill.h"

class XMLPaser;

class PasonCache  
{
public:
	struct PasonST : public PasonStatus
	{
		LearningSkill	SkillList;
		std::vector<short> EmployEquip;
	};
	typedef std::set<long> MinorMap;
public:
	PasonCache();
	virtual ~PasonCache();

	HRESULT ReadPasonData();
	HRESULT ReadPasonData( LPCTSTR FileName );
	const PasonST& RefDefault() const;
	const PasonST& Get( const String& PasonName ) const;
	const PasonST& Get( long PasonID ) const;
	long GetPasonID( const String& PasonName ) const;
	const MinorMap& RefMinor() const{return MinorList;}
	bool IsMain( long PasonID ) const;

private:
	HRESULT ReadPasonData( const XMLPaser& PasonData, bool bMain, bool bEmploy );

	IDManager PasonIDList;
	typedef std::map<long,PasonST> PasonMap;
	typedef std::set<long> MinorMap;
	PasonMap PasonList;
	MinorMap MinorList;
	PasonST DefaultData;
};

typedef PasonCache::PasonST PasonSt;

#endif // !defined(AFX_PASONCACHE_H__57773E45_904E_4938_B958_3FF5BF6964A1__INCLUDED_)
