// SplashWindow.cpp: SplashWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <process.h>
#include "resource.h"
#include "GameSystem.h"
#include "SplashWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SplashWindow* g_pSplashWnd = NULL;
extern HINSTANCE hInst;

LPCTSTR SplashWindow::WndClassName = _T("_RB2_Splash_");

SplashWindow::SplashMSG::SplashMSG( SplashWindow& Wnd, LPCTSTR Msg )
: MyWnd( Wnd ), bLocked( false )
{
	if( g_pSystem == NULL )return;
	if( g_pSystem->IsFullScreen() )return;
	MyWnd.LockSplash( Msg );
	bLocked = true;
}

SplashWindow::SplashMSG::~SplashMSG()
{
	if( bLocked )MyWnd.ReleaseSplash();
}

SplashWindow::SplashWindow()
{
	RefCount = 0;
	hBitMap = NULL;
	hWakeup = ::CreateEvent( NULL, FALSE, FALSE, NULL );
}

SplashWindow::~SplashWindow()
{
	if( hBitMap != NULL )DeleteObject( hBitMap );
	if( hWakeup != NULL )CloseHandle( hWakeup );
}

HWND SplashWindow::CreateWnd( HINSTANCE hInstance )
{
	CriticalSection Lock( CriticalObject );

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style		= CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
	wcex.lpfnWndProc	= (WNDPROC)SplashWindow::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_RIBE2);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= WndClassName;
	wcex.hIconSm		= LoadIcon(hInstance, (LPCTSTR)IDI_RIBE2);
	RegisterClassEx( &wcex );

	int DispWidth = GetSystemMetrics(SM_CXSCREEN);
	int DispHeight = GetSystemMetrics(SM_CYSCREEN);
	LPCTSTR FileName = _T("System/NowLoading.bmp");
	if( hBitMap == NULL )hBitMap = (HBITMAP)LoadImage(NULL,FileName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if( hBitMap == NULL )hBitMap = ::LoadBitmap( hInstance, MAKEINTRESOURCE(IDB_SPLASH_BMP) );
	BITMAP bmpInfo;
	GetObject(hBitMap, sizeof(bmpInfo), &bmpInfo);
	Width = bmpInfo.bmWidth;
	Height = bmpInfo.bmHeight;

	int x = (DispWidth/2) - (Width/2);
	int y = (DispHeight/2) - (Height/2);

	HWND hWnd = CreateWindow( WndClassName, _T("NowLoading"),
		WS_POPUP | WS_VISIBLE,
		x, y, bmpInfo.bmWidth, bmpInfo.bmHeight,
		NULL, NULL, hInstance, NULL );

	ShowWindow( hWnd, SW_SHOW );
	SetWindowPos( hWnd, HWND_TOP, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );	
	RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE );

	m_hWnd = hWnd;
	return hWnd;
}

LRESULT CALLBACK SplashWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( g_pSplashWnd->m_hWnd == hWnd )if( g_pSplashWnd->WindowProc( message, wParam, lParam ) )return 0;

	return DefWindowProc( hWnd, message, wParam, lParam );
}

bool SplashWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_DESTROY )
	{
		CriticalSection Lock( CriticalObject );
		PostQuitMessage( 0 );
		m_hWnd = NULL;
		return false;
	}
	if( message != WM_PAINT )return false;
	String DrawMsg;
	{
		CriticalSection Lock( CriticalObject );
		DrawMsg = ViewMsg;
	}

	HDC hDC = GetDC(m_hWnd);
	HDC hdcImage=CreateCompatibleDC(NULL);
	SelectObject( hdcImage, hBitMap );

    BitBlt( hDC, 0,0, Width, Height, hdcImage, 0,0, SRCCOPY );

	RECT TextRect;
	TextRect.left = TextRect.top = 0;
	TextRect.right = Width-1;
	TextRect.bottom = Height-1;
	SetBkMode( hDC, TRANSPARENT );
	DrawText( hDC, DrawMsg, -1, &TextRect, 0 );

	DeleteDC( hdcImage );
	ReleaseDC( m_hWnd, hDC );
	return true;
}

void SplashWindow::LockSplash( LPCTSTR ViewString )
{
	CriticalSection Lock( CriticalObject );
	long Count = ::InterlockedIncrement( &RefCount );
	if( ViewString != NULL )ViewMsg = ViewString;

	SetEvent( hWakeup );
	if( Count == 1 )BeginWindow();
}

void SplashWindow::ReleaseSplash()
{
	CriticalSection Lock( CriticalObject );
	long Count = ::InterlockedDecrement( &RefCount );
	if( Count == 0 )
	{
		SetEvent( hWakeup );
	}
}

void SplashWindow::BeginWindow()
{
	::_beginthread( SplashWindow::MainRoop, 0, this );
}

void SplashWindow::MainRoop( void* pParam )
{
	SplashWindow* pMyClass = (SplashWindow*)pParam;
	HWND hWnd = pMyClass->CreateWnd( hInst );
	while( true )
	{
		DWORD WaitValue = MsgWaitForMultipleObjects( 1, &(pMyClass->hWakeup), FALSE, INFINITE, QS_ALLINPUT );
		bool bWakeup = ( WAIT_OBJECT_0 == WaitValue );
		if( bWakeup )
		{
			CriticalSection Lock( pMyClass->CriticalObject );
			if( pMyClass->RefCount == 0 )DestroyWindow( hWnd );
			else
			{
				::RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT );
				UpdateWindow( hWnd );
			}
		}
		else
		{
			MSG msg;
			PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
			if( msg.message == WM_QUIT )return;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}// while true
}
