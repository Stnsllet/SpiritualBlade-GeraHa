// MoneyWindow.cpp: MoneyWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbWindowSystem.h"
#include "RbGameData.h"
#include "MoneyWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MoneyWindow::MoneyWindow()
{
	bDestroyOK =  bMoveOK = false;

	FloatPos.x = FloatPos.y = 1;
	FloatPos.bLock = true;

	NowMoney = 0;
}

MoneyWindow::~MoneyWindow()
{
}

LPCTSTR MoneyWindow::WindowName() const
{
	return _T("Money");
}

// レストア
HRESULT MoneyWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;

	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes( WindowName() );
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	ExSprite Surfase;
	HRESULT ret = RestoreBase( pDevice, Surfase, System );
	if( FAILED(ret) )return ret;

	DrawWindowTag( System, WindowName() );

	const WindowPlane* pMoneyPlane = System.Define().GetWindowPlane( WindowName(), _T("money") );
	if( pMoneyPlane == NULL )return E_FAIL;

	String Format;
	Format.Format(_T("%d"), NowMoney );
	System.Font().Draw( Format, RbRect(pMoneyPlane->Plane), DT_RIGHT );

	String UnitStr = GetGameData().RefSystemData().MoneyUnit;
	RbRect UnitRect = pPlanes->GetRect(_T("unit"));

	System.Font().Draw( UnitStr, UnitRect );

	return S_OK;
}

HRESULT MoneyWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	long NewMoney = GetGameData().GetMoney();
	if( NewMoney != NowMoney )
	{
		TexWindow.Release();
		NowMoney = NewMoney;
	}
	return BaseWindow::Draw( Display, System );
}
