// TimerWindow.h: TimerWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMERWINDOW_H__890B3F8E_922D_40ED_B441_F253650205A7__INCLUDED_)
#define AFX_TIMERWINDOW_H__890B3F8E_922D_40ED_B441_F253650205A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"

class TimerWindow : public BaseWindow  
{
public:
	TimerWindow();
	virtual ~TimerWindow();

	virtual LPCTSTR WindowName() const;
	virtual bool SetParameter( const CComVariant& Value );

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );

	double MaxTime;
	double StartTime;
	RbRect TimerRect;
	Plane<> Timer_Full;
	Plane<> Timer_Empty;
};

#endif // !defined(AFX_TIMERWINDOW_H__890B3F8E_922D_40ED_B441_F253650205A7__INCLUDED_)
