// PolyCache.h: PolyCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYCACHE_H__3F1907B9_D8B8_41F4_9027_A40519E473F0__INCLUDED_)
#define AFX_POLYCACHE_H__3F1907B9_D8B8_41F4_9027_A40519E473F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "StringIDMap.h"
#include "BackReader.h"
#include "RbPolyPoly.h"

class PolyCache : protected BackReader<String>
{
public:
	PolyCache();
	virtual ~PolyCache();

	void Cache( const String& Add ){Que.Push(Add);}
	void Clear();

	RbPolyPoly* Get( const String& Name );
	RbPolyPoly* Get( long PolyID );
	long GetID( const String& Name );
protected:
	RbPolyPoly* Find( long PolyID );
	bool LoadCache( long LoadID );
	virtual bool LoadCache( const String& ReadData );
	std::map<long,RefPtr<RbPolyPoly> > PolyMap;
	IDManager PolyID;
};

#endif // !defined(AFX_POLYCACHE_H__3F1907B9_D8B8_41F4_9027_A40519E473F0__INCLUDED_)
