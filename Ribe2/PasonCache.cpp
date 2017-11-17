// PasonCache.cpp: PasonCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "Plane.h"
#include "StringSplitter.h"
#include "PasonCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

PasonCache::PasonCache()
{
	AllZero( (PasonStatus&)DefaultData );
}

PasonCache::~PasonCache()
{

}

const PasonCache::PasonST& PasonCache::RefDefault() const
{
	return DefaultData;
}

bool PasonCache::IsMain( long PasonID ) const
{
	const PasonST& Ref = Get( PasonID );
	return Ref.bMain;
}

long PasonCache::GetPasonID( const String& PasonName ) const
{
	return PasonIDList.GetID( PasonName );
}

const PasonCache::PasonST& PasonCache::Get( const String& PasonName ) const
{
	return Get( GetPasonID( PasonName ) );
}

const PasonCache::PasonST& PasonCache::Get( long PasonID ) const
{
	PasonMap::const_iterator itr = PasonList.find( PasonID );
	ATLASSERT( itr != PasonList.end() );
	if( itr == PasonList.end() )return DefaultData;
	return itr->second;
}

HRESULT PasonCache::ReadPasonData()
{
	XMLPaser XMLFile;
	HRESULT ret = XMLFile.Load( _T("Data/PasonFiles.xml") );
	if( FAILED(ret) )return ret;

	XMLList PasonFileList;
	ret = PasonFileList.Query( XMLFile, TAG::FILE );
	if( FAILED(ret) )return ret;

	int Count = PasonFileList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		String Path = PasonFileList.GetItem(i).AttributeString(TAG::PATH);
		ReadPasonData( Path );
	}
	return S_OK;
}

HRESULT PasonCache::ReadPasonData( LPCTSTR FileName )
{
	XMLPaser XMLFile;
	HRESULT ret = XMLFile.Load( FileName );
	if( FAILED(ret) )return ret;

	bool bMain = XMLFile.AttributeBool(_T("main"));
	bool bEmploy = XMLFile.AttributeBool(_T("employ"));

	XMLList PasonList;
	ret = PasonList.Query( XMLFile, _T("Pason") );
	if( FAILED(ret) )return ret;

	int Count = PasonList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadPasonData( PasonList.GetItem(i), bMain, bEmploy );
	}
	return S_OK;
}

HRESULT PasonCache::ReadPasonData( const XMLPaser& PasonData, bool bMain, bool bEmploy )
{
	PasonST ReadData;
	AllZero( (PasonStatus&)ReadData );
	ReadData.NameID = PasonData.AttributeString( TAG::NAME );
	ReadData.Name = PasonData.AttributeString( TAG::STR );
	ReadData.Help = PasonData.AttributeString( TAG::HELP );
	ReadData.bMain = bMain;

	if( ReadData.NameID.empty() )return E_FAIL;

	XMLPaser GrowthXML;
	HRESULT ret = PasonData.Find( _T("Grow"), GrowthXML );

	ReadData.Grow.Status = DefaultData.Grow.Status;
	if( !FAILED(ret) )
	{
		StatusEdit<> Read(ReadData.Grow.Status);
		ReadData.Grow.Status *= 100.0f;
		Read.SkipReadXML( GrowthXML );
		ReadData.Grow.Status *= .01f;
	}

	XMLPaser ParamXML;
	ret = PasonData.Find( _T("Param"), ParamXML );
	ReadData.FirstStatus = DefaultData.FirstStatus;
	if( !FAILED(ret) )
	{
		StatusEdit<> Read(ReadData.FirstStatus);
		ReadData.FirstStatus *= 100.0f;
		Read.SkipReadXML( ParamXML );
		ReadData.FirstStatus *= .01f;
	}

	XMLPaser ExXML;
	ret = PasonData.Find( _T("Ex"), ExXML );
	if( !FAILED(ret) )
	{
		ReadData.BaseJob = ExXML.AttributeString( _T("job") );
		ReadData.Texture = ExXML.AttributeString( _T("tex") );
		ReadData.News = ExXML.AttributeString( _T("news") );
		ReadData.EquipType = ExXML.AttributeString( _T("equip") );
		StringSplitter<> EmpEqs = String(ExXML.AttributeString( _T("empeq") ));
		ReadData.EmployEquip.reserve(EmpEqs.Count());
		for( int i = 0; i < EmpEqs.Count(); i++ )
		{
			long Item = EmpEqs[i].ToLong();
			if( Item != 0 )ReadData.EmployEquip.push_back(Item);
		}
	}

	ReadData.SkillList.SetXML( PasonData );
//	ReadSkillData( ReadData, PasonData );

	if( ReadData.NameID == _T("default") )
	{
		DefaultData = ReadData;
	}
	else
	{
		if( ReadData.Help.empty() )ReadData.Help = DefaultData.Help;
	}

	long PasonID = PasonIDList.GetID( ReadData.NameID );
	ATLASSERT( PasonList.find( PasonID ) == PasonList.end() );
	PasonList[ PasonID ] = ReadData;
	if( bEmploy )MinorList.insert( PasonID );
	return S_OK;
}
