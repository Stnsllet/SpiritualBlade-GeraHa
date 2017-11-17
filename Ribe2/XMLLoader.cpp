// XMLLoader.cpp: XMLLoader クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileLoader.h"
#include "XMLEditer.h"
#include "XMLLoader.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XMLLoader::XMLLoader()
{

}

XMLLoader::~XMLLoader()
{

}

HRESULT XMLLoader::Search( LPCTSTR pPath )
{
	FilePathList.clear();
	Root = pPath;

	FileLoader Directorys;
	HRESULT ret = Directorys.SearchDirectory( pPath );
	if( FAILED(ret) )return E_FAIL;

	int Count = Directorys.GetCount();
	for( int i = 0; i< Count; i++ )
	{
		Path Search = Root;
		Search += Directorys[i];
		SearchXML( Search );
	}
	return S_OK;
}

HRESULT XMLLoader::SearchXML( LPCTSTR pPath )
{
	FileLoader Files;
	HRESULT ret = Files.SearchFile( pPath, _T("*.xml") );
	if( FAILED(ret) )return E_FAIL;

	int Count = Files.GetCount();
	for( int i = 0; i< Count; i++ )
	{
		XMLFile ReadData;
		ReadData.FilePath = pPath;
		ReadData.FilePath += Files[i];

		XMLPaser XMLText;
		ret = XMLText.Load( ReadData.FilePath );
		if( FAILED(ret) )continue;
		CComBSTR Tag;
		ret = XMLText.TagName( &Tag );
		ReadData.Tag = Tag;
		if( FAILED(ret) )continue;
		ReadData.name = XMLText.AttributeString( _T("name") );
		if( ReadData.name.length() <= 0 )continue;

		FilePathList.push_back( ReadData );
	}
	return S_OK;
}

HRESULT XMLLoader::Save() const
{
	Path SavePath = Root;
	SavePath += _T("FileList.xml");

	return Save( SavePath );
}

HRESULT XMLLoader::Save( LPCTSTR FileName ) const
{
	XMLEditer SaveXML;
	HRESULT ret = SaveXML.Create( _T("FileList") );
	if( FAILED(ret) )return ret;

	int FileCount = FilePathList.size();
	for( int i = 0; i < FileCount; i++ )
	{
		XMLEditer File;
		ret = SaveXML.CreateChild( _T("File"), File );
		if( FAILED(ret) )return ret;

		ret = File.SetAttribute( _T("tag"), FilePathList[i].Tag );
		if( FAILED(ret) )return ret;
		ret = File.SetAttribute( _T("name"), FilePathList[i].name );
		if( FAILED(ret) )return ret;
		ret = File.SetAttribute( _T("path"), FilePathList[i].FilePath );
		if( FAILED(ret) )return ret;
	}

	return SaveXML.Save( FileName );
}
