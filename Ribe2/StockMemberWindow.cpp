// StockMemberWindow.cpp: StockMemberWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbGameData.h"
#include "RbMember.h"
#include "RbPaty.h"
#include "RbWindowSystem.h"
#include "DragItemWindow.h"
#include "StrCommander.h"
#include "StockMemberWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

StockMemberWindow::StockMemberWindow()
: UserData( GetGameData() )
{
	bCursorSound = false;
}

StockMemberWindow::~StockMemberWindow()
{
}

LPCTSTR StockMemberWindow::WindowName() const
{
	return _T("StockMember");
}

bool StockMemberWindow::StockData::SetData( long SrcPasonID, const MemberStatus& State, const RbGameData& UserData )
{
	if( UserData.GetPatyID( State.PasonName ) >= 0 )return false;
//	if( UserData.InExPaty( State.PasonName ) )return false;
	RbMember Mem(State);

	PasonID = SrcPasonID;
	Name = Mem.PasonData().Name;
	Level = Mem.Ref.Level;
	TexName = Mem.GetTexture();
	return true;
}

bool StockMemberWindow::StockData::IsFront( const StockData& Ref ) const
{
	if( Level > Ref.Level )return true;
	if( Level < Ref.Level )return false;
	return (Name < Ref.Name);
}

HRESULT StockMemberWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( UserData.UpDateID() != OldUpdateID )ScrollTexRelease();
	return ScrollWindow::Draw( Display, System );
}

HRESULT StockMemberWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	Rects.ListArea = pPlanes->GetRect(_T("list"));
	Rects.Member = pPlanes->GetRect(_T("member"));
	Rects.Image = pPlanes->GetRect(_T("image"));
	Rects.Name = pPlanes->GetRect(_T("name"));
	Rects.Level = pPlanes->GetRect(_T("level"));

	{
		ExSprite Surface;
		RestoreBase( pDevice, Surface, System );

		DrawWindowTag( System, WindowName() );
	}

	ScrollList.resize(1);
	RestoreScroll( pDevice, System, 0 );
	return S_OK;
}

bool StockMemberWindow::ReLoadStockMember()
{
	typedef std::map<long, MemberStatus> MemberMap;
	const MemberMap& Members = UserData.RefMembers();

	StockMemberList.clear();
	MemberMap::const_iterator itr = Members.begin();
	for( ;itr != Members.end(); itr++ )
	{
		StockData NewData;
		if( !NewData.SetData( itr->first, itr->second, UserData ) )continue;

		std::list<StockData>::iterator inspos = StockMemberList.begin();
		for( ;inspos != StockMemberList.end(); inspos++ )
			if( NewData.IsFront( *inspos ) )break;
		StockMemberList.insert( inspos, NewData );
	}

	return true;
}

HRESULT StockMemberWindow::RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index )
{
	if( Index != 0 )return ScrollWindow::RestoreScroll( pDevice, System, Index );

	OldUpdateID = UserData.UpDateID();
	ScrollInfo& Ref = ScrollList[0];
	ScrollInfo::PosStock PosStock( Ref );

	ReLoadStockMember();
	long Count = StockMemberList.size();

	float ScrollHeight = Rects.Member.Height()*Count;
	ScrollHeight += Rects.Member.top*2;
	Ref.SetInfo( Rects.ListArea, ScrollHeight );

	ExSprite Surface;
	HRESULT hr = Ref.SetSprite( pDevice, Surface );
	if( FAILED(hr) )return hr;

	RbRect MemberRect = Rects.Member;
	std::list<StockData>::const_iterator itr = StockMemberList.begin();
	while( itr != StockMemberList.end() )
	{
		DrawMember( Surface, System, *itr, MemberRect );
		itr++;
		MemberRect.BoxMove( 0, 1 );
	}

	return S_OK;
}

bool StockMemberWindow::DrawMember( RbSprite& Surface, WindowSystem& System, const StockData& Data, const RbRect& Rect ) const
{
	const int x = Rect.left;
	const int y = Rect.top;
	Surface.DrawRbImage( Data.TexName, Rects.Image.Shift(x,y), true );

	String Format;
	Format.Format( _T("L%2d"), Data.Level );
	System.Font().Draw( Format, Rects.Level.Shift(x,y) );
	System.Font().Draw( Data.Name, Rects.Name.Shift(x,y), DT_CENTER );

	return true;
}

RbRect StockMemberWindow::HitCursor( WindowSystem& System, int x, int y )
{
	long Index = GetHitIndex(x,y);
	if( Index < 0 )return RbRect();
	RbRect Member = Rects.Member.Shift(Rects.ListArea.left,Rects.ListArea.top);
	Member.Move( 0, -ScrollList[0].NowPos );

	return Member.BoxMove( 0, Index ).Clip(Rects.ListArea);
}

long StockMemberWindow::GetHitIndex( int x, int y ) const
{
	if( ScrollList.size() <= 0 )return -1;
	if( !Rects.ListArea.Pin(x,y) )return -1;
	x -= Rects.ListArea.left+Rects.Member.left;
	y -= Rects.ListArea.top+Rects.Member.top;
	y += ScrollList[0].NowPos;
	if( x < 0 || x > Rects.Member.Width() )return -1;
	if( y < 0 )return -1;

	long MemHight = Rects.Member.Height();
	if( MemHight <= 0 )return -1;
	long Index = y/MemHight;
	if( Index >= StockMemberList.size() )return -1;
	return Index;
}

long StockMemberWindow::GetHitPasonID( int x, int y ) const
{
	long Index = GetHitIndex( x, y );
	if( Index < 0 )return -1;

	std::list<StockData>::const_iterator itr = StockMemberList.begin();
	while( (Index--)>0 )itr++;
	return (*itr).PasonID;
}

bool StockMemberWindow::DropJob( const DragItemWindow& DragItem )
{
	if( DragItem.DragType() == eDragMember )
		return StockJob( DragItem.DragData().Member, DragItem.DragData().bFromExPaty );

	return false;
}

bool StockMemberWindow::DragStartJob( WindowSystem& System, GameMessage& State )
{
	if( State.IsDraging() )return false;
	int x = State.x - Position.x;
	int y = State.y - Position.y;

	long PasonID = GetHitPasonID(x,y);
	MemberStatus Mem;
	if( !UserData.GetMember( PasonID, Mem ) )return false;

	RefPtr<DragItemWindow> pDrag( new DragItemWindow );
	if( !pDrag->SetMemberData( Mem ) )return false;

	pDrag->SetDrag( System, State );
	return true;
}

bool StockMemberWindow::ClickJob( int x, int y )
{
	long PasonID = GetHitPasonID(x,y);
	StrCommander().ShowMemberState( PasonID );
	return true;
}

bool StockMemberWindow::StockJob( const MemberStatus& State, bool bFromEx ) const
{
	if( bFromEx )return UserData.DropPatyMember( State.PasonName, UserData.RefExPatyNum() );
	return UserData.DropPatyMember( State.PasonName );
}
