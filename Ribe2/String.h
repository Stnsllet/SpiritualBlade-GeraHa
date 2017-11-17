
#ifndef __RB_STRING__
#define __RB_STRING__


#include <string>

class String : public std::basic_string<TCHAR>
{
public:
	String(){}
//	String(LPCTSTR Src) : std::basic_string<TCHAR>(Src){}
	String(LPCTSTR Src){*this = Src;}
	String(const BSTR Src){*this = Src;}
	operator LPCTSTR() const{return begin();}
	String& operator=(LPCTSTR NewString)
	{
		if( NewString == NULL )erase();
		else assign(NewString);
		return *this;
	}
	String& operator=(const BSTR NewString)
	{
		USES_CONVERSION;
		return (*this = OLE2A(NewString));
	}

	String& Format(const TCHAR* pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);
		Format( pMessage, args );
		va_end(args);
		return *this;
	}

	String& SetVariant( const VARIANT& NewValue )
	{
		CComVariant New;
		if( FAILED(New.ChangeType(VT_BSTR,&NewValue)) )return *this;
		(*this) = New.bstrVal;
		return *this;
	}

	int Format(const TCHAR* pMessage, va_list& args)
	{
		int countBuffer = 256;
		int formatLength = -1;
		do
		{
			TCHAR* pDestBuf = (TCHAR*)::_alloca( sizeof(TCHAR)*countBuffer );
			pDestBuf[countBuffer-1] = 0;
			formatLength = _vsntprintf(pDestBuf, countBuffer-1, pMessage, args);
			if( formatLength < 0 )continue;
			*this = pDestBuf;
			break;
		}while( countBuffer*= 2 );
		return formatLength;
	}
	bool IsFirst( LPCTSTR CheckStr ) const
	{
		if( CheckStr == NULL )return empty();
		long Count = ::_tcslen( CheckStr );
		if( empty() )return (Count<=0);
		return (0==::_tcsncmp( CheckStr, *this, Count ));
	}

	long ToLong() const{return (empty())?(0):(_ttol(*this));}
	float ToFloat() const{return (empty())?(0):(atof(*this));}

	bool operator==(const String& CheckString) const{return (length()!=CheckString.length())?(false):(*this==(LPCTSTR)CheckString);}
	bool operator==(LPCTSTR CheckString) const{return (CheckString==NULL)?(empty()):(0==compare( CheckString ));}
	bool operator<(LPCTSTR CheckString) const{return (CheckString==NULL)?(false):(0>compare( CheckString ));}
	bool operator>(LPCTSTR CheckString) const{return (CheckString==NULL)?(!empty()):(0<compare( CheckString ));}
};

#endif //__RB_STRING__