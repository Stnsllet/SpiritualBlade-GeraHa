// XMLTranser.h: XMLTranser クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLTRANSER_H__F36D5FD8_F3A2_49C8_99BA_ECDCFC4B29CB__INCLUDED_)
#define AFX_XMLTRANSER_H__F36D5FD8_F3A2_49C8_99BA_ECDCFC4B29CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#import "libid:F5078F18-C551-11D3-89B9-0000F81FE221"
#import <msxml3.dll> raw_interfaces_only	//no_namespace
#include <atlbase.h>

class XMLTranser  
{
public:
typedef MSXML2::IXMLDOMNode MyNode;
typedef CComBSTR MySTR;
public:
	XMLTranser();
	virtual ~XMLTranser();
	HRESULT SetTemplate( LPCTSTR FilePath );
	HRESULT SetTemplate( MyNode* pNode );
	class Processor
	{
	public:
		Processor( const XMLTranser& Trans );
		HRESULT AddParam( const MySTR& Name, const VARIANT& Value );
		HRESULT Trans( MyNode* pInput, MyNode** ppOutput );
	protected:
		CComPtr<MSXML2::IXSLProcessor> pProcess;
	};

	operator MSXML2::IXSLTemplate* () const{return pTemplate;}
protected:
	CComPtr<MSXML2::IXSLTemplate> pTemplate;
};

#endif // !defined(AFX_XMLTRANSER_H__F36D5FD8_F3A2_49C8_99BA_ECDCFC4B29CB__INCLUDED_)
