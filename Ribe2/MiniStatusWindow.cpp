// MiniStatusWindow.cpp: MiniStatusWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "RbCache.h"
#include "RbWindowSystem.h"
#include "StatusWindow.h"
#include "MiniStatusWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MiniStatusWindow::MiniStatusWindow()
{

}

MiniStatusWindow::~MiniStatusWindow()
{
}

LPCTSTR MiniStatusWindow::WindowName() const
{
//	return _T("Status");
	return _T("MiniStatus");
}

// レストア
HRESULT MiniStatusWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;

	LastRedraw = RbTimer();
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes( WindowName() );
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	ExSprite Surfase;
	RestoreBase( pDevice, Surfase, System );

	DrawWindowTag( System, _T("Status") );

	if( pStatus == NULL )return E_FAIL;
	UpdateID = pStatus->GetUpdateID();
	const RbCache& Cache = GetCache();
	const TextFont& Font = System.Font();

	ImageArea = pPlanes->GetRect(_T("image"));

	Rects.Name = pPlanes->GetRect(_T("name"));

	Rects.Tag = pPlanes->GetRect(_T("tag"));
	Rects.Value = pPlanes->GetRect(_T("value"));
	Rects.Now = pPlanes->GetRect(_T("now"));
	Rects.Slash = pPlanes->GetRect(_T("slash"));
	Rects.Source = pPlanes->GetRect(_T("src"));

	const MemberStatus& Refmem = pStatus->MemberState();
	const String& PasonName = Cache.Pason.Get(Refmem.PasonName).Name;
	Font.Draw( StatusFont, PasonName, Rects.Name, DT_CENTER );

	RbRect Rect = pPlanes->GetRect(_T("Level"));
	Font.Draw( TagFont, _T("Level"), Rects.Tag+Rect.LT(), DT_CENTER );
	Font.Draw( StatusFont, String().Format(_T("%d"),Refmem.Level),
		Rects.Value+Rect.LT(), DT_CENTER );

	DrawValue( Font, pPlanes, _T("HitPoint"), _T("HP") );
	DrawValue( Font, pPlanes, _T("MagicPoint"), _T("MP") );

	return S_OK;
}

void MiniStatusWindow::DrawValue( const TextFont& Font, const WindowPlanes* pPlanes, const String& Name, LPCTSTR ShowName ) const
{
	if( pPlanes == NULL )return;
	if( pStatus == NULL )return;
	RbRect AllRect = pPlanes->GetRect(Name);
	if( AllRect.IsNone() )return;
	RbPoint LT=AllRect.LT();

	Font.Draw( TagFont, ShowName, Rects.Tag+LT, DT_CENTER );

	Font.Draw( NumFont, _T("/"), Rects.Slash+LT, DT_CENTER|DT_VCENTER );

	float Value = GetValue(pStatus->NowState(),Name);
	Font.Draw( NumFont, String().Format(_T("%.0f"), Value), Rects.Now+LT, DT_RIGHT|DT_VCENTER );
	Value = GetValue(pStatus->AllState(),Name);
	Font.Draw( NumFont, String().Format(_T("%.0f"), Value), Rects.Source+LT, DT_LEFT|DT_VCENTER );
}

bool MiniStatusWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	switch( State.Msg() )
	{
	case eScrollDown:
		if( !bDestroyOK )break;
		{
			RefPtr<StatusWindow> pWnd(new StatusWindow);
			if( !CopyStatusWindow( System, pWnd ) )break;
			Close( System );
		}
		return true;
	}
	return StatusBaseWinow::MouseJob( System, State );
}
