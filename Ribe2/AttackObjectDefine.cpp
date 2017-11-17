// AttackObjectDefine.cpp: AttackObjectDefine クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "AttackObjectDefine.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

AttackObjectDefine::AttackObjectDefine()
{
}

AttackObjectDefine::~AttackObjectDefine()
{

}

const AttackObjectDefine::AtcDef& AttackObjectDefine::RefData( const String& Name ) const
{
	std::map<String,AtcDef>::const_iterator itr = DefList.find(Name);
	if( itr == DefList.end() )return DefaultData;
	return itr->second;
}

bool AttackObjectDefine::ReadDef()
{
	return ReadDef( _T("System/AttackObj.xml") );
}

bool AttackObjectDefine::ReadDef( LPCTSTR FileName )
{
	XMLPaser ReadXML;
	if( FAILED(ReadXML.Load(FileName)) )return false;

	XMLList DefXMLList;
	if( FAILED(DefXMLList.Query(ReadXML,_T("Obj"))) )return false;

	const long Count = DefXMLList.GetCount();
	if( Count <= 0 )return false;

	for( int i = 0; i < Count; i++ )
	{
		ReadData( DefXMLList.GetItem(i) );
	}
	ReadAllExData( ReadXML );
	return true;
}

bool AttackObjectDefine::ReadAllExData( XMLPaser& ReadXML )
{
	XMLList DefXMLList;
	if( FAILED(DefXMLList.Query(ReadXML,_T("Ex"))) )return false;

	const long Count = DefXMLList.GetCount();
	if( Count <= 0 )return false;
	ExList.reserve( Count );

	for( int i = 0; i < Count; i++ )
	{
		ReadExData( DefXMLList.GetItem(i) );
	}
	return true;
}

bool AttackObjectDefine::ReadData( XMLPaser& ReadXML )
{
	String Name = ReadXML.AttributeString(TAG::NAME);
	if( Name.empty() )return false;

	AtcDef ReadData;
	ReadData.UseMp = ReadXML.AttributeFloat(TAG::MP);

	ATLASSERT( DefList.find(Name) == DefList.end() );
	DefList[Name] = ReadData;
	return true;
}

bool AttackObjectDefine::ReadExData( XMLPaser& ReadXML )
{
	RefPtr<BaseDef> pNewData( new BaseDef );
	if( !pNewData->ReadXML( ReadXML ) )return false;
	ExList.push_back( pNewData );
	return true;
}
