// EquipStatusWindow.cpp: EquipStatusWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbGameData.h"
#include "StrCommander.h"
#include "RbMember.h"
#include "RbWindowSystem.h"
#include "DragItemWindow.h"
#include "EquipStatusWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

EquipStatusWindow::EquipStatusWindow()
: UserData(GetGameData())
{
	AllZero( EquipItem );
	bEquipMode = true;
	bExPatyMode = false;
}

EquipStatusWindow::~EquipStatusWindow()
{
}

LPCTSTR EquipStatusWindow::WindowName() const
{
	return _T("EqStatus");
}

bool EquipStatusWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	switch( State.Msg() )
	{
	case eLDblClick:
		if( UnEquipJob(State.x-Position.x,State.y-Position.y) )return true;
		break;
	}
	return StatusWindow::MouseJob( System, State );
}

bool EquipStatusWindow::RedrawCheck() const
{
	return (OldUpdateID != UserData.UpDateID());
}

HRESULT EquipStatusWindow::Draw( RbSprite& Display, WindowSystem& System )
{
//	if( OldUpdateID != UserData.UpDateID() )TexWindow.Release();
	return StatusWindow::Draw( Display, System );
}

bool EquipStatusWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt == VT_BSTR )return SetMember( String(Value.bstrVal) );
	if( Value.vt == VT_I4 )return SetItem( Value.lVal );
	if( Value.vt == VT_BOOL )
	{
		if( Value.boolVal==VARIANT_FALSE )return (bExPatyMode = true);
		else return EquipJob();
	}
	return false;
}

bool EquipStatusWindow::ClickJob( int x, int y )
{
	if( Rects.Wepon.Pin(x,y) )return ShowItemList( 1 );
	if( Rects.Body.Pin(x,y) )return ShowItemList( 2 );
	if( Rects.Acs.Pin(x,y) )return ShowItemList( 3 );

	return StatusWindow::ClickJob( x,y );
}

bool EquipStatusWindow::ShowItemList( short Type ) const
{
	RbWindowSystem& System = GetWindowSystem();
	RefPtr<BaseWindow> pItemList = StrCommander().ShowWindow(_T("ItemList"));
	RefPtr<BaseWindow> pShopList = StrCommander().ShowWindow(_T("ShopList"));

	String EquipStrng;
	if( pStatus != NULL )
	{
		RbMember Mem( pStatus->MemberState() );
		EquipStrng = Mem.GetEquip();
	}

	if( pItemList != NULL )
	{
		pItemList->SetParameter( Type );
		pItemList->SetParameter( (LPCTSTR)EquipStrng );
	}
	if( pShopList != NULL )
	{
		pShopList->SetParameter( Type );
		pShopList->SetParameter( (LPCTSTR)EquipStrng );
	}
	return true;
}

HRESULT EquipStatusWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( !RestoreStatus() )
	{
		Close( System );
		return E_FAIL;
	}
	return StatusWindow::Restore( pDevice, System );
}

bool EquipStatusWindow::SetMember( LPCTSTR PasonName )
{
	MemberStatus Mem;
	if( !UserData.GetMember( PasonName, Mem ) )return false;
	if( EquipItem.MakeLong() == 0 && MemberName == PasonName )return true; 
	MemberName = PasonName;
	bExPatyMode = false;

	SetItem( 0 );
	TexWindow.Release();
	return true;
}

bool EquipStatusWindow::SetItem( long ItemLong )
{
	AllZero( EquipItem );
	EquipItem.SetLong( ItemLong );
	TexWindow.Release();
	return true;
}

bool EquipStatusWindow::DropJob( const DragItemWindow& DragItem )
{
	if( DragItem.DragType() == eDragMember )
		return SetMember( DragItem.DragData().Member.PasonName );
	if( DragItem.DragType() == eDragItem )
	{
		if( !SetItem( DragItem.DragData().Item.id.MakeLong() ) )return false;
		return EquipJob();
	}
	return false;
}

bool EquipStatusWindow::RestoreStatus()
{
	OldUpdateID = UserData.UpDateID();

	MemberStatus Mem;
	if( !UserData.GetMember( MemberName, Mem ) )return false;

	MemberStatus Leader;
	if( bExPatyMode )
	{
		if( !UserData.GetMember( UserData.RefExPatyNum(), 0, Leader ) )return false;
	}else
	{
		if( !UserData.GetLeader( MemberName, Leader ) )return false;
	}

	RefPtr<RbStatus> pNewStatus( new RbStatus );
	if( Mem.PasonName == Leader.PasonName )pNewStatus->SetStatus( Mem );
	else pNewStatus->SetStatus( Mem, Leader );

	AllZero( EquipDiff );
	pStatus = pNewStatus;

	RbMember EqMem(Mem);
	if( !EqMem.SetItem( EquipItem ) )return true;

	RefPtr<RbStatus> pEquipStatus( new RbStatus );
	if( Mem.PasonName == Leader.PasonName )pEquipStatus->SetStatus( Mem );
	else pEquipStatus->SetStatus( Mem, Leader );

	EquipDiff = pEquipStatus->NowState();
	EquipDiff -= pNewStatus->NowState();
	pStatus = pNewStatus;
	return true;
}

void EquipStatusWindow::DrawValue( WindowSystem& System, const WindowPlanes* pPlanes, const String& Name ) const
{
	float DiffValue = GetValue(EquipDiff,Name);
	if( DiffValue == 0 )
	{
		StatusWindow::DrawValue( System, pPlanes, Name );
		return;
	}

	if( pPlanes == NULL )return;
	if( pStatus == NULL )return;
	RbRect AllRect = pPlanes->GetRect(Name);
	if( AllRect.IsNone() )return;
	RbPoint LT=AllRect.LT();

	System.Font().Draw( TagFont, Name, Rects.Tag+LT, DT_CENTER );
	float AllValue = GetValue(pStatus->NowState(),Name)+DiffValue;

	LPCTSTR UpDown = _T("▲ %.0f");
	ARGB Color = 0xff00ffff;
	if( DiffValue < 0 )
	{
		UpDown = _T("▼ %.0f");
		Color.color = 0xffff0000;
		DiffValue = -DiffValue;
	}
	String Format;
	Format.Format( UpDown, DiffValue );

	DrawFloat( System, NumFont, AllValue, Rects.Now+LT, DT_RIGHT|DT_VCENTER );
	System.Font().Draw( NumFont, Format, Rects.UpDown+LT, DT_LEFT|DT_VCENTER, Color );
}

bool EquipStatusWindow::EquipJob()
{
	bool bRet = UserData.ChangeEquip( MemberName, EquipItem );
	SetItem( 0 );
	return bRet;
}

bool EquipStatusWindow::UnEquipJob( int x, int y )
{
	if( Rects.Body.Pin(x,y) )return UnEquipJob(2);
	if( Rects.Acs.Pin(x,y) )return UnEquipJob(3);
	if( Rects.Wepon.Pin(x,y) )
	{
		SystemPopup( _T("wepislock") );
		return true;
	}
	return false;
}

bool EquipStatusWindow::UnEquipJob( short Type )
{
	ItemID UnEquip;
	UnEquip.EquipType = Type;
	UnEquip.Number = 0;
	EquipItem = UnEquip;
	return EquipJob();
}
