// DummyWindow.cpp: DummyWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameSystem.h"
#include "DummyWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DummyWindow::DummyWindow()
{
	bDestroyOK =  bMoveOK = false;
	bCursorSound = false;
	bHide = false;
	TopLevel = -5;

	pWorld = NULL;
	pBattle = NULL;
}

DummyWindow::~DummyWindow()
{
}

LPCTSTR DummyWindow::WindowName() const
{
	return _T("none");
}

HRESULT DummyWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	return S_FALSE;
}

HRESULT DummyWindow::DrawCursor( RbSprite& Display, WindowSystem& System, const GameMessage& State )
{
	return S_FALSE;
}

bool DummyWindow::SetMode( GameMode Mode )
{
	switch( Mode )
	{
	default:
		return false;
	case eWorldMode:
		pWorld = &GetWorldSystem();
		break;
	case eBattleMode:
		pBattle = &GetBattleSystem();
		break;
	 case eEventMode:
		 return true;
	}
	return true;
}

bool DummyWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	RbDisplay& Disp = GetRbDisplay();
	bool bRet = false;
	bool bOldDrag = State.IsDraging();
	if( pBattle != NULL )bRet = pBattle->MouseJob( Disp, State );
	if( pWorld != NULL )bRet = pWorld->MouseJob( Disp, State );

	if( !bOldDrag )if( State.IsDraging() )System.SetJobWindow( this );
	switch( State.Msg() )
	{
	case eDrop:
		State.DragEnd();
		return true;
	}
	return bRet;
}

// レストア
HRESULT DummyWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	return S_FALSE;
}
