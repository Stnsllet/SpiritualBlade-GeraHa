// DivisionWindow.cpp: DivisionWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbPaty.h"
#include "RbCache.h"
#include "RbGameData.h"
#include "RbWindowSystem.h"
#include "DragItemWindow.h"
#include "StrCommander.h"
#include "RbEventSystem.h"
#include "AllWorld.h"
#include "DivisionWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DivisionWindow::DivisionWindow()
: UserData( GetGameData() )
{
	PatyAreaCount = MaxPaty = UserData.RefSystemData().MaxPaty;
	bCursorSound = false;
}

DivisionWindow::~DivisionWindow()
{
}

LPCTSTR DivisionWindow::WindowName() const
{
	return _T("Division");
}

HRESULT DivisionWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( UserData.UpDateID() != OldUpdateID )ScrollTexRelease();
	return ScrollWindow::Draw( Display, System );
}

// レストア
HRESULT DivisionWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	Rects.Patys = pPlanes->GetRect(_T("patys"));
	Rects.Paty = pPlanes->GetRect(_T("paty"));
	Rects.Member = pPlanes->GetRect(_T("member"));
	Rects.LevTag = pPlanes->GetRect(_T("levtag"));

	Rects.Commnand = pPlanes->GetRect(_T("com"));
	Rects.Training = pPlanes->GetRect(_T("training"));
	Rects.AutoBuy = pPlanes->GetRect(_T("buy"));
	Rects.Formation = pPlanes->GetRect(_T("form"));
	Rects.StockMember = pPlanes->GetRect(_T("stock"));
	Rects.Employ = pPlanes->GetRect(_T("employ"));
	Rects.bNewPatyOk = !(pPlanes->XML.AttributeBool(_T("nonewpaty")));

	{
		ExSprite Surface;
		RestoreBase( pDevice, Surface, System );

		DrawWindowTag( System, _T("MainMember") );
		RestoreCommand( Surface, System, Rects.Commnand );
	}

	ScrollList.resize(1);
	RestoreScroll( pDevice, System, 0 );
	return S_OK;
}

HRESULT DivisionWindow::RestoreCommand( RbSprite& Surface, WindowSystem& System, const RbRect& ComArea )
{
	const RbPoint ComLT = ComArea.LT();
	DrawMyPlane( Surface, _T("mem_training"), Rects.Training, ComLT );
	DrawMyPlane( Surface, _T("mem_buy"), Rects.AutoBuy, ComLT );
	DrawMyPlane( Surface, _T("mem_form"), Rects.Formation, ComLT );
	DrawMyPlane( Surface, _T("mem_stock"), Rects.StockMember, ComLT );
	DrawMyPlane( Surface, _T("mem_employ"), Rects.Employ, ComLT );
	return S_OK;
}

void DivisionWindow::DrawMyPlane( RbSprite& Surface, LPCTSTR Name, const RbRect& Rect, const RbPoint& Pos ) const
{
	if( Rect.IsNone() )return;
	Surface.DrawPlane( Name, Rect.left+Pos.x, Rect.top+Pos.y);
}

HRESULT DivisionWindow::RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index )
{
	if( Index != 0 )return ScrollWindow::RestoreScroll( pDevice, System, Index );

	OldUpdateID = UserData.UpDateID();
	ScrollInfo& Ref = ScrollList[0];
	ScrollInfo::PosStock PosStock( Ref );

	long PatyCount = UserData.PatyCount();
	long PatySize = PatyCount+((Rects.bNewPatyOk)?(1):(0));
	PatySize = PatyAreaCount = min( PatySize, MaxPaty );
//	PatySize = MaxPaty;

	float ScrollHeight = Rects.Paty.Height()*PatySize;
	ScrollHeight += Rects.Paty.top*2;
	Ref.SetInfo( Rects.Patys, ScrollHeight );

	ExSprite Surface;
	HRESULT hr = Ref.SetSprite( pDevice, Surface );
	if( FAILED(hr) )return hr;

	for( int i = 0; i < PatyCount; i++ )
	{
		RbRect Rect = Rects.Paty;
		Rect.BoxMove( 0, i );
		PatyStatus State;
		if( UserData.GetPaty(i, State) )
			DrawPaty( Surface, System, State, Rect.left, Rect.top );
	}
	return S_OK;
}

HRESULT DivisionWindow::DrawPaty( RbSprite& Display, WindowSystem& System, const PatyStatus& State, float x, float y )
{
	RbRectF BaseRect( x, y, Rects.Member.Width(), Rects.Member.Height() );
	RbPaty Paty( State );
	long Count = Paty.MemberCount();
	for( int i = Count-1; i >= 0; i-- )
	{
		RbRectF DrawRect = BaseRect;
		DrawRect.BoxMove( i, 0 );
		MemberStatus Mem;
		if( UserData.GetMember(State.Member[i].id, Mem) )
		DrawMember( Display, System, Mem, DrawRect.left, DrawRect.top );
	}
	return S_OK;
}

HRESULT DivisionWindow::DrawMember( RbSprite& Display, WindowSystem& System, const MemberStatus& State, float x, float y )
{
	RbRect TagRect = Rects.LevTag;
	TagRect.Move( x, y );

	HRESULT hr = Display.DrawRbImage( State, Rects.Member.Shift(x,y) );
	if( FAILED(hr) )return hr;

	if( GlayDown.pTexture == NULL )GlayDown = GetCache().SystemPlane.GetPlane(_T("glaydwn"));
	Display.Draw( GlayDown, RbRect(GlayDown), TagRect );
	String LevMsg;
	LevMsg.Format(_T("L .%2d"), State.Level );
	System.Font().Draw(_T("min_num"), LevMsg, TagRect, DT_CENTER );
	return S_OK;
}

bool DivisionWindow::DragStartJob( WindowSystem& System, GameMessage& State )
{
	if( State.IsDraging() )return false;
	int x = State.x - Position.x;
	int y = State.y - Position.y;

	CheckData NumData = HitCheck( x, y );
	MemberStatus Mem;
	if( !UserData.GetMember( NumData.PatyNum, NumData.MemberNum, Mem ) )return false;

	RefPtr<DragItemWindow> pDrag( new DragItemWindow );
	if( !pDrag->SetMemberData( Mem ) )return false;

	pDrag->SetDrag( System, State );
	return true;
}

bool DivisionWindow::ClickJob( int x, int y )
{
	if( ClickCommand(x,y) )return true;

	CheckData NumData = HitCheck( x, y );
	MemberStatus Mem;
	if( !UserData.GetMember( NumData.PatyNum, NumData.MemberNum, Mem ) )
		return true;

	if( NumData.MemberNum==0 )
		StrCommander().ShowWindow( _T("PatyEdit"), NumData.PatyNum );
	StrCommander().ShowMemberState( Mem.PasonName );
	return true;
}

bool DivisionWindow::Floating( int x, int y )
{
	if( !Rects.Commnand.Pin(x,y) )return false;
	x -= Rects.Commnand.left;
	y -= Rects.Commnand.top;
	RbWindowSystem& System = GetWindowSystem();
	RbCache& Cache = GetCache();

	if( Rects.Formation.Pin(x,y) )
		System.Help( Cache.GetMessage(_T("com_form")),NULL,ToGlobalRect(Rects.Commnand) );

	if( Rects.AutoBuy.Pin(x,y) )
		System.Help( Cache.GetMessage(_T("com_autobuy")),NULL,ToGlobalRect(Rects.Commnand) );

	if( Rects.StockMember.Pin(x,y) )
		System.Help( Cache.GetMessage(_T("com_stock")),NULL,ToGlobalRect(Rects.Commnand) );

	if( Rects.Employ.Pin(x,y) )
		System.Help( Cache.GetMessage(_T("com_employ")),NULL,ToGlobalRect(Rects.Commnand) );

	if( Rects.Training.Pin(x,y) )
		System.Help( Cache.GetMessage(_T("com_training")),NULL,ToGlobalRect(Rects.Commnand) );
	return true;
}

bool DivisionWindow::ClickCommand( int x, int y )
{
	if( !Rects.Commnand.Pin(x,y) )return false;
	x -= Rects.Commnand.left;
	y -= Rects.Commnand.top;

	if( Rects.Formation.Pin(x,y) )
		ShowFormation();

	if( Rects.StockMember.Pin(x,y) )
		StrCommander().ShowWindow( _T("StockMember") );

	if( Rects.Employ.Pin(x,y) )
		StrCommander().ShowWindow( _T("Employ"), true );

	if( Rects.AutoBuy.Pin(x,y) )
		StrCommander().ShowWindow( _T("AutoBuy"), true );

	if( Rects.Training.Pin(x,y) )
		if( GetWorldSystem().IsMoveStop() )
		GetEventSystem().LoadEvent(_T("Event/Training.xml"));

	return true;
}

bool DivisionWindow::ShowFormation( LPCTSTR Name, bool bFromEx ) const
{
	if( bFromEx )return true;

	RefPtr<BaseWindow> pWnd = StrCommander().ShowWindow( _T("PatyEdit") );
	if( Name != NULL && pWnd != NULL )
	{
		long PatyID = UserData.GetPatyID( Name );

		if( PatyID > 0 )pWnd->SetParameter( PatyID );
	}
	return true;
}

RbRect DivisionWindow::HitCursor( WindowSystem& System, int x, int y )
{
	CheckData Hit = HitCheck( x, y );
	if( !Hit )return HitComCursor(System,x,y);
	return GetCheckRect( Hit );
}

RbRect DivisionWindow::HitComCursor( WindowSystem& System, int x, int y )
{
	int ComX = Rects.Commnand.left;
	int ComY = Rects.Commnand.top;

	RbRect Rect = Rects.Training.Shift(ComX,ComY);
	if( Rect.Pin(x,y) )return Rect;

	Rect = Rects.AutoBuy.Shift(ComX,ComY);
	if( Rect.Pin(x,y) )return Rect;

	Rect = Rects.Employ.Shift(ComX,ComY);
	if( Rect.Pin(x,y) )return Rect;

	Rect = Rects.Formation.Shift(ComX,ComY);
	if( Rect.Pin(x,y) )return Rect;

	Rect = Rects.StockMember.Shift(ComX,ComY);
	if( Rect.Pin(x,y) )return Rect;

	return RbRect();
}

RbRect DivisionWindow::GetCheckRect( const CheckData& Pos ) const
{
	
	if( !Pos )return RbRect();
	RbRect Ret = Rects.Member;
	int x = Rects.Patys.left + Rects.Paty.left + Rects.Member.left;
	int y = Rects.Patys.top + Rects.Paty.top + Rects.Member.top;
	x += Rects.Member.Width()*Pos.MemberNum;
	y += Rects.Paty.Height()*Pos.PatyNum;
	y -= ScrollPos();
	Ret.Move( x, y );
	Ret.Clip( Rects.Patys );
	return Ret;
}

DivisionWindow::CheckData DivisionWindow::HitCheck( int x, int y ) const
{
	if( Rects.Patys.Pin( x, y ) )return HitCheckMain(x-Rects.Patys.left,y-Rects.Patys.top);
	return CheckData();
}

DivisionWindow::CheckData DivisionWindow::HitCheckMain( int x, int y ) const
{
	y += ScrollPos();
	for( long i = 0; i < PatyAreaCount; i++ )
	{
		RbRect Paty = Rects.Paty;
		Paty.BoxMove( 0, i );
		if( Paty.Pin( x, y ) )
		{
			int dx = x - Paty.left;
			int dy = y - Paty.top;
			for( long j = 0; j < count_of(PatyStatus().Member); j++ )
			{
				RbRect Member = Rects.Member;
				Member.BoxMove( j, 0 );
				if( Member.Pin( dx, dy ) )return CheckData( i, j );
			}
		}
	}
	return CheckData();
}

bool DivisionWindow::DropJob( const DragItemWindow& DragItem )
{
	const RbPoint& Pos = DragItem.Pos();
	if( DragItem.DragType() == eDragItem )
		return EquipJob( Pos.x, Pos.y,  DragItem.DragData().Item.id );
	if( DragItem.DragType() == eDragMember )
		return MemberJob( Pos.x, Pos.y, DragItem.DragData().Member, DragItem.DragData().bFromExPaty );

	return false;
}

bool DivisionWindow::MemberJob( int x, int y, const MemberStatus& State, bool bFromEx ) const
{
	if( Rects.Commnand.Pin(x,y) )
	{
		int dx = x-Rects.Commnand.left;
		int dy = y-Rects.Commnand.top;
		if( Rects.StockMember.Pin(dx,dy) )
			if( bFromEx )return UserData.DropPatyMember( State.PasonName, UserData.RefExPatyNum() );
			else return UserData.DropPatyMember( State.PasonName );
		if( Rects.Formation.Pin(dx,dy) )
			return ShowFormation( State.PasonName, bFromEx );
		if( Rects.AutoBuy.Pin(dx,dy) )
		{
			StrCommander().ShowWindow( _T("AutoBuy"), (LPCTSTR)State.PasonName );
			return true;
		}
		if( Rects.Employ.Pin(dx,dy) )
		{
			StrCommander().ShowWindow( _T("Discharge"), (LPCTSTR)State.PasonName );
			return true;
		}
	}

	CheckData NumDat = HitCheck( x, y );
	if( !NumDat )return false;

	return UserData.ChangePaty( State, NumDat.PatyNum, NumDat.MemberNum );
}

bool DivisionWindow::EquipJob( int x, int y, const ItemID& Item ) const
{
	CheckData NumDat = HitCheck( x, y );
	if( !NumDat )return false;

	MemberStatus Mem;
	if( !UserData.GetMember( NumDat.PatyNum, NumDat.MemberNum, Mem ) )return false;

	return UserData.ChangeEquip( Mem.PasonName, Item );
}
