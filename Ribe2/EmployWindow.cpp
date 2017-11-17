// EmployWindow.cpp: EmployWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbWindowSystem.h"
#include "ShopSystem.h"
#include "RbMember.h"
#include "RbSound.h"
#include "RbGameData.h"
#include "StrCommander.h"
#include "EmployWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

EmployWindow::EmployWindow()
{
	AcceptString = _T("雇う");
}

EmployWindow::~EmployWindow()
{
}

LPCTSTR EmployWindow::WindowName() const
{
	return _T("Employ");
}

bool EmployWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt != VT_BOOL )return false;
	NextEmploy();
	return true;
}

HRESULT EmployWindow::RestoreWindow( RbSprite& Surface, WindowSystem& System, const WindowPlanes* pPlanes )
{
	if( pPlanes == NULL )return E_FAIL;
	if( TagetMember.PasonName.IsNone() )NextEmploy();

	HRESULT hr = DischargeWindow::RestoreWindow( Surface, System, pPlanes );
	if( FAILED(hr) )return hr;

	RbRect MoneyRect = pPlanes->GetRect( _T("money") );
	String Format;
	Format.Format( _T("%d %s"), GetPrice(), (LPCTSTR)GetGameData().RefSystemData().MoneyUnit );
	System.Font().Draw( Format, MoneyRect, DT_CENTER );

	NextRect = pPlanes->GetRect( _T("next") );
	System.Font().Draw( _T("次へ"), NextRect, DT_CENTER );

	return S_OK;
}

bool EmployWindow::NextEmploy()
{
	MemberStatus NewMem;
	if( !GetShop().EmployMember( NewMem ) )return false;
	GetSoundSystem().Play(_T("click"));
	TagetMember = NewMem;
	TexWindow.Release();
	return true;
}

long EmployWindow::GetPrice() const
{
	return ( TagetMember.Level * 1000);
}

RbRect EmployWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( NextRect.Pin(x,y) )return NextRect;
	return DischargeWindow::HitCursor( System, x, y );
}

bool EmployWindow::ClickJob( int x, int y )
{
	if( NextRect.Pin(x,y) )return NextEmploy();

	return DischargeWindow::ClickJob( x, y );
}

bool EmployWindow::AcceptJob()
{
	StrCommander().ShowWindow(_T("StockMember"));
	RbGameData& UserData = GetGameData();
	long StockCount = UserData.GetStockCount();
	if( StockCount >= UserData.RefSystemData().MaxStock )
	{
		SystemPopup( _T("stockover") );
		return false;
	}
	if( !UserData.PayMoney( GetPrice() ) )return false;
	GetSoundSystem().Play(_T("money"));

	return GetGameData().AddMember( TagetMember );
}
