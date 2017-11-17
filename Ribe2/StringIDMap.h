// StringIDMap.h: StringIDMap クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGIDMAP_H__01E39358_3757_463F_A8F9_51EF46E71DC7__INCLUDED_)
#define AFX_STRINGIDMAP_H__01E39358_3757_463F_A8F9_51EF46E71DC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "String.h"
#include "CriticalSection.h"

class StringIDMap  : protected std::map<String,long>
{
public:
	StringIDMap(){}
	~StringIDMap(){}

	LPCTSTR GetString( long ID ) const
	{
		const_iterator itr = begin();
		while( itr != end() )
		{
			if( ID == itr->second )return itr->first;
			itr++;
		}
		return NULL;
	}

	long GetID( const String& Key ) const
	{
		const_iterator itr = find( Key );
		if( itr == end() )return -1;
		return itr->second;
	}

	long GetID( const BSTR Key ) const
	{
		return GetID(String(Key));
	}

	long AddID( const String& Key, long NewID )
	{
		const_iterator itr = find( Key );
		if( itr != end() )return itr->second;

		(*this)[Key] = NewID;
		return NewID;
	}

	long AddID( BSTR Key, long NewID )
	{
		return AddID( String(Key), NewID );
	}

};

class IDManager
{
public:
	IDManager(){NextID = 1;}
	LPCTSTR GetString( long ID ) const{return IDMap.GetString(ID);}
	long GetID( BSTR name ){return GetID(String(name));}
	long GetID( const String& name )
	{
		CriticalSection Locker( Lock );
		long RetID = IDMap.GetID( name );
		if( RetID > 0 )return RetID;
		return IDMap.AddID( name, NextID++ );
	}
	long GetID( BSTR name ) const{return GetID(String(name));}
	long GetID( const String& name ) const
	{
		CriticalSection Locker( Lock );
		return IDMap.GetID( name );
	}
private:
	CriticalSection::CriticalSectionObject Lock;
	StringIDMap IDMap;
	long NextID;
};

class StockID
{
public:
	StockID( LPCTSTR Name ){StaticID=0;IDName=Name;}
	long ID( const IDManager& Manager )
	{
		if( StaticID != 0 )return StaticID;
		StaticID = Manager.GetID( IDName );
		return StaticID;
	}
	template<class T> long IDT( const T& Manager )
	{
		if( StaticID != 0 )return StaticID;
		StaticID = Manager.GetID( IDName );
		return StaticID;
	}
	operator long() const{return StaticID;}
private:
	LPCTSTR IDName;
	long StaticID;
};

#endif // !defined(AFX_STRINGIDMAP_H__01E39358_3757_463F_A8F9_51EF46E71DC7__INCLUDED_)
