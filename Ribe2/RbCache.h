// RbCache.h: RbCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBCACHE_H__F2BF7388_DA17_4CEF_AF70_8280704165D4__INCLUDED_)
#define AFX_RBCACHE_H__F2BF7388_DA17_4CEF_AF70_8280704165D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "RbTexture.h"
#include "StringIDMap.h"
#include "JobCache.h"
#include "ItemCache.h"
#include "PasonCache.h"
#include "SkillCache.h"
#include "TextureCache.h"
#include "DefaultPlanes.h"
#include "XMLCache.h"
#include "PathTexCache.h"
#include "RbPlaneCache.h"
#include "LandType.h"
#include "PolyCache.h"
#include "FaceCache.h"

class RbCache  
{
public:
	typedef RefPtr<RbTexture> TexPtr;
	RbCache();
	~RbCache();

	bool Clear();
	bool LightCache();

	HRESULT InitCache( DISPDEVICE* pDevice );

	bool GetTexture( const String& NameID, TexPtr& pTexture );
	const String& GetMessage( const String& name ) const;

public:
	IDManager ActID;
	ItemCache Item;
	JobCache Job;
	PasonCache Pason;
	SkillCache Skill;

	DefaultPlanes SystemPlane;

	XMLCache XML;
	PathTexCache PathTex;
	RbPlaneCache Plane;
	TextureCache RbTex;
	FaceCache Face;
	PolyCache Poly;
	LandType Land;
protected:
	std::map<String,String> SystemMsg;
	HRESULT LoadSystemMsg( LPCTSTR FileName );
};

RbCache& GetCache();

#endif // !defined(AFX_RBCACHE_H__F2BF7388_DA17_4CEF_AF70_8280704165D4__INCLUDED_)
