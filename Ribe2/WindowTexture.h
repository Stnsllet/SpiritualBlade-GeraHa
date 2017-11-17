// WindowTexture.h: WindowTexture クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWTEXTURE_H__1BE06895_FA9E_4B4B_BE20_70A0374D20DF__INCLUDED_)
#define AFX_WINDOWTEXTURE_H__1BE06895_FA9E_4B4B_BE20_70A0374D20DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DefaultPlanes.h"

class WindowTexture : public Texture  
{
public:
	WindowTexture();
	virtual ~WindowTexture();

	HRESULT Load( DISPDEVICE* pDevice, int no=0 );
	HRESULT Load( DISPDEVICE* pDevice, LPCTSTR FileName, int no=0 );
	virtual bool LoadSystemXML( const XMLPaser& SystemXML );
	RECT GetRECT( LPCTSTR name ) const;
	HRESULT SetTexture( DISPDEVICE* pDevice, int no );
private:
	HRESULT ReadPlane( const XMLPaser& PlaneXML );
	std::map<long,String> FileMap;
	struct Plane
	{
		int x, y, Width, Height;
	};
	std::map<String,Plane> Planes;
};

#endif // !defined(AFX_WINDOWTEXTURE_H__1BE06895_FA9E_4B4B_BE20_70A0374D20DF__INCLUDED_)
