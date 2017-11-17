// WindowHandle.cpp: WindowHandle クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbRect.h"
#include "WindowHandle.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

WindowHandle::WindowHandle()
{
	m_hWnd = NULL;
}

WindowHandle::WindowHandle( HWND hWnd ) : m_hWnd(hWnd)
{
}

WindowHandle::~WindowHandle()
{

}

bool WindowHandle::SetHandle( HWND hWnd )
{
	m_hWnd = hWnd;
	return true;
}

RECT WindowHandle::ClientToWindow( UINT Width, UINT Height )
{
	RbRect RcNewWnd( 0,0, Width, Height);
	RbRect RcWnd;
	if( FALSE == ::GetWindowRect( m_hWnd, &((RECT&)RcWnd) ) )return RcNewWnd;

	POINT LT = { 0, 0 };
	::ClientToScreen( m_hWnd, &LT );
	POINT RB = { Width, Height };
	::ClientToScreen( m_hWnd, &RB );

	RcNewWnd.left = LT.x;
	RcNewWnd.top = LT.y;
	RcNewWnd.right = RB.x;
	RcNewWnd.bottom = RB.y;
	RcNewWnd.SetCenter( RcWnd.Center() );
	return RcNewWnd;
}

bool WindowHandle::SetClientSize( UINT Width, UINT Height )
{
	RbRect RcNewWnd = ClientToWindow( Width, Height );
	return FALSE!=SetWindowPos( m_hWnd, HWND_NOTOPMOST,
		RcNewWnd.left, RcNewWnd.top, RcNewWnd.Width(), RcNewWnd.Height(), SWP_NOZORDER );
}

RECT WindowHandle::ScalingSize( const RECT& Rect, float ScaleWidth, float ScaleHeight, bool bChangeHeight )
{
	RbRect NewRect(Rect);
	if( ScaleHeight <= 0 || ScaleHeight <= 0 )return Rect;

	if( bChangeHeight )
	{
		NewRect.bottom = NewRect.top
			+ NewRect.Width() * (ScaleHeight/ScaleWidth);
	}
	else
	{
		NewRect.right = NewRect.left
			+ NewRect.Height() * (ScaleWidth/ScaleHeight);
	}
	return NewRect;
}

RECT WindowHandle::ScaleSizing( const RECT& TagetRect, float ScaleWidth, float ScaleHeight, UINT MinWidth, UINT MinHeight )
{
	RbRect OldRect;
	if( ::GetWindowRect( m_hWnd, &((RECT&)OldRect) )==FALSE )return TagetRect;

	RbRect NewRect = ScalingSize( TagetRect, ScaleWidth, ScaleHeight,
					OldRect.Width() != TagetRect.right-TagetRect.left );
	if( NewRect.Width() < MinWidth )NewRect.right = NewRect.left+MinWidth;
	if( NewRect.Height() < MinHeight )NewRect.bottom = NewRect.top+MinHeight;

	NewRect.SetCenter( OldRect.Center() );

	if( OldRect.top != TagetRect.top )NewRect.Move( 0, OldRect.bottom - NewRect.bottom );
	if( OldRect.bottom != TagetRect.bottom )NewRect.Move( 0, OldRect.top - NewRect.top );
	if( OldRect.left != TagetRect.left )NewRect.Move( OldRect.right - NewRect.right, 0 );
	if( OldRect.right != TagetRect.right )NewRect.Move( OldRect.left - NewRect.left, 0 );

	return NewRect;
}

RECT WindowHandle::MiniClip( const RECT& TagetRect, UINT MinWidth, UINT MinHeight )
{
	RbRect NewRect( TagetRect );
	if( NewRect.Width() < MinWidth )NewRect.right = NewRect.left+MinWidth;
	if( NewRect.Height() < MinHeight )NewRect.bottom = NewRect.top+MinHeight;

	return NewRect;
}
