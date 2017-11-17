// StringSplitter.h: StringSplitter クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGSPLITTER_H__5268F2D8_F637_4174_84AF_D449B3E0B1BC__INCLUDED_)
#define AFX_STRINGSPLITTER_H__5268F2D8_F637_4174_84AF_D449B3E0B1BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "String.h"

template<TCHAR S='/'> class StringSplitter  
{
public:
	StringSplitter(LPCTSTR SetData){SetString(SetData);}
	StringSplitter(){}
	~StringSplitter(){}
	long SetString( LPCTSTR SetData )
	{
		StringList.clear();
		if( SetData == NULL )return 0;
		long Count = 0;
		LPCTSTR pFirst = SetData;
		do
		{
			LPCTSTR pRead = pFirst;
			while( *pRead != 0 && *pRead != S )pRead++;
			String Append;
			long Length = pRead-pFirst;
			Append.append( pFirst, Length );
			StringList.push_back( Append );
			Count++;
			pFirst = pRead;
			if( *pFirst != 0 )pFirst++;
		}while( *pFirst != 0 );
		return Count;
	}
	long Count() const{return StringList.size();}
	const String& operator [] (int Index) const
	{
		if( StringList.size() > Index )return StringList[Index];
		return Dum;
	}
	String& operator [] (int Index)
	{
		if( StringList.size() > Index )return StringList[Index];
		return Dum;
	}
	StringSplitter<S>& Add( const String& AddStr )
	{
		StringList.push_back( AddStr );
		return *this;
	}
	String Joint(long Index=0) const
	{
		long Count = StringList.size();
		if( Count <= Index )return String();
		String Ret = StringList[Index];
		for( int i = Index+1; i < Count; i++ )
		{
			Ret += S;
			Ret += StringList[i];
		}
	}
	long Find( const String& Data ) const
	{
		for( int i = 0; i < StringList.size(); i++ )
		{
			if( StringList[i] == Data )return i;
		}
		return -1;
	}
protected:
	std::vector<String> StringList;
	String Dum;
};

#endif // !defined(AFX_STRINGSPLITTER_H__5268F2D8_F637_4174_84AF_D449B3E0B1BC__INCLUDED_)
