// XMLPaser.h: XMLPaser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLPASER_H__CDE20A72_376D_4DD2_96C9_C2CA820D62D5__INCLUDED_)
#define AFX_XMLPASER_H__CDE20A72_376D_4DD2_96C9_C2CA820D62D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#import "libid:F5078F18-C551-11D3-89B9-0000F81FE221"
#import <msxml3.dll> raw_interfaces_only	//no_namespace
#include <atlbase.h>

class XMLPaser  
{
public:
typedef MSXML2::IXMLDOMNode MyNode;
typedef CComBSTR MySTR;
	XMLPaser();
	virtual ~XMLPaser();
	operator MyNode*()const{return pXMLNode;}
	bool operator !() const{return (pXMLNode==NULL);}

	virtual HRESULT Load( LPCTSTR pFileName );
	HRESULT XMLRead( const BSTR& XMLStr );
	HRESULT XMLRead( MyNode* pXML );

	HRESULT Find( const MySTR& TagName, XMLPaser& FindTag ) const;

	MySTR TextString() const;
	HRESULT Text( BSTR* pOutput ) const;
	HRESULT TagName( BSTR* pOutput ) const;

	bool HasAttribute( const MySTR& Name ) const;
	MySTR AttributeString( const MySTR& Name ) const;
	DWORD AttributeHex( const MySTR& Name ) const;
	bool AttributeBool( const MySTR& Name ) const{return 0!=AttributeLong(Name);}
	long AttributeLong( const MySTR& Name ) const;
	float AttributeFloat( const MySTR& Name ) const;
	virtual HRESULT Attribute( const MySTR& Name, BSTR* pOutput ) const;
	MySTR AllXML() const;

protected:
	virtual HRESULT CreateDocument( CComPtr<MSXML2::IXMLDOMDocument>& Output ) const;
	HRESULT FoundBase( MyNode* pXML, MyNode** ppOutput, MSXML2::IXMLDOMNamedNodeMap** ppAttOut ) const;
	CComPtr<MyNode> pXMLNode;
	CComPtr<MSXML2::IXMLDOMNamedNodeMap> pAttributes;
};

#endif // !defined(AFX_XMLPASER_H__CDE20A72_376D_4DD2_96C9_C2CA820D62D5__INCLUDED_)
