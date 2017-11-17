// XMLCache.cpp: XMLCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XMLCache::XMLCache()
{
}

XMLCache::~XMLCache()
{
	TermThread();
}

void XMLCache::Clear()
{
	CriticalSection Locker(Lock);
	CacheList.clear();
}

bool XMLCache::LoadCache( const String& ReadData )
{
	XMLPaser FindXML = Find( ReadData );
	if( !!FindXML )return false;
	HRESULT hr = FindXML.Load( ReadData );
	if( FAILED(hr) )return false;
	{
		CriticalSection Locker(Lock);
		if( CacheList.find( ReadData )!=CacheList.end() )return false;
		CacheList[ReadData] = FindXML;
	}
	return true;
}

XMLPaser XMLCache::Get( const String& Path )
{
	if( Path.empty() )return XMLPaser();
	XMLPaser FindXML = Find( Path );
	if( !!FindXML )return FindXML;

	LoadCache( Path );
	FindXML = Find( Path );
	return FindXML;
}

XMLPaser XMLCache::Find( const String& Path )
{
	if( Path.empty() )return XMLPaser();
	CriticalSection Locker(Lock);
	std::map<String,XMLPaser>::const_iterator itr = CacheList.find( Path );
	if( itr == CacheList.end() )return XMLPaser();
	return itr->second;

}
