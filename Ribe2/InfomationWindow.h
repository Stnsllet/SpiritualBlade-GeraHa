// InfomationWindow.h: InfomationWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOMATIONWINDOW_H__89813F38_3470_4D46_BE29_5C2A6C99D097__INCLUDED_)
#define AFX_INFOMATIONWINDOW_H__89813F38_3470_4D46_BE29_5C2A6C99D097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"

class InfomationWindow : public BaseWindow  
{
public:
	InfomationWindow();
	virtual ~InfomationWindow();
	virtual LPCTSTR WindowName() const;
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );

	virtual bool HitTest( int x, int y ) const{return false;}
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );

	double Time;
	RbPosition Pos;
};

#endif // !defined(AFX_INFOMATIONWINDOW_H__89813F38_3470_4D46_BE29_5C2A6C99D097__INCLUDED_)
