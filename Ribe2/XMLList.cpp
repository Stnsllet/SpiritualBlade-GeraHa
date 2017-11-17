// XMLList.cpp: XMLList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

XMLList::XMLList()
{
}

XMLList::~XMLList()
{

}

// 数取得
long XMLList::GetCount() const
{
	long Count = 0;
	pXMLList->get_length( &Count );

	return Count;
}

// 指定した番号取得
HRESULT XMLList::GetPaser( int Index, XMLPaser& Output ) const
{
	if( pXMLList == NULL )return E_PENDING;

	CComPtr<MyNode> pNode;
	pXMLList->get_item( Index, &pNode );

	return Output.XMLRead( pNode );
}

// 指定した番号取得
XMLPaser XMLList::GetItem( int Index ) const
{
	XMLPaser RetXML;
	GetPaser( Index, RetXML );
	return RetXML;
}

// すべての子供取得
HRESULT XMLList::Child( MyNode* pNode )
{
	if( pNode == NULL )return E_POINTER;

	CComPtr<MSXML2::IXMLDOMNodeList> pList;
	HRESULT ret = pNode->get_childNodes( &pList );
	if( pList == NULL )return E_FAIL;

	pXMLList = pList;

	return S_OK;
}

// 指定した条件で取得
HRESULT XMLList::Query( MyNode* pNode, LPCTSTR pQuery )
{
	if( pQuery == NULL )return E_POINTER;

	return Query( pNode, CComBSTR(pQuery) );
}

// 指定した条件で取得
HRESULT XMLList::Query( MyNode* pNode, const CComBSTR& Query )
{
	if( pNode == NULL )return E_POINTER;

	CComPtr<MSXML2::IXMLDOMNodeList> pList;
	pNode->selectNodes( Query, &pList );
	if( pList == NULL )return E_FAIL;

	pXMLList = pList;

	return S_OK;
}
