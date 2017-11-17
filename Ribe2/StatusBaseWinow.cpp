// StatusBaseWinow.cpp: StatusBaseWinow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "RbTimer.h"
#include "RbWindowSystem.h"
#include "StatusBaseWinow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LPCTSTR StatusBaseWinow::StatusFont	= _T("default");
LPCTSTR StatusBaseWinow::NumFont	= _T("num");
LPCTSTR StatusBaseWinow::MinNumFont	= _T("min_num");
LPCTSTR StatusBaseWinow::TagFont	= _T("tag");

StatusBaseWinow::StatusBaseWinow()
{
	UpdateID = 0;
	LastRedraw = 0;
}

StatusBaseWinow::~StatusBaseWinow()
{
}

bool StatusBaseWinow::RedrawCheck() const
{
	if( pStatus == NULL )return false;
	if( UpdateID == pStatus->GetUpdateID() )return false;
	if( RbTimer().Passed( LastRedraw ) <= 500 )return false;
	return true;
}

bool StatusBaseWinow::CheckParameter( const CComVariant& Value ) const
{
	if( pStatus == NULL )return false;
	if( Value.vt == VT_PTR )return (pStatus==(RbStatus*)Value.byref);
	if( Value.vt == VT_BSTR )
		return (pStatus->MemberState().PasonName==String(Value.bstrVal));

	return false;
}

bool StatusBaseWinow::SetParameter( const CComVariant& Value )
{
	if( Value.vt == VT_PTR )
	{
		pStatus = (RbStatus*)Value.byref;
		TexWindow.Release();
		return true;
	}

	return false;
}

long StatusBaseWinow::GetBattleID() const
{
	return GetBattleSystem().GetBattleID( pStatus );
}

bool StatusBaseWinow::SetViewOn() const
{
	return GetBattleSystem().SetViewOn( GetBattleID() );
}

HRESULT StatusBaseWinow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( RedrawCheck() )
	{
		TexWindow.Release();
	}
	HRESULT ret = BaseWindow::Draw( Display, System );
	DrawImage( Display, ImageArea );
	return ret;
}

// イメージ描写
HRESULT StatusBaseWinow::DrawImage( RbSprite& Display, const RbRect& Rect )
{
	if( Rect.IsNone() )return S_FALSE;
	float x = (Rect.left+Rect.right)/2.0f;
	float y = Rect.bottom;
	return DrawImage( Display, x, y );
}

// イメージ描写
HRESULT StatusBaseWinow::DrawImage( RbSprite& Display, float x, float y )
{
	if( pStatus == NULL )return S_FALSE;

	const RbTexture::ActTime* pImage = pStatus->GetNowImage();
	if( pImage == NULL )return S_FALSE;

	x += Position.x;
	y += Position.y;

	return Display.DrawRbTexture( *pImage, x, y, pStatus->GetWeponImage2() );

}

float StatusBaseWinow::GetValue( const DefaultStatus& Ref, const String& Name ) const
{
	if( Name==_T("Hp") )return Ref.Hp;
	if( Name==_T("Mp") )return Ref.Mp;
	if( Name==_T("HitPoint") )return Ref.Hp;
	if( Name==_T("MagicPoint") )return Ref.Mp;
	if( Name==_T("Attack") )return Ref.Attack;
	if( Name==_T("Defence") )return Ref.Defence;
	if( Name==_T("Magic") )return Ref.Magic;
	if( Name==_T("Speed") )return Ref.Speed;
	if( Name==_T("Move") )return Ref.Move;
	return 0;
}

bool StatusBaseWinow::CopyStatusWindow( WindowSystem& System, BaseWindow* pWnd )const
{
	if( pWnd == NULL )return false;
	CComVariant Param;
	Param.vt = VT_PTR;
	Param.byref = (RbStatus*)pStatus;
	if( !pWnd->SetParameter( Param ) )return false;
	const RbPointF& Pos = GetFloatPosition( System );
	pWnd->SetFloatPosition( System, Pos.x, Pos.y );
	System.AddWindow( pWnd );
	return true;
}
