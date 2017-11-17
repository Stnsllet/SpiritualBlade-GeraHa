// BaseWindow.cpp: BaseWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WindowSystem.h"
#include "BaseWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LPCTSTR BaseWindow::WindowTagFont = _T("wnd_tag");

BaseWindow::BaseWindow()
{
	AllZero( Position );
	AllZero( FloatPos );
	Position.Width = Position.Height = 0;
	bDestroyOK =  bMoveOK = true;
	bCursorSound = true;
	bHide = false;
	bOutView = false;
	TopLevel = 0;

	NEW_CHECKP(WindowName(),0,this);
}

BaseWindow::~BaseWindow()
{

}

LPCTSTR BaseWindow::WindowName() const
{
	return _T("BaseWindow");
}

bool BaseWindow::HitTest( int x, int y ) const
{
	if( bHide )return false;
	if( x < Position.x )return false;
	if( y < Position.y )return false;
	if( x >= Position.x+Position.Width )return false;
	if( y >= Position.y+Position.Height )return false;
	return true;
}

bool BaseWindow::SetDefaultPosition( WindowSystem& System )
{
	const WindowPlanes* pDefine = System.Define().GetWindowPlanes( WindowName() );
	if( pDefine == NULL )return false;
	SetFloatPosition( System, pDefine->Base.Plane.x/100.0f, pDefine->Base.Plane.y/100.0f );
	return true;
}

RbPointF BaseWindow::GetFloatPosition( WindowSystem& System ) const
{
	RbPoint Size = System.GetWindowSize();
	Size.x -= Position.Width;
	Size.y -= Position.Height;
	RbPointF Ret( 0,0 );
	if( Size.x > 0 )Ret.x = (Position.x-FloatPos.sx) / (float)Size.x;
	if( Size.y > 0 )Ret.y = (Position.y-FloatPos.sy) / (float)Size.y;

	return Ret;
}

void BaseWindow::SetFloatPosition( WindowSystem& System, float x, float y )
{
	RbPoint Size = System.GetWindowSize();
	if( Position.Width <= 0 && Position.Height <= 0 )
	{
		const WindowPlanes* pDefine = System.Define().GetWindowPlanes( WindowName() );
		if( pDefine == NULL )return;
		Position.Width = pDefine->Base.Plane.w;
		Position.Height = pDefine->Base.Plane.h;
	}
	SetFloatPosition( Size.x, Size.y, x, y );
}

void BaseWindow::SetFloatPosition( int Width, int Height, float x, float y )
{
	x = Trim( x, 1.0f, 0.0f );
	y = Trim( y, 1.0f, 0.0f );
	FloatPos.x = x;
	FloatPos.y = y;
	Width -= Position.Width;
	Height -= Position.Height;
	Position.x = (Width * x) + FloatPos.sx;
	Position.y = (Height * y) + FloatPos.sy;
}

bool BaseWindow::ClipPosition( WindowSystem& System )
{
	RbPoint Size = System.GetWindowSize();
	return ClipPosition( Size.x, Size.y );
}

bool BaseWindow::ClipPosition( RbSprite& Display )
{
	return ClipPosition( Display.GetSize().x, Display.GetSize().y );
}

bool BaseWindow::ClipPosition( int Width, int Height )
{
	bool bx = true, by = true;
	if( Position.x < 0 )Position.x = 0;
	else if( Position.x + Position.Width > Width )Position.x = Width - Position.Width;
	else bx = false;
	if( Position.y < 0 )Position.y = 0;
	else if( Position.y + Position.Height > Height )Position.y = Height - Position.Height;
	else by = false;
	return (bx || by);
}

bool BaseWindow::RestoreCheck( RbSprite& Display, WindowSystem& System )
{
	if( bHide )return false;
	if( TexWindow == NULL )
	{
		HRESULT ret = SpriteRestore( Display, System );
		if( FAILED(ret) )return false;
	}
	if( FloatPos.bLock )
		SetFloatPosition( Display.GetSize().x, Display.GetSize().y, FloatPos.x, FloatPos.y );

	return true;
}

HRESULT BaseWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( !RestoreCheck(Display,System) )return S_FALSE;
	if( !TexWindow || bHide )return S_FALSE;
	return Display.Draw( TexWindow, Position.x, Position.y );
}

HRESULT BaseWindow::NormalCursor( RbSprite& Display, WindowSystem& System, int x, int y )
{
	RbRect CursorRect = HitCursor( System, x, y );
	if( CursorRect.IsNone() )return S_FALSE;

	System.ChangeDrawCursor( this, CursorRect, bCursorSound );
	CursorRect.Move( Position.x, Position.y );
	return Display.DrawCursor( CursorRect );
}

RbRect BaseWindow::HitCursor( WindowSystem& System, int x, int y )
{
	return RbRect();
}

HRESULT BaseWindow::DrawCursor( RbSprite& Display, WindowSystem& System, const GameMessage& State )
{
	if( bHide )return S_FALSE;
//	if( State.IsDraging() )return S_FALSE;
	return NormalCursor( Display, System, State.x-Position.x, State.y-Position.y );
}

bool BaseWindow::Close( WindowSystem& System )
{
	if( bDestroyOK )
	{
		Command( NULL, System, eDestroy );
		return true;
	}
	if( bOutView )
	{
		if( FloatPos.sx == 0 )FloatPos.sx = Position.Width * 3/4;
		else FloatPos.sx = 0;
		if( FloatPos.x < .5f )FloatPos.sx = -FloatPos.sx;
		SetFloatPosition( System, FloatPos.x, FloatPos.y );
		return false;
	}
	return false;
}

HRESULT BaseWindow::Command( DISPDEVICE* pDevice, WindowSystem& System, command com )
{
	switch( com )
	{
	case eCreate:
		return CreateBase( pDevice, System, Position.Width , Position.Height );
	case eRestore:
		TexWindow.Release();
		return Restore( pDevice, System );
	case eReleaseSurface:
		TexWindow.Release();
		return true;
	case eDestroy:
		System.DestroyWindow( this );
		return true;
	}
	return S_OK;
}

bool BaseWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	if( bHide )return false;

	switch( State.Msg() )
	{
	case eLDown: case eRDown: case eLRDown: case eLDblClick: case eRDblClick:
	case eScrollUp: case eScrollDown:
		System.MoveTop( this );
	}

	switch( State.Msg() )
	{
	case eLDown: case eLDblClick:
		return ClickJob( State.x-Position.x, State.y-Position.y );
	case eRDown:
		if( State.IsDraging() )break;
		if( DragStartJob( System, State ) )return true;
		if( !bMoveOK )break;
		System.SetJobWindow( this );
		State.DragStart();
		DragPoint.x = State.x - Position.x;
		DragPoint.y = State.y - Position.y;
		return true;
	case eRDblClick:
		Close( System );
		return true;
	case eMouseMove:
		if( !State.IsDraging() )return Floating( State.x-Position.x, State.y-Position.y );
		if( !bMoveOK )break;
		{
			Position.x = State.x - DragPoint.x;
			Position.y = State.y - DragPoint.y;
			ClipPosition( System );
		}
		return true;
	case eDrop:
		State.DragEnd();
		return true;
	}
	System.SetJobWindow( NULL );
	return true;
}

// 何もないウインドウを作る
HRESULT BaseWindow::CreateBase( DISPDEVICE* pDevice, WindowSystem& System, int Width, int Height )
{
	Position.Width = Width;
	Position.Height = Height;

	TexWindow.Release();

	return Restore( pDevice, System );
}

// スプライトからレストア
HRESULT BaseWindow::SpriteRestore( RbSprite& Display, WindowSystem& System )
{
	CComPtr<DISPDEVICE> pDevice;
	Display.GetDevice( &pDevice );
	return Restore( pDevice, System );
}

// レストア
HRESULT BaseWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;
	ExSprite Surface;
	return RestoreBase( pDevice, Surface ,System );
}

HRESULT BaseWindow::RestoreBase( DISPDEVICE* pDevice, RbSprite& Surface, WindowSystem& System )
{
	if( TexWindow == NULL )
	{
		HRESULT ret = TexWindow.Create( pDevice, Position.Width, Position.Height );
		if( FAILED(ret) )return ret;
	}

	HRESULT ret = Surface.InitSprite( pDevice, TexWindow );
	if( FAILED(ret) )return ret;

	Surface.Clear();
	ret = DrawSurfaceBase( Surface, System, RbRect(0,0,Position.Width,Position.Height) );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// ウインドウ名を書く
HRESULT BaseWindow::DrawWindowTag( WindowSystem& System, LPCTSTR Name, int x, int y ) const
{
	return System.Font().Draw( WindowTagFont, Name, x, y );
}

// 何もないウインドウを描く
HRESULT BaseWindow::DrawSurfaceBase( RbSprite& Surface, WindowSystem& System, const RbRect& Rect )
{
	if( Rect.Width() <=0 || Rect.Height() <= 0 )return S_FALSE;
	const WindowTexture& Base = System.Texture();

	RbRect SrcRect, DestRect;

	SrcRect = Base.GetRECT(_T("lt"));
	Surface.Draw( Base, Rect.left, Rect.top, SrcRect );
	CenterRect.left = Rect.left + SrcRect.Width();
	CenterRect.top = Rect.top + SrcRect.Height();

	SrcRect = Base.GetRECT(_T("rb"));
	CenterRect.right = Rect.right-SrcRect.Width();
	CenterRect.bottom = Rect.bottom-SrcRect.Height();

	Surface.Draw( Base, CenterRect.right, CenterRect.bottom, SrcRect );

	SrcRect = Base.GetRECT(_T("rt"));
	Surface.Draw( Base, CenterRect.right, Rect.top, SrcRect );
	SrcRect = Base.GetRECT(_T("lb"));
	Surface.Draw( Base, Rect.left, CenterRect.bottom, SrcRect );

	SrcRect = Base.GetRECT(_T("l"));
	DestRect = CenterRect;
	DestRect.left = Rect.left;
	DestRect.right = Rect.left+SrcRect.Width();
	Surface.Draw( Base, SrcRect, DestRect );
	SrcRect = Base.GetRECT(_T("r"));
	DestRect = CenterRect;
	DestRect.left = CenterRect.right;
	DestRect.right = DestRect.left+SrcRect.Width();
	Surface.Draw( Base, SrcRect, DestRect );
	SrcRect = Base.GetRECT(_T("t"));
	DestRect = CenterRect;
	DestRect.top = Rect.top;
	DestRect.bottom = Rect.top + SrcRect.Height();
	Surface.Draw( Base, SrcRect, DestRect );
	SrcRect = Base.GetRECT(_T("b"));
	DestRect = CenterRect;
	DestRect.top = CenterRect.bottom;
	DestRect.bottom = DestRect.top+SrcRect.Height();
	Surface.Draw( Base, SrcRect, DestRect );

	SrcRect = Base.GetRECT(_T("c"));
	Surface.Draw( Base, SrcRect, CenterRect );

	return S_OK;;
}
