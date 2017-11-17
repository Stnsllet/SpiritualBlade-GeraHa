// FaceCache.cpp: FaceCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringSplitter.h"
#include "RbDisplay.h"
#include "FaceCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

FaceCache::FaceCache()
{
}

FaceCache::~FaceCache()
{
}

void FaceCache::Clear()
{
	PlaneList.clear();
}

Plane<> FaceCache::FacePlane::Get( const String& Type ) const
{
	static LPCTSTR DEFAULT =  _T("default");
	if( Type.empty() )return GetPlane( DEFAULT );
	Plane<>& Ret = GetPlane( Type );
	if( Ret.pTexture != NULL )return Ret;

	return GetPlane( DEFAULT );
}

Plane<> FaceCache::Get( const String& DataStr )
{
	if( DataStr.empty() )return AllZero(Plane<>());
	StringSplitter<> Split;
	Split.SetString( DataStr );
	const String& FaceName = Split[0];

	const FacePlane* pPlanes = Find( FaceName );
	if( pPlanes == NULL )
	{
		LoadCache( FaceName );
		pPlanes = Find( FaceName );
		if( pPlanes == NULL )return AllZero(Plane<>());
	}
	return pPlanes->Get( Split[1] );
}

String FaceCache::GetFilePath( const String& Name ) const
{
	String Ret = _T("Tex/Face/");
	Ret += Name;
	Ret += _T(".xml");
	return Ret;
}

const FaceCache::FacePlane* FaceCache::Find( const String& FaceName )
{
	std::map<String,FacePlane>::const_iterator itr = PlaneList.find( FaceName );
	if( itr == PlaneList.end() )return NULL;
	return &(itr->second);
}

bool FaceCache::LoadCache( const String& ReadData )
{
	const String& FilePath = GetFilePath( ReadData );
	return !FAILED(PlaneList[ReadData].LoadPlane( GetRbDisplay(), FilePath, false ));
}
