// KeyEvents.cpp: KeyEvents クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "KeyEvents.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

KeyEvents::KeyEvents()
{
}

KeyEvents::~KeyEvents()
{
}

bool KeyEvents::LoadXML()
{
	return LoadXML( _T("System/KeyEvents.xml") );
}

bool KeyEvents::LoadXML( LPCTSTR FileName )
{
	XMLPaser LoadXML;
	if( FAILED(LoadXML.Load(FileName)) )return false;

	XMLList DataList;
	DataList.Query(LoadXML,_T("Event"));

	long Count = DataList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		ReadXML( DataList.GetItem(i));
	}
	return true;
}

bool KeyEvents::ReadXML( const XMLPaser& ReadXML )
{
	long KeyCode = ReadXML.AttributeLong(_T("key"));
	if( KeyCode <= 0 )return false;
	String XMLPath = ReadXML.AttributeString(_T("path"));
	if( XMLPath.empty() )return false;

	ATLASSERT( EventMap.find(KeyCode) == EventMap.end() );
	EventMap[KeyCode] = XMLPath;
	return true;
}

String KeyEvents::GetPath( long KeyCode ) const
{
	if( EventMap.size() <= 0 )return String();
	std::map<long,String>::const_iterator itr = EventMap.find( KeyCode );
	if( itr == EventMap.end() )return String();

	return itr->second;
}

