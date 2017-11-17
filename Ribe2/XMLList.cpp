// XMLList.cpp: XMLList �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

XMLList::XMLList()
{
}

XMLList::~XMLList()
{

}

// ���擾
long XMLList::GetCount() const
{
	long Count = 0;
	pXMLList->get_length( &Count );

	return Count;
}

// �w�肵���ԍ��擾
HRESULT XMLList::GetPaser( int Index, XMLPaser& Output ) const
{
	if( pXMLList == NULL )return E_PENDING;

	CComPtr<MyNode> pNode;
	pXMLList->get_item( Index, &pNode );

	return Output.XMLRead( pNode );
}

// �w�肵���ԍ��擾
XMLPaser XMLList::GetItem( int Index ) const
{
	XMLPaser RetXML;
	GetPaser( Index, RetXML );
	return RetXML;
}

// ���ׂĂ̎q���擾
HRESULT XMLList::Child( MyNode* pNode )
{
	if( pNode == NULL )return E_POINTER;

	CComPtr<MSXML2::IXMLDOMNodeList> pList;
	HRESULT ret = pNode->get_childNodes( &pList );
	if( pList == NULL )return E_FAIL;

	pXMLList = pList;

	return S_OK;
}

// �w�肵�������Ŏ擾
HRESULT XMLList::Query( MyNode* pNode, LPCTSTR pQuery )
{
	if( pQuery == NULL )return E_POINTER;

	return Query( pNode, CComBSTR(pQuery) );
}

// �w�肵�������Ŏ擾
HRESULT XMLList::Query( MyNode* pNode, const CComBSTR& Query )
{
	if( pNode == NULL )return E_POINTER;

	CComPtr<MSXML2::IXMLDOMNodeList> pList;
	pNode->selectNodes( Query, &pList );
	if( pList == NULL )return E_FAIL;

	pXMLList = pList;

	return S_OK;
}
