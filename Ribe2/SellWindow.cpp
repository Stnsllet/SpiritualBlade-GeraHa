// SellWindow.cpp: SellWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbGameData.h"
#include "DragItemWindow.h"
#include "SellWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SellWindow::SellWindow()
{
	AcceptMsg = _T("売却する");
}

SellWindow::~SellWindow()
{
}

LPCTSTR SellWindow::WindowName() const
{
	return _T("Sell");
}

long SellWindow::GetPrice() const
{
	return ((long)(ItemData.Price/2)) * ItemCount;
}

bool SellWindow::SetItem( short ItemNum )
{
	if( ItemNum <= 0 )return false;
	if( ItemData.id.Number == ItemNum )return true;

	const ItemStatus& RefItem = GetCache().Item.Get( ItemNum );
	if( RefItem.id.Number <= 0 )return false;

	ItemData = RefItem;
	ItemCount = UserData.GetItemCount( ItemNum );
	TexWindow.Release();
	return true;
}

bool SellWindow::DropJob( const DragItemWindow& DragItem )
{
	if( DragItem.DragType() != eDragItem )return false;
	short ItemNum = DragItem.DragData().Item.id.Number;
	return SetItem( ItemNum );
}

bool SellWindow::AcceptJob()
{
	ItemCount = UserData.GetItemCount( ItemData.id.Number );
	long AllPrice = GetPrice();
	UserData.AddItem( ItemData.id.Number, -ItemCount );
	UserData.Money( AllPrice );
	return true;
}
