// EmployWindow.h: EmployWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMPLOYWINDOW_H__DCC1EB6A_382B_4B1C_BB48_DBE000B8A7C7__INCLUDED_)
#define AFX_EMPLOYWINDOW_H__DCC1EB6A_382B_4B1C_BB48_DBE000B8A7C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DischargeWindow.h"

class EmployWindow : public DischargeWindow
{
public:
	EmployWindow();
	virtual ~EmployWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool SetParameter( const CComVariant& Value );
protected:
	virtual HRESULT RestoreWindow( RbSprite& Surface, WindowSystem& System, const WindowPlanes* pPlanes );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual bool ClickJob( int x, int y );
	virtual bool AcceptJob();
	bool NextEmploy();
	long GetPrice() const;
	RbRect NextRect;
};

#endif // !defined(AFX_EMPLOYWINDOW_H__DCC1EB6A_382B_4B1C_BB48_DBE000B8A7C7__INCLUDED_)
