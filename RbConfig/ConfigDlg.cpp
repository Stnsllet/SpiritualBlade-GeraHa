// ConfigDlg.cpp: ConfigDlg クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include <commctrl.h>
#include "..\Ribe2\String.h"
#include "..\Ribe2\XMLList.h"
#include "ConfigDlg.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

static ConfigDlg* g_pPtr = NULL;

int ConfigDlg::DoModal( HINSTANCE hInstance )
{
	return DialogBox( hInstance, MAKEINTRESOURCE(IDD_CONFIG_DLG),
		NULL, ConfigDialogProc );
}

ConfigDlg::ConfigDlg()
{
	g_pPtr = this;
	WindowTexCount = 0;
	MinScale = 0.5f;
	MaxScale = 2.0f;
}

ConfigDlg::~ConfigDlg()
{

}

void ConfigDlg::ReadDefault()
{
	Config.ReadConfig( _T("system/DefaultConfig.xml") );
	SetConfigData();
}

BOOL CALLBACK ConfigDlg::ConfigDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam,  LPARAM lParam )
{
	return g_pPtr->DialogProc( hwndDlg, uMsg, wParam, lParam );
}

BOOL ConfigDlg::DialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_INITDIALOG:
		return InitDialog( hwndDlg );
	case WM_COMMAND:
		return Command( LOWORD(wParam), (HWND)lParam );
	case WM_HSCROLL:
		return TrackBarMsg(::GetDlgCtrlID((HWND)lParam), LOWORD( wParam ), (short)HIWORD(wParam) );
	}
	return FALSE;
}

BOOL ConfigDlg::TrackBarMsg( int ID, int Msg, int Value )
{
	switch( Msg )
	{
	default:
		Value = GetTrackBarPos( ID );
		break;
	case TB_THUMBTRACK:
	case TB_THUMBPOSITION:
		break;
	}
	switch( ID )
	{
	case IDC_DISPLAY_SLIDE:
		Config.Display.Scale = SlideToScale(Value);
		GWnd.SendCast( GlobalWindow::eDispScale, Config.Display.Scale );
		break;
	case IDC_EFFECT_SLIDE:
	case IDC_BGM_SLIDE:
		{
			if( ID == IDC_EFFECT_SLIDE )
			{
				Config.Volume.SoundVolume = (Value-20)*500;
				GWnd.SendCast( GlobalWindow::eSoundVol, Config.Volume.SoundVolume );
			}
			if( ID == IDC_BGM_SLIDE )
			{
				Config.Volume.BGMVolume = (Value-20)*500;
				GWnd.SendCast( GlobalWindow::eBGMVol, Config.Volume.BGMVolume );
			}
		}
		break;
	case IDC_ADJUSTER:
		{
			Config.Display.Adjuster = -Value/2.0f;
			GWnd.SendCast( GlobalWindow::eMojiAdjust, Config.Display.Adjuster );
		}
		break;
	}
	SetConfigData(true);
	return FALSE;
}

bool ConfigDlg::SetSystemXML( const XMLPaser& XML )
{
	WindowName = XML.AttributeString(_T("name"));
	if( !WindowName )WindowName = L"Config Dialog";
	else WindowName += L" Config";
	XMLPaser WindowXML;
	XML.Find(_T("Window"), WindowXML );
	XMLList FileList;
	FileList.Query(WindowXML,_T("File"));
	WindowTexCount = FileList.GetCount();
	XMLPaser BattleXML;
	if( !FAILED(XML.Find(_T("Battle"), BattleXML )) )
	{
		float Scale = BattleXML.AttributeFloat(_T("minscale"));
		if( Scale > 0 )MinScale = Scale;
		Scale = BattleXML.AttributeFloat(_T("maxscale"));
		if( Scale > 0 )MaxScale = Scale;
	}
	
	return true;
}

BOOL ConfigDlg::InitDialog( HWND hWnd )
{
	Config.ReadConfig();
	m_hWnd = hWnd;
	::SetWindowText(hWnd,String(WindowName));

	if( WindowTexCount >= 2 )
		::ShowWindow( ::GetDlgItem( hWnd, IDC_RADIO2 ), SW_SHOWNA );
	if( WindowTexCount >= 3 )
		::ShowWindow( ::GetDlgItem( hWnd, IDC_RADIO3 ), SW_SHOWNA );
	if( WindowTexCount >= 4 )
		::ShowWindow( ::GetDlgItem( hWnd, IDC_RADIO4 ), SW_SHOWNA );

	bool bRefModeOK = (0<SearchPath( NULL, _T("d3dref"), _T(".dll"), 0, NULL, NULL ));
	if( bRefModeOK )
		::ShowWindow( ::GetDlgItem( hWnd, IDC_UN_HAL ), SW_SHOWNA );

	long Min = ScaleToSlide(MinScale);
	long Max = ScaleToSlide(MaxScale);
	SetTrackBarRange( IDC_DISPLAY_SLIDE, Min, Max );
	SetTrackBarRange( IDC_BGM_SLIDE, 0, 20 );
	SetTrackBarRange( IDC_EFFECT_SLIDE, 0, 20 );
	SetTrackBarRange( IDC_ADJUSTER, 0, 2 );

	SetConfigData();
	return TRUE;
}

long ConfigDlg::ScaleToSlide( float Scale ) const
{
	if( Scale <= 0 )return 0;
	Scale *= 10;
	long Slide = Scale+0.5f;
	Slide -= 10;
	return Slide;
}

float ConfigDlg::SlideToScale( long Slide ) const
{
	float Scale = Slide;
	Scale += 10;
	Scale /= 10.0f;
	return Scale;
}

void ConfigDlg::SetTrackBarRange(int ID, int Min, int Max)
{
	SendDlgItemMessage( m_hWnd, ID, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG( Min, Max ) );
}

void ConfigDlg::SetTrackBarPos(int ID, int Pos)
{
	SendDlgItemMessage( m_hWnd, ID, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) Pos );
}

int ConfigDlg::GetTrackBarPos(int ID )
{
	return SendDlgItemMessage( m_hWnd, ID, TBM_GETPOS, 0, 0 );
}

BOOL ConfigDlg::Command( int ID, HWND hWnd )
{
	if( ID == IDOK )return EndDialog(m_hWnd, IDOK);
	if( ID == IDCANCEL )return EndDialog(m_hWnd, IDCANCEL);
	if( ID == IDC_BTN_DEFAULT )ReadDefault();
	if( ID == IDC_RADIO1 )
	{	GWnd.Send(GlobalWindow::eWndChange, 0);
		Config.Window.WindowTexNo=0;
		return TRUE;}
	if( ID == IDC_RADIO2 )
	{	GWnd.Send(GlobalWindow::eWndChange, 1);
		Config.Window.WindowTexNo=1;
		return TRUE;}
	if( ID == IDC_RADIO3 )
	{	GWnd.Send(GlobalWindow::eWndChange, 2);
		Config.Window.WindowTexNo=2;
		return TRUE;}
	if( ID == IDC_RADIO4 )
	{	GWnd.Send(GlobalWindow::eWndChange, 4);
		Config.Window.WindowTexNo=3;
		return TRUE;}
	if( ID == IDC_UN_HAL )
	{
		Config.Display.bDrawSoft = !Config.Display.bDrawSoft;
		Config.Display.bVertexSoft=Config.Display.bDrawSoft;
		SaveConfig();
		::MessageBox( m_hWnd, _T("この設定を有効にするにはゲームを再起動します"), _T("Config"), MB_OK );
		return TRUE;}
	if( ID == IDC_SAFETY )
	{
		bool bSafety = ( Config.System.bUseManaged );//&& Config.System.bDontCache );
	//	Config.System.bDontCache = !bSafety;
		Config.System.bUseManaged = !bSafety;
		SaveConfig();
		::MessageBox( m_hWnd, _T("この設定を有効にするにはゲームを再起動します"), _T("Config"), MB_OK );
		return TRUE;}
	return FALSE;
}

void ConfigDlg::SetConfigData( bool bValuOnly )
{
	String FormatMsg;
	FormatMsg.Format( "%.0f%%", Config.Display.Scale*100 );
	::SetDlgItemText( m_hWnd, IDC_DISPLAY_SIZE, FormatMsg );

	long Pos = ScaleToSlide( Config.Display.Scale );
	if(!bValuOnly)SetTrackBarPos( IDC_DISPLAY_SLIDE, Pos );

	long BGMVar = 10000 + (long)(Config.Volume.BGMVolume);
	BGMVar /= 500;

	long SoundVar = 10000 + (long)(Config.Volume.SoundVolume);
	SoundVar /= 500;

	FormatMsg.Format( "%d", BGMVar*5 );
	::SetDlgItemText( m_hWnd, IDC_BGM_VOL, FormatMsg );
	if(!bValuOnly)SetTrackBarPos( IDC_BGM_SLIDE, BGMVar );

	FormatMsg.Format( "%d", SoundVar*5 );
	::SetDlgItemText( m_hWnd, IDC_EFFECT_VOL, FormatMsg );
	if(!bValuOnly)SetTrackBarPos( IDC_EFFECT_SLIDE, SoundVar );

	if(!bValuOnly)SetTrackBarPos( IDC_ADJUSTER, -Config.Display.Adjuster*2 );

	CheckDlgButton( m_hWnd, IDC_UN_HAL, (Config.Display.bDrawSoft)?(BST_CHECKED):(BST_UNCHECKED) );
	bool bSafety = ( Config.System.bUseManaged );//&& Config.System.bDontCache );
	CheckDlgButton( m_hWnd, IDC_SAFETY, (bSafety)?(BST_CHECKED):(BST_UNCHECKED) );

	switch( Config.Window.WindowTexNo )
	{
	default:
		CheckRadioButton( m_hWnd, IDC_RADIO1, IDC_RADIO4, IDC_RADIO1 );
		if( !bValuOnly )GWnd.Send(GlobalWindow::eWndChange, 0);
		break;
	case 1:
		CheckRadioButton( m_hWnd, IDC_RADIO1, IDC_RADIO4, IDC_RADIO2 );
		if( !bValuOnly )GWnd.Send(GlobalWindow::eWndChange, 1);
		break;
	case 2:
		CheckRadioButton( m_hWnd, IDC_RADIO1, IDC_RADIO4, IDC_RADIO3 );
		if( !bValuOnly )GWnd.Send(GlobalWindow::eWndChange, 2);
		break;
	case 3:
		CheckRadioButton( m_hWnd, IDC_RADIO1, IDC_RADIO4, IDC_RADIO4 );
		if( !bValuOnly )GWnd.Send(GlobalWindow::eWndChange, 3);
		break;
	}
}
