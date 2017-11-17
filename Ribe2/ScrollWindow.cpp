// ScrollWindow.cpp: ScrollWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "ScrollWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ScrollWindow::ScrollWindow()
{

}

ScrollWindow::~ScrollWindow()
{
}

long ScrollWindow::ScrollPos( long Index ) const
{
	if( Index < 0 || Index >= ScrollList.size() )return 0;
	return ScrollList[Index].NowPos;
}

HRESULT ScrollWindow::Command( DISPDEVICE* pDevice, WindowSystem& System, command com )
{
	switch( com )
	{
	case eReleaseSurface:
		for( int i = 0; i < ScrollList.size(); i++ )
		{
			ScrollList[i].Tex.Release();
		}
		break;
	}
	return BaseWindow::Command( pDevice, System, com );
}

bool ScrollWindow::AllTexRelease()
{
	TexWindow.Release();
	return ScrollTexRelease();
}

bool ScrollWindow::ScrollTexRelease()
{
	for( int i = 0; i < ScrollList.size(); i++ )
	{
		ScrollList[i].Tex.Release();
	}
	return true;
}

HRESULT ScrollWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	HRESULT hr = BaseWindow::Draw( Display, System );
	if( FAILED(hr) )return hr;

	return DrawScrollList( Display, System );
}

HRESULT ScrollWindow::DrawScrollList( RbSprite& Display, WindowSystem& System )
{
	for( int i = 0; i < ScrollList.size(); i++ )
	{
		ScrollInfo& Ref = ScrollList[i];
		if( Ref.Tex == NULL )
		{
			CComPtr<DISPDEVICE> pDevice;
			Display.GetDevice( &pDevice );
			RestoreScroll( pDevice, System, i );
		}
		Ref.Draw( Display, Position.x, Position.y );
	}
	return S_OK;
}

HRESULT ScrollWindow::RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index )
{
	if( Index < 0 || Index >= ScrollList.size() )return E_FAIL;
	ScrollInfo& Ref = ScrollList[Index];
	if( Ref.Tex != NULL )return S_FALSE;
	HRESULT hr = Ref.CreateTexture( pDevice );
	if( FAILED(hr) )return hr;
	return S_OK;
}

ScrollWindow::ScrollInfo::ScrollInfo()
{
	AllZero(*this);
}

bool ScrollWindow::ScrollInfo::SetInfo( const RbRect& Area, int SrcHeight )
{
	NowPos = 0;
	ViewArea = Area;
	Height = SrcHeight;
	ScrollDepth = Height/16;
	Tex.Release();
	if( Height <= 0 )return false;
	return true;
}

bool ScrollWindow::ScrollInfo::CanScroll() const
{
	return (Height > ViewArea.Height());
}

bool ScrollWindow::ScrollInfo::Scroll( int y )
{
	if( !CanScroll() )return false;
	NowPos = Trim( NowPos+y, Height-ViewArea.Height(), 0 );
	return true;
}

bool ScrollWindow::ScrollInfo::Pin( int x, int y ) const
{
	RbRect Rect = ViewArea;
	if( CanScroll() )Rect.right += BarPlane.w;
	return Rect.Pin( x, y );
}

HRESULT ScrollWindow::ScrollInfo::CreateTexture( DISPDEVICE* pDevice )
{
	Tex.Release();
	HRESULT hr = Tex.Create( pDevice, ViewArea.Width(), Height );
	if( FAILED(hr) )return hr;
	return S_OK;
}

HRESULT ScrollWindow::ScrollInfo::SetSprite( DISPDEVICE* pDevice, RbSprite& Surface )
{
	HRESULT hr = CreateTexture( pDevice );
	if( FAILED(hr) )return hr;

	hr = Surface.InitSprite( pDevice, Tex );
	if( FAILED(hr) )return hr;
	Surface.Clear();
	return S_OK;
}

HRESULT ScrollWindow::ScrollInfo::Draw( RbSprite& Display, float x, float y )
{
	if( Tex == NULL )return S_FALSE;
	long RectH = min(Height-NowPos,ViewArea.Height());
	RbRect DrawRect( 0, NowPos, ViewArea.Width(), RectH );

	DrawBar( Display, x, y );
	return Display.Draw( Tex, x+ViewArea.left, y+ViewArea.top, DrawRect );
}

HRESULT ScrollWindow::ScrollInfo::DrawBar( RbSprite& Display, float x, float y )
{
	if( Height <= 0 )return E_FAIL;
	if( !CanScroll() )return S_FALSE;

	const DefaultPlanes& SysMsg = GetCache().SystemPlane;
	if( TopPlane.pTexture == NULL )TopPlane = SysMsg.GetPlane(_T("scr_top"));
	if( BarPlane.pTexture == NULL )BarPlane = SysMsg.GetPlane(_T("scr_bar"));
	if( BottomPlane.pTexture == NULL )BottomPlane = SysMsg.GetPlane(_T("scr_bottom"));

	float Scale = ((float)ViewArea.Height())/Height;
	float h = Scale * ViewArea.Height();
	float bary = Scale * NowPos;
	float barh = h - TopPlane.h - BottomPlane.h;
	x = ViewArea.right+x;
	y = ViewArea.top+y;
	if( barh < 0 )barh = 0;
	if( barh > 0 )
	{
		RbRect BarRect( x, y+bary+TopPlane.h,
			BarPlane.w, barh );
		Display.Draw( BarPlane, RbRect(BarPlane), BarRect );
	}
	Display.DrawPlane( TopPlane, x, y+bary );
	Display.DrawPlane( BottomPlane, x, y+bary+h-BottomPlane.h );

	return S_OK;
}

bool ScrollWindow::ScrollInfo::MouseJob( WindowSystem& System, GameMessage& State, int x, int y )
{
	x -= ViewArea.left;
	y -= ViewArea.top;
	switch( State.Msg() )
	{
	case eScrollUp:
		Scroll( -ScrollDepth );
		return true;
	case eScrollDown:
		Scroll( ScrollDepth );
		return true;
	case eMouseMove:
		if( State.IsDraging() )break;
		if( !State.LDown )break;
	case eLDown:
		if( Height > 0 && CanScroll() )
		{
			int Width = ViewArea.Width();
			if( x >= Width && x <= Width+BarPlane.w )
			{
				int VHeight = ViewArea.Height();
				float Scale = Height/(float)VHeight;
				float Pos = y*Scale;
				Pos -= VHeight/2;
				return Scroll( Pos-NowPos );
			}
		}
	}
	return false;
}

bool ScrollWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	int x = State.x-Position.x;
	int y = State.y-Position.y;
	for( int i = 0; i < ScrollList.size(); i++ )
	{
		ScrollInfo& Ref = ScrollList[i];
		if( !Ref.Pin( x, y ) )continue;
		if( ScrollList[i].MouseJob( System, State, x, y ) )return true;
	}
	return BaseWindow::MouseJob( System, State );
}

ScrollWindow::ScrollInfo::PosStock::PosStock( ScrollInfo& Base )
:Ref( Base )
{
	OldPos = Ref.NowPos;
}

ScrollWindow::ScrollInfo::PosStock::~PosStock()
{
	Ref.Scroll( OldPos-Ref.NowPos );
}