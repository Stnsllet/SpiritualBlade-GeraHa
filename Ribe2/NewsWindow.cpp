// NewsWindow.cpp: NewsWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbWindowSystem.h"
#include "RbCache.h"
#include "NewsWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

NewsWindow::NewsWindow()
{
}

NewsWindow::~NewsWindow()
{
}

LPCTSTR NewsWindow::WindowName() const
{
	return _T("News");
}

bool NewsWindow::CheckParameter( const CComVariant& Value ) const
{
	if( Value.vt != VT_BSTR )return false;
	return ( NewsName == String(Value.bstrVal) );
}

bool NewsWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt != VT_BSTR )return false;
	String Name = Value.bstrVal;
	if( Name.empty() )return false;
	if( NewsName == Name )return true;

	WindowSystem& System = GetWindowSystem();
	const RbPointF& Pos = GetFloatPosition( System );
	if( !ReadNews( System, GetFilePath(Name) ) )return false;
//	if( (long)GetFileAttributes(GetFilePath(Name)) == -1 )return false;

	SetFloatPosition( System, Pos.x, Pos.y );
	NewsName = Name;
	TexWindow.Release();
	return true;
}

String NewsWindow::ReadTitle( const String& NewsName ) const
{
	XMLPaser ReadXML;
	if( FAILED(ReadXML.Load( GetFilePath(NewsName) )) )return String();

	return String(ReadXML.AttributeString(_T("title")));
}

bool NewsWindow::SetDefaultPosition( WindowSystem& System )
{
	const WindowPlanes* pDefine = System.Define().GetWindowPlanes( WindowName() );
	if( pDefine == NULL )return false;
	SetFloatPosition( System, pDefine->Base.Plane.x/100.0f, pDefine->Base.Plane.y/100.0f );
	return true;
}

String NewsWindow::GetFilePath( const String& name ) const
{
	String FilePath = _T("Data/News/");
	FilePath += name;
	FilePath += _T(".xml");

	return FilePath;
}

HRESULT NewsWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( Msg.Msg.empty() )if( !ReadNews( System, GetFilePath(NewsName) ) )return S_FALSE;

	ExSprite Surface;
	HRESULT hr = RestoreBase( pDevice, Surface, System );
	if( FAILED(hr) )return hr;
	DrawWindowTag(System, _T("Document"));

	Surface.Draw( ImagePlane, RbRect(ImagePlane), ImageRect );
	System.Font().Draw( Title.Font, Title.Msg, Title.MsgRect, DT_CENTER );
	System.Font().Draw( Msg.Font, Msg.Msg, Msg.MsgRect );
	return S_OK;
}

bool NewsWindow::MsgData::SetXML( const XMLPaser& BaseXML, LPCTSTR Name )
{
	if( Name == NULL )return false;
	String Format;
	Format.Format( _T("Plane[@name='%s']"), Name );
	XMLPaser PlaneXML;
	if( FAILED(BaseXML.Find( CComBSTR(Format), PlaneXML )) )return false;
	Area = RbRect( Plane<>().ReadXML(PlaneXML) );
	Font = PlaneXML.AttributeString(_T("font"));
	if( Font.empty() )Font = _T("default");
	return true;
}

bool NewsWindow::MsgData::SetMSG( WindowSystem& System, const String& NewMsg )
{
	if( NewMsg.empty() )return false;
	MsgRect = System.Font().GetTextRect(Font,NewMsg);
	MsgRect.Move( Area.left, Area.top );
	Msg = NewMsg;
	return true;
}

bool NewsWindow::ReadNews( WindowSystem& System, LPCTSTR FilePath )
{
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return false;

	Msg.SetXML( pPlanes->XML, _T("msg") );
	Title.SetXML( pPlanes->XML, _T("title") );

	RbRect AllArea = Title.Area;
	AllArea.OR( Msg.Area );

	XMLPaser ReadXML;
	if( FAILED(ReadXML.Load( FilePath )) )return false;

	Title.SetMSG( System, String(ReadXML.AttributeString(_T("title"))) );
	Msg.SetMSG( System, String(ReadXML.TextString()) );

	ImagePlane = AllZero(Plane<>());
	String ImageName = ReadXML.AttributeString(_T("img"));
	if( !ImageName.empty() )
	{
		RbCache& Cache = GetCache();
		ImagePlane = Cache.PathTex.Get( ImageName );
		if( ImagePlane.pTexture == NULL )
			ImagePlane = Cache.SystemPlane.GetPlane( ImageName );
		if( ImagePlane.pTexture == NULL )
			ImagePlane = Cache.Face.Get( ImageName );
	}

	ImageRect = RbRect( Title.MsgRect.left, Title.MsgRect.top, ImagePlane.w, ImagePlane.h );
	Title.MsgRect.Move( ImagePlane.w, 0 );

	RbRect AllMsgRect = Title.MsgRect;
	AllMsgRect.OR( ImageRect );
	const RbPoint& Center = AllMsgRect.Center();
	Title.MsgRect.SetCenterY( Center.y );
	ImageRect.SetCenterY( Center.y );

	int MsgShift = AllMsgRect.Height() - Title.Area.Height();
	Msg.MsgRect.Move( 0, MsgShift );

	AllMsgRect.OR( Msg.MsgRect );
	Title.MsgRect.ReSize( AllMsgRect.Width()-ImagePlane.w, Title.MsgRect.Height() );

	Position.Width = pPlanes->Base.Plane.w + AllMsgRect.Width() - AllArea.Width();
	Position.Height = pPlanes->Base.Plane.h + AllMsgRect.Height() - AllArea.Height();

	return true;
}
