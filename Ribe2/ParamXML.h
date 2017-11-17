
#ifndef __PARAM_XML__
#define __PARAM_XML__

#include "XMLPaser.h"

class ParamXML : public XMLPaser
{
public:
	ParamXML(){}
	ParamXML( const XMLPaser& Ref ){(XMLPaser&)(*this) = Ref;}
	virtual ~ParamXML(){}
	virtual HRESULT Attribute( const MySTR& Name, BSTR* pOutput ) const
	{
		HRESULT hr = XMLPaser::Attribute( Name, pOutput );
		if( FAILED(hr) )return hr;
		CComBSTR Check;
		Check.Attach( *pOutput );
		UserOrStr( Check );
		*pOutput = Check.Detach();
		return hr;
	}
};

#endif // __PARAM_XML__
