// XMLCache.h: XMLCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLCACHE_H__FE4CC85C_3DF6_423A_BC45_3166F634E3AF__INCLUDED_)
#define AFX_XMLCACHE_H__FE4CC85C_3DF6_423A_BC45_3166F634E3AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "String.h"
#include "XMLPaser.h"
#include "BackReader.h"

class XMLCache : protected BackReader<String>
{
public:
	XMLCache();
	virtual ~XMLCache();
	XMLPaser Get( const String& Path );

	void Clear();
	void Cache( const String& Add ){Que.Push(Add);}
protected:
	XMLPaser Find( const String& Path );
	virtual bool LoadCache( const String& ReadData );
	std::map<String,XMLPaser> CacheList;
};

#endif // !defined(AFX_XMLCACHE_H__FE4CC85C_3DF6_423A_BC45_3166F634E3AF__INCLUDED_)
