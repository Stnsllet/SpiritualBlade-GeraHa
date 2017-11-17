// RbParameter.cpp: RbParameter クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"
#include "StrCommander.h"
#include "RbGameData.h"
#include "RbParameter.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbParameter::RbParameter()
{
}

RbParameter::RbParameter( const CComVariant& Source )
{
	Value = GetVariant(ToBSTR(Source));
}

RbParameter::~RbParameter()
{
}

bool RbParameter::IsSysCommand( const String& Name )
{
	if( Name.empty() )return false;
	return ((LPCTSTR)Name)[0]==_T('_');
}

CComBSTR RbParameter::ToBSTR( const VARIANT& Source )
{
	CComVariant StrValue;
	if( FAILED(StrValue.ChangeType( VT_BSTR, &Source )) )return CComBSTR();
	return CComBSTR(StrValue.bstrVal);
}

CComVariant RbParameter::GetVariant( const CComBSTR& StrValue ) const
{
	CComVariant NewValue( StrValue );
	if( !FAILED(NewValue.ChangeType(VT_R4)) )return NewValue;
	return NewValue;
}

HRESULT RbParameter::Save( File& fp ) const
{
	HRESULT hr = fp.Write( Value.vt );
	if( FAILED(hr) )return hr;
	switch( Value.vt )
	{
	case VT_BSTR:
		hr = fp.WriteStr( String(Value.bstrVal) );
		break;
	case VT_R4:
		hr = fp.Write( Value.fltVal );
		break;
	}
	if( FAILED(hr) )return hr;
	return S_OK;
}

HRESULT RbParameter::Load( File& fp )
{
	CComVariant NewValue;
	AllZero(NewValue);
	HRESULT hr = fp.Read( NewValue.vt );
	if( FAILED(hr) )return hr;
	switch( NewValue.vt )
	{
	case VT_BSTR:
		{
			String ReadStr;
			hr = fp.ReadStr( ReadStr );
			NewValue = ReadStr;
		}
		break;
	case VT_R4:
		hr = fp.Read( NewValue.fltVal );
		break;
	}
	if( FAILED(hr) )return hr;
	Value = NewValue;
	return S_OK;
}

bool RbParameter::ComPlus( CComBSTR StrValue )
{
	UserOrStr(StrValue);
	const CComVariant& NewValue = GetVariant(StrValue);
	if( Value.vt == VT_EMPTY )Value.ChangeType(NewValue.vt);

	switch( Value.vt )
	{
	case VT_BSTR:
		Value = CComBSTR(Value.bstrVal)+=StrValue;
		return true;
	case VT_R4:
		if( Value.vt != NewValue.vt )return false;
		Value.fltVal += NewValue.fltVal;
		return true;
	}
	return false;
}

bool RbParameter::ComMinus( CComBSTR StrValue )
{
	UserOrStr(StrValue);
	const CComVariant& NewValue = GetVariant(StrValue);
	if( Value.vt == VT_EMPTY )Value.ChangeType(NewValue.vt);

	switch( Value.vt )
	{
	case VT_BSTR:
		return false;
	case VT_R4:
		if( Value.vt != NewValue.vt )return false;
		Value.fltVal -= NewValue.fltVal;
		return true;
	}
	return false;
}

bool RbParameter::ComMultiply( CComBSTR StrValue )
{
	UserOrStr(StrValue);
	switch( Value.vt )
	{
	default:
		return ComPlus( StrValue );
	case VT_R4:
		break;
	}
	const CComVariant& NewValue = GetVariant(StrValue);
	if( Value.vt == VT_EMPTY )Value.ChangeType(NewValue.vt);

	if( Value.vt != NewValue.vt )return false;
	Value.fltVal *= NewValue.fltVal;

	return false;
}

bool RbParameter::ComDivide( CComBSTR StrValue )
{
	UserOrStr(StrValue);
	switch( Value.vt )
	{
	default:
		return false;
	case VT_R4:
		break;
	}
	const CComVariant& NewValue = GetVariant(StrValue);
	if( Value.vt == VT_EMPTY )Value.ChangeType(NewValue.vt);

	if( Value.vt != NewValue.vt )return false;
	if( !(NewValue.fltVal != 0) )return false;
	Value.fltVal /= NewValue.fltVal;

	return false;
}

bool RbParameter::ComEqual( const CComBSTR& StrValue )
{
	const CComVariant& NewValue = GetVariant(StrValue);
	switch( Value.vt )
	{
	case VT_BSTR:
		return (StrValue==Value.bstrVal);
	case VT_R4:
		if( Value.vt != NewValue.vt )return false;
		return ((int)((Value.fltVal*1000)+0.5)==(int)((NewValue.fltVal*1000)+0.5));
	}
	return false;
}

bool RbParameter::ComLD( const CComBSTR& StrValue )
{
	const CComVariant& NewValue = GetVariant(StrValue);
	switch( Value.vt )
	{
	case VT_BSTR:
		return String(Value.bstrVal)<String(StrValue);
	case VT_R4:
		if( Value.vt != NewValue.vt )return false;
		return (Value.fltVal<NewValue.fltVal);
	}
	return false;
}

bool RbParameter::ComRD( const CComBSTR& StrValue )
{
	const CComVariant& NewValue = GetVariant(StrValue);
	switch( Value.vt )
	{
	case VT_BSTR:
		return String(Value.bstrVal)>String(StrValue);
	case VT_R4:
		if( Value.vt != NewValue.vt )return false;
		return (Value.fltVal>NewValue.fltVal);
	}
	return false;
}

bool RbParameter::GoCommand( const String& Com, const CComBSTR& StrValue )
{
	if( Com.empty() )return Input( StrValue );
	if( Com==_T("+") )return ComPlus( StrValue );
	if( Com==_T("-") )return ComMinus( StrValue );
	if( Com==_T("*") )return ComMultiply( StrValue );
	if( Com==_T("/") )return ComDivide( StrValue );
	if( Com==_T("-") )return ComPlus( StrValue );
	if( Com==_T("=") )return ComEqual( StrValue );
	if( Com==_T("ld") )return ComLD( StrValue );
	if( Com==_T("rd") )return ComRD( StrValue );
	return false;
}

bool RbParameter::Input( const CComBSTR& StrValue )
{
	Value = GetVariant(StrValue);
	return true;
}
