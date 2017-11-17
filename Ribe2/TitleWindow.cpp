// TitleWindow.cpp: TitleWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "GameSystem.h"
#include "RbSprite.h"
#include "TitleWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LPCTSTR TitleWindow::TitleFont = _T("title");

TitleWindow::TitleWindow()
{
	bDestroyOK =  bMoveOK = false;
	bMsged = false;
	FloatPos.x = FloatPos.y = 0.5;
	FloatPos.bLock = true;
	TopLevel = 5;

	StartTime = RbTimer();
	Life = 3000;
	ShowType = eShowLabel;
	Color.color = MYWHITE;
}

TitleWindow::~TitleWindow()
{
}

LPCTSTR TitleWindow::WindowName() const
{
	return _T("TitleWindow");
}

bool TitleWindow::HitTest( int x, int y ) const
{
	if( !bMsged )return false;
	return BaseWindow::HitTest( x, y );
}

bool TitleWindow::SetXML( const XMLPaser& MsgXML )
{
	TexString = MsgXML.AttributeString(_T("tex"));
	Font = MsgXML.AttributeString(_T("font"));

	TitleMsg = MsgXML.AttributeString(TAG::MSG);
	UserOrStr(TitleMsg);
	if( TitleMsg.empty() )TitleMsg = MsgXML.TextString();
	if( TitleMsg.empty() && TexString.empty() )return false;
	String Type = MsgXML.AttributeString(_T("show"));
	UserOrStr(Type);

	if( Type==_T("slit") )ShowType = eShowSlit;
	else if( Type==_T("curtain") )ShowType = eShowCurtain;
	else if( Type==_T("label") )ShowType = eShowLabel;
	else if( Type==_T("slash") )ShowType = eShowSideSlash;
	else if( Type==_T("old") )ShowType = eShowSideSlash;
	else if( Type==_T("fade") )ShowType = eShowFade;
	else if( Type==_T("cutin") )ShowType = eShowCutin;
	else if( Type==_T("rotate") )ShowType = eShowRotate;
	else if( Type==_T("mist") )ShowType = eShowMist;
	else if( Type==_T("fall") )ShowType = eShowFall;
	else if( Type==_T("none") )ShowType = eShowNone;
	else if( Type==_T("random") )ShowType = (_ShowType)Rnd(eShowMax-1);

	if( MsgXML.HasAttribute(_T("col")) )
		Color.color = MsgXML.AttributeHex(_T("col"));

	float NewLife = MsgXML.AttributeFloat(_T("life"));
	if( NewLife != 0 )Life = NewLife;
//	if( Life <= 0 )bMsged = true;

	if( bMsged )BattleLocker.Lock( GetBattleSystem().BattleLock );
	return true;
}

HRESULT TitleWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	RbTimer NewTime;
	double Passed = NewTime.Passed( StartTime );
	if( Life > 0 )if( Passed > Life )
	{
		Command( NULL, System, eDestroy );
		g_pSystem->DummyMsg();
		return S_FALSE;
	}

	if( !RestoreCheck(Display,System) )return S_FALSE;

	switch( ShowType )
	{
	case eShowSideSlash:
		return DrawSideSlash( Passed, Display, System );
	case eShowCurtain:
		return DrawCurtain( Passed, Display, System );
	case eShowLabel:
		return DrawLabel( Passed, Display, System );
	case eShowSlit:
		return DrawSlit( Passed, Display, System );
	case eShowFade:
		return DrawFade( Passed, Display, System );
	case eShowCutin:
		return DrawCutin( Passed, Display, System );
	case eShowRotate:
		return DrawRotate( Passed, Display, System );
	case eShowMist:
		return DrawMist( Passed, Display, System );
	case eShowFall:
		return DrawFall( Passed, Display, System );
	}
	return DrawDefault( Display, System );
}

bool TitleWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	switch( State.Msg() )
	{
	case eLDown: case eLDblClick: case eRDblClick:
		if( bMsged )Command( NULL, System, eDestroy );
		return true;
	}

	return BaseWindow::MouseJob( System, State );
}

// レストア
HRESULT TitleWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;
	if( TitleMsg.empty() && TexString.empty() )return S_FALSE;
	if( Font.empty() )Font = TitleFont;

	RbRect MsgRect = System.Font().GetTextRect( Font, TitleMsg );
	RbRect AllRect = MsgRect;
	Plane<> TexPlane;
	if( TexString.empty() )AllZero( TexPlane );
	else TexPlane = GetCache().PathTex.Get( TexString );
	RbRect TexRect = TexPlane;
	AllRect.OR( TexRect );
	const RbPoint& Center = AllRect.Center();
	MsgRect.SetCenter( Center.x, Center.y );
	TexRect.SetCenter( Center.x, Center.y );

	Position.Width = AllRect.Width();
	Position.Height = AllRect.Height();

	HRESULT ret = TexWindow.Create( pDevice, Position.Width, Position.Height );
	if( FAILED(ret) )return ret;

	ExSprite Surface;
	ret = Surface.InitSprite( pDevice, TexWindow );
	if( FAILED(ret) )return ret;

	Surface.Clear();

	Surface.Draw( TexPlane, TexRect );
	System.Font().Draw( Font, TitleMsg, MsgRect, DT_CENTER, Color );

	return S_OK;
}

// なし
HRESULT TitleWindow::DrawDefault( RbSprite& Display, WindowSystem& System )
{
	return BaseWindow::Draw( Display, System );
}

// りべいす風
HRESULT TitleWindow::DrawSideSlash( double Passed, RbSprite& Display, WindowSystem& System )
{
	if( Passed >= 1000 )return DrawDefault( Display, System );

	RbRect Rect( Position.Width, 1 );
	float Scale = 2.0f * ((1000-Passed) / 1000.0f);
	for( int i = 0; i < Position.Height; i++ )
	{
		float x = i*Scale;
		if( i&1 )x = -x;

		Display.Draw( TexWindow, Position.x + x, Position.y+i, Rect );
		Rect.Move( 0, 1 );
	}
	return S_OK;
}

// カーテン風
HRESULT TitleWindow::DrawCurtain( double Passed, RbSprite& Display, WindowSystem& System )
{
	if( Passed >= 1000 )return DrawDefault( Display, System );

	float Turn = (Passed/1000.0f);
	for( int i = 0; i < Position.Width; i++ )
	{
		float y = D3DX_PI * (i%200)/100.0f;
		y = ((::sinf(y)+1)/2)+1;
		y = y*Turn*Position.Height;
		if( y > Position.Height )y = Position.Height;
		RbRect Rect( i, 0, 1, y );
		Display.Draw( TexWindow, Position.x+i, Position.y, Rect );
	}
	return S_OK;
}

// ラベル風
HRESULT TitleWindow::DrawLabel( double Passed, RbSprite& Display, WindowSystem& System )
{
	if( Passed >= 1000 )return DrawDefault( Display, System );

	float Turn = (Passed/1000.0f);
	RbRect Rect( Position.Width*Turn, Position.Height );
	Display.Draw( TexWindow, Position.x, Position.y, Rect );

	int Left = (int)(Turn*Position.Width);
	Rect = RbRect( Left, 0, 1, Position.Height );
	for( int i = 0; i < Position.Width-Left; i++ )
	{
		float x = Left-i;

		Display.Draw( TexWindow, Position.x + x, Position.y+i, Rect );
		Rect.Move( 1, 0 );
	}
	return S_OK;
}

// スリット風
HRESULT TitleWindow::DrawSlit( double Passed, RbSprite& Display, WindowSystem& System )
{
	if( Passed >= 1000 )return DrawDefault( Display, System );

	int Turn = (int)(Passed/100.0f);
	RbRect Rect( Position.Width*Turn/10, Position.Height );
	Display.Draw( TexWindow, Position.x, Position.y, Rect );

	int Left = (int)(Turn*Position.Width);
	Rect = RbRect( Rect.right, 0, Position.Width/10, Position.Height );
	for( int i = 0; i < 2; i++ )
	{
		if( i > 10 )break;
		float y = (100+(Turn*100)-(Passed))/2;
		if( i&1 )y = -y;

		Display.Draw( TexWindow, Position.x + Rect.left, Position.y+y, Rect );
		Rect.Move( Position.Width/10, 0 );
	}
	return S_OK;
}

// フェード風
HRESULT TitleWindow::DrawFade( double Passed, RbSprite& Display, WindowSystem& System )
{
	if( Passed >= 1000 )return DrawDefault( Display, System );

	float Turn = (Passed/1000.0f);
	ARGBF Color;
	Color.a = Turn;

	Color = Display.SetDrawColor( Color );
	HRESULT hr =  Display.Draw( TexWindow, Position.x, Position.y );
	Display.SetDrawColor( Color );
	return hr;
}

// カットイン風
HRESULT TitleWindow::DrawCutin( double Passed, RbSprite& Display, WindowSystem& System )
{
	if( Passed >= 1000 )return DrawDefault( Display, System );

	float Turn = (Passed/1000.0f);
	ARGBF Color;
	Color.a = Turn;

	float x = (1-Turn)*Position.Width;
	Color = Display.SetDrawColor( Color );
	HRESULT hr =  Display.Draw( TexWindow, Position.x+x, Position.y );
	Display.SetDrawColor( Color );
	return hr;
}

// 回転風
HRESULT TitleWindow::DrawRotate( double Passed, RbSprite& Display, WindowSystem& System )
{
	if( Passed >= 1000 )return DrawDefault( Display, System );

	float Turn = (Passed/1000.0f);

	RbTransform Trans;
	Trans.AddScroll( -Position.Width/2, -Position.Height/2 );
	Trans.RotateZ( Turn*4 );
	Trans.Scale( 2-Turn );
	Trans.AddScroll( Position.x+Position.Width/2, Position.y+Position.Height/2 );
	return Display.Draw( TexWindow, Trans );
}

// 霧風
HRESULT TitleWindow::DrawMist( double Passed, RbSprite& Display, WindowSystem& System )
{
	if( Passed >= 1000 )return DrawDefault( Display, System );

	float Turn = (Passed/1000.0f);
	ARGBF Color;
	Color.a = Turn;
	Color = Display.SetDrawColor( Color );

	RbRect Rect( Position.Width, 1 );
	for( int i = 0; i < Position.Height; i++ )
	{
		float x = (i+(int)(Turn*100))%100;
		x = ::sinf( D3DX_PI * x/50.0f );
		x = x*(1-Turn)*Position.Width;
		Display.Draw( TexWindow, Position.x+x, Position.y+i, Rect );
		Rect.Move( 0, 1 );
	}
	Display.SetDrawColor( Color );
	return S_OK;
}

// 落下風
HRESULT TitleWindow::DrawFall( double Passed, RbSprite& Display, WindowSystem& System )
{
	if( Passed >= 1000 )return DrawDefault( Display, System );

	float Turn = (Passed/1000.0f);
	ARGBF Color;
	Color.a = Turn;

	float y = (Turn-1)*Position.Height*2.0f;
	Color = Display.SetDrawColor( Color );
	HRESULT hr =  Display.Draw( TexWindow, Position.x, Position.y+y );
	Display.SetDrawColor( Color );
	return hr;
}
