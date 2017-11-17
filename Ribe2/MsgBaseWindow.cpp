// MsgBaseWindow.cpp: MsgBaseWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbWindowSystem.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "GameSystem.h"
#include "MsgBaseWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LPCTSTR MsgBaseWindow::MsgFont = _T("msg");
RbPointF MsgBaseWindow::DefaultMsgPos = RbPointF(0.5f,0.5f);

bool MsgBaseWindow::SetMsgBaseInfo( const WindowSystem& System )
{
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes( _T("MsgBase") );
	if( pPlanes == NULL )return false;
	DefaultMsgPos.x = pPlanes->Base.Plane.x / 100.0f;
	DefaultMsgPos.y = pPlanes->Base.Plane.y / 100.0f;
	return true;
}

MsgBaseWindow::MsgBaseWindow()
{
	bMoveOK = bDestroyOK = false;
	TopLevel = 3;
	bTextRead = true;
	FloatPos.x = DefaultMsgPos.x;
	FloatPos.y = DefaultMsgPos.y;
}

MsgBaseWindow::~MsgBaseWindow()
{
}

LPCTSTR MsgBaseWindow::WindowName() const
{
	return _T("MsgBase");
}

RbRect MsgBaseWindow::GetMsgRect( LPCTSTR Message )
{
	if( Font.empty() )return GetWindowSystem().Font().GetTextRect(MsgFont, Message );

	return GetWindowSystem().Font().GetTextRect(Font, Message );
}

Plane<> MsgBaseWindow::GetFace() const
{
	return GetCache().Face.Get( FaceStr );
}

RbRect MsgBaseWindow::GetFaceRect( const TexRect& Face ) const
{
	const RbPoint& Chip = ChipSize();
	RbRect FaceRect(Chip.x, Chip.y, Face.w, Face.h);
	return FaceRect;
}

bool MsgBaseWindow::IsScrollLock() const
{
	return (eBattleMode == g_pSystem->NowMode() );
}

// ベースの作成
HRESULT MsgBaseWindow::RestoreMsgBase( DISPDEVICE* pDevice, RbSprite& Surface, WindowSystem& System )
{
	if( TexWindow == NULL )
	{
		HRESULT ret = TexWindow.Create( pDevice, Position.Width, Position.Height );
		if( FAILED(ret) )return ret;
	}

	HRESULT ret = Surface.InitSprite( pDevice, TexWindow );
	if( FAILED(ret) )return ret;

	RbRect MsgArea( 0,0, Position.Width, Position.Height );
	ret = DrawMsgBase( Surface, MsgArea, System );
	if( FAILED(ret) )return ret;
	return S_OK;
}

// ベースの描写
HRESULT MsgBaseWindow::DrawMsgBase( RbSprite& Surface, const RbRect& MsgArea, WindowSystem& System )
{
	Plane<> lt = System.MsgPlane().GetPlane(_T("lt"));
	Plane<> rt = System.MsgPlane().GetPlane(_T("rt"));
	Plane<> lb = System.MsgPlane().GetPlane(_T("lb"));
	Plane<> rb = System.MsgPlane().GetPlane(_T("rb"));
	Plane<> t  = System.MsgPlane().GetPlane(_T("t"));
	Plane<> b  = System.MsgPlane().GetPlane(_T("b"));
	Plane<> l  = System.MsgPlane().GetPlane(_T("l"));
	Plane<> r  = System.MsgPlane().GetPlane(_T("r"));
	Plane<> c  = System.MsgPlane().GetPlane(_T("c"));
	Fukidashi = System.MsgPlane().GetPlane(_T("fuki"));

	Surface.Clear();

	RbRect Center( lt.w, lt.h,
		MsgArea.Width()-rb.w-lt.w, MsgArea.Height()-rb.h-lt.h );

	Surface.Draw( lt, MsgArea.left, MsgArea.top, RbRect(lt) );
	Surface.Draw( rt, Center.right, MsgArea.top, RbRect(rt) );
	Surface.Draw( lb, MsgArea.left, Center.bottom, RbRect(lb) );
	Surface.Draw( rb, Center.right, Center.bottom, RbRect(rb) );
	Surface.Draw( c, RbRect(c), Center );

	Surface.Draw( t, RbRect(t),
		RbRect(Center.left, MsgArea.top, Center.Width(), t.h ) );
	Surface.Draw( b, RbRect(b),
		RbRect(Center.left, Center.bottom, Center.Width(), b.h ) );
//	Surface.Draw( l, RbRect(l),
//		RbRect(MsgArea.left, Center.top, l.w, l.h ) );
//	Surface.Draw( r, RbRect(r),
//		RbRect(Center.right, Center.top, r.w, r.h ) );
	Surface.Draw( l, RbRect(l),
		RbRect(MsgArea.left, Center.top, l.w, Center.Height() ) );
	Surface.Draw( r, RbRect(r),
		RbRect(Center.right, Center.top, r.w, Center.Height() ) );
	return S_OK;
}

// レストア
HRESULT MsgBaseWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;
	if( Font.empty() )Font = MsgFont;

	const RbPoint& Chip = ChipSize();

	RbRect MsgRect = BaseMsgRect;
	RbRect Bound = MsgRect;

	const Plane<>& Face = GetFace();
	if( Face.pTexture != NULL )
	{
		RbRect FaceRect = GetFaceRect(Face);
		FaceRect.ReSize( FaceRect.Width()+2, FaceRect.Height() );
		Bound.OR( FaceRect );

		MsgRect.Move( Face.w+2, 0 );
		if( Face.h > Position.Height )
		{
			Position.Height = Face.h;
		}
		Bound.OR( MsgRect );
		int Center = Bound.Center().y;
		MsgRect.SetCenterY( Center );
		FaceRect.SetCenterY( Center );
	}

	Position.Width = Bound.Width();
	Position.Height = Bound.Height();

	Position.Width += Chip.x*2;
	Position.Height += Chip.y*2;

	ExSprite Surfase;
	HRESULT ret = RestoreMsgBase( pDevice, Surfase, System );
	if( FAILED(ret) )return ret;

	DrawFace( Surfase );
	DMsg( System, Message, MsgRect );

	return S_OK;
}

bool MsgBaseWindow::SetFont( LPCTSTR FontName )
{
	Font = FontName;
	return true;
}

bool MsgBaseWindow::SetMessage( LPCTSTR NewMessage )
{
	if( bTextRead && NewMessage == NULL )return false;
	Message = NewMessage;

	const RbPoint& Chip = ChipSize();
	BaseMsgRect = GetMsgRect( Message );
	BaseMsgRect.Move( Chip.x, Chip.y );

	return true;
}

bool MsgBaseWindow::SetXML( const XMLPaser& MsgXML )
{
	SetFont( String(MsgXML.AttributeString(_T("font"))) );

	String Msg = MsgXML.AttributeString(TAG::MSG);
	UserOrStr(Msg);
	if( bTextRead )if( Msg.empty() )Msg = MsgXML.TextString();

	String Face = MsgXML.AttributeString(_T("face"));
	UserOrStr(Face);

	SetFace( Face );

	return SetMessage( Msg );
}

bool MsgBaseWindow::SetCharactor( RbBattleStatus* pData )
{
	pStatus = pData;
	return true;
}

bool MsgBaseWindow::SetFace( LPCTSTR FaceName )
{
	if( FaceName == NULL )return false;
	FaceStr = FaceName;
	return true;
}

// メッセージ描写
HRESULT MsgBaseWindow::DMsg( WindowSystem& System, LPCTSTR Message, const RECT& MsgRect ) const
{
	return System.Font().Draw( Font, Message, MsgRect, DT_LEFT, D3DCOLOR_XRGB(0,0,0) );
}

// 位置設定
HRESULT MsgBaseWindow::DrawStatusPos( RbSprite& Display, WindowSystem& System )
{
	if( pStatus == NULL )return E_FAIL;

	RbDisplay& Disp = GetRbDisplay();
	RbPosition Head = Disp.ViewToDisp( pStatus->GetHeadPosition(Disp) );
	Head.x = (int)Head.x;
	Head.y = (int)Head.y;
	Position.x = (int)(Head.x - Position.Width/2);
	Position.y = (int)(Head.y - Position.Height);

	HRESULT ret = BaseWindow::Draw( Display ,System );
	if( FAILED(ret) )return ret;

	return Display.DrawPlane( Fukidashi, Head.x, Head.y-1 );
}

// 位置設定
HRESULT MsgBaseWindow::DrawCenterPos( RbSprite& Display, WindowSystem& System )
{
	const POINT& Center = Display.GetSize();
	SetFloatPosition( Center.x, Center.y, FloatPos.x, FloatPos.y );

	return BaseWindow::Draw( Display ,System );
}

// 顔描写
HRESULT MsgBaseWindow::DrawFace(RbSprite& Surface ) const
{
	Plane<> Face = GetFace();
	if( Face.pTexture == NULL )return S_FALSE;
	RbRect FaceRect = GetFaceRect(Face);
	float y = Position.Height/2 - Face.h/2;
	return Surface.Draw( Face, FaceRect.left, y, RbRect(Face) );
}

// 描写
HRESULT MsgBaseWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( !RestoreCheck(Display,System) )return S_FALSE;

	if( pStatus != NULL )return DrawStatusPos( Display, System );
	return DrawCenterPos( Display, System );
}
