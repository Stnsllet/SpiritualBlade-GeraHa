// DischargeWindow.cpp: DischargeWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbMember.h"
#include "RbWindowSystem.h"
#include "RbGameData.h"
#include "RbCache.h"
#include "RbSound.h"
#include "DischargeWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

DischargeWindow::DischargeWindow()
{
	AllZero(TagetMember);
	AcceptString = _T("解雇");
}

DischargeWindow::~DischargeWindow()
{
}

LPCTSTR DischargeWindow::WindowName() const
{
	return _T("Discharge");
}

bool DischargeWindow::CheckParameter( const CComVariant& Value ) const
{
	if( Value.vt != VT_BSTR )return false;
	return (String(Value.bstrVal) == (LPCTSTR)TagetMember.PasonName);
}

bool DischargeWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt != VT_BSTR )return false;
	MemberStatus Mem;
	if( !GetGameData().GetMember( String(Value.bstrVal), Mem ) )return false;
	TagetMember = Mem;
	TexWindow.Release();
	return true;
}

HRESULT DischargeWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_FALSE;

	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	ExSprite Surface;
	HRESULT hr = RestoreBase( pDevice, Surface, System );
	if( FAILED(hr) )return hr;

	return RestoreWindow( Surface, System, pPlanes );
}

HRESULT DischargeWindow::RestoreWindow( RbSprite& Surface, WindowSystem& System, const WindowPlanes* pPlanes )
{
	if( pPlanes == NULL )return E_FAIL;
	DrawWindowTag(System,WindowName());

	RbRect LevRect = pPlanes->GetRect( _T("level") );
	RbRect NameRect = pPlanes->GetRect( _T("name") );
	RbRect JobRect = pPlanes->GetRect( _T("job") );
	RbRect Image = pPlanes->GetRect( _T("img") );
	AcceptRect = pPlanes->GetRect( _T("accept") );

	if( TagetMember.PasonName.IsNone() )return S_FALSE;

	RbMember DrawMem(TagetMember);
	const String& PasonName = DrawMem.PasonData().Name;
	const String& JobName = DrawMem.JobData().Name;

	Surface.DrawRbImage( TagetMember, Image );
	String Format;
	Format.Format( _T("Level %2d"), TagetMember.Level );
	System.Font().Draw( WindowTagFont, Format, LevRect, DT_CENTER );

	System.Font().Draw( PasonName, NameRect, DT_CENTER );
	System.Font().Draw( JobName, JobRect, DT_CENTER );

	System.Font().Draw( AcceptString, AcceptRect, DT_CENTER );
	return S_OK;
}

RbRect DischargeWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( AcceptRect.Pin(x,y) )return AcceptRect;
	return RbRect();
}

bool DischargeWindow::ClickJob( int x, int y )
{
	if( !AcceptRect.Pin(x,y) )return true;
	GetSoundSystem().Play(_T("click"));
	if( AcceptJob() )
		Command( NULL, GetWindowSystem(), eDestroy );

	return true;
}

bool DischargeWindow::AcceptJob()
{
	const PasonCache& Cache = GetCache().Pason;
	long PasonID = Cache.GetPasonID( TagetMember.PasonName );
	if( Cache.IsMain( PasonID ) )
	{
		SystemPopup( _T("dontdischage") );
		return false;
	}
	return GetGameData().DischargeMember( PasonID );
}
