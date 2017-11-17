// RbCache.cpp: RbCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "timer.h"
#include "XMLList.h"
#include "RbDisplay.h"
#include "RbCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbCache::RbCache()
{

}

RbCache::~RbCache()
{

}

HRESULT RbCache::InitCache( DISPDEVICE* pDevice )
{
	TIMECHK( "RbCache::InitCache" );

	HRESULT ret = S_OK;
	{
		TIMECHK("SystemMsg Cache");
		ret = LoadSystemMsg( _T("system/SystemMsg.xml") );
		if( FAILED(ret) )return ret;
	}

	{
		TIMECHK("Skill Cache");
		ret = Skill.ReadSkillData( pDevice );
		if( FAILED(ret) )return ret;
	}

	{
		TIMECHK("Item Cache");
		ret = Item.ReadItemData( pDevice );
		if( FAILED(ret) )return ret;
	}

	{
		TIMECHK("Job Cache");
		ret = Job.ReadJobData();
		if( FAILED(ret) )return ret;
	}

	{
		TIMECHK("Pason Cache");
		ret = Pason.ReadPasonData();
		if( FAILED(ret) )return ret;
	}

	{
		TIMECHK("SystemPlane Cache");
		ret = SystemPlane.LoadPlane( pDevice, _T("System/SystemPlane.xml") );
		if( FAILED(ret) )return ret;
	}

	{
		TIMECHK("ReTexture Cache");
		ret = RbTex.Init();
		if( FAILED(ret) )return ret;
	}

	{
		TIMECHK("LandType Cache");
		ret = Land.LoadLandType();
		if( FAILED(ret) )return ret;
	}

	return S_OK;
}

bool RbCache::Clear()
{
	LightCache();
	RbTex.Clear();
	Plane.Clear();
	PathTex.Clear();
	return true;
}

bool RbCache::LightCache()
{
	Face.Clear();
	Poly.Clear();
	XML.Clear();
	return true;
}

bool RbCache::GetTexture( const String& NameID, TexPtr& pTexture )
{
	pTexture = RbTex.Get( NameID );
	return (pTexture!=NULL)?(true):(false);
}

const String& RbCache::GetMessage( const String& name ) const
{
	static const String DumMsg;
	std::map<String,String>::const_iterator itr = SystemMsg.find( name );
	if( itr == SystemMsg.end() )return DumMsg;
	return itr->second;
}

HRESULT RbCache::LoadSystemMsg( LPCTSTR FileName )
{
	XMLPaser FileXML;
	HRESULT hr = FileXML.Load( FileName );
	if( FAILED(hr) )return hr;
	XMLList MsgList;
	
	MsgList.Query(FileXML,_T("Msg"));
	long Count = MsgList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		const XMLPaser& Ref = MsgList.GetItem(i);
		String Name = Ref.AttributeString(TAG::NAME);
		if( Name.empty() )continue;
		String Str = Ref.AttributeString(TAG::STR);
		if( Str.empty() )continue;
		ATLASSERT( SystemMsg.find(Name)==SystemMsg.end() );
		SystemMsg[Name] = Str;
	}
	return S_OK;
}
