// RbConfig.cpp: RbConfig クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLEditer.h"
#include "XMLList.h"
#include "String.h"
#include "RbConfig.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbConfig::RbConfig()
{
	AllZero(Volume);
	AllZero(Display);
	AllZero(Window);
	AllZero(System);
	Display.x = 0;
	Display.y = 0;
	Display.Width = 640;
	Display.Height = 480;

}

RbConfig::~RbConfig()
{

}

HRESULT RbConfig::ReadConfig()
{
	return ReadConfig( _T("system/config.xml") );
}

// 保存
HRESULT RbConfig::SaveConfig()
{
	XMLEditer SaveXML;
	HRESULT ret = SaveXML.Create( _T("config") );
	if( FAILED(ret) )return ret;

	XMLEditer DisplayXML;
	ret = SaveXML.CreateChild( _T("display"), DisplayXML );
	if( FAILED(ret) )return ret;

	ret = DisplayXML.SetAttribute( _T("width"), Display.Width );
	if( FAILED(ret) )return ret;
	ret = DisplayXML.SetAttribute( _T("height"), Display.Height );
	if( FAILED(ret) )return ret;
	ret = DisplayXML.SetAttribute( _T("scale"), Display.Scale );
	if( FAILED(ret) )return ret;
	ret = DisplayXML.SetAttribute( _T("adjust"), Display.Adjuster );
	if( FAILED(ret) )return ret;

	String Emulation;
	if( Display.bVertexSoft )Emulation += _T("v");
	if( Display.bDrawSoft )Emulation += _T("d");
	ret = DisplayXML.SetAttribute( _T("emulation"), Emulation );
	if( FAILED(ret) )return ret;

	XMLEditer BGMXML;
	ret = SaveXML.CreateChild( _T("BGM"), BGMXML );
	if( FAILED(ret) )return ret;
	ret = BGMXML.SetAttribute( _T("vol"), Volume.BGMVolume );
	if( FAILED(ret) )return ret;

	XMLEditer SoundXML;
	ret = SaveXML.CreateChild( _T("Sound"), SoundXML );
	if( FAILED(ret) )return ret;
	ret = SoundXML.SetAttribute( _T("vol"), Volume.SoundVolume );
	if( FAILED(ret) )return ret;

	XMLEditer WindowXML;
	ret = SaveXML.CreateChild( _T("Window"), WindowXML );
	if( FAILED(ret) )return ret;
	ret = WindowXML.SetAttribute( _T("ministate"), Window.bMiniStateMode );
	if( FAILED(ret) )return ret;
	ret = WindowXML.SetAttribute( _T("texno"), Window.WindowTexNo );
	if( FAILED(ret) )return ret;

	XMLEditer SystemXML;
	ret = SaveXML.CreateChild( _T("System"), SystemXML );
	if( FAILED(ret) )return ret;
	ret = SystemXML.SetAttribute( _T("nocache"), System.bDontCache );
	if( FAILED(ret) )return ret;
	ret = SystemXML.SetAttribute( _T("managetex"), System.bUseManaged );
	if( FAILED(ret) )return ret;


	return SaveXML.Save( _T("system/config.xml") );
}

HRESULT RbConfig::ReadConfig( LPCTSTR FileName )
{
	XMLPaser Config;
	HRESULT ret = Config.Load( FileName );
	if( FAILED(ret) )return ret;

	ReadDisplay( Config );
	ReadBGM( Config );
	ReadSound( Config );
	ReadWindow( Config );
	ReadSystem( Config );

	return S_OK;
}

HRESULT RbConfig::ReadDisplay( const XMLPaser& ConfigFile )
{
	XMLPaser DisplayXML;
	HRESULT ret = ConfigFile.Find( _T("display"), DisplayXML );
	if( FAILED(ret) )return ret;

	Display.Width = DisplayXML.AttributeLong("width");
	Display.Height = DisplayXML.AttributeLong("height");
	Display.Scale = DisplayXML.AttributeFloat("scale");
	if( Display.Scale < 0.5f )Display.Scale = 0.5f;
	Display.Adjuster =  DisplayXML.AttributeFloat("adjust");

	String Emulation = DisplayXML.AttributeString(_T("emulation"));
	Display.bVertexSoft = (0<=(long)Emulation.find(_T('v')));
	Display.bDrawSoft = (0<=(long)Emulation.find(_T('d')));

	return S_OK;
}

HRESULT RbConfig::ReadBGM( const XMLPaser& ConfigFile )
{
	XMLPaser BGMXML;
	HRESULT ret = ConfigFile.Find( _T("BGM"), BGMXML );
	if( FAILED(ret) )return ret;

	Volume.BGMVolume = BGMXML.AttributeLong("vol");

	return S_OK;
}

HRESULT RbConfig::ReadSound( const XMLPaser& ConfigFile )
{
	XMLPaser SoundXML;
	HRESULT ret = ConfigFile.Find( _T("Sound"), SoundXML );
	if( FAILED(ret) )return ret;

	Volume.SoundVolume = SoundXML.AttributeLong("vol");

	return S_OK;
}

HRESULT RbConfig::ReadWindow( const XMLPaser& ConfigFile )
{
	XMLPaser WindowXML;
	HRESULT ret = ConfigFile.Find( _T("Window"), WindowXML );
	if( FAILED(ret) )return ret;

	Window.WindowTexNo = WindowXML.AttributeLong(_T("texno"));
	Window.bMiniStateMode = WindowXML.AttributeBool(_T("ministate"));

	return S_OK;
}

HRESULT RbConfig::ReadSystem( const XMLPaser& ConfigFile )
{
	XMLPaser SystemXML;
	HRESULT ret = ConfigFile.Find( _T("System"), SystemXML );
	if( FAILED(ret) )return ret;

	System.bDontCache = SystemXML.AttributeBool(_T("nocache"));
	System.bUseManaged = SystemXML.AttributeBool(_T("managetex"));

	return S_OK;
}

// フルスクリーンサイズかチェック
bool RbConfig::IsFullScreen( HWND hWnd ) const
{
	RECT WindowRect;
	GetWindowRect( hWnd, &WindowRect );
	RECT ScreenRect;
	GetWindowRect( GetDesktopWindow(), &ScreenRect );
	// それ、フルスクリーン？
	return ( 0==::memcmp( &WindowRect, &ScreenRect, sizeof(RECT) ) );
}

void RbConfig::SetDisplayConfig( HWND hWnd )
{
	// それ、フルスクリーン？
	if( IsFullScreen(hWnd) )return;

	RECT WindowRect;
	GetWindowRect( hWnd, &WindowRect );

	Display.x = WindowRect.left;
	Display.y = WindowRect.top;
	Display.Width = WindowRect.right - WindowRect.left;
	Display.Height = WindowRect.bottom - WindowRect.top;
}
