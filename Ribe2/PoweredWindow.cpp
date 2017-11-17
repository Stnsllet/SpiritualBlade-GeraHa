// PoweredWindow.cpp: PoweredWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "RbTimer.h"
#include "RbCache.h"
#include "RbWindowSystem.h"
#include "PoweredWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

PoweredWindow::PoweredWindow()
{
	BackColor.color = 0x60000000;
	bMoveOK = bDestroyOK = false;
	TopLevel = 5;
	FloatPos.bLock = true;

	StartTime = RbTimer();
	Life = 4000;
}

PoweredWindow::~PoweredWindow()
{

}

LPCTSTR PoweredWindow::WindowName() const
{
	return _T("Powered");
}

HRESULT PoweredWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	ARGBF DrawColor;
	double Passed = RbTimer().Passed( StartTime );
	if( Life > 0 && Passed >= Life )
	{
		Command(NULL, System, eDestroy );
		return S_FALSE;
	}

	if( Passed <= 500 )DrawColor.a = Passed/500;
	if( Life > 0 && Passed >= Life-500 )DrawColor.a = (Life-Passed)/500;

	DrawColor = Display.SetDrawColor( DrawColor );
	HRESULT hr = BaseWindow::Draw( Display, System );
	Display.SetDrawColor( DrawColor );
	return hr;
}

bool PoweredWindow::SetXML( const XMLPaser& MsgXML )
{
	FloatPos.x = MsgXML.AttributeFloat(_T("x"))/100;
	FloatPos.y = MsgXML.AttributeFloat(_T("y"))/100;
	Position.Width = MsgXML.AttributeLong(_T("w"));
	Position.Height = MsgXML.AttributeLong(_T("h"));
	Life = MsgXML.AttributeFloat(_T("life"));

	if( MsgXML.HasAttribute(_T("col")) )BackColor.color = MsgXML.AttributeHex(_T("col"));

	XMLList PowXMLList;
	PowXMLList.Query( MsgXML, _T("Tag") );
	long Count = PowXMLList.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		AddPow( PowXMLList.GetItem(i) );
	}
	return (PowList.size()>0);
}

bool PoweredWindow::AddPow( const XMLPaser& PowXML )
{
	PowMsg AddData;
	AddData.Rect = Plane<>().ReadXML( PowXML );
	AddData.TexPath = PowXML.AttributeString(_T("tex"));

	AddData.Msg = PowXML.AttributeString(TAG::MSG);
	UserOrStr(AddData.Msg);
	if( AddData.Msg.empty() && AddData.TexPath.empty() )return false;

	AddData.Color.color = MYWHITE;
	if( PowXML.HasAttribute(_T("col")) )AddData.Color.color = PowXML.AttributeHex(_T("col"));
	AddData.Font = PowXML.AttributeString(_T("font"));
	if( AddData.Font.empty() )AddData.Font = _T("default");
	PowList.push_back( AddData );
	return true;
}

// レストア
HRESULT PoweredWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;

	HRESULT ret = TexWindow.Create( pDevice, Position.Width, Position.Height );
	if( FAILED(ret) )return ret;

	ExSprite Surface;
	ret = Surface.InitSprite( pDevice, TexWindow );
	if( FAILED(ret) )return ret;

	Surface.ColorClear( BackColor );

	std::list<PowMsg>::iterator itr = PowList.begin();
	while( itr != PowList.end() )
	{
		PowMsg& Ref = (*itr++);
		if( !Ref.TexPath.empty() )
		{
			Plane<> Tex = GetCache().SystemPlane.GetPlane(Ref.TexPath);
			if( Tex.pTexture != NULL )
			{
				if( Ref.Rect.IsNone() )Ref.Rect.ReSize( Tex.w, Tex.h );
				Surface.DrawPlane( Tex, Ref.Rect.left, Ref.Rect.top );
			}
			else
			{
				Plane<> Tex = GetCache().PathTex.Get(Ref.TexPath);
				if( Ref.Rect.IsNone() )Ref.Rect.ReSize( Tex.w, Tex.h );
				Surface.Draw( Tex, Ref.Rect );
			}
			continue;
		}
		if( Ref.Rect.IsNone() )
		{
			RbRect TextRect = System.Font().GetTextRect(Ref.Font,Ref.Msg);
			Ref.Rect.ReSize( TextRect.Width(), TextRect.Height() );
		}

		System.Font().Draw(Ref.Font,Ref.Msg,Ref.Rect,DT_LEFT,Ref.Color);
	}

	return S_OK;
}
