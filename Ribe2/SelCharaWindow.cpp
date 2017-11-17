// SelCharaWindow.cpp: SelCharaWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbMember.h"
#include "BattleSystem.h"
#include "RbWindowSystem.h"
#include "StrCommander.h"
#include "SelCharaWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SelCharaWindow::SelCharaWindow()
{
	bDestroyOK = bMoveOK = false;
	bOutView = true;
	FloatPos.bLock = true;
	TopLevel = 1;
}

SelCharaWindow::~SelCharaWindow()
{
}

LPCTSTR SelCharaWindow::WindowName() const
{
	return _T("SelectCharactor");
}

bool SelCharaWindow::RedrawCheck() const
{
	return (GetBattleSystem().GetSelectCharactor() != pStatus);
}

// レストア
HRESULT SelCharaWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;

	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes( WindowName() );
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	ExSprite Surfase;
	RestoreBase( pDevice, Surfase, System );

	pStatus = GetBattleSystem().GetSelectCharactor();
	if( pStatus == NULL )return S_FALSE;

	const WindowPlane* pNamePlane = pPlanes->Find(_T("name"));
	if( pNamePlane == NULL )return E_FAIL;

	DrawWindowTag( System, WindowName(), pNamePlane->Plane.x );

	const WindowPlane* pImagePlane = pPlanes->Find(_T("image"));
	if( pImagePlane == NULL )return E_FAIL;

	ImageArea = pImagePlane->Plane;

	RbMember Mem( pStatus->MemberState() );

	RbRect TextRect( pNamePlane->Plane );
	System.Font().Draw( _T("funcy"), Mem.PasonData().Name, TextRect, DT_CENTER );

	return S_OK;
}

bool SelCharaWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	switch( State.Msg() )
	{
	case eLDown:
		if( pStatus != NULL )SetViewOn();
		return true;
	case eLDblClick:
		if( pStatus != NULL )StrCommander().ShowBattleStatus( pStatus );
		return true;
	}
	return StatusBaseWinow::MouseJob( System, State );
}
