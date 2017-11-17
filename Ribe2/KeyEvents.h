// KeyEvents.h: KeyEvents クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYEVENTS_H__776C1042_6ACA_4381_BFC6_8ACF882E95B9__INCLUDED_)
#define AFX_KEYEVENTS_H__776C1042_6ACA_4381_BFC6_8ACF882E95B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "String.h"
class XMLPaser;

class KeyEvents  
{
public:
	KeyEvents();
	virtual ~KeyEvents();
	bool LoadXML();
	bool LoadXML( LPCTSTR FileName );
	String GetPath( long KeyCode ) const;

protected:
	bool ReadXML( const XMLPaser& ReadXML );
	std::map<long,String> EventMap;
};

#endif // !defined(AFX_KEYEVENTS_H__776C1042_6ACA_4381_BFC6_8ACF882E95B9__INCLUDED_)
