// XMLPaser.cpp: XMLPaser クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLPaser.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XMLPaser::XMLPaser()
{

}

XMLPaser::~XMLPaser()
{
}

HRESULT XMLPaser::CreateDocument( CComPtr<MSXML2::IXMLDOMDocument>& Output ) const
{
//	const HRESULT hr = Output.CoCreateInstance( L"Msxml2.DOMDocument" );
	const HRESULT hr = Output.CoCreateInstance( L"Msxml2.FreeThreadedDOMDocument" );
	if( FAILED(hr) )return hr;
//	Output->put_preserveWhiteSpace( VARIANT_TRUE );
	return hr;
}

// 文字列にする
XMLPaser::MySTR XMLPaser::AllXML() const
{
	if( pXMLNode == NULL )return MySTR();
	MySTR Return;
	pXMLNode->get_xml( &Return );
	return Return;
}

// 最初のエレメントを見つける
HRESULT XMLPaser::FoundBase( MyNode* pXML, MyNode** ppOutput, MSXML2::IXMLDOMNamedNodeMap** ppAttOut ) const
{
	if( pXML == NULL || ppOutput == NULL )return E_POINTER;

	MSXML2::DOMNodeType Type = MSXML2::NODE_INVALID;
	HRESULT ret = pXML->get_nodeType( &Type );
	if( Type == MSXML2::NODE_ELEMENT )
	{
		*ppOutput = CComPtr<MyNode>(pXML).Detach();
		return S_OK;
	}

	CComPtr<MSXML2::IXMLDOMNodeList> pChild;
	ret = pXML->get_childNodes( &pChild );
	if( pChild == NULL )return E_FAIL;

	CComPtr<MyNode> pFound;
	while(true)
	{
		pChild->nextNode( &pFound );
		if( pFound == NULL )return E_FAIL;
		pFound->get_nodeType( &Type );
		if( Type == MSXML2::NODE_ELEMENT )
		{
			ret = pFound->get_attributes( ppAttOut );
			*ppOutput = pFound.Detach();
			return S_OK;
		}
		pFound.Release();
	}

	return E_FAIL;
}

// ファイルから読み込み
HRESULT XMLPaser::Load( LPCTSTR pFileName )
{
	CComPtr<MSXML2::IXMLDOMDocument> pDoc;
	HRESULT ret = CreateDocument( pDoc );
	if( FAILED(ret) )return ret;

	VARIANT_BOOL BLoad;
	ret = pDoc->put_async( VARIANT_FALSE );
	if( FAILED(ret) )return ret;
	ret = pDoc->load( CComVariant(pFileName), &BLoad );
	if( FAILED(ret) )return ret;

	pXMLNode.Release();
	pAttributes.Release();
	return FoundBase( pDoc, &pXMLNode, &pAttributes );
}

// XML読み取り
HRESULT XMLPaser::XMLRead( const BSTR& XMLStr )
{
	CComPtr<MSXML2::IXMLDOMDocument> pDoc;
	HRESULT ret = CreateDocument( pDoc );
	if( FAILED(ret) )return ret;

	VARIANT_BOOL BLoad;
	ret = pDoc->loadXML( XMLStr, &BLoad );
	if( FAILED(ret) )return ret;

	pXMLNode.Release();
	pAttributes.Release();
	return FoundBase( pDoc, &pXMLNode, &pAttributes );
}

// XML読み込み
HRESULT XMLPaser::XMLRead( MyNode* pXML )
{
	if( pXML == NULL )return E_POINTER;

	pXMLNode = pXML;
	pAttributes.Release();
	pXMLNode->get_attributes( &pAttributes );
	return S_OK;
}

// テキスト取得
HRESULT XMLPaser::Text( BSTR* pOutput ) const
{
	if( pXMLNode == NULL )return E_PENDING;
	return pXMLNode->get_text( pOutput );
}

// テキスト取得
XMLPaser::MySTR XMLPaser::TextString() const
{
	CComBSTR Ret;
	Text( &Ret );
	return Ret;
}

// タグ名取得
HRESULT XMLPaser::TagName( BSTR* pOutput ) const
{
	if( pXMLNode == NULL )return E_PENDING;
	return pXMLNode->get_nodeName( pOutput );
}

// 属性取得
HRESULT XMLPaser::Attribute( const MySTR& Name, BSTR* pOutput ) const
{
	if( !Name || pOutput == NULL )return E_POINTER;
	if( pAttributes == NULL )return E_PENDING;

	CComPtr<MyNode> pAttribute;
	pAttributes->getNamedItem( Name, &pAttribute );
	if( pAttribute == NULL )return E_FAIL;

//	pAttribute->get_text( pOutput );
	CComVariant Value;
	pAttribute->get_nodeValue( &Value );
	if( Value.vt != VT_BSTR )return E_FAIL;
	*pOutput = Value.bstrVal;
	Value.bstrVal = NULL;

	return S_OK;
}

// 属性持っているか？
bool XMLPaser::HasAttribute( const MySTR& Name ) const
{
	if( !Name )return false;
	if( pAttributes == NULL )return false;
	CComPtr<MyNode> pAttribute;
	pAttributes->getNamedItem( Name, &pAttribute );
	return (pAttribute != NULL);
}

// 属性取得
XMLPaser::MySTR XMLPaser::AttributeString( const MySTR& Name ) const
{
	CComBSTR Data;
	Attribute( Name, &Data );
	return Data;
}

// 属性取得
DWORD XMLPaser::AttributeHex( const MySTR& Name ) const
{
	CComBSTR Data;
	Attribute( Name, &Data );
	if( Data.Length() <= 2 )return AttributeLong(Name);
	if( Data[0] != L'0' )return AttributeLong(Name);
	if( Data[1] != L'x' && Data[1] != L'X' )return AttributeLong(Name);

	DWORD Ret = 0;
	LPCWSTR pRead = Data;
	pRead += 2;
	while( *pRead != 0 )
	{
		DWORD Add = 0;
		if( *pRead >= L'0' && *pRead <= L'9' )Add = *pRead - L'0';
		else if ( *pRead >= L'a' && *pRead <= L'f' )Add = *pRead - L'a' + 10;
		else if ( *pRead >= L'A' && *pRead <= L'F' )Add = *pRead - L'A' + 10;
		else break;

		Ret <<= 4;
		Ret += Add;
		pRead++;
	}
	return Ret;
}

// 属性取得
long XMLPaser::AttributeLong( const MySTR& Name ) const
{
	CComBSTR StrValue;
	HRESULT ret = Attribute( Name, &StrValue );
	if( FAILED(ret) )return 0;

	CComVariant Value( StrValue );
	ret = Value.ChangeType( VT_I4 );
	if( FAILED(ret) )return 0;

	return Value.lVal;
}

// 属性取得
float XMLPaser::AttributeFloat( const MySTR& Name ) const
{
	CComBSTR StrValue;
	HRESULT ret = Attribute( Name, &StrValue );
	if( FAILED(ret) )return 0;

	CComVariant Value( StrValue );
	ret = Value.ChangeType( VT_R4 );
	if( FAILED(ret) )return 0;

	return Value.fltVal;
}

// ひとつだけ見つける
HRESULT XMLPaser::Find( const MySTR& TagName, XMLPaser& FindTag ) const
{
	if( pXMLNode == NULL )return E_PENDING;

	CComPtr<MyNode> pNode;
	HRESULT ret = pXMLNode->selectSingleNode( TagName, &pNode );

	return FindTag.XMLRead( pNode );
}
