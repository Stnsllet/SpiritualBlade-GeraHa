// RbParameter.h: RbParameter クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBPARAMETER_H__759AE8C2_4544_45CD_82B8_9CB0A75E352A__INCLUDED_)
#define AFX_RBPARAMETER_H__759AE8C2_4544_45CD_82B8_9CB0A75E352A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "String.h"
class File;

class RbParameter  
{
public:
	RbParameter();
	RbParameter( const CComVariant& Source );
	~RbParameter();

	static bool IsSysCommand( const String& Name );
	static CComBSTR ToBSTR( const VARIANT& Source );

	bool GoCommand( const String& Com, const CComBSTR& StrValue );
	bool Input( const CComBSTR& Value );
	HRESULT Save( File& fp ) const;
	HRESULT Load( File& fp );

	operator const CComVariant&() const{return Value;}
protected:
	bool ComEqual( const CComBSTR& StrValue );
	bool ComLD( const CComBSTR& StrValue );
	bool ComRD( const CComBSTR& StrValue );
	bool ComPlus( CComBSTR StrValue );
	bool ComMinus( CComBSTR StrValue );
	bool ComMultiply( CComBSTR StrValue );
	bool ComDivide( CComBSTR StrValue );
	CComVariant GetVariant( const CComBSTR& StrValue ) const;
	CComVariant Value;
};

#endif // !defined(AFX_RBPARAMETER_H__759AE8C2_4544_45CD_82B8_9CB0A75E352A__INCLUDED_)
