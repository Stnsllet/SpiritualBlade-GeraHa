// AutoBuyWindow.cpp: AutoBuyWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbGameData.h"
#include "RbWindowSystem.h"
#include "RbPaty.h"
#include "ShopSystem.h"
#include "RbSound.h"
#include "AutoBuyWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

AutoBuyWindow::AutoBuyWindow()
: UserData( GetGameData() )
{

}

AutoBuyWindow::~AutoBuyWindow()
{

}

LPCTSTR AutoBuyWindow::WindowName() const
{
	return _T("AutoBuy");
}

bool AutoBuyWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt == VT_BOOL )
	{
		if( MemberName.empty() )return true;
		MemberName.erase();
		TexWindow.Release();
		return true;
	}
	if( Value.vt == VT_BSTR )
	{
		MemberName = Value.bstrVal;
		TexWindow.Release();
		return true;
	}
	return false;
}

HRESULT AutoBuyWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( UserData.UpDateID() != OldUpdateID )TexWindow.Release();
	return BaseWindow::Draw( Display, System );
}

// レストア
HRESULT AutoBuyWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	RbRect CountRect = pPlanes->GetRect(_T("count"));
	RbRect PriceRect = pPlanes->GetRect(_T("money"));
	AcceptRect = pPlanes->GetRect(_T("accept"));

	OldUpdateID = UserData.UpDateID();

	ExSprite Surface;
	RestoreBase( pDevice, Surface, System );

	DrawWindowTag( System, WindowName() );

	long OldMoney = UserData.GetMoney();
	long NewMoney = OldMoney;
	long ChangeCount = 0;
	if( MemberName.empty() )ChangeCount = AutoBuyAll( NewMoney );
	else ChangeCount = AutoBuyMember( MemberName, NewMoney );

	String Format;
	Format.Format( _T("装備変更 %2d 個"), ChangeCount );
	System.Font().Draw( Format, CountRect, DT_CENTER );

	long Price = OldMoney-NewMoney;
	Format.Format( _T("費用 %6d %s"), Price, (LPCTSTR)UserData.RefSystemData().MoneyUnit );
	System.Font().Draw( Format, PriceRect, DT_CENTER );

	System.Font().Draw( _T("購入する"), AcceptRect, DT_CENTER );


	return S_OK;
}

RbRect AutoBuyWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( AcceptRect.Pin(x,y) )return AcceptRect;
	return RbRect();
}

bool AutoBuyWindow::ClickJob( int x, int y )
{
	if( !AcceptRect.Pin(x,y) )return true;

	long OldMoney = UserData.GetMoney();
	long NewMoney = OldMoney;
	long ChangeCount = 0;
	if( MemberName.empty() )ChangeCount = AutoBuyAll( NewMoney, true );
	else ChangeCount = AutoBuyMember( MemberName, NewMoney, true );
	if( ChangeCount <= 0 )return true;

	long Price = OldMoney-NewMoney;
	if( !UserData.PayMoney( Price ) )return false;
	GetSoundSystem().Play(_T("money"));

	Close( GetWindowSystem() );
	return true;
}

long AutoBuyWindow::AutoBuyAll( long& Money, bool bUpdate )
{
	long ChangeCount = 0;

	long MemberCount = count_of(PatyStatus().Member);
	long PatyCount = UserData.PatyCount();
	for( int j = 0; j < MemberCount; j++ )
		for( long i = 0; i < PatyCount; i++ )
		{
			PatyStatus Paty;
			if( !UserData.GetPaty(i,Paty) )continue;
			if( Paty.Member[j].id.IsNone() )continue;
			ChangeCount += AutoBuyMember( Paty.Member[j].id, Money, bUpdate );
		//	ChangeCount += AutoBuyPaty( Paty, Money, bUpdate );
		}
	return ChangeCount;
}

long AutoBuyWindow::AutoBuyPaty( const PatyStatus& PatyData, long& Money, bool bUpdate )
{
	RbPaty Paty(PatyData);
	long ChangeCount = 0;
	long Count = Paty.MemberCount();
	for( int i = 0; i < Count; i++ )
	{
		ChangeCount += AutoBuyMember( PatyData.Member[i].id, Money, bUpdate );
	}
	return ChangeCount;
}

long AutoBuyWindow::AutoBuyMember( LPCTSTR MemberName, long& Money, bool bUpdate )
{
	MemberStatus Mem;
	if( !UserData.GetMember( MemberName, Mem ) )return 0;

	const ShopSystem& Shop= GetShop();
	long ChangeCount = Shop.AutoBuyMember( Mem, Money );
	if( ChangeCount > 0 && bUpdate )UserData.UpDateMemberStatus( Mem );
	return ChangeCount;
}
