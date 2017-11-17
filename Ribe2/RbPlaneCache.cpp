// RbPlaneCache.cpp: RbPlaneCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbDisplay.h"
#include "RbPlaneCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbPlaneCache::RbPlaneCache()
{
}

RbPlaneCache::~RbPlaneCache()
{
	TermThread();
}

void RbPlaneCache::Clear()
{
	CriticalSection Locker(Lock);
	PlaneList.clear();
}

RbPlaneCache::PlanePtr RbPlaneCache::Find( const String& Path )
{
	if( Path.empty() )return PlanePtr();
	CriticalSection Locker(Lock);
	std::map<String,PlanePtr>::const_iterator itr = PlaneList.find( Path );
	if( itr == PlaneList.end() )return PlanePtr();
	return itr->second;
}

RbPlaneCache::PlanePtr RbPlaneCache::Get( const String& Path )
{
	if( Path.empty() )return PlanePtr();
	PlanePtr FindPlane = Find( Path );
	if( FindPlane != NULL )return FindPlane;

	LoadCache( Path );
	FindPlane = Find( Path );
	return FindPlane;
}

bool RbPlaneCache::LoadCache( const String& ReadData )
{
	PlanePtr FindPlane = Find( ReadData );
	if( FindPlane != NULL )return false;

	PlanePtr pNewPlane(new RefPlane);
	HRESULT hr = pNewPlane->LoadPlane( GetRbDisplay(), ReadData );
	if( FAILED(hr) )return false;

	{
		CriticalSection Locker(Lock);
		if( PlaneList.find( ReadData )!=PlaneList.end() )return false;
		PlaneList[ReadData] = pNewPlane;
	}
	return true;
}
