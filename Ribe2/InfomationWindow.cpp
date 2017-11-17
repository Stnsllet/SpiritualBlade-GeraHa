// InfomationWindow.cpp: InfomationWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameSystem.h"
#include "InfomationWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

InfomationWindow::InfomationWindow()
{
	FloatPos.x = FloatPos.y = 0;
	FloatPos.bLock = true;
	bDestroyOK =  bMoveOK = true;
	TopLevel = 1;

	Time = RbTimer();
}

InfomationWindow::~InfomationWindow()
{
}

LPCTSTR InfomationWindow::WindowName() const
{
	return _T("Infomation");
}

HRESULT InfomationWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( RbTimer().Passed( Time ) > 500 )
	{
		if( Pos != g_pSystem->RefMessage().ViewPos )TexWindow.Release();
	}
	return BaseWindow::Draw( Display, System );
}

HRESULT InfomationWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;

	Pos = g_pSystem->RefMessage().ViewPos;
	String Msg;
	Msg.Format( _T("x %4.0f Y %4.0f z %3.0f"),
		Pos.x, Pos.y, Pos.z);

	LandEdit& Edit = GetLandEdit();
	if( Edit.Edit.IsEditMode() )
	{
		LandMapData::MapID ID = Edit.Map.GetMapID( Pos.x, Pos.y );
		const String& Type = GetCache().Land.GetTypeData( ID ).Type;
		String EditInfo;
		EditInfo.Format( _T("\n%s:%02d"), (LPCTSTR)Type, (long)ID);
		Msg += EditInfo;
	}

	RbRect Rect = System.Font().GetTextRect( _T("default"), Msg );
	Position.Width = Rect.Width();
	Position.Height = Rect.Height();

	HRESULT ret = TexWindow.Create( pDevice, Position.Width, Position.Height );
	if( FAILED(ret) )return ret;

	ExSprite Surface;
	ret = Surface.InitSprite( pDevice, TexWindow );
	if( FAILED(ret) )return ret;

	Surface.ColorClear( 0x80000000 );
	System.Font().Draw( Msg, Rect );

	Time = RbTimer();
	return S_OK;
}
