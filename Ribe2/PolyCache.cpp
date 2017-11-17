// PolyCache.cpp: PolyCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbDisplay.h"
#include "PolyCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

PolyCache::PolyCache()
{
}

PolyCache::~PolyCache()
{
	TermThread();
}

void PolyCache::Clear()
{
	CriticalSection Locker(Lock);
	PolyMap.clear();
}

RbPolyPoly* PolyCache::Get( const String& Name )
{
	long PolyID = GetID( Name );
	return Get( PolyID );
}

RbPolyPoly* PolyCache::Get( long PolyID )
{
	RbPolyPoly* pGet = Find( PolyID );
	if( pGet != NULL )return pGet;

	LoadCache( PolyID );
	pGet = Find( PolyID );
	return pGet;
}

long PolyCache::GetID( const String& Name )
{
	CriticalSection Locker(Lock);
	return PolyID.GetID( Name );
}

RbPolyPoly* PolyCache::Find( long PolyID )
{
	CriticalSection Locker(Lock);
	std::map<long,RefPtr<RbPolyPoly> >::const_iterator itr = PolyMap.find( PolyID );
	if( itr == PolyMap.end() )return NULL;
	return itr->second;
}

bool PolyCache::LoadCache( long LoadID )
{
	String Name;
	{
		CriticalSection Locker(Lock);
		Name = PolyID.GetString( LoadID );
		if( Name.empty() )return false;
	}
	return LoadCache( Name );
}

bool PolyCache::LoadCache( const String& ReadData )
{
	long PolyID = GetID( ReadData );
	RbPolyPoly* pFind = Find( PolyID );
	if( pFind != NULL )return false;

	RefPtr<RbPolyPoly> pNew( new RbPolyPoly );
	if( FAILED(pNew->LoadByName( GetRbDisplay(), ReadData )) )return false;

	{
		CriticalSection Locker(Lock);
		if( PolyMap.find( PolyID )!=PolyMap.end() )return false;
		PolyMap[PolyID] = pNew;
	}
	return true;
}
