// PathTexCache.h: PathTexCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHTEXCACHE_H__A019CDBC_AB8E_49F6_9136_737DFB4A4091__INCLUDED_)
#define AFX_PATHTEXCACHE_H__A019CDBC_AB8E_49F6_9136_737DFB4A4091__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "String.h"
#include "Plane.h"
#include "BackReader.h"

class PathTexCache : protected BackReader<String>
{
public:
	PathTexCache();
	virtual ~PathTexCache();
	Plane<> Get( const String& Path );
	void Clear();

	void Cache( const String& Add ){Que.Push(Add);}
protected:
	Plane<> Find( const String& Path );
	virtual bool LoadCache( const String& ReadData );
	std::map<String,Plane<> > TextureMap;
};

#endif // !defined(AFX_PATHTEXCACHE_H__A019CDBC_AB8E_49F6_9136_737DFB4A4091__INCLUDED_)
