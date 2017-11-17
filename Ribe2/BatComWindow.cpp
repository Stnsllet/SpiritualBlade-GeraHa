// BatComWindow.cpp: BatComWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbSound.h"
#include "BattleSystem.h"
#include "RbWindowSystem.h"
#include "BatComWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BatComWindow::BatComWindow()
{
	 bDestroyOK = bMoveOK = false;
	 FloatPos.bLock = true;
	 bRestored = false;
	 bBackDraw = true;
	 bOutView = true;
}

BatComWindow::~BatComWindow()
{
}

LPCTSTR BatComWindow::WindowName() const
{
	return _T("BattleCommand");
}

bool BatComWindow::Buttons::Read( const XMLPaser& XML )
{
	const DefaultPlanes& Cache = GetCache().SystemPlane;
	up = Cache.GetPlane( String(XML.AttributeString(_T("up"))) );
	down = Cache.GetPlane( String(XML.AttributeString(_T("down"))) );
	Rect = Plane<>().ReadXML( XML );
	return true;
}

HRESULT BatComWindow::Buttons::Draw( RbSprite& Display, int x, int y, bool bDown ) const
{
	if( bDown )return Display.DrawPlane( down, x+Rect.left, y+Rect.top );
	return Display.DrawPlane( up, x+Rect.left, y+Rect.top );
}

// レストア
HRESULT BatComWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( !bBackDraw && bRestored )return S_OK;
	if( bBackDraw && TexWindow != NULL )return S_OK;

	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes( WindowName() );
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	if( bBackDraw )
	{
		ExSprite Surfase;
		HRESULT ret = RestoreBase( pDevice, Surfase, System );
		if( FAILED(ret) )return ret;
	}

	const XMLPaser& Ref = pPlanes->XML;
	XMLPaser FindXML;
	Ref.Find(_T("Plane[@name='move']"), FindXML );
	Move.Read( FindXML );
	Ref.Find(_T("Plane[@name='stop']"), FindXML );
	Stop.Read( FindXML );
	Ref.Find(_T("Plane[@name='speed']"), FindXML );
	Speed.Read( FindXML );

	bRestored = true;
	return S_OK;
}

HRESULT BatComWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( bBackDraw )
	{
		HRESULT hr = BaseWindow::Draw( Display, System );
		if( FAILED(hr) )return hr;
	}
	else if( !bRestored )SpriteRestore( Display, System );
	const BattleSystemStatus& Battle = GetBattleSystem().BStatus;
	Move.Draw( Display, Position.x, Position.y, Battle.IsMoveMode() );
	Speed.Draw( Display, Position.x, Position.y, Battle.IsSpeedMode() );
	Stop.Draw( Display, Position.x, Position.y, Battle.IsWaitMode() );
	return S_OK;
}

RbRect BatComWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( Move.Rect.Pin( x, y ) )return Move.Rect;
	if( Stop.Rect.Pin( x, y ) )return Stop.Rect;
	if( Speed.Rect.Pin( x, y ) )return Speed.Rect;
	return RbRect();
}

bool BatComWindow::ClickJob( int x, int y )
{
	BattleSystem& Battle = GetBattleSystem();
	bool bJob = true;
	if( Move.Rect.Pin( x, y ) )Battle.BStatus.MoveChange();
	else if( Stop.Rect.Pin( x, y ) )Battle.BStatus.WaitChange();
	else if( Speed.Rect.Pin( x, y ) )Battle.BStatus.SpeedChange();
	else bJob = false;
	if( bJob )GetSoundSystem().Play(_T("click"));
	return true;
}
