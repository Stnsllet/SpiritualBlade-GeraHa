// BatComWindow.h: BatComWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATCOMWINDOW_H__EF46E24C_D94D_4E7A_9AEB_299CE51EACCC__INCLUDED_)
#define AFX_BATCOMWINDOW_H__EF46E24C_D94D_4E7A_9AEB_299CE51EACCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"

class BatComWindow : public BaseWindow
{
public:
	BatComWindow();
	virtual ~BatComWindow();
	virtual LPCTSTR WindowName() const;

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
protected:
	virtual bool ClickJob( int x, int y );
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	struct Buttons
	{
		HRESULT Draw( RbSprite& Display, int x, int y, bool bDown ) const;
		bool Read( const XMLPaser& XML );
		Plane<> down;
		Plane<> up;
		RbRect Rect;
	};
	Buttons Move, Stop, Speed;
	bool bRestored;
	bool bBackDraw;
};

#endif // !defined(AFX_BATCOMWINDOW_H__EF46E24C_D94D_4E7A_9AEB_299CE51EACCC__INCLUDED_)
