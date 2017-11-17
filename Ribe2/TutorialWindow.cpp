// TutorialWindow.cpp: TutorialWindow ƒNƒ‰ƒX‚ÌƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbWindowSystem.h"
#include "RbSprite.h"
#include "TutorialWindow.h"

//////////////////////////////////////////////////////////////////////
// \’z/Á–Å
//////////////////////////////////////////////////////////////////////

RbPointF TutorialWindow::DefaultMsgPos = RbPointF(0.5f,0.5f);

bool TutorialWindow::SetMsgBaseInfo( const WindowSystem& System )
{
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes( _T("Tutorial") );
	if( pPlanes == NULL )return false;
	DefaultMsgPos.x = pPlanes->Base.Plane.x / 100.0f;
	DefaultMsgPos.y = pPlanes->Base.Plane.y / 100.0f;
	return true;
}

TutorialWindow::TutorialWindow()
{
	FloatPos.x = DefaultMsgPos.x;
	FloatPos.y = DefaultMsgPos.y;
}

TutorialWindow::~TutorialWindow()
{
}

LPCTSTR TutorialWindow::WindowName() const
{
	return _T("Tutorial");
}

bool TutorialWindow::SetWnd( BaseWindow* pNewWnd )
{
	TagetWnd.pWnd = pNewWnd;
	TagetWnd.Rect = RbRect();
	return true;
}

bool TutorialWindow::SetWndRect( WindowSystem& System, LPCTSTR RectName )
{
	if( TagetWnd.pWnd == NULL )return true;

	TagetWnd.Rect = TagetWnd.pWnd->GetRect();
	TagetWnd.Rect.Move( -TagetWnd.Rect.left, -TagetWnd.Rect.top );
	if( RectName == NULL )return true;

	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes( TagetWnd.pWnd->WindowName() );
	if( pPlanes == NULL )return true;

	const RbRect& TagetRect = GetTagetRect( pPlanes, RectName );
	if( !TagetRect.IsNone() )TagetWnd.Rect = TagetRect;

	return true;
}

RbRect TutorialWindow::GetTagetRect( const WindowPlanes* pPlanes, LPCTSTR RectName ) const
{
	if( pPlanes == NULL )return RbRect();
	if( RectName == NULL )return RbRect();
	StringSplitter<> Split;
	Split.SetString( RectName );

	RbRect RetRect = pPlanes->GetRect(Split[0]);
	for( int i = 1; i < Split.Count(); i++ )
	{
		const RbPoint LT = RetRect.LT();
		RetRect = RbRect(pPlanes->GetRect(Split[i]));
		RetRect.Move( LT.x, LT.y );
	}
	return RetRect;
}

bool TutorialWindow::SetXML( const XMLPaser& MsgXML )
{
	if( !SelectMsgWindow::SetXML(MsgXML) )return false;

	String RectName = MsgXML.AttributeString(_T("taget"));
	return SetWndRect( GetWindowSystem(), RectName );
}

bool TutorialWindow::_TutoriaTex::Set( LPCTSTR Name )
{
	GetCache().GetTexture( Name, pTexture );
	if( pTexture == NULL )return false;

	Size = pTexture->GetSize();
	Height = pTexture->GetHight();
	Img.pAct = &(pTexture->GetAction(_T("image"), 0, 0) );
//	Img.Bound = Img.pAct->BoundRect();
	LPCTSTR ImageName = _T("tutorial");
	pImg = &(pTexture->GetAction( ImageName, 1.0f, 0 ) );
	pRight = &(pTexture->GetAction( ImageName, 0.5f, 0 ) );
	pLeft = &(pTexture->GetAction( ImageName, 1.5f, 0 ) );

	return true;
}

// •`ŽÊ
HRESULT TutorialWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( !RestoreCheck(Display,System) )return S_FALSE;

	if( TutoriaTex.pTexture == NULL )TutoriaTex.Set( _T("tutoria") );
	if( FukiL.pTexture == NULL )FukiL = System.MsgPlane().GetPlane(_T("fuki_l"));

	if( TagetWnd.pWnd == NULL )return DrawNoWindow( Display ,System );

	RbRect WndRect = TagetWnd.pWnd->GetRect();
	RbRect TagetRect = TagetWnd.Rect;
	TagetRect.Move( WndRect.left, WndRect.top );

	if( TagetRect.left - TutoriaTex.Size < 0 )
		return DrawR( Display, System, TagetRect );

	return DrawL( Display, System, TagetRect );
}

// •`ŽÊ
HRESULT TutorialWindow::DrawL( RbSprite& Display, WindowSystem& System, const RbRect& TagetRect )
{
	int TexX = TagetRect.left - TutoriaTex.Size/2;
	int TexY = TagetRect.Center().y + TutoriaTex.Height/2;

	bool Reverse = false;
	Position.x = TexX - TutoriaTex.Size/2;
	Position.y = TexY - Position.Height - TutoriaTex.Height;
	if( Position.y < 0 )
	{
		Reverse = true;
		Position.y += Position.Height + TutoriaTex.Height;
	}

	ClipPosition( Display );

	if( TutoriaTex.pRight != NULL )
		Display.DrawRbTexture( *TutoriaTex.pRight, TexX, TexY, 0 );

	HRESULT hr = BaseWindow::Draw( Display ,System );
	if( FAILED(hr) )return hr;

	return DrawFuki( Display, System, TexX, TexY, Reverse );
}

// •`ŽÊ
HRESULT TutorialWindow::DrawR( RbSprite& Display, WindowSystem& System, const RbRect& TagetRect )
{
	int TexX = TagetRect.right + TutoriaTex.Size/2;
	int TexY = TagetRect.Center().y + TutoriaTex.Height/2;

	bool Reverse = false;
	Position.x = TexX - Position.Width + TutoriaTex.Size/2;
	Position.y = TexY - Position.Height - TutoriaTex.Height;
	if( Position.y < 0 )
	{
		Reverse = true;
		Position.y += Position.Height + TutoriaTex.Height;
	}

	ClipPosition( Display );

	if( TutoriaTex.pRight != NULL )
		Display.DrawRbTexture( *TutoriaTex.pLeft, TexX, TexY, 0 );

	HRESULT hr = BaseWindow::Draw( Display ,System );
	if( FAILED(hr) )return hr;

	return DrawFuki( Display, System, TexX, TexY, Reverse );
}

HRESULT TutorialWindow::DrawFuki( RbSprite& Display, WindowSystem& System, int x, int y, bool Rev )
{
	if( Fukidashi.pTexture == NULL )Fukidashi = System.MsgPlane().GetPlane(_T("fuki"));
	if( FukiT.pTexture == NULL )FukiT = System.MsgPlane().GetPlane(_T("fuki_t"));

	Plane<> DPlane;
	if( Rev )
	{
		DPlane = FukiT;
		y -= FukiT.h-1;
	}
	else
	{
		DPlane = Fukidashi;
		y -= TutoriaTex.Height+1;
	}

	return Display.DrawPlane( DPlane, x, y);
}

HRESULT TutorialWindow::DrawNoWindow( RbSprite& Display, WindowSystem& System )
{
	HRESULT hr = SelectMsgWindow::Draw( Display ,System );
	if( FAILED(hr) )return hr;

	int x = Position.x - FukiL.w +1;
	int y = Position.y+Position.Height / 2;

	Display.DrawRbTexture( *TutoriaTex.pImg,
		x-TutoriaTex.Size/2, y+TutoriaTex.Height/2, 0 );
	return Display.DrawPlane( FukiL, x, y);
}

bool TutorialWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	switch( State.Msg() )
	{
	case eRDblClick:
		Command( NULL, System, eDestroy );
		return true;
	}

	return SelectMsgWindow::MouseJob( System, State );
}
