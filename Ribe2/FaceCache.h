// FaceCache.h: FaceCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACECACHE_H__BE000D53_BF1E_40BB_822D_1E24453DF105__INCLUDED_)
#define AFX_FACECACHE_H__BE000D53_BF1E_40BB_822D_1E24453DF105__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DefaultPlanes.h"

class FaceCache  
{
public:
	FaceCache();
	virtual ~FaceCache();

	Plane<> Get( const String& DataStr );
	void Clear();

protected:
	class FacePlane : public DefaultPlanes
	{
	public:
		Plane<> Get( const String& Type ) const;
	};
	const FacePlane* Find( const String& FaceName );
	virtual bool LoadCache( const String& ReadData );
	String GetFilePath( const String& Name ) const;
	std::map<String,FacePlane> PlaneList;
};

#endif // !defined(AFX_FACECACHE_H__BE000D53_BF1E_40BB_822D_1E24453DF105__INCLUDED_)
