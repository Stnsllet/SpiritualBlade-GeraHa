// ItemListWindow.cpp: ItemListWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShopSystem.h"
#include "RbWindowSystem.h"
#include "RbCache.h"
#include "RbSound.h"
#include "RbGameData.h"
#include "DragItemWindow.h"
#include "ItemListWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ItemListWindow::ItemListWindow()
{
	NowItemType = 1;
	bEquipJobOK = true;
	AllZero( ShowType );
}

ItemListWindow::~ItemListWindow()
{
}

bool ItemListWindow::EquipCancel() const
{
	RefPtr<BaseWindow> pFindWnd = GetWindowSystem().GetNameWindow(_T("EqStatus"));
	if( pFindWnd == NULL )return false;
	pFindWnd->SetParameter( (long)0 );
	return true;
}

HRESULT ItemListWindow::Command( DISPDEVICE* pDevice, WindowSystem& System, command com )
{
	if( com == eDestroy )EquipCancel();
	return ScrollWindow::Command( pDevice, System, com );
}

LPCTSTR ItemListWindow::WindowName() const
{
	return _T("ItemList");
}

bool ItemListWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	switch( State.Msg() )
	{
	case eLDblClick:
		if( !bEquipJobOK )break;
		if( EquipJob( GetItemNum(State.x-Position.x,State.y-Position.y), true ) )
			return true;
	}
	return ScrollWindow::MouseJob( System, State );
}

bool ItemListWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt == VT_I2 )return ChangeItemType( Value.iVal );

	if( Value.vt != VT_BSTR )return false;
	String NewEquip = Value.bstrVal;
	if( EquipType == NewEquip )return true;
	EquipType = NewEquip;
	AllTexRelease();
	return true;
}

HRESULT ItemListWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	long NewUpdateID = GetGameData().UpDateID();
	if( NewUpdateID != OldUpdateID )AllTexRelease();

	return ScrollWindow::Draw( Display, System );
}

ARGB ItemListWindow::GetCol( long Type ) const
{
	if( Type == NowItemType )return ARGB(MYWHITE);
	return ARGB(0xff808080);
}

HRESULT ItemListWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_FALSE;
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	const RbGameData& UserData = GetGameData();

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	Rects.ListArea = pPlanes->GetRect(_T("list"));
	Rects.Item = pPlanes->GetRect(_T("item"));
	Rects.Name = pPlanes->GetRect(_T("name"));
	Rects.Count = pPlanes->GetRect(_T("count"));
	Rects.Price = pPlanes->GetRect(_T("price"));

	Rects.WepTag = pPlanes->GetRect(_T("wep"));
	Rects.BodyTag = pPlanes->GetRect(_T("body"));
	Rects.AcsTag = pPlanes->GetRect(_T("acs"));
	Rects.PowTag = pPlanes->GetRect(_T("pow"));
	Rects.EventTag = pPlanes->GetRect(_T("event"));
	RbRect Info = pPlanes->GetRect(_T("info"));
	UnitStr = UserData.RefSystemData().MoneyUnit;

	Rects.HighStr = pPlanes->XML.AttributeString(_T("highmoney"));

	ExSprite Surface;
	HRESULT hr = RestoreBase( pDevice, Surface, System );
	if( FAILED(hr) )return hr;

	DrawSurfaceBase( Surface, System, pPlanes->GetRect(_T("subwnd")) );

	DrawWindowTag(System,WindowName());
	LPCTSTR TagFont = WindowTagFont;

	ReLoadList();

	DrawType( System, _T("WEP"), Rects.WepTag, eWeponTypeItem );
	DrawType( System, _T("BODY"), Rects.BodyTag, eBodyTypeItem );
	DrawType( System, _T("ACS"), Rects.AcsTag, eAcsTypeItem );
	DrawType( System, _T("POW"), Rects.PowTag, ePowTypeItem );
	DrawType( System, _T("EVENT"), Rects.EventTag, eEventTypeItem );

	System.Font().Draw(TagFont,_T("Item"),
		RbRect(Rects.Name).Move(Info.left,Info.top));
	System.Font().Draw(TagFont,_T("Stock"),
		RbRect(Rects.Count).Move(Info.left,Info.top),DT_CENTER);
	System.Font().Draw(TagFont,_T("Price"),
		RbRect(Rects.Price).Move(Info.left,Info.top));

	ScrollList.resize(1);
	RestoreScroll( pDevice, System, 0 );

	OldUpdateID = UserData.UpDateID();
	return S_OK;
}

void ItemListWindow::DrawType( WindowSystem& System, LPCTSTR Name, const RbRect& Rect, long Type ) const
{
	if( Type <= 0 || Type >= count_of(ShowType) )return;
	if( Rect.IsNone() )return;
	if( !ShowType[Type] )return;

	System.Font().Draw(WindowTagFont, Name,
		Rect, DT_CENTER, GetCol(Type) );
}

HRESULT ItemListWindow::RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index )
{
	if( Index >= ScrollList.size() )return S_FALSE;
	if( Index != 0 )return ScrollWindow::RestoreScroll( pDevice, System, Index );

	ScrollInfo& Ref = ScrollList[Index];
	ScrollInfo::PosStock PosStock( Ref );

	long Count = List.size();
	long ScrollHeight = Rects.Item.Height() * Count;
	ScrollHeight += Rects.Item.top * 2;
	Ref.SetInfo( Rects.ListArea, ScrollHeight );

	HRESULT hr = Ref.CreateTexture( pDevice );
	if( FAILED(hr) )return hr;

	ExSprite Surface;
	Surface.InitSprite( pDevice, Ref.Tex );
	Surface.Clear();

	RbRect ItemBox = Rects.Item;
	ShopSystem::ItemList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		const ShopSystem::ShopData& RefItem = *itr++;
		DrawItem( System, ItemBox, RefItem );
		if( !RefItem.bEquipOK )Surface.DrawBreak( ItemBox );
		ItemBox.BoxMove(0,1);
	}
	return S_OK;
}

HRESULT ItemListWindow::DrawItem( WindowSystem& System, const RbRect& Rect, const ShopSystem::ShopData& Data ) const
{
	System.Font().Draw( Data.ItemName,
		RbRect(Rects.Name).Move(Rect.left,Rect.top));

	String Format;
	Format.Format( _T("%02d"), GetGameData().GetItemCount(Data.ItemID) );

	System.Font().Draw( Format,
		RbRect(Rects.Count).Move(Rect.left,Rect.top), DT_CENTER);

	if( Data.Price < 100000 || Rects.HighStr.empty() )
		Format.Format( _T("%d %s"), Data.Price, (LPCTSTR)UnitStr );
	else Format.Format( _T("%d%s %s"), (Data.Price+5000)/10000, (LPCTSTR)Rects.HighStr, (LPCTSTR)UnitStr );
	System.Font().Draw( Format,
		RbRect(Rects.Price).Move(Rect.left,Rect.top), DT_RIGHT);

	return S_OK;
}

bool ItemListWindow::ReLoadList()
{
	for( int i= 1; i < count_of(ShowType); i++ )ShowType[i]=true;

	List = GetShop().GetHaveItemList(NowItemType, EquipType);
	return true;
}

RbRect ItemListWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( ShowType[1] )if( Rects.WepTag.Pin(x,y) )return Rects.WepTag;
	if( ShowType[2] )if( Rects.BodyTag.Pin(x,y) )return Rects.BodyTag;
	if( ShowType[3] )if( Rects.AcsTag.Pin(x,y) )return Rects.AcsTag;
	if( ShowType[4] )if( Rects.PowTag.Pin(x,y) )return Rects.PowTag;
	if( ShowType[4] )if( Rects.EventTag.Pin(x,y) )return Rects.EventTag;

	if( ScrollList.size() <= 0 )return RbRect();
	long ItemIndex = GetItemIndex( x, y );
	if( ItemIndex < 0 )return RbRect();

	RbRect ItemRect = Rects.Item;
	ItemRect.Move( Rects.ListArea.left, Rects.ListArea.top-ScrollList[0].NowPos );
	ItemRect.BoxMove(0,ItemIndex);
	ItemRect.Clip( Rects.ListArea );

	return ItemRect;
}

long ItemListWindow::GetItemIndex( int x, int y ) const
{
	if( !Rects.ListArea.Pin(x,y) )return -1;

	RbRect Item = Rects.Item;
	Item.Move( Rects.ListArea.left, Rects.ListArea.top-ScrollList[0].NowPos );
	long Count = List.size();
	int i = 0;
	while( i < Count )
	{
		if( Item.Pin(x,y) )return i;
		i++;
		Item.BoxMove(0,1);
	}
	return -1;
}

long ItemListWindow::GetItemNum( int x, int y ) const
{
	long ItemIndex = GetItemIndex( x, y );
	if( ItemIndex < 0 )return 0;

	ShopSystem::ItemList::const_iterator itr = List.begin();
	while( ItemIndex-- > 0 )itr++;
	const ShopSystem::ShopData& RefItem = *itr;
	return RefItem.ItemID;
}

bool ItemListWindow::ChangeItemType( long NewType )
{
	NowItemType = NewType;

	GetSoundSystem().Play(_T("click"));
	TexWindow.Release();
	EquipCancel();
	return true;
}

bool ItemListWindow::Floating( int x, int y )
{
	long ItemNum = GetItemNum( x, y );
	if( ItemNum <= 0 )return true;
	GetWindowSystem().HelpItem( ItemNum, ToGlobalRect(Rects.ListArea) );
	return true;
}

bool ItemListWindow::ClickJob( int x, int y )
{
	if( ShowType[1] )if( Rects.WepTag.Pin(x,y) )return ChangeItemType( 1 );
	if( ShowType[2] )if( Rects.BodyTag.Pin(x,y) )return ChangeItemType( 2 );
	if( ShowType[3] )if( Rects.AcsTag.Pin(x,y) )return ChangeItemType( 3 );
	if( ShowType[4] )if( Rects.PowTag.Pin(x,y) )return ChangeItemType( 4 );
	if( ShowType[5] )if( Rects.EventTag.Pin(x,y) )return ChangeItemType( 5 );

	long ItemNum = GetItemNum(x,y);
	if( ItemNum > 0 )EquipJob( ItemNum, false );

	return true;
}

bool ItemListWindow::EquipJob( long ItemNum, bool bChange ) const
{
//	if( !bEquipJobOK )return false;

	if( ItemNum <= 0 )return false;
	const ItemStatus& RefItem = GetCache().Item.Get( ItemNum );
	RefPtr<BaseWindow> pFindWnd = GetWindowSystem().GetNameWindow(_T("EqStatus"));
	if( pFindWnd != NULL )
	{
		pFindWnd->SetParameter( RefItem.id.MakeLong() );
		if( bChange )pFindWnd->SetParameter( bChange );
	}

	return true;
}

bool ItemListWindow::DragStartJob( WindowSystem& System, GameMessage& State )
{
	int x = State.x - Position.x;
	int y = State.y - Position.y;
	long ItemNo = GetItemNum( x, y );
	if( ItemNo <= 0 )return false;

	RefPtr<DragItemWindow> pDrag( new DragItemWindow );
	if( !pDrag->SetItemData( ItemNo ) )return false;

	pDrag->SetDrag( System, State );
	return true;
}
