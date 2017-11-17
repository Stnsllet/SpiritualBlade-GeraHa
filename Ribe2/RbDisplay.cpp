// RbDisplay.cpp: RbDisplay クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbConfig.h"
#include "RbTimer.h"
#include "XMLList.h"
#include "Material.h"
#include "RbDisplay.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbDisplay::RbDisplay()
{
	BackColor = D3DCOLOR_ARGB( 0xff, 0xdf, 0xff, 0xdf );

	AllZero( Zoom );
	Zoom.Base = 1;
	Zoom.Now = Zoom.Base;
	AllZero( AngleInfo );
	AngleInfo.Base = 30;
	AngleInfo.Now = AngleInfo.Base;

	AllZero( Shake );
	AllZero( Scroll );
	AllZero( ViewLock );

	JobInterval = 20;
	JobOldTime = 0;
	DefaltScale = 1.0;
}

RbDisplay::~RbDisplay()
{
}

bool RbDisplay::LoadSystemXML( const XMLPaser& ReadXML )
{
	SetDefaultScale( ReadXML.AttributeFloat(_T("scale")) );
	SetDefaultAngle( ReadXML.AttributeFloat(_T("angle")) );
	SetFishEye( ReadXML.AttributeFloat(_T("fisheye")) );
	BackColor = ReadXML.AttributeHex(_T("backcolor"));
	Material ReadMaterial;
	if( ReadMaterial.SetXML( ReadXML ) )
		DispMaterial = ReadMaterial;
	XMLPaser FindXML;
	if( !FAILED(ReadXML.Find(_T("BillBord"),FindXML)) )
	{
		Material ReadMaterial;
		if( ReadMaterial.SetXML( FindXML ) )
			PlaneMaterial = ReadMaterial;
	}

	return true;
}

bool RbDisplay::SetDefaultScale( float Scale )
{
	if( Scale <= 0 )return false;
	DefaltScale = Scale;
	return true;
}

bool RbDisplay::SetDefaultAngle( float angle )
{
	if( angle <= 0 )return false;
	AngleInfo.Base = angle;
	return true;
}

void RbDisplay::SetShake( float Scale, double Life )
{
	if( Shake.Life <= 0 )Shake.Scale = Scale;
	else Shake.Scale = max(Shake.Scale,Scale);

	if( Life <= 0 )Shake.Life = 0;
	else Shake.Life = max(Shake.Life,Life);
	Shake.StartTime = RbTimer();
}

// ズームイン設定
void RbDisplay::SetZoom( float NewZoom, double Life )
{
	if( NewZoom > 0 )Zoom.Dest = 1 / NewZoom;
	else Zoom.Dest = Zoom.Base;
	if( Life <=0 )
	{
		Zoom.Now = Zoom.Dest;
		ResetViewData();
	}
	Zoom.Life = Life;
	Zoom.StartTime = RbTimer();
}

// ズームイン設定
void RbDisplay::SetAngle( float NewAngle, double Life )
{
	if( NewAngle > 0 )AngleInfo.Dest = NewAngle;
	else AngleInfo.Dest = AngleInfo.Base;
	if( Life <=0 )
	{
		AngleInfo.Now = AngleInfo.Dest;
		ResetViewData();
	}
	AngleInfo.Life = Life;
	AngleInfo.StartTime = RbTimer();
}

// 表示領域設定
void RbDisplay::SetViewArea( const RbRectF& Area )
{
	ViewArea.Area = Area;
}

// 表示スクロールロック設定
void RbDisplay::LockView( const D3DVECTOR& Pos, double Life )
{
	if( (++ViewLock.LockCount) == 1 )
	{
		ViewLock.OldPos = Scroll.Dest;
	}
	SetViewOn( Pos, Life );
}

// 表示スクロールロック解除
void RbDisplay::UnLockView( double Life )
{
	if( ViewLock.LockCount <= 0 )return;
	if( (--ViewLock.LockCount) <= 0 )
	{
		SetViewOn( ViewLock.OldPos, Life );
		ViewLock.LockCount = 0;
	}
}

// 表示スクロール設定
void RbDisplay::SetViewOn( const D3DVECTOR& Pos, double Life )
{
	Scroll.Dest = Pos;
	Scroll.Life = Life;
	if( Life <=0 )
	{
		SetViewPos( Pos.x, Pos.y, Pos.z );
	}
	Zoom.StartTime = RbTimer();
}

// 表示位置設定
HRESULT RbDisplay::SetViewPos( float x, float y, float z )
{
	Scroll.Dest.x = x;
	Scroll.Dest.y = y;
	Scroll.Dest.z = z;
	HRESULT hr = Display::SetViewPos( x, y, z );
	ClipView();
	return hr;
}

// スクロール
HRESULT RbDisplay::ScrollView( float x, float y, float z )
{
	Scroll.Dest.x += x;
	Scroll.Dest.y += y;
	Scroll.Dest.z += z;
	HRESULT hr = Display::ScrollView( x, y, z );
	ClipView();
	return hr;
}

// 表示クリップ
bool RbDisplay::ClipView()
{
	const RbPosition& Now = GetViewPos();
	if( ViewArea.Area.Pin( Now.x, Now.y ) )return false;
	RbPosition NewPos = Now;
	NewPos.x = Trim( Now.x, ViewArea.Area.right-1, ViewArea.Area.left+1);
	NewPos.y = Trim( Now.y, ViewArea.Area.bottom-1, ViewArea.Area.top+1);
	Display::SetViewPos( NewPos.x, NewPos.y, NewPos.z );
	return true;
}

// ゆれ取得
float RbDisplay::GetShake() const
{
	if( Shake.Life <= 0 )return 0;
	return Shake.Scale;
}

// クリア
void RbDisplay::ClearJob()
{
	Scroll.Life = AngleInfo.Life = Zoom.Life = Shake.Life = 0;
	ViewLock.LockCount = 0;
}

// 仕事
HRESULT RbDisplay::Job()
{
	RbTimer NewTime;
	double Passed = NewTime.Passed( JobOldTime );
	if( Passed < JobInterval )return S_OK;

	bool ChangeAngle = false;
	if( Shake.Life > 0 )
	{
		Shake.Life -= Passed;
	}
	if( Zoom.Life > 0 )
	{
		Zoom.Now = Sizing( Zoom.Dest, Zoom.Now, Passed, Zoom.Life );
		Zoom.Life -= Passed;
		ChangeAngle = true;
	}
	if( AngleInfo.Life > 0 )
	{
		AngleInfo.Now = Sizing( AngleInfo.Dest, AngleInfo.Now, Passed, AngleInfo.Life );
		AngleInfo.Life -= Passed;
		ChangeAngle = true;
	}
	if( Scroll.Life > 0 )
	{
		RbPosition Now = GetViewPos();
		Now = Sizing( Scroll.Dest, Now, Passed, Scroll.Life );
		Scroll.Life -= Passed;
		Display::SetViewPos( Now.x, Now.y, Now.z );
	}

	if( ChangeAngle )ResetViewData();

	JobOldTime = NewTime;
	return S_OK;
}

// 画面スケール変更
HRESULT RbDisplay::ChangeDisplayBaseScale( float NewScale, bool bNow )
{
	if( NewScale <= 0 ) return E_INVALIDARG;
	NewScale = 1 / NewScale;
	Zoom.Base = NewScale;
	if( !IsViewLock() )
		if( bNow )
		{
			Zoom.Now = Zoom.Base;
			ResetViewData();
		}
		else SetZoom( 1/NewScale );

	return S_OK;
}

HRESULT RbDisplay::SetRenderState( const D3DCAPS8& HalCaps )
{
	HRESULT ret = Display::SetRenderState( HalCaps );
	if( FAILED(ret) )return ret;

	ResetViewData();
	return S_OK;
}

HRESULT RbDisplay::SetConfig( RbConfig& Config ) const
{
	Config.Display.Width = PresentParams.BackBufferWidth;
	Config.Display.Height = PresentParams.BackBufferHeight;
	return S_OK;
}

// 変更にあわせる
void RbDisplay::ChangeDispSize( float Width, float Height )
{
	if( Width <= 0 || Height <= 0 )return;

	float Length = Height;
	Length *= (DefaltScale * Zoom.Now );

	HRESULT ret = UpView( Length, AngleInfo.Now );
	if( FAILED(ret) )return;

	float aspect = Width / Height;
	ret = SetProjection( aspect );
	if( FAILED(ret) )return;

	return;
}

// 変更にあわせる
bool RbDisplay::ResetViewData()
{
	ChangeDispSize( (float)NowViewPort.Width,
		(float)NowViewPort.Height );
	return true;
}

// ウインドウサイズを変更
HRESULT RbDisplay::WindowSizeChange( UINT Width, UINT Height )
{
	HRESULT ret = Device::WindowSizeChange( Width, Height );
	if( FAILED(ret) )return ret;

	ChangeDispSize( Width, Height );

	return S_OK;
}

// フルスクリーンに変更
HRESULT RbDisplay::FullScreenWindow( const D3DDISPLAYMODE& DisplayMode )
{
	return FullScreenWindow( DisplayMode, DisplayMode.Width, DisplayMode.Height );
}

// フルスクリーンに変更
HRESULT RbDisplay::FullScreenWindow( const D3DDISPLAYMODE& DisplayMode, UINT Width, UINT Height )
{
	Width = Height * ((float)DisplayMode.Width/(float)DisplayMode.Height);
	HRESULT ret = Device::FullScreenWindow( DisplayMode, Width, Height );
	if( FAILED(ret) )return ret;

	ChangeDispSize( Width, Height );

	return S_OK;
}
