// XMLEditer.h: XMLEditer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLEDITER_H__0000848D_C299_447F_88F7_945BE8EEC5A4__INCLUDED_)
#define AFX_XMLEDITER_H__0000848D_C299_447F_88F7_945BE8EEC5A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLPaser.h"

class XMLEditer : public XMLPaser  
{
public:
	XMLEditer();
	virtual ~XMLEditer();

	template<class T> HRESULT SetAttribute(const MySTR& Name, const T& Value )
	{
		CComVariant VValue( Value );
		VValue.ChangeType( VT_BSTR );
		return SetAttribute( Name, VValue.bstrVal );
	}
	HRESULT SetAttribute( const MySTR& AttName, BSTR NewValue );

	HRESULT Create( const MySTR& RootTagName );
	HRESULT CreateChild( const MySTR& TagName, XMLEditer& NewChild ) const;

	virtual HRESULT Save( LPCTSTR pFileName ) const;
protected:
	virtual HRESULT CreateDocument( CComPtr<MSXML2::IXMLDOMDocument>& Output ) const;
};

#endif // !defined(AFX_XMLEDITER_H__0000848D_C299_447F_88F7_945BE8EEC5A4__INCLUDED_)
