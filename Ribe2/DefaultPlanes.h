// DefaultPlanes.h: DefaultPlanes クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFAULTPLANES_H__D0865851_0485_4B7C_946D_314222907C73__INCLUDED_)
#define AFX_DEFAULTPLANES_H__D0865851_0485_4B7C_946D_314222907C73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

#include "String.h"
#include "Plane.h"
class Texture;

class XMLList;

template<class T=TexRect> class PlaneReader
{
public:
	typedef std::map<String,Plane<T> > PlaneMap;
	PlaneReader();
	virtual ~PlaneReader();

	virtual HRESULT LoadPlane( DISPDEVICE* pDevice, const String& FileName, bool bAdd=false );
	virtual HRESULT LoadPlane( DISPDEVICE* pDevice, const XMLPaser& DefineFile, bool bAdd=false );
	Plane<T> GetPlane( const String& name ) const;

	bool AddPlane( const Plane<T>& Data, const String& name );
	bool AddPlane( const PlaneReader<HandTexRect>& Data );
	const PlaneMap& RefAllData() const{return PlaneList;}
	void Clear(){PlaneList.clear();}

protected:
	HRESULT ReadFile( DISPDEVICE* pDevice, const XMLPaser& File );
	HRESULT ReadPlane( const Texture& PlaneTexture, const XMLPaser& PlaneXML );

protected:
	PlaneMap PlaneList;
};

typedef PlaneReader<> DefaultPlanes;
typedef PlaneReader<HandTexRect> HandPlanes;

#endif // !defined(AFX_DEFAULTPLANES_H__D0865851_0485_4B7C_946D_314222907C73__INCLUDED_)
