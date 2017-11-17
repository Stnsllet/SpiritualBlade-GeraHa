// WindowDefine.h: WindowDefine クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWDEFINE_H__8725F1EC_2C43_4AED_8A2B_9636324960F6__INCLUDED_)
#define AFX_WINDOWDEFINE_H__8725F1EC_2C43_4AED_8A2B_9636324960F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "Plane.h"
class XMLPaser;

class WindowDefine  
{
public:
	struct WindowPlane
	{
		String name;
		TexRect Plane;
	};
	typedef std::map<String,WindowPlane> WindowPlaneMap;
	struct WindowPlanes
	{
		XMLPaser XML;
		WindowPlane Base;
		WindowPlaneMap PlaneList;
		const WindowPlanes* pBasePlanes;
		const WindowPlane* Find( LPCTSTR Name ) const;
		TexRect GetRect( LPCTSTR Name ) const
		{
			const WindowPlane* pPlane = Find( Name );
			if( pPlane != NULL )return pPlane->Plane;
			return AllZero(TexRect());
		}
	};

	WindowDefine();
	~WindowDefine();

	const WindowPlanes* GetWindowPlanes( LPCTSTR WindowName ) const;
	const WindowPlane* GetWindowPlane( LPCTSTR WindowName, LPCTSTR PlaneName ) const;

	HRESULT LoadWindowDefine();
	HRESULT LoadWindowDefine( LPCTSTR FileName );
	HRESULT ReadWindowDefine( const XMLPaser& WindowXML );
	HRESULT ReadPlane( const XMLPaser& PlaneXML, WindowPlanes& Dest );

protected:
	typedef std::map<String,WindowPlanes> WindowDefineMap;
	WindowDefineMap WindowDefineList;
};

#endif // !defined(AFX_WINDOWDEFINE_H__8725F1EC_2C43_4AED_8A2B_9636324960F6__INCLUDED_)
