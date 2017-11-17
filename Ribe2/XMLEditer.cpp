// XMLEditer.cpp: XMLEditer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLEditer.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XMLEditer::XMLEditer()
{

}

XMLEditer::~XMLEditer()
{

}

HRESULT XMLEditer::CreateDocument( CComPtr<MSXML2::IXMLDOMDocument>& Output ) const
{
	const HRESULT hr = Output.CoCreateInstance( L"Msxml2.DOMDocument" );
	if( FAILED(hr) )return hr;
//	Output->put_preserveWhiteSpace( VARIANT_TRUE );
	return hr;
}

// ファイルに書き込み
HRESULT XMLEditer::Save( LPCTSTR pFileName ) const
{
	if( pXMLNode == NULL )return E_PENDING;

	CComPtr<MSXML2::IXMLDOMDocument> pDoc;
	HRESULT ret = CreateDocument( pDoc );
	if( FAILED(ret) )return ret;

	CComBSTR XML;
	ret = pXMLNode->get_xml( &XML );
	if( FAILED(ret) )return ret;

	VARIANT_BOOL BLoad;
	ret = pDoc->loadXML( XML, &BLoad );
	if( FAILED(ret) )return ret;

	return pDoc->save( CComVariant(pFileName) );
}

// 属性に新しい値セット
HRESULT XMLEditer::SetAttribute( const MySTR& AttName, BSTR NewValue )
{
	if( !AttName || NewValue == NULL )return E_POINTER;
	if( pAttributes == NULL )return E_PENDING;

	CComPtr<MyNode> pAttribute;
	pAttributes->getNamedItem( AttName, &pAttribute );
	if( pAttribute != NULL )return pAttribute->put_text( NewValue );

	CComPtr<MSXML2::IXMLDOMDocument> pDoc;
	HRESULT ret = pXMLNode->get_ownerDocument( &pDoc );
	if( FAILED(ret) )return ret;

	CComPtr<MSXML2::IXMLDOMAttribute> pNewAtt;
	ret = pDoc->createAttribute( AttName, &pNewAtt );
	if( FAILED(ret) )return ret;

	pNewAtt->put_text( NewValue );
	return pAttributes->setNamedItem( pNewAtt, NULL );
}

// 新しく作る
HRESULT XMLEditer::Create( const MySTR& RootTagName )
{
	CComPtr<MSXML2::IXMLDOMDocument> pDoc;
	HRESULT ret = CreateDocument( pDoc );
	if( FAILED(ret) )return ret;

	CComPtr<MSXML2::IXMLDOMElement> pElement;
	ret = pDoc->createElement( RootTagName, &pElement );
	if( FAILED(ret) )return ret;

	ret = pDoc->appendChild( pElement, NULL );
	if( FAILED(ret) )return ret;

	return XMLRead( pElement );
}

// 新しいタグを作る
HRESULT XMLEditer::CreateChild( const MySTR& TagName, XMLEditer& NewChild ) const
{
	if( pXMLNode == NULL )return E_PENDING;

	CComPtr<MSXML2::IXMLDOMDocument> pDoc;
	HRESULT ret = pXMLNode->get_ownerDocument( &pDoc );
	if( FAILED(ret) )return ret;
	if( pDoc == NULL )return E_FAIL;

	CComPtr<MSXML2::IXMLDOMElement> pElement;
	ret = pDoc->createElement( TagName, &pElement );
	if( FAILED(ret) )return ret;

	ret = pXMLNode->appendChild( pElement, NULL );
	if( FAILED(ret) )return ret;

	return NewChild.XMLRead( pElement );
}
