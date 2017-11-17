// ComBaseWindow.h: ComBaseWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBASEWINDOW_H__B548F5A3_95ED_40DC_875F_71EE96DC9E11__INCLUDED_)
#define AFX_COMBASEWINDOW_H__B548F5A3_95ED_40DC_875F_71EE96DC9E11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "BaseWindow.h"
#include "ParamXML.h"
class RbGameData;

class ComBaseWindow : public BaseWindow  
{
public:
	ComBaseWindow();
	virtual ~ComBaseWindow();
	virtual LPCTSTR WindowName() const;

	bool SetCommand( WindowSystem& System, LPCTSTR WindowName );
	bool SetXML( WindowSystem& System, const XMLPaser& ComXML );
	virtual bool SetDefaultPosition( WindowSystem& System );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual bool ClickJob( int x, int y );
	bool AddPlane( const XMLPaser& PlaneXML );
	struct CommandPlane
	{
		bool SetXML( const XMLPaser& PlaneXML );
		bool CheckView( const RbGameData& UserData );
		RbRect Rect;
		String Msg;
		String Tex;
		String Com;
		String Event;
		String FlgOn,FlgOff;
		bool bExec;
		bool bView;
	};
	std::list<CommandPlane> ComPlaneList;
	String MyWindowName;
	String Title;
	String Font;
	ParamXML SourceXML;
};

#endif // !defined(AFX_COMBASEWINDOW_H__B548F5A3_95ED_40DC_875F_71EE96DC9E11__INCLUDED_)
