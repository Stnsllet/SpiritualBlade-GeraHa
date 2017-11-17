// DefaultPlanes.cpp: DefaultPlanes クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include "XMLList.h"
#include "RbCache.h"
#include "DefaultPlanes.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

template<class T> PlaneReader<T>::PlaneReader()
{

}

template<class T> PlaneReader<T>::~PlaneReader()
{

}

template<class T> Plane<T> PlaneReader<T>::GetPlane( const String& name ) const
{
	PlaneMap::const_iterator itr = PlaneList.find(name);
	if( itr == PlaneList.end() )return AllZero(Plane<T>());

	return itr->second;
}

template<class T> bool PlaneReader<T>::AddPlane( const Plane<T>& Data, const String& name )
{
	if( Data.pTexture == NULL )return false;
	PlaneMap::const_iterator itr = PlaneList.find(name);
	if( itr != PlaneList.end() )return false;

	PlaneList[name] = Data;
	return true;
}

template<class T> bool PlaneReader<T>::AddPlane( const PlaneReader<HandTexRect>& Data )
{
	typedef PlaneReader<HandTexRect>::PlaneMap::const_iterator srcitr;
	srcitr st = Data.RefAllData().begin();
	const srcitr ed = Data.RefAllData().end();
	while( st != ed )
	{
		PlaneList[st->first] = st->second;
		++st;
	}

	return true;
}

template<class T> HRESULT PlaneReader<T>::LoadPlane( DISPDEVICE* pDevice, const String& FileName, bool bAdd )
{
	XMLPaser DefineFile;
	HRESULT ret = DefineFile.Load( FileName );
	ATLASSERT( !FAILED(ret) );
	if( FAILED(ret) )return ret;

	return LoadPlane( pDevice, DefineFile, bAdd );
}

template<class T> HRESULT PlaneReader<T>::LoadPlane( DISPDEVICE* pDevice, const XMLPaser& DefineFile, bool bAdd )
{
	if( !bAdd )PlaneList.clear();
	long OldCount = PlaneList.size();

	XMLList FileList;
	HRESULT ret = FileList.Query( DefineFile, TAG::FILE );
	if( FAILED(ret) )return ret;

	int Count = FileList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ret = ReadFile( pDevice, FileList.GetItem(i) );
		if( FAILED(ret) )return ret;
	}

	return (PlaneList.size()>OldCount)?(S_OK):(E_FAIL);
}

template<class T> HRESULT PlaneReader<T>::ReadFile( DISPDEVICE* pDevice, const XMLPaser& File )
{
	String Source = File.AttributeString(TAG::SRC);
	if( !Source.empty() )
	{
		RbPlaneCache::PlanePtr pGet = GetCache().Plane.Get( Source );
		if( pGet == NULL )return E_FAIL;
		AddPlane( *pGet );
		return S_OK;
	//	return LoadPlane( pDevice, Source, true );
	}
	String Path = File.AttributeString(TAG::PATH);
	Texture PlaneTexture;
	HRESULT ret = PlaneTexture.SetTexture( GetCache().PathTex.Get(Path) );
	ATLASSERT( !FAILED(ret) );
//	HRESULT ret = PlaneTexture.LoadTexture( pDevice, Path );
	if( FAILED(ret) )return ret;

	XMLList Planes;
	ret = Planes.Query( File, TAG::PLANE );
	if( FAILED(ret) )return ret;

	int Count = Planes.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadPlane( PlaneTexture, Planes.GetItem(i) );
	}

	return S_OK;
}

template<class T> HRESULT PlaneReader<T>::ReadPlane( const Texture& PlaneTexture, const XMLPaser& PlaneXML )
{
	Plane<T> ReadData;

	String name = PlaneXML.AttributeString( TAG::NAME );
	if( name.length() <= 0 )return E_FAIL;

	ReadData.ReadXML( PlaneXML );
	ReadData.pTexture = PlaneTexture;

	ATLASSERT( PlaneList.find(name) == PlaneList.end() );
	PlaneList[name] = ReadData;

	return S_OK;
}

template class PlaneReader<>;
template class PlaneReader<HandTexRect>;
