// LandType.cpp: LandType クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "LandType.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LandType::LandType()
{

}

LandType::~LandType()
{

}

const LandMoveType& LandType::MoveDat( const String& Name ) const
{
	if( Name == NULL )return DefaultMoveType;
	std::map<String,LandMoveType>::const_iterator itr = MoveTypeList.find( Name );
	if( itr == MoveTypeList.end() )return DefaultMoveType;
	return itr->second;
}

const LandType::LandTypeData& LandType::GetTypeData( long ID ) const
{
	std::map<long,LandTypeData>::const_iterator itr = LandTypeList.find( ID );
	if( itr == LandTypeList.end() )return LandTypeList.begin()->second;
	return itr->second;
}

const LandType::LandTex& LandType::GetTexData( const String& Name ) const
{
	std::map<String,LandTex>::const_iterator itr = LandTexList.find( Name );
	if( itr == LandTexList.end() )return LandTexList.begin()->second;
	return itr->second;
}

long LandType::SetMoveValue( LandMoveType& MoveData, const String& Type, const LandMoveType::MoveData& Value ) const
{
	long ExecCount = 0;
	std::map<long,LandTypeData>::const_iterator itr = LandTypeList.begin();
	while( itr != LandTypeList.end() )
	{
		const LandTypeData& Ref = itr->second;
		itr++;
		if( Ref.Type != Type )continue;
		MoveData.SetValue( Ref.ID, Value );
		ExecCount++;
	}
	return ExecCount;
}

HRESULT LandType::LoadLandType()
{
	HRESULT hr =  LoadLandType( _T("Data/LandType.xml") );
	if( FAILED(hr) )return hr;
	hr = LoadMoveType( _T("Data/MovType.xml") );
	if( FAILED(hr) )return hr;
	return S_OK;
}

HRESULT LandType::LoadLandType( LPCTSTR FilePath )
{
	LandTypeList.clear();

	XMLPaser AllXML;
	HRESULT ret = AllXML.Load( FilePath );
	if( FAILED(ret) )return ret;

	XMLList TypeXMLList;
	ret = TypeXMLList.Query( AllXML, _T("Type") );
	if( FAILED(ret) )return ret;

	int Count = TypeXMLList.GetCount();
	for( int i = 0; i < Count; i ++ )
	{
		ReadMapData( TypeXMLList.GetItem(i) );
	}

	XMLList TexXMLList;
	ret = TexXMLList.Query( AllXML, _T("Tex") );
	if( FAILED(ret) )return ret;

	Count = TexXMLList.GetCount();
	for( i = 0; i < Count; i ++ )
	{
		ReadTexData( TexXMLList.GetItem(i) );
	}
	return S_OK;
}

HRESULT LandType::ReadMapData( const XMLPaser& TypeXML )
{
	LandTypeData ReadData;
	ReadData.Type = TypeXML.AttributeString(_T("type"));
	ReadData.ID = TypeXML.AttributeLong(_T("num"));
	if( ReadData.ID <= 0 || ReadData.ID >= 256 )return E_FAIL;

	ATLASSERT( LandTypeList.find(ReadData.ID) == LandTypeList.end() );
	LandTypeList[ReadData.ID] = ReadData;

	return S_OK;
}

HRESULT LandType::ReadTexData( const XMLPaser& TypeXML )
{
	String Name = TypeXML.AttributeString(TAG::NAME);
	if( Name.empty() )return E_FAIL;

	LandTex ReadData;
	ReadData.Path = TypeXML.AttributeString(TAG::PATH);

	ATLASSERT( LandTexList.find(Name) == LandTexList.end() );
	LandTexList[Name] = ReadData;
	return S_OK;
}

HRESULT LandType::LoadMoveType( LPCTSTR FilePath )
{
	MoveTypeList.clear();

	XMLPaser AllXML;
	HRESULT ret = AllXML.Load( FilePath );
	if( FAILED(ret) )return ret;

	XMLList TypeXMLList;
	ret = TypeXMLList.Query( AllXML, _T("MoveType") );
	if( FAILED(ret) )return ret;

	int Count = TypeXMLList.GetCount();
	for( int i = 0; i < Count; i ++ )
	{
		LandMoveType NewData;
		if( !NewData.SetData( *this, TypeXMLList.GetItem(i) ) )continue;
		if( NewData.Name() == _T("default") )DefaultMoveType = NewData;
		ATLASSERT( MoveTypeList.find(NewData.Name())==MoveTypeList.end() );
		MoveTypeList[NewData.Name()] = NewData;
	}
	return S_OK;
}
