// ShopListWindow.cpp: ShopListWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShopSystem.h"
#include "StrCommander.h"
#include "RbWindowSystem.h"
#include "DragItemWindow.h"
#include "ShopListWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ShopListWindow::ShopListWindow()
{
	bEquipJobOK = false;
}

ShopListWindow::~ShopListWindow()
{
}

LPCTSTR ShopListWindow::WindowName() const
{
	return _T("ShopList");
}

bool ShopListWindow::ReLoadList()
{
	const ShopSystem& Shop = GetShop();
	for( int i= 1; i < count_of(ShowType); i++ )
		ShowType[i] = Shop.CheckShopItemType(i);

	List = Shop.GetShopList(NowItemType, EquipType);
	return true;
}

bool ShopListWindow::ClickJob( int x, int y )
{
	short ItemNum = (short)GetItemNum( x, y );
	if( ItemNum <= 0 )return ItemListWindow::ClickJob( x, y );

	EquipJob( ItemNum, false );

	RefPtr<BaseWindow> pBuy = StrCommander().ShowWindow( _T("Buy"), ItemNum );
	return true;
}

bool ShopListWindow::DropJob( const DragItemWindow& DragItem )
{
	if( DragItem.DragType() != eDragItem )return false;
	short ItemNum = DragItem.DragData().Item.id.Number;

	RefPtr<BaseWindow> pSell = StrCommander().ShowWindow(_T("Sell"), ItemNum);
	return true;
}
