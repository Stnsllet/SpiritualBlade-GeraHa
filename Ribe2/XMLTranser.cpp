// XMLTranser.cpp: XMLTranser クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLPaser.h"
#include "XMLTranser.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XMLTranser::XMLTranser()
{
}

XMLTranser::~XMLTranser()
{
}

XMLTranser::Processor::Processor( const XMLTranser& Trans )
{
	CComPtr<MSXML2::IXSLTemplate> pTemplate = Trans;
	if( pTemplate == NULL )return;
	pTemplate->createProcessor( &pProcess );
}

HRESULT XMLTranser::Processor::AddParam( const MySTR& Name, const VARIANT& Value )
{
	if( pProcess == NULL )return E_PENDING;
	return pProcess->addParameter( Name, Value, NULL );
}

HRESULT XMLTranser::Processor::Trans( MyNode* pInput, MyNode** ppOutput )
{
	if( pProcess == NULL )return E_PENDING;
	if( ppOutput == NULL )return E_POINTER;
	HRESULT hr = pProcess->put_input( CComVariant(pInput) );
	if( FAILED(hr) )return hr;
	VARIANT_BOOL Ret = VARIANT_TRUE;
	hr = pProcess->transform( &Ret );
	CComVariant GetValue;
	hr = pProcess->get_output( &GetValue );
	if( FAILED(hr) )return hr;
	CComQIPtr<MyNode> pGet;
	switch( GetValue.vt )
	{
	default:return E_FAIL;
	case VT_UNKNOWN: case VT_DISPATCH:
		pGet = GetValue.punkVal;
		break;
	case VT_BSTR:
		{
			XMLPaser GetXML;
			GetXML.XMLRead( GetValue.bstrVal );
			pGet = GetXML;
		}
	}
	*ppOutput = pGet.Detach();
	return S_OK;
}

HRESULT XMLTranser::SetTemplate( LPCTSTR FilePath )
{
	XMLPaser ReadXML;
	HRESULT hr = ReadXML.Load( FilePath );
	if( FAILED(hr) )return hr;

	return SetTemplate( ReadXML );
}

HRESULT XMLTranser::SetTemplate( MyNode *pNode )
{
	CComPtr<MSXML2::IXSLTemplate> pNewTemplate;
	HRESULT hr = pNewTemplate.CoCreateInstance(L"Msxml2.XSLTemplate");
	if( FAILED(hr) )
	{
		hr = pNewTemplate.CoCreateInstance(L"Msxml2.XSLTemplate.3.0");
		if( FAILED(hr) )return hr;
	}
	hr = pNewTemplate->putref_stylesheet( pNode );
	if( FAILED(hr) )return hr;

	pTemplate = pNewTemplate;
	return S_OK;
}
