// DischargeWindow.h: DischargeWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISCHARGEWINDOW_H__9FFDF252_D2AC_4F80_BE4F_F4CD900CB121__INCLUDED_)
#define AFX_DISCHARGEWINDOW_H__9FFDF252_D2AC_4F80_BE4F_F4CD900CB121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"

class DischargeWindow : public BaseWindow  
{
public:
	DischargeWindow();
	virtual ~DischargeWindow();

	virtual LPCTSTR WindowName() const;
	virtual bool SetParameter( const CComVariant& Value );
	virtual bool CheckParameter( const CComVariant& Value ) const;
protected:
	virtual HRESULT RestoreWindow( RbSprite& Surface, WindowSystem& System, const WindowPlanes* pPlanes );
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual bool ClickJob( int x, int y );
	virtual bool AcceptJob();
	bool NextEmploy();
	RbRect AcceptRect;
	String AcceptString;

	MemberStatus TagetMember;
};

#endif // !defined(AFX_DISCHARGEWINDOW_H__9FFDF252_D2AC_4F80_BE4F_F4CD900CB121__INCLUDED_)
