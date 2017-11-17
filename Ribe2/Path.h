// Path.h: Path クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATH_H__89C11BA1_A2D7_4584_87DD_3D0A54058381__INCLUDED_)
#define AFX_PATH_H__89C11BA1_A2D7_4584_87DD_3D0A54058381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "String.h"

class Path  : public String
{
public:
	Path(){}
	Path(LPCTSTR Src) : String(Src){}
	Path(const BSTR Src) : String(Src){}
	~Path(){}

	template <class T> Path& operator=( const T& Data )
	{
		*((String*)this) = Data;
		return *this;
	}

	Path& operator += ( LPCTSTR NewString )
	{
		if( NewString == NULL )return *this;
		if( length() <= 0 )return *this = NewString;
		TCHAR Back = ( rfind(_T("/")) <= -1 )?(_T('/')):(_T('\\'));
		TCHAR Last = (*this)[length()-1];
		if( Last != Back )append( 1, Back );

		append( NewString );
		return *this;
	}
	Path& ChangeExt( LPCTSTR NewExt )
	{
		if( NewExt == NULL )return *this;
		long Find = rfind(_T('.'));
		long FindS = rfind(_T('/'));
		if( Find>FindS )
		{
			erase(Find);
		}
		append(_T("."));
		append(NewExt);
		return *this;
	}
	Path Ext() const
	{
		long Find = rfind(_T('.'));
		long FindS = rfind(_T('/'));
		if( Find>FindS )
		{
			Path Ret = substr(Find+1).data();
			return Ret;
		}
		return Path();
	}

};

#endif // !defined(AFX_PATH_H__89C11BA1_A2D7_4584_87DD_3D0A54058381__INCLUDED_)
