// XMLList.h: XMLList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLLIST_H__DF5B3AA5_161C_4873_B491_3D345EFE283D__INCLUDED_)
#define AFX_XMLLIST_H__DF5B3AA5_161C_4873_B491_3D345EFE283D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLPaser.h"

class XMLList  
{
public:
	typedef XMLPaser::MyNode MyNode;
	XMLList();
	virtual ~XMLList();
	HRESULT Child( MyNode* pNode );
	HRESULT Query( MyNode* pNode, LPCTSTR pQuery );
	HRESULT Query( MyNode* pNode, const CComBSTR& Query );

	HRESULT GetPaser( int Index, XMLPaser& Output ) const;
	XMLPaser GetItem( int Index ) const;
	long GetCount() const;

protected:
	CComPtr<MSXML2::IXMLDOMNodeList> pXMLList;

};

#endif // !defined(AFX_XMLLIST_H__DF5B3AA5_161C_4873_B491_3D345EFE283D__INCLUDED_)
