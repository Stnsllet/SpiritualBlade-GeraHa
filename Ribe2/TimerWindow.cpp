// TimerWindow.cpp: TimerWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbWindowSystem.h"
#include "BattleSystem.h"
#include "TimerWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TimerWindow::TimerWindow()
{
	StartTime = MaxTime = 0;
	FloatPos.bLock = true;
	bCursorSound = bDestroyOK = bMoveOK = false;
}

TimerWindow::~TimerWindow()
{
}

LPCTSTR TimerWindow::WindowName() const
{
	return _T("Timer");
}

bool TimerWindow::SetParameter( const CComVariant& Value )
{
	CComVariant NewValue;
	if( FAILED(NewValue.ChangeType( VT_R4, &Value )) )return false;

	MaxTime = NewValue.fltVal;
	StartTime = GetBattleSystem().GetBattleTime();

	return true;
}

HRESULT TimerWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	HRESULT hr = BaseWindow::Draw( Display, System );
	if( FAILED(hr) )return hr;
	if( MaxTime <= 0 )return S_FALSE;
	RbRect Rect = TimerRect;
	Rect.Move( Position.x, Position.y );
	double Time = MaxTime - Trim( GetBattleSystem().GetBattleTime() - StartTime, MaxTime, 0.0 );
	Rect.right = Rect.left + (Time/MaxTime) * TimerRect.Width();

	Display.Draw( Timer_Full, RbRect(Timer_Full), Rect );

	return S_OK;
}

HRESULT TimerWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_FALSE;
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	const DefaultPlanes& Cache = GetCache().SystemPlane;
	TimerRect = pPlanes->GetRect(_T("time"));
	Timer_Full = Cache.GetPlane(_T("time_f"));
	Timer_Empty = Cache.GetPlane(_T("time_e"));

	ExSprite Surface;
	RestoreBase( pDevice, Surface, System );

	DrawWindowTag( System, WindowName() );
	RbRect SourceRect = Timer_Empty;
	Surface.Draw( Timer_Empty, SourceRect, TimerRect );

	return S_OK;
}
