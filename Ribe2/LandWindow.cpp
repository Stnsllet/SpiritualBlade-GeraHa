// LandWindow.cpp: LandWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTransform.h"
#include "DispStocker.h"
#include "GameSystem.h"
#include "BackSwapper.h"
#include "TexRender.h"
#include "LandWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LandWindow::LandWindow()
{
	FloatPos.bLock = true;
	bDestroyOK = bMoveOK = false;
	bOutView = true;
	LandScale = 1.0f;
	ViewScale = 1.0f;
	UpdateTime = 0;
}

LandWindow::~LandWindow()
{
}

LPCTSTR LandWindow::WindowName() const
{
	return _T("LandMap");
}

HRESULT LandWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_FALSE;
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	UpdateTime = RbTimer();

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;
	if( Position.Width <= 0 )Position.Width = 100;
	if( Position.Height <= 0 )Position.Height = 100;
	White = GetCache().SystemPlane.GetPlane(_T("white"));
	FriColor = ARGBF(pPlanes->XML.AttributeHex(_T("col")) );
	EneColor = ARGBF(pPlanes->XML.AttributeHex(_T("enecol")) );

	ViewScale = 1.0f;
	RbDisplay& Disp = GetRbDisplay();
//	Disp.FreeMemory();

	if( TexWindow == NULL )
	{
		HRESULT ret = TexWindow.Create( pDevice, Position.Width, Position.Height );
		if( FAILED(ret) )return ret;
	}

/*	{
		ExSprite BaseSurface;
		HRESULT ret = RestoreBase( pDevice, BaseSurface, System );
		FAILEDTRACE(ret);
		if( FAILED(ret) )return ret;
		DrawWindowTag( System, _T("Map") );
	}
*/
	LandRect = pPlanes->GetRect(_T("land"));
	if( LandRect.Width() <= 0 )return S_FALSE;
	LandRect.ReSize( LandRect.Width(), LandRect.Width() );

	Texture LandTex;
	HRESULT hr = LandTex.Create( pDevice, LandRect.Width(), LandRect.Height() );
	if( FAILED(hr) )
	{
		TexWindow.Release();
		return hr;
	}

	hr = DrawLand( Disp, g_pSystem->ActiveLand(), LandTex );
	if( FAILED(hr) )
	{
		TexWindow.Release();
		return hr;
	}

	ExSprite Surface;
	hr = Surface.InitSprite( pDevice, TexWindow );
	if( FAILED(hr) )return hr;
//	return Surface.Draw( LandTex, LandRect.left, LandRect.top );

	Surface.Clear();
	hr = DrawSurfaceBase( Surface, System, RbRect(0,0,Position.Width,Position.Height) );
	if( FAILED(hr) )return hr;
	DrawWindowTag( System, _T("Map") );

	Surface.Draw( LandTex, LandRect.left, LandRect.top );

	return S_OK;

}

HRESULT LandWindow::DrawLand( RbDisplay& Disp, LandScape& Land, Texture& Tex )
{
	int Width = Tex.ImageInfo().Width;
	int Height = Tex.ImageInfo().Height;

	CComPtr<MYSURFACE> pTaget;
	HRESULT hr = Tex.GetSurface( &pTaget );
	if( FAILED(hr) )return hr;

	D3DFORMAT StencilFormat = g_pSystem->DirectXSystem.GetDepthStencilFormat();

	TexRender Render;
	hr = Render.Swap( Disp, pTaget, StencilFormat );
	if( FAILED(hr) )
	{
		Render.Restore();
		Disp.FreeMemory();
		hr = Render.Swap( Disp, pTaget, StencilFormat );
	}

	FAILEDTRACE(hr);
	if( FAILED(hr) )return hr;

	RbRect SurfaceArea( Render.RefDesc().Width, Render.RefDesc().Height );
	if( SurfaceArea.Width() <= 0 || SurfaceArea.Height() <= 0 )return E_FAIL;

	const RbRectF& LandArea = Land.Map.LandArea();
	if( LandArea.Width() <= 0 || LandArea.Height() <= 0 )return E_FAIL;

	RbPoint CenterShift = LandRect.Center() - SurfaceArea.Center();
	RbTransform WorldTrans;
	WorldTrans.Scale( LandRect.Width()/(float)SurfaceArea.Width(),
		LandRect.Height()/(float)SurfaceArea.Height(), 1 );

	LandScale = 1/(float)max(LandArea.Width(),LandArea.Height());
	ViewScale = LandScale*LandRect.Width();
	ATLTRACE("LandWnd Scale %.3f\n", LandScale );
	return DrawLand( Disp, Land, LandArea.Center(), LandScale, WorldTrans );
}

HRESULT LandWindow::DrawLand( RbDisplay& Disp, LandScape& Land, const RbPointF& Center, float Scale, const RbTransform& WorldTrans )
{
	if( Scale <= 0 )Scale = 0.001f;
	Scale = 1/Scale;
	DispStocker::Transform VT( Disp, D3DTS_VIEW );
	DispStocker::Transform PT( Disp, D3DTS_PROJECTION );
	DispStocker::Transform WT( Disp, D3DTS_WORLD );

	RbTransform ProjMat;
//	D3DXMatrixPerspectiveLH( &ProjMat, 1,1, 1, Scale*2);
	D3DXMatrixPerspectiveOffCenterLH( &ProjMat, -0.5,0.5,-0.5,0.5, 1, Scale*2 );
	ProjMat.AddScroll( 0, 0, -1.0 );
	ProjMat.Scale( 1, 1, Scale/2.0f );
	ProjMat.AddScroll( 0, 0, 0.75 );

	RbTransform ViewMat;
	D3DXMatrixLookAtLH(&ViewMat
		,&D3DXVECTOR3(Center.x,Center.y,Scale)	// カメラ位置
		,&D3DXVECTOR3(Center.x,Center.y,0)		// カメラの注目点
		,&D3DXVECTOR3(0,-1,0)					// 上の向き
		);

	PT.Set( ProjMat );
	VT.Set( ViewMat );
	WT.Set( WorldTrans );

	DispStocker::ViewTransSet View( Disp );

	return DrawLand( Disp, Land );
}

bool LandWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	if( GetWindowSystem().OnMsg() )return true;
	if( State.IsDraging() )return BaseWindow::MouseJob( System, State );
	switch( State.Msg() )
	{
	case eMouseMove:
		if( !State.LDown )break;
	case eLDown:
		if( !LandRect.Pin(State.x-Position.x,State.y-Position.y) )break;
		Scroll( State.x, State.y );
		return true;
	}
	return BaseWindow::MouseJob( System, State );
}

bool LandWindow::Scroll( int x, int y ) const
{
	TransData Trans( LandRect.Shift( Position.x, Position.y ), ViewScale );
	RbPointF Pos = Trans.ToView( x, y );
	float z = Trans.Land.GetHight(Pos.x,Pos.y);
	Trans.Disp.SetViewPos( Pos.x, Pos.y, z );

	return true;
}

HRESULT LandWindow::DrawLand( RbDisplay& Disp, LandScape& Land )
{
	DispStocker::Render Fog( Disp, D3DRS_FOGENABLE );
	Fog.Set( FALSE );
	DispStocker::Render Ambient( Disp, D3DRS_AMBIENT );
	Ambient.Set( Disp.RefAbientColor() );
	DispStocker::Render ZEnable( Disp, D3DRS_ZENABLE );
	ZEnable.Set( TRUE );
	DispStocker::Render CullMode( Disp, D3DRS_CULLMODE );
	DispStocker::Material MateStock( Disp );
	MateStock.Set( Disp.RefMaterial() );
	DispStocker::Render Normal( Disp, D3DRS_NORMALIZENORMALS );
	Normal.Set( TRUE );

	HRESULT hr = Disp.BeginScene(false);
//	if( FAILED(hr) )return hr;
	bool bBegin = SUCCEEDED(hr);

	Disp->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(0,0,0,0),1.0f,0);

	hr = Land.Draw( Disp );
	hr = Land.DrawGhost( Disp );
	if( bBegin )Disp.EndScene();
	return hr;
}

HRESULT LandWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( g_pSystem->ActiveLand().IsUpDate( UpdateTime ) )
		TexWindow.Release();

	HRESULT hr = BaseWindow::Draw( Display, System );
	if( FAILED(hr) )return hr;

	if( ViewScale >= 1 )return S_FALSE;

	ARGBF Old = Display.SetDrawColor( ARGBF() );

	TransData Trans( LandRect.Shift( Position.x, Position.y ), ViewScale );

	hr = DrawPoints( Display, Trans );
	hr = DrawViewArea( Display, Trans );

	Display.SetDrawColor( Old );

	return hr;
}

HRESULT LandWindow::DrawViewArea( RbSprite& Display, const TransData& Trans )
{
	POINT Size = Display.GetSize();
	RbPosition Center = Trans.Disp.GetViewPos();

	float Scale = Trans.Disp.GetDisplayScale( Center );
	if( Scale <= 0 )Scale = 1.0f;

	RbPointF Pos = Trans.ToDisplay( Center.x, Center.y );

	float ScaleX = Size.x/Scale*ViewScale;
	float ScaleY = Size.y/Scale*ViewScale;
	RbRect Taget( Pos.x-ScaleX/2, Pos.y-ScaleY/2, ScaleX, ScaleY );
	Taget.Clip( GetRect() );

	ARGBF Color( 0.25f, 0, 0, 0 );
//	Color.color = 0;
//	Color.a() = 64;
	Display.SetDrawColor( Color );
	return Display.Draw( White, RbRect(White), Taget );
}

HRESULT LandWindow::DrawPoints( RbSprite& Display, const TransData& Trans )
{
	RbRect WhiteRect( White );
	const BattleSystem& System = GetBattleSystem();
	const BattleDataManager::CharaList& List = System.RefAllCharactor();

	BattleDataManager::CharaList::const_iterator itr = List.begin();
	while( itr != List.end() )
	{
		const RbActStatus& Ref = *(*itr++);
		if( Ref.AlgoMove.IsUnRadar() )continue;
		if( Ref.IsDead() )continue;
		float Size = Ref.Size() * ViewScale;
		if( Size < 1 )Size = 1.0f;
		const RbPosition& RefPos = Ref.GetPosition();
		const RbPointF& Pos = Trans.ToDisplay( RefPos.x, RefPos.y );

		const ARGBF& Col = (Ref.GetArmyID()==0)
			?(FriColor):(EneColor);

		RbRect Taget( Pos.x-Size/2, Pos.y-Size/2, Size, Size );
		Display.SetDrawColor( Col );
		Display.Draw( White, WhiteRect, Taget );
	}
	return S_OK;
}

LandWindow::TransData::TransData( const RbRect& RefDrawArea, float RefViewScale )
: Disp( GetRbDisplay() )
, Land( g_pSystem->ActiveLand() )
, DrawArea( RefDrawArea )
{
	Scale = RefViewScale;
	LandArea = Land.Map.LandArea();
	LandCenter = LandArea.Center();
	DrawCenter.x = (DrawArea.right + DrawArea.left)/2.0f;
	DrawCenter.y = (DrawArea.bottom + DrawArea.top)/2.0f;
}

RbPointF LandWindow::TransData::ToDisplay( float x, float y ) const
{
	RbPointF Ret( x, y );
	Ret -= LandCenter;
	Ret *= Scale;
	Ret += DrawCenter;
	return Ret;
}

RbPointF LandWindow::TransData::ToView( float x, float y ) const
{
	RbPointF Ret( x, y );
	if( Scale < 0 )return Ret;
	Ret -= DrawCenter;
	Ret /= Scale;
	Ret += LandCenter;
	return Ret;
}

