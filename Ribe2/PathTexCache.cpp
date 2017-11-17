// PathTexCache.cpp: PathTexCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SquareTexture.h"
#include "RbDisplay.h"
#include "PathTexCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

PathTexCache::PathTexCache()
{
}

PathTexCache::~PathTexCache()
{
	TermThread();
}

void PathTexCache::Clear()
{
	CriticalSection Locker(Lock);
	TextureMap.clear();
}

bool PathTexCache::LoadCache( const String& ReadData )
{
	Plane<> FindPlane = Find( ReadData );
	if( FindPlane.pTexture != NULL )return false;
	SquareTexture Tex;
	HRESULT hr = Tex.LoadSquare( GetRbDisplay(), ReadData );
	FAILEDTRACE( hr );
//	ATLASSERT( !FAILED(hr) );
	if( FAILED(hr) )return false;
	{
		CriticalSection Locker(Lock);
		if( TextureMap.find( ReadData )!=TextureMap.end() )return false;
		Plane<>& Ref = TextureMap[ReadData];
		Ref.pTexture = Tex;
		Ref.x = Ref.y = 0;
		Ref.w = Tex.ImageInfo().Width;
		Ref.h = Tex.ImageInfo().Height;

	//	ATLTRACE(_T("Cache File(%04x) %s\n"), GetCurrentThreadId(),(LPCTSTR)ReadData );

	}
	return true;
}

Plane<> PathTexCache::Get( const String& Path )
{
	if( Path.empty() )return AllZero(Plane<>());
	Plane<> FindPlane = Find( Path );
	if( FindPlane.pTexture != NULL )return FindPlane;

	LoadCache( Path );
	FindPlane = Find( Path );
	return FindPlane;
}

Plane<> PathTexCache::Find( const String& Path )
{
	if( Path.empty() )return AllZero(Plane<>());
	CriticalSection Locker(Lock);
	std::map<String,Plane<> >::const_iterator itr = TextureMap.find( Path );
	if( itr == TextureMap.end() )return AllZero(Plane<>());
	return itr->second;

}