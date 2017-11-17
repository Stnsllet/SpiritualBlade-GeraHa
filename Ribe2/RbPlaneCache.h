// RbPlaneCache.h: RbPlaneCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBPLANECACHE_H__2BF2DB9B_1197_4FE8_A8FA_AB515C04C4F4__INCLUDED_)
#define AFX_RBPLANECACHE_H__2BF2DB9B_1197_4FE8_A8FA_AB515C04C4F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RefObject.h"
#include "DefaultPlanes.h"
#include "BackReader.h"

class RbPlaneCache : protected BackReader<String>
{
public:
	RbPlaneCache();
	virtual ~RbPlaneCache();

	struct RefPlane : public HandPlanes, public RefAuto
	{
	};
	typedef RefPtr<RefPlane> PlanePtr;

	void Cache( const String& Add ){Que.Push(Add);}
	PlanePtr Get( const String& DataStr );
	void Clear();

protected:
	PlanePtr Find( const String& FaceName );
	virtual bool LoadCache( const String& ReadData );

	std::map<String,PlanePtr> PlaneList;
};

#endif // !defined(AFX_RBPLANECACHE_H__2BF2DB9B_1197_4FE8_A8FA_AB515C04C4F4__INCLUDED_)
