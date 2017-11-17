// GameSystem.cpp: GameSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbRect.h"
#include "DispStocker.h"
#include "GameSystem.h"

#include "WindowHandle.h"
#include "SplashWindow.h"
#include "PaformanceChecker.h"
#include "Timer.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbCache& GetCache()
{
	ATLASSERT( g_pSystem );
	return g_pSystem->Cache;
}

RbDisplay& GetRbDisplay()
{
	ATLASSERT( g_pSystem );
	return g_pSystem->DisplaySystem;
}

RbGameData& GetGameData()
{
	ATLASSERT( g_pSystem );
	return g_pSystem->GameData;
}

BattleSystem& GetBattleSystem()
{
	ATLASSERT( g_pSystem );
	return g_pSystem->BattleSys;
}

RbEventSystem& GetEventSystem()
{
	ATLASSERT( g_pSystem );
	return g_pSystem->EventSystem;
}

RbSound& GetSoundSystem()
{
	ATLASSERT( g_pSystem );
	return g_pSystem->SoundSystem;
}

LandEdit& GetLandEdit()
{
	return GetBattleSystem().Land;
}

RbWindowSystem& GetWindowSystem()
{
	ATLASSERT( g_pSystem );
	return g_pSystem->WindowSys;
}

AllWorld& GetWorldSystem()
{
	ATLASSERT( g_pSystem );
	return g_pSystem->WorldSystem;
}

ShopSystem& GetShop()
{
	ATLASSERT( g_pSystem );
	return g_pSystem->Shop;
}

LPCTSTR GameSystem::GameStartXMLPath = _T("Event/GameStart.xml");
D3DCOLOR MYWHITE = D3DCOLOR_XRGB(255,255,255);

GameSystem::GameSystem() : Fliper( DisplaySystem )
{
	OldJobTime = -1000;
	m_hWnd = NULL;
	m_hMenu = NULL;
	MiniWindowWidth = 320;
	MiniWindowHeight = 240;
	AllZero(MouseState);
	bWindowActive = true;
	bInitFinish = false;
	bFullScreen = false;

	Config.ReadConfig();

	NowGameMode = eNoneMode;
}

bool ErrorPopup( LPCTSTR Msg )
{
	GetSoundSystem().Play(_T("cancel"));
	return GetWindowSystem().PopupMsg( NULL, Msg );
}

bool InfoPopup( LPCTSTR Msg )
{
	GetSoundSystem().Play(_T("question"));
	return GetWindowSystem().PopupMsg( NULL, Msg );
}

bool SystemPopup( LPCTSTR Msg )
{
	GetSoundSystem().Play(_T("sysinfo"));
	return GetWindowSystem().PopupMsg( NULL, GetCache().GetMessage(Msg) );
}

GameSystem::~GameSystem()
{
	DisplaySystem.SetConfig( Config );
}

// システム初期化
HRESULT GameSystem::Init( HWND hWnd )
{
	TIMECHK("System Init.");
	srand( (unsigned)Timer() );	//乱数初期化

	SplashMsgX(1, _T("DirectX を初期化中"));

	MouseState.hWnd = hWnd;
	m_hWnd = hWnd;
	m_hMenu = ::GetMenu( hWnd );
	SetMenu( true );
	Config.SetDisplayConfig( m_hWnd );
	DragAcceptFiles( m_hWnd, TRUE );

	HRESULT ret = S_OK;
	{
		TIMECHK("DirectX Init");
		DirectXSystem.SetEmurationMode( Config.Display.bDrawSoft, Config.Display.bVertexSoft );
		ret = DirectXSystem.Init();
		if( FAILED(ret) )return ret;

		ret = DisplaySystem.Create( DirectXSystem, hWnd );
		if( FAILED(ret) )return ret;

		D3DFORMAT RenderFormat = DirectXSystem.GetRenderFormat(DisplaySystem);
		D3DFORMAT TextureFormat = DirectXSystem.GetTextureFormat(DisplaySystem);
		if( TextureFormat==RenderFormat || SystemData.bNoEx
			|| DirectXSystem.IsAlphaFormat( RenderFormat ) )
		{
			Texture::SetUseFormat( RenderFormat );
		}
		else
		{
			Texture::SetUseFormat( TextureFormat, RenderFormat );
			if( ExSprite::SetEx( RenderFormat ) )
				MYWHITE = D3DCOLOR_XRGB(240,240,240);
		}

		if( Config.System.bUseManaged )
		{
			Texture::SetUseFormat( TextureFormat );
			Texture::SetUseManaged( true );
		}
		if( SystemData.bForceMirror )Texture::SetAlgin( true );
		else Texture::CheckAlgin( DisplaySystem );

		RbSprite::SetAduster( Config.Display.Adjuster );

		ret = DisplaySystem.SetRenderState( DirectXSystem.GetDeviceCaps(D3DDEVTYPE_HAL) );
		if( FAILED(ret) )return ret;
	}

	SplashMsgX(2, _T("データ読み込み中"));

	ret = Cache.InitCache( DisplaySystem );
	if( FAILED(ret) )return ret;

	SplashMsgX(3, _T("サウンド初期化中"));

	{
		TIMECHK("SoundSystem Init");
		ret = SoundSystem.InitRb(hWnd);
		if( FAILED(ret) )return ret;
	}

	{
		TIMECHK("BGMSystem Init");
		ret = BGMSystem.InitSystem();
		if( FAILED(ret) )return ret;
		BGMSystem.Cache(_T("world"));
	}

	SplashMsgX(4, _T("システム初期化中"));

	ret = BattleSys.InitSystem( DisplaySystem );
	if( FAILED(ret) )return ret;

	{
		TIMECHK("WindowSystem Init");
		ret = WindowSys.InitSystem( DisplaySystem );
		if( FAILED(ret) )return ret;
	}

	{
		TIMECHK("WorldSystem Init");
		ret = WorldSystem.InitWorld( DisplaySystem );
		if( FAILED(ret) )return ret;
	}

	GameData.InitData();

	bInitFinish = true;

	ResetConfig();
	EventSystem.LoadEvent( GameStartXMLPath );
	KeyEventList.LoadXML();

//	DisplaySystem.FreeMemory();
	TimeSystem.Sleep();

	return S_OK;
}

bool GameSystem::LoadSystemXML( LPCTSTR FileName )
{
	LPCTSTR FilePath =  _T("System/System.xml") ;
	XMLPaser SystemXML;
	HRESULT hr = SystemXML.Load( FilePath );
	if( FAILED(hr) )
	{
		::PopupMsg( _T("システムファイルが見つかりません") );
		if( ::GetFileAttributes( FilePath ) == ((DWORD)-1) )
		{
			::PopupMsg( _T("ファイルが正常に展開されていない可能性があります") );
		}
		else
		{
			::PopupMsg( _T("MSXMLがインストールされてない可能性があります") );
		}
		return 0;
	}
	SystemData.GameName = SystemXML.AttributeString(TAG::NAME);
	if( SystemData.GameName.empty() )return false;

	SystemData.bWindowSizeLock = SystemXML.AttributeBool(_T("windowlock"));
	SystemData.WindowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	if( SystemData.bWindowSizeLock )SystemData.WindowStyle &= ~WS_MAXIMIZEBOX;

	SystemData.bNoEx = SystemXML.AttributeBool(_T("noex"));
	SystemData.bForceMirror = SystemXML.AttributeBool(_T("mirror"));

	XMLPaser Sub;
	if( SUCCEEDED( SystemXML.Find(_T("Display"),Sub) ) )
	{
		DisplaySystem.LoadSystemXML( Sub );
	}

	if( SUCCEEDED( SystemXML.Find(_T("Window"),Sub) ) )
	{
		WindowSys.LoadSystemXML( Sub );
	//	LevelUpSystem::SetMaxLevel( Sub.AttributeFloat(_T("maxlev")) );
	}

	if( SUCCEEDED( SystemXML.Find(_T("Battle"),Sub) ) )
	{
		BattleSys.LoadSystemXML( Sub );
	//	LevelUpSystem::SetMaxLevel( Sub.AttributeFloat(_T("maxlev")) );
	}

	if( SUCCEEDED( SystemXML.Find(_T("World"),Sub) ) )
		WorldSystem.SetWorldSystem( Sub );

	if( SUCCEEDED( SystemXML.Find(_T("GameData"),Sub) ) )
		GameData.LoadSystemXML( Sub );

	if( SUCCEEDED( SystemXML.Find(_T("Item"),Sub) ) )
		Shop.LoadSystemXML( Sub );

	return true;
}

// アクティブなランドスケープ取得
LandScape& GameSystem::ActiveLand()
{
	switch( NowGameMode )
	{
	case eWorldMode:
		return WorldSystem.Land;
	}
	return BattleSys.Land;
}

// ダミーのマウスメッセージ
bool GameSystem::DummyMsg() const
{
	::PostMessage( m_hWnd, WM_USER_DUMMY, 0, 0 );
	return true;
}

// メッセージ進めクリックメッセージ
bool GameSystem::NextMsg() const
{
	if( !WindowSys.OnMsg() )return false;
	::PostMessage( m_hWnd, WM_RBUTTONDBLCLK, 0, 0 );
	return true;
}

// ウインドウメッセージ処理
bool GameSystem::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( !bInitFinish )return false;
	if( GlobalSystem.WndProc(hWnd,message,wParam, lParam) )return true;

	if( message >= WM_MOUSEFIRST && message <= WM_MOUSELAST )
		return MouseJob(hWnd, message, wParam, lParam);
	if( message == WM_USER_DUMMY )
		return MouseJob(hWnd, message, wParam, lParam);
	HRESULT ret = S_OK;

	switch( message )
	{
	case WM_KEYDOWN:
		return KeyJob( (int)wParam, (lParam&(1<<30))>0 );
	case WM_DISPLAYCHANGE: case WM_PAINT:
		DrawMain();
		ValidateRect(hWnd,NULL);
		return false;
	case WM_DROPFILES:
		JobFile( (HDROP)wParam );
		break;
	case WM_SIZING:
		{
			RbRect* pRect = (RbRect*)lParam;
			if( pRect == NULL )return false;
			if( SystemData.bWindowSizeLock )
			{
				GetWindowRect(hWnd, (RECT*)lParam );
			}
			else
			{
				WindowHandle Handle(hWnd);
				*pRect = Handle.MiniClip( *pRect, MiniWindowWidth, MiniWindowHeight );
			}
			return true;
		}
	case WM_SIZE:
		WindowSizeChange( wParam, LOWORD(lParam), HIWORD(lParam) );
		break;
	case WM_ACTIVATE:
		if( HIWORD(wParam)!=0 )break;
		SetActive( LOWORD(wParam) != WA_INACTIVE );
		break;
	case WM_DESTROY:
		BGMSystem.StopAll();
		break;
	}

	return false;
}

// ウインドウのアクティブ状態変更
bool GameSystem::SetActive( bool bActive )
{
	bWindowActive = bActive;
	if( !bWindowActive )BGMSystem.SetBaseVolume( -10000 );
	else
	{
		::ShowWindow( m_hWnd, SW_RESTORE );
		BGMSystem.SetBaseVolume( Config.Volume.BGMVolume );
	}
	return bWindowActive;
}

// ファイル受け取り
void GameSystem::JobFile( HDROP hDrop )
{
	int FileCont = DragQueryFile( hDrop, -1, NULL, 0 );
	int BufCount = DragQueryFile( hDrop, 0, NULL, 0 )+1;
	LPTSTR pBuffer = (TCHAR*)_alloca( sizeof(TCHAR)*BufCount );
	ZeroMemory( pBuffer, sizeof(TCHAR)*BufCount );
	DragQueryFile( hDrop, 0, pBuffer, BufCount );

	SetForegroundWindow( m_hWnd );
	Cache.XML.Clear();
	EventSystem.LoadEvent( pBuffer );
}

// サイズ変更メッセージ
void GameSystem::WindowSizeChange( DWORD Msg, int Width, int Height )
{
	HRESULT ret = S_OK;
	switch( Msg )
	{
	case SIZE_MINIMIZED:
		Release();
		SetActive( false );
		break;
	case SIZE_MAXIMIZED:
		SetMenu( false );
		SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE );
		Release();
		ret = DisplaySystem.FullScreenWindow(DirectXSystem.DisplayMode());
	//	ret = DisplaySystem.FullScreenWindow(DirectXSystem.DisplayMode(),
	//		Config.Display.Width, Config.Display.Height);
		RedrawWindow();
		bFullScreen = true;
		break;
	case SIZE_RESTORED:
		SetMenu( true );
		SetWindowLong( m_hWnd, GWL_STYLE, WindowStyle() );
	//	Config.SetDisplayConfig( m_hWnd );
		Release();
		ret = DisplaySystem.WindowSizeChange( Width, Height);
	//	ret = DisplaySystem.WindowSizeChange( Config.Display.Width, Config.Display.Height );
		RedrawWindow();
		bFullScreen = false;
		break;
	}
	if( IsMemoryError(ret) )
	{
		if( MiniWindowWidth < Width || MiniWindowHeight < Height )
		{	// 小さめにリサイズ
			::SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, MiniWindowWidth, MiniWindowHeight, SWP_NOMOVE | SWP_NOZORDER );
		}
		MemoryErrorMsg(ret);
	}
}

// マウス用メッセージ処理
bool GameSystem::MouseJob(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GameMessage OldState = MouseState;
	MouseState = eNonMessage;

	MouseState.LDown = (MK_LBUTTON & wParam)!=false;
	MouseState.RDown = (MK_RBUTTON & wParam)!=false;

	MouseState.OldX = OldState.x;	MouseState.OldY = OldState.y;
	MouseState.OldViewPos = OldState.ViewPos;
	POINT Pos;
	GetCursorPos( &Pos );
	ScreenToClient( m_hWnd, &Pos );
	MouseState.x = Pos.x;
	MouseState.y = Pos.y;
	MouseState.ViewPos = ActiveLand().GetClickPos( DisplaySystem, Pos.x, Pos.y );
	switch( message ) 
	{
	case WM_USER_DUMMY:
		MouseState = eNonMessage;
		break;
	case WM_LBUTTONDOWN:
		if( MouseState.RDown == true )MouseState = eLRDown;
		else MouseState = eLDown;
		break;
	case WM_RBUTTONDOWN:
		if( MouseState.LDown == true )MouseState = eLRDown;
		else MouseState = eRDown;
		break;
	case WM_LBUTTONUP:
		MouseState = eLUp;
		break;
	case WM_RBUTTONUP:
		MouseState = eRUp;
		break;
	case WM_LBUTTONDBLCLK:
		MouseState = eLDblClick;
		break;
	case WM_RBUTTONDBLCLK:
		MouseState = eRDblClick;
		break;
	case WM_MOUSEWHEEL:
		if( (short) HIWORD(wParam) <= 0 )MouseState = eScrollDown;
		else MouseState = eScrollUp;
		break;
	case WM_MOUSEMOVE:
		MouseState = eMouseMove;
		break;
	}
	if( WindowSys.MouseJob( MouseState ) )return true;

	return SystemMessage( MouseState );
}

// メッセージ処理
bool GameSystem::SystemMessage( const GameMessage& State )
{
	return true;
}

bool GameSystem::SetMenu( bool bOn ) const
{
//	if( bOn )::SetMenu( m_hWnd, n_hMenu );
	if( bOn )::SetMenu( m_hWnd, NULL );
	else::SetMenu( m_hWnd, NULL );
	return true;
}

// フルスクリーン？
bool GameSystem::IsFullScreen() const
{
	return bFullScreen;
	return  0<(WS_POPUP & ::GetWindowLong( m_hWnd, GWL_STYLE ) );
}

bool GameSystem::KeyJob( int KeyCode, bool bPress )
{
	// ドラッグ中は処理しない
	if( MouseState.IsDraging() )return false;

	const String& UserEvent = KeyEventList.GetPath( KeyCode );
	if( !UserEvent.empty() )
	{
		if( bPress )return false;
		return SUCCEEDED(EventSystem.LoadEvent( UserEvent ));
	}

	switch( NowGameMode )
	{
	case eBattleMode:// case eEventMode:
		if( BattleSys.KeyJob( KeyCode, bPress ) )return true;
//	case eWorldMode: WorldSystem; break;
	}
	if( WindowSys.KeyJob( KeyCode, bPress, MouseState ) )return true;

	// 押しっぱなし不許可
	if( bPress )return false;
	bool bFullScreen = IsFullScreen();

	if( KeyCode == VK_ESCAPE && bFullScreen )KeyCode = VK_F2;

	if( KeyCode == VK_ESCAPE )
	{
		DestroyWindow(m_hWnd);
		return true;
	}
	if( KeyCode == VK_F2 && !SystemData.bWindowSizeLock )
	{
		if( bFullScreen )
		{
			int x = Config.Display.x;
			int y = Config.Display.y;
			int Width = Config.Display.Width;
			int Height = Config.Display.Height;
			SetMenu( true );
			SetWindowLong( m_hWnd, GWL_STYLE, WindowStyle() );
			SetWindowPos( m_hWnd, HWND_NOTOPMOST, x, y, Width, Height, SWP_SHOWWINDOW );
		}
		else
		{
			SetMenu( false );
			SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE );
			ShowWindow( m_hWnd, SW_MAXIMIZE );
		}
		return true;
	}
	if( KeyCode == VK_F5 )
	{
		Release();
		SetActive(true);
		return true;
	}
	return false;
}

bool GameSystem::ReleaseCache( bool bWindowMemory )
{
	WindowSys.Release( DisplaySystem );
	BGMSystem.ReleaseCache();
	DisplaySystem.FreeMemory();
	Cache.Clear();
	SoundSystem.ClearCache();
	return true;
}

void GameSystem::Release()
{
	DisplaySystem.FreeMemory();
	WindowSys.Release( DisplaySystem );
}

// 再描写
bool GameSystem::RedrawWindow() const
{
	return (0!=::RedrawWindow( m_hWnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE ));
}

// デバイス復旧
HRESULT GameSystem::RestoreDevice()
{
	HRESULT ret = DisplaySystem.RestoreDevice();
	if( FAILED(ret) )
	{
		FAILEDTRACE( ret );
		ATLTRACE("\nFailed Restore!!\n\n");
		Release();
	}
	return ret;
}

// あじゃすたー変更
bool GameSystem::ChangeAduster( float New )
{
	if( New == RbSprite::SetAduster( New ) )return false;
	Release();
	return true;
}

// 画面描写
HRESULT GameSystem::Job()
{
	if( !bInitFinish )return S_FALSE;
	bool bMinWindow = 0<(WS_MINIMIZE & ::GetWindowLong( m_hWnd, GWL_STYLE ) );
	if( !bWindowActive || bMinWindow )
	{
		MsgWaitForMultipleObjects( 0, NULL, FALSE, 200, QS_ALLINPUT );
		TimeSystem.Sleep();
		return S_FALSE;
	}

	TimeSystem.Check();
	RbTimer NewTime;
	if( NewTime.Passed( OldJobTime ) < 20 )
	{
		DWORD Sleep = 20-(DWORD)NewTime.Passed( OldJobTime );
		MsgWaitForMultipleObjects( 0, NULL, FALSE, Sleep, QS_ALLINPUT );
		return S_FALSE;
	}
	OldJobTime = NewTime;

	HRESULT ret = RestoreDevice();
	if( FAILED(ret) )return ret;

	PEFORMANCE("GameSystem Job");
	DisplaySystem.Job();
	switch( NowGameMode )
	{
	case eEventMode:
	case eBattleMode: BattleSys.Job(); break;
	case eWorldMode: WorldSystem.Job(); break;
	default: ATLASSERT(NULL); break;
	}

	return DrawMain();
}

// 画面描写
HRESULT GameSystem::DrawMain()
{
	if( !bInitFinish )return S_FALSE;
	if( !DisplaySystem )return E_PENDING;
//	if( !bWindowActive )return S_FALSE;

	PEFORMANCE("GameSystem Draw");

	if( DisplaySystem.IsLostDevice() )
	{
		ATLTRACE("\nDevice Lost!!\n\n");
		Release();
	}

	HRESULT ret = RestoreDevice();
	if( FAILED(ret) )return ret;

	// time out ?
//	if( !Fliper.FlipWait() )
//	{
//		ATLTRACE("\nFlip TimeOut!!\n\n");
//		return E_FAIL;
//	}

	ret = DisplaySystem.BeginScene();
	if( FAILED(ret) )return ret;
	DispStocker::Shaker Shaker( DisplaySystem );
	float Shake = DisplaySystem.GetShake(); 
	if( Shake > 0 )Shaker.Set( Shake );
	DispStocker::ViewTransSet ViewStock( DisplaySystem );

	switch( NowGameMode )
	{
	case eEventMode:
	case eBattleMode: BattleSys.DrawMain( DisplaySystem, MouseState ); break;
	case eWorldMode: WorldSystem.Draw( DisplaySystem ); break;
	default:ATLASSERT(NULL);break;
	}

	ret = WindowSys.DrawMain( DisplaySystem, MouseState );
	if( FAILED(ret) )return ret;

	ret = DisplaySystem.EndScene();
	if( FAILED(ret) )return ret;

//	Fliper.Flip();
	DisplaySystem.Flip();

	return S_OK;
}

// ゲームオーバー処理
bool GameSystem::GameOverJob()
{
	GameData.AllClear();
	g_pSystem->ChangeGameMode( eNoneMode );
	EventSystem.LoadEvent( GameStartXMLPath );
	return true;
}

// コンフィグ状態反映
void GameSystem::ResetConfig( bool bRedraw )
{
	if( !bInitFinish )return;

	DisplaySystem.ChangeDisplayBaseScale( Config.Display.Scale );
	BGMSystem.SetBaseVolume( Config.Volume.BGMVolume );
	SoundSystem.ChangeVolume( Config.Volume.SoundVolume );

	if( bRedraw )RedrawWindow();
}

// ゲームモード変更
bool GameSystem::ChangeGameMode( GameMode NewMode )
{
	if( NewMode == NowGameMode )return false;
	EventSystem.ClearJob();
	if( NewMode == eNoneMode )EventSystem.ClearListEvent();

	WindowSys.ClearData();
	BattleSys.ClearData();
	WorldSystem.ClearData();
	GameData.ClearHotFlg();

	DisplaySystem.ClearJob();

	NowGameMode = NewMode;
	switch( NowGameMode )
	{
	case eBattleMode:GameData.SetFlg(_T("m_battle"));break;
	case eWorldMode:GameData.SetFlg(_T("m_world"));break;
	case eEventMode:GameData.SetFlg(_T("m_event"));break;
	}

	const LandScape& Land = ActiveLand();
	DisplaySystem.SetViewArea( Land.Map.LandArea() );

	WindowSys.ChangeGameMode( NewMode );
	BattleSys.ChangeGameMode( NewMode );
	WorldSystem.ChangeGameMode( NewMode );

	return true;
}
