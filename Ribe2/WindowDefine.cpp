// WindowDefine.cpp: WindowDefine クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "WindowDefine.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

WindowDefine::WindowDefine()
{

}

WindowDefine::~WindowDefine()
{

}

// プレーン定義取得
const WindowDefine::WindowPlane* WindowDefine::WindowPlanes::Find( LPCTSTR Name ) const
{
	WindowPlaneMap::const_iterator itr = PlaneList.find(Name);
	if( itr != PlaneList.end() )return &(itr->second);

	if( pBasePlanes == NULL )return NULL;
	return pBasePlanes->Find( Name );
}

// ウインドウ定義取得
const WindowDefine::WindowPlanes* WindowDefine::GetWindowPlanes( LPCTSTR WindowName ) const
{
	WindowDefineMap::const_iterator itr = WindowDefineList.find( WindowName );
	if( itr == WindowDefineList.end() )return NULL;
	return &(itr->second);
}

// プレーン定義取得
const WindowDefine::WindowPlane* WindowDefine::GetWindowPlane( LPCTSTR WindowName, LPCTSTR PlaneName ) const
{
	const WindowPlanes* pWindow = GetWindowPlanes( WindowName );
	if( pWindow == NULL )return NULL;

	return pWindow->Find( PlaneName );
}


// 定義ロード
HRESULT WindowDefine::LoadWindowDefine()
{
	return LoadWindowDefine( _T("System/WindowDefine.xml") );
}

// 定義ロード
HRESULT WindowDefine::LoadWindowDefine( LPCTSTR FileName )
{
	XMLPaser WindowDefXML;
	HRESULT ret = WindowDefXML.Load( FileName );
	if( FAILED(ret) )return ret;

	XMLList WindowList;
	ret = WindowList.Query( WindowDefXML, _T("Window") );
	if( FAILED(ret) )return ret;

	int Count = WindowList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadWindowDefine( WindowList.GetItem(i) );
	}
	return S_OK;
}

// 定義読み込み
HRESULT WindowDefine::ReadWindowDefine( const XMLPaser& WindowXML )
{
	WindowPlanes ReadData;
	ReadData.Base.name = WindowXML.AttributeString(TAG::NAME);
	if( ReadData.Base.name.empty() )return E_FAIL;

	ReadData.pBasePlanes = NULL;
	String BaseName = WindowXML.AttributeString(_T("base"));
	if( !BaseName.empty() )
	{
		ReadData.pBasePlanes = GetWindowPlanes( BaseName );
	}

	ReadData.XML.XMLRead( WindowXML );

	ReadData.Base.Plane = Plane<>().ReadXML( WindowXML );

	XMLList PlanesXML;
	HRESULT ret = PlanesXML.Query( WindowXML, TAG::PLANE );
	if( FAILED(ret) )return ret;

	int Count = PlanesXML.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadPlane( PlanesXML.GetItem(i), ReadData );
	}

	ATLASSERT( WindowDefineList.find(ReadData.Base.name) == WindowDefineList.end() );
	WindowDefineList[ReadData.Base.name] = ReadData;
	return S_OK;
}

// 定義読み込み
HRESULT WindowDefine::ReadPlane( const XMLPaser& PlaneXML, WindowPlanes& Dest )
{
	WindowPlane ReadData;
	ReadData.name = PlaneXML.AttributeString(TAG::NAME);
	if( ReadData.name.empty() )return E_FAIL;

	ReadData.Plane = Plane<>().ReadXML( PlaneXML );

	ATLASSERT( Dest.PlaneList.find(ReadData.name) == Dest.PlaneList.end() );
	Dest.PlaneList[ReadData.name] = ReadData;
	return S_OK;
}
