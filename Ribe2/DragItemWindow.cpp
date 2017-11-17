// DragItemWindow.cpp: DragItemWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameSystem.h"
#include "RbMember.h"
#include "DragItemWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DragItemWindow::DragItemWindow()
{
	bDestroyOK = false;
	Data.StartTime = RbTimer();
	Data.bFromExPaty = false;
	Type = eNonDraging;
}

DragItemWindow::~DragItemWindow()
{

}

bool DragItemWindow::SetDrag( WindowSystem& System, GameMessage& State )
{
	System.AddWindow( this );
	System.SetJobWindow( this );
	State.DragStart();
	return true;
}

HRESULT DragItemWindow::DrawCursor( RbSprite& Display, WindowSystem& System, const GameMessage& State )
{
	if( !State.IsDraging() )
	{
		Command( NULL, System, eDestroy );
		return S_FALSE;
	}
	RefPtr<BaseWindow> pHitWnd = System.GetHitWindow( State.x, State.y, this );
	if( pHitWnd == NULL )return S_FALSE;
	if( pHitWnd == this )return S_FALSE;
	GameMessage DumState = State;
	DumState.Drag.bDrag = false;
	return pHitWnd->DrawCursor( Display, System, DumState );
}

bool DragItemWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	RefPtr<BaseWindow> pHitWnd;
	switch( State.Msg() )
	{
	case eDrop:
		pHitWnd = System.GetHitWindow( State.x, State.y, this );
		if( pHitWnd != NULL )
		{
			RbRect Rect = pHitWnd->GetRect();
			DropPos.x = State.x - Rect.left;
			DropPos.y = State.y - Rect.top;
			pHitWnd->DropJob( *this );
		}
		State.DragEnd();
		Command( NULL, System, eDestroy );
		return true;
	}
	return BaseWindow::MouseJob( System, State );
}

bool DragItemWindow::SetItemData( long ItemNum )
{
	Type = eNonDraging;
	Data.Item = GetCache().Item.Get( ItemNum );
	if( Data.Item.id.Number <= 0 )return false;
	Data.Name = Data.Item.Name;
	Type = eDragItem;
	return true;
}

bool DragItemWindow::SetMemberData( const MemberStatus& State, bool bFromEx )
{
	Type = eNonDraging;
	Data.Member = State;
	RbMember Mem(State);
	Data.pRbTex = GetCache().RbTex.Get( Mem.GetTexture() );
	if( Data.pRbTex == NULL )return false;
	Data.Name = (LPCTSTR)Data.Member.PasonName;
	Data.bFromExPaty = bFromEx;
	Type = eDragMember;
	return true;
}

// レストア
HRESULT DragItemWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;

	switch( Type )
	{
	case eDragItem:
		return RestoreString( pDevice, System );
	case eDragMember:
		return S_OK;
	}
	return E_PENDING;
}

HRESULT DragItemWindow::RestoreString( DISPDEVICE* pDevice, WindowSystem& System )
{
	LPCTSTR Font = _T("default");
	RbRect TextRect = System.Font().GetTextRect( Font, Data.Name );
	Position.Width = TextRect.Width();
	Position.Height = TextRect.Height();

	HRESULT ret = TexWindow.Create( pDevice, Position.Width, Position.Height );
	if( FAILED(ret) )return ret;

	ExSprite Surface;
	ret = Surface.InitSprite( pDevice, TexWindow );
	if( FAILED(ret) )return ret;

	Surface.ColorClear(0x80000000);

	return System.Font().Draw( Font, Data.Name, TextRect );
}

// 描写
HRESULT DragItemWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	const GameMessage& Mouse = g_pSystem->RefMessage();
	if( !Mouse.IsDraging() )
	{
		Command( NULL, System, eDestroy );
		return S_FALSE;
	}
	if( IsHide() )return S_FALSE;
	switch( Type )
	{
	case eDragItem:
		return DrawString( Display, System );
	case eDragMember:
		return DrawMember( Display, System );
	}
	return S_FALSE;
}

// 描写
HRESULT DragItemWindow::DrawString( RbSprite& Display, WindowSystem& System )
{
	if( !RestoreCheck(Display,System) )return S_FALSE;
	const GameMessage& Mouse = g_pSystem->RefMessage();
	Position.x = Mouse.x - Position.Width/2;
	Position.y = Mouse.y - Position.Height/2;

	return BaseWindow::Draw( Display, System );
}

// 描写
HRESULT DragItemWindow::DrawMember( RbSprite& Display, WindowSystem& System )
{
	if( Data.pRbTex == NULL )return S_FALSE;
	const GameMessage& Mouse = g_pSystem->RefMessage();
	Position.x = Mouse.x;
	Position.y = Mouse.y;
	DWORD Passed = RbTimer().Passed( Data.StartTime );
	long h = Data.pRbTex->GetHight()/2.0f;

	return Display.DrawRbTexture( Data.pRbTex, Position.x, Position.y+h, _T("move"), 1.25f, Passed );
}
