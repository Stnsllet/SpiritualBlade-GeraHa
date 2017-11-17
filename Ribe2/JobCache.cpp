// JobCache.cpp: JobCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "Plane.h"
#include "JobCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

JobCache::JobCache()
{
	AllZero(DefaultData);
}

JobCache::~JobCache()
{

}

const JobCache::JobST& JobCache::Get( const String& JobName ) const
{
	JobMap::const_iterator itr = JobList.find( JobName );
	ATLASSERT( itr != JobList.end() );
	if( itr == JobList.end() )return DefaultData;
	return itr->second;
}

HRESULT JobCache::ReadJobData()
{
	return ReadJobData( _T("Data/Job.xml") );
}

HRESULT JobCache::ReadJobData( LPCTSTR FileName )
{
	XMLPaser XMLFile;
	HRESULT ret = XMLFile.Load( FileName );
	if( FAILED(ret) )return ret;

	XMLList JobList;
	ret = JobList.Query( XMLFile, _T("Job") );
	if( FAILED(ret) )return ret;

	int Count = JobList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadJobData( JobList.GetItem(i) );
	}
	return S_OK;
}

HRESULT JobCache::ReadJobData( const XMLPaser& JobData )
{
	JobST ReadData;
	AllZero( (JobStatus&)ReadData );
	ReadData.NameID = JobData.AttributeString( TAG::NAME );
	ReadData.Name = JobData.AttributeString( TAG::STR );
	ReadData.Help = JobData.AttributeString( TAG::HELP );

	if( ReadData.NameID.empty() )return E_FAIL;

	XMLPaser GrowthXML;
	HRESULT ret = JobData.Find( _T("Grow"), GrowthXML );
	ReadData.Grow.Status = DefaultData.Grow.Status;
	if( !FAILED(ret) )
	{
		StatusEdit<> Read(ReadData.Grow.Status);
		ReadData.Grow.Status *= 100.0f;
		Read.SkipReadXML( GrowthXML );
		ReadData.Grow.Status *= .01f;
	//	ReadData.Grow.GrowType = GrowthXML.AttributeString( _T("type") );
	}

	XMLPaser BattleXML;
	ret = JobData.Find( _T("Battle"), BattleXML );
	if( !FAILED(ret) )
	{
		ReadData.BattleType = GetBatType(String(BattleXML.AttributeString( TAG::TYPE )));
		ReadData.Texture = BattleXML.AttributeString( _T("tex") );
		ReadData.MoveType = BattleXML.AttributeString( TAG::MOV );
		ReadData.WeponRandom = BattleXML.AttributeLong( _T("weprnd") );
	}

	XMLPaser EquipXML;
	ret = JobData.Find( _T("Ex"), EquipXML );
	if( !FAILED(ret) )
	{
		ReadData.EquipType = EquipXML.AttributeString( TAG::TYPE );
		ReadData.NextJob = EquipXML.AttributeString( _T("next") );
	}
	ReadData.SkillList.SetXML( JobData );

	if( ReadData.NameID == _T("default") )DefaultData = ReadData;
	else
	{
		if( ReadData.EquipType.empty() )ReadData.EquipType = DefaultData.EquipType;
	}

	ATLASSERT( JobList.find( ReadData.NameID ) == JobList.end() );
	JobList[ ReadData.NameID ] = ReadData;
	return S_OK;
}


BattleAlgoType JobCache::GetBatType( const String& Name ) const
{
	if( Name.empty() )return eBatDefault;
	if( Name==_T("nobat"))return eBatNotBat;
	if( Name==_T("nomove"))return eBatDontMove;
	if( Name==_T("raging"))return eBatRaging;	//バーサク
	if( Name==_T("escape"))return eBatEscape;

	return eBatDefault;
}
