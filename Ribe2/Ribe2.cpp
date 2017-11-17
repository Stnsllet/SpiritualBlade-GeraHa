// Ribe2.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;					// 現在のインスタンス
TCHAR szTitle[MAX_LOADSTRING];				// タイトル バー テキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// タイトル バー テキスト

// このコード モジュールに含まれる関数の前宣言:
ATOM			MyRegisterClass( HINSTANCE hInstance );
HWND			InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

#define WTAGSIZE (GetSystemMetrics(SM_CXDLGFRAME) * 2)
#define HTAGSIZE (GetSystemMetrics(SM_CYDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION))

#include "SplashWindow.h"
#include "GameSystem.h"
#include "PaformanceChecker.h"
GameSystem* g_pSystem = NULL;
bool GameEnd = false;

POINTMANAGER();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	CoInitialize(NULL);
 	// TODO: この位置にコードを記述してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル ストリングを初期化します
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RIBE2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass( hInstance );

	g_pSplashWnd = new SplashWindow();
	g_pSystem = new GameSystem();
	if( !g_pSystem->GlobalSystem.CheckMaster()
		|| !g_pSystem->LoadSystemXML(_T("System/System.xml")) )
	{
		SafeDelete( g_pSystem );
		SafeDelete( g_pSplashWnd );
		CoUninitialize();
		return 0;
	}

	// アプリケーションの初期化を行います:
	HWND hWnd = InitInstance( hInstance, nCmdShow );
	if( hWnd == NULL ) 
	{
		SafeDelete( g_pSystem );
		SafeDelete( g_pSplashWnd );
		CoUninitialize();
		return 0;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_RIBE2);

	HRESULT ret = g_pSystem->Init( hWnd );
	if( FAILED(ret) )
	{
		SafeDelete( g_pSystem );
		SafeDelete( g_pSplashWnd );
		CoUninitialize();
		return 0;
	}

	// メイン メッセージ ループ:
	while( !GameEnd )
	{
		PEFORMANCE("All Game Job");
		while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
		{
			if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ) 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		g_pSystem->Job();
	}

	delete g_pSystem;
	delete g_pSplashWnd;
	CoUninitialize();

	MANAGER_CLEAR();

	return msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  用途: ウィンドウ クラスの登録
//
//  コメント:
//
//    この関数およびその使用はこのコードを Windows 95 で先に追加された
//    'RegisterClassEx' 関数と Win32 システムの互換性を保持したい場合に
//    のみ必要となります。アプリケーションが、アプリケーションに関連付け
//    られたスモール アイコンを取得できるよう、この関数を呼び出すことは
//    重要です。
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

//	LPCTSTR FileName=_T("System/Game.ico");
//	HICON hIcon = (HICON)LoadImage(NULL,FileName,IMAGE_ICON,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style		= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= CS_HREDRAW | CS_VREDRAW ;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= LoadIcon(hInstance, (LPCTSTR)IDI_RIBE2);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName	= (LPCSTR)IDC_RIBE2;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(hInstance, (LPCTSTR)IDI_RIBE2);//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx( &wcex );
}

//
//   関数: InitInstance(HANDLE, int)
//
//   用途: インスタンス ハンドルの保存とメイン ウィンドウの作成
//
//   コメント:
//
//        この関数では、インスタンス ハンドルをグローバル変数に保存し、プログラムの
//        メイン ウィンドウを作成し表示します。
//
HWND InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	HWND hWnd;

	hInst = hInstance; // グローバル変数にインスタンス ハンドルを保存します

	int Width = g_pSystem->Config.Display.Width;
	int Height = g_pSystem->Config.Display.Height;

	DWORD Style = (g_pSystem->WindowStyle());

	RbRect Rect( 0,0, Width, Height );
	AdjustWindowRect( &(RECT&)Rect, Style, FALSE );
	Width = Rect.Width();
	Height = Rect.Height();

	int x = (::GetSystemMetrics(SM_CXSCREEN)-Width)/2;
	int y = (::GetSystemMetrics(SM_CYSCREEN)-Height)/2;

   hWnd = CreateWindow(szWindowClass, g_pSystem->WindowName(), Style,
      x, y, Width, Height, NULL, NULL, hInstance, NULL);

   if( !hWnd ) 
   {
      return hWnd;
   }

   ShowWindow( hWnd, nCmdShow );
   UpdateWindow( hWnd );

   return hWnd;
}

//
//  関数: WndProc(HWND, unsigned, WORD, LONG)
//
//  用途: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 終了メッセージの通知とリターン
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	if(g_pSystem)if (g_pSystem->WindowProc( hWnd, message, wParam, lParam ) )return 0;

	switch( message ) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// メニュー選択の解析:
			switch( wmId ) 
			{
			//	case IDM_ABOUT:
			//	   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			//	   break;
				case IDM_EXIT:
				   DestroyWindow( hWnd );
				   break;
				default:
				   return DefWindowProc( hWnd, message, wParam, lParam );
			}
			break;
		case WM_DESTROY:
			GameEnd = true;
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
   }
   return 0;
}

// バージョン情報ボックス用メッセージ ハンドラ
LRESULT CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if( LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL ) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
