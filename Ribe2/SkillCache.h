// SkillCache.h: SkillCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLCACHE_H__B543C48C_5BA6_41DC_9C38_87931FD2C128__INCLUDED_)
#define AFX_SKILLCACHE_H__B543C48C_5BA6_41DC_9C38_87931FD2C128__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbRect.h"
#include "StringIDMap.h"
#include "DefaultPlanes.h"

class XMLPaser;

class SkillCache  
{
public:
	struct SkillData
	{
		long NoID;
		String NameID;
		String Type;
		String Name;
		String Help;
		String Action;
		String EffectName;
		Plane<> Pos;
		float Shake;
		std::map<long,SkillEffect> LevelList;
		SkillEffect Level( long Lev ) const;
	};

	SkillCache();
	virtual ~SkillCache();

	long GetSkillNo( const String& name ) const;
	const SkillData* GetSkill( const String& name ) const;
	const SkillData* GetSkill( long ID ) const;
	SkillEffect GetEffect( long ID, long Level ) const;
	RbRect GetBaseRect() const;
	Plane<> GetBasePlane( LPCTSTR name ) const;
	Plane<> GetLevelPlane( long Level ) const;
	HRESULT ReadSkillData( DISPDEVICE* pDevice );
	HRESULT ReadSkillData(  DISPDEVICE* pDevice , LPCTSTR FileName );

	const IDManager& RefIDManager() const{return SkillNo;}
	static long MaxLevel;
protected:
	HRESULT ReadSkill( const XMLPaser& SkillXML );
	HRESULT ReadSkillList( const XMLPaser& AllXML );
	HRESULT ReadLevelPlane( const XMLPaser& AllXML );
	HRESULT ReadBasePlane( const XMLPaser& AllXML );

	IDManager SkillNo;
	std::map<long,Plane<> > LevelPlaneList;
	std::map<String,Plane<> > BasePlaneList;
	std::map<long,SkillData> SkillList;
	Texture SkillTextue;
	DefaultPlanes NamedPlane;
};

#endif // !defined(AFX_SKILLCACHE_H__B543C48C_5BA6_41DC_9C38_87931FD2C128__INCLUDED_)
