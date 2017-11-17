// SkillCache.cpp: SkillCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "SkillCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
long SkillCache::MaxLevel = 4;

SkillCache::SkillCache()
{

}

SkillCache::~SkillCache()
{

}

SkillEffect SkillCache::SkillData::Level( long Lev ) const
{
	std::map<long,SkillEffect>::const_iterator itr = LevelList.find( Lev );
	if( itr != LevelList.end() )return itr->second;

	return SkillEffect();
}


HRESULT SkillCache::ReadSkillData( DISPDEVICE* pDevice )
{
	return ReadSkillData( pDevice, _T("Data/Skill.xml") );
}

long SkillCache::GetSkillNo( const String& name ) const
{
	return SkillNo.GetID(name);
}

const SkillCache::SkillData* SkillCache::GetSkill( const String& name ) const
{
	return GetSkill( GetSkillNo(name) );
}

const SkillCache::SkillData* SkillCache::GetSkill( long ID ) const
{
	std::map<long,SkillData>::const_iterator itr = SkillList.find( ID );
	if( itr == SkillList.end() )return NULL;
	return &(itr->second);
}

SkillEffect SkillCache::GetEffect( long ID, long Level ) const
{
	const SkillData* pData = GetSkill( ID );
	if( pData == NULL )return SkillEffect();
	return pData->Level( Level );
}

RbRect SkillCache::GetBaseRect() const
{
	if( BasePlaneList.size() <= 0 )return RbRect();
	RbRect Rect = BasePlaneList.begin()->second;
	Rect.Move( -Rect.left, -Rect.top );
	return Rect;
}

Plane<> SkillCache::GetBasePlane( LPCTSTR name ) const
{
	std::map<String,Plane<> >::const_iterator itr = BasePlaneList.find( name );
	if( itr == BasePlaneList.end() )return AllZero(Plane<>());
	return itr->second;
}

Plane<> SkillCache::GetLevelPlane( long Level ) const
{
	std::map<long,Plane<> >::const_iterator itr = LevelPlaneList.find( Level );
	if( itr == LevelPlaneList.end() )return AllZero(Plane<>());
	return itr->second;
}

HRESULT SkillCache::ReadSkillData(  DISPDEVICE* pDevice , LPCTSTR FileName )
{
	XMLPaser SkillDefineXML;
	HRESULT ret = SkillDefineXML.Load( FileName );
	if( FAILED(ret) )return ret;

	XMLPaser TextureXML;
	ret = SkillDefineXML.Find( TAG::FILE, TextureXML );
	if( FAILED(ret) )return ret;

	String FilePath = TextureXML.AttributeString(TAG::PATH);
	ret = SkillTextue.LoadTexture( pDevice, FilePath );
	if( FAILED(ret) )return ret;

	ret = ReadLevelPlane( SkillDefineXML );
	if( FAILED(ret) )return ret;

	ret = ReadBasePlane( SkillDefineXML );
	if( FAILED(ret) )return ret;

	ret = SkillDefineXML.Find( _T("AddTex"), TextureXML );
	if( SUCCEEDED(ret) )NamedPlane.LoadPlane( pDevice, TextureXML );

	ret = ReadSkillList( SkillDefineXML );
	if( FAILED(ret) )return ret;

	NamedPlane.Clear();
	return S_OK;
}

HRESULT SkillCache::ReadLevelPlane( const XMLPaser& AllXML )
{
	LevelPlaneList.clear();

	XMLList LevelPlane;
	HRESULT ret = LevelPlane.Query( AllXML, _T("LevelPlane") );
	if( FAILED(ret) )return ret;

	int Count = LevelPlane.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		XMLPaser LevelXML;
		LevelPlane.GetPaser( i, LevelXML );
		long Level = LevelXML.AttributeLong(TAG::LEV);
		Plane<> ReadData;
		ReadData.ReadXML( LevelXML );
		ReadData.pTexture = SkillTextue;
		ATLASSERT( LevelPlaneList.find(Level) == LevelPlaneList.end() );
		LevelPlaneList[ Level ] = ReadData;
	}
	return S_OK;
}

HRESULT SkillCache::ReadBasePlane( const XMLPaser& AllXML )
{
	BasePlaneList.clear();

	XMLList BaseListXML;
	HRESULT ret = BaseListXML.Query( AllXML, _T("BasePlane") );
	if( FAILED(ret) )return ret;

	int Count = BaseListXML.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		XMLPaser BaseXML;
		BaseListXML.GetPaser( i, BaseXML );
		String name = BaseXML.AttributeString(TAG::NAME);
		Plane<> ReadData;
		ReadData.ReadXML( BaseXML );
		ReadData.pTexture = SkillTextue;
		ATLASSERT( BasePlaneList.find(name) == BasePlaneList.end() );
		BasePlaneList[ name ] = ReadData;
	}
	return S_OK;
}


HRESULT SkillCache::ReadSkillList( const XMLPaser& AllXML )
{
	SkillList.clear();

	XMLList SkillListXML;
	HRESULT ret = SkillListXML.Query( AllXML, _T("Skill") );
	if( FAILED(ret) )return ret;

	int Count = SkillListXML.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadSkill( SkillListXML.GetItem(i) );
	}
	return S_OK;
}


HRESULT SkillCache::ReadSkill( const XMLPaser& SkillXML )
{
	SkillData ReadData;
	ReadData.NameID = SkillXML.AttributeString(TAG::NAME);
	if( ReadData.NameID.empty() )return E_FAIL;
	ReadData.NoID = SkillNo.GetID( ReadData.NameID );
	if( ReadData.NoID <= 0 )return E_FAIL;
	ReadData.Name = SkillXML.AttributeString(TAG::STR);
	ReadData.Type = SkillXML.AttributeString(TAG::TYPE);
	ReadData.Help = SkillXML.AttributeString(TAG::HELP);
	ReadData.Action = SkillXML.AttributeString(_T("act"));
	ReadData.Shake = SkillXML.AttributeFloat(_T("shake"));
	ReadData.EffectName = SkillXML.AttributeString(_T("effect"));

	ReadData.Pos = NamedPlane.GetPlane(ReadData.NameID);
	if( ReadData.Pos.pTexture == NULL )
	{
		ReadData.Pos.ReadXML( SkillXML );
		ReadData.Pos.pTexture = SkillTextue;
	}

	if( ReadData.EffectName.empty() )ReadData.EffectName = ReadData.NameID;

	XMLList LevelListXML;
	HRESULT ret = LevelListXML.Query( SkillXML, _T("Level") );
	if( FAILED(ret) )return ret;

	int Count = LevelListXML.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		XMLPaser LevelXML;
		LevelListXML.GetPaser( i, LevelXML );
		long Level = LevelXML.AttributeLong(TAG::LEV);
		SkillEffect Effect;
		Effect.Pow = LevelXML.AttributeFloat(TAG::POW);
	//	if( Effect.Pow <= 0 )Effect.Pow = 1;
		Effect.Range = LevelXML.AttributeFloat(TAG::RANGE);
		Effect.Count = LevelXML.AttributeLong(TAG::COUNT);

		ATLASSERT( ReadData.LevelList.find(Level) == ReadData.LevelList.end() );
		ReadData.LevelList[ Level ] = Effect;
	}
	ATLASSERT( SkillList.find(ReadData.NoID) == SkillList.end() );
	SkillList[ ReadData.NoID ] = ReadData;

	return S_OK;
}
