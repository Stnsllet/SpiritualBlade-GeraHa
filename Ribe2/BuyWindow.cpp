// BuyWindow.cpp: BuyWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbGameData.h"
#include "RbSound.h"
#include "RbWindowSystem.h"
#include "BuyWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BuyWindow::BuyWindow()
: UserData( GetGameData() )
{
	AllZero(ItemData);
	ItemCount = 0;
	AcceptMsg = _T("購入する");
}

BuyWindow::~BuyWindow()
{
}

LPCTSTR BuyWindow::WindowName() const
{
	return _T("Buy");
}

bool BuyWindow::CheckParameter( const CComVariant& Value ) const
{
	if( Value.vt != VT_I4 || Value.vt != VT_I2 )return false;
	short ItemNum = Value.iVal;
	return (ItemData.id.Number == ItemNum);
}

bool BuyWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt == VT_BOOL )return AddCount();

	if( Value.vt == VT_I4 || Value.vt == VT_I2 )
		return SetItem( Value.iVal );

	CComVariant New;
	New.ChangeType( VT_I2, &Value );
	return SetItem( New.iVal );

	return false;
}

bool BuyWindow::AddCount()
{
	if( ItemCount >= 99 )return false;
	ItemCount++;
	TexWindow.Release();
	return true;
}

bool BuyWindow::SetItem( short ItemNum )
{
	if( ItemNum <= 0 )return false;
	if( ItemData.id.Number == ItemNum )return AddCount();

	const ItemStatus& RefItem = GetCache().Item.Get( ItemNum );
	if( RefItem.id.Number <= 0 )return false;

	ItemData = RefItem;
	ItemCount = 1;
	TexWindow.Release();
	return true;
}

long BuyWindow::GetPrice() const
{
	return ItemData.Price * ItemCount;
}

HRESULT BuyWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_FALSE;
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	RbRect NameRect = pPlanes->GetRect(_T("name"));
	RbRect CountRect = pPlanes->GetRect(_T("count"));
	RbRect MoneyRect = pPlanes->GetRect(_T("money"));
	ClickArea = pPlanes->GetRect(_T("accept"));
	String UnitStr = UserData.RefSystemData().MoneyUnit;

	ExSprite Surface;
	HRESULT hr = RestoreBase( pDevice, Surface, System );
	if( FAILED(hr) )return hr;
	DrawWindowTag(System,WindowName());

	System.Font().Draw( ItemData.Name, NameRect, DT_CENTER );
	String Format;
	Format.Format( _T("%d 個"), ItemCount );
	System.Font().Draw( Format, CountRect, DT_CENTER );
	Format.Format( _T("%d %s"), GetPrice(), (LPCTSTR)UnitStr );
	System.Font().Draw( Format, MoneyRect, DT_CENTER );

	System.Font().Draw( AcceptMsg, ClickArea, DT_CENTER );
	return S_OK;
}

RbRect BuyWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( ClickArea.Pin(x,y) )return ClickArea;
	return RbRect();
}

bool BuyWindow::ClickJob( int x, int y )
{
	if( !ClickArea.Pin(x,y) )return true;

	if( AcceptJob() )
	{
		Command( NULL, GetWindowSystem(), eDestroy );
		GetSoundSystem().Play(_T("money"));
	}
	return true;
}

bool BuyWindow::AcceptJob()
{
	long AllPrice = GetPrice();
	if( !UserData.PayMoney( AllPrice ) )return false;

	UserData.AddItem( ItemData.id.Number, ItemCount );
	return true;
}
