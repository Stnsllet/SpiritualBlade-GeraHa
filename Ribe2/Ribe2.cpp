// Ribe2.cpp : �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;					// ���݂̃C���X�^���X
TCHAR szTitle[MAX_LOADSTRING];				// �^�C�g�� �o�[ �e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// �^�C�g�� �o�[ �e�L�X�g

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̑O�錾:
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
 	// TODO: ���̈ʒu�ɃR�[�h���L�q���Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o�� �X�g�����O�����������܂�
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

	// �A�v���P�[�V�����̏��������s���܂�:
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

	// ���C�� ���b�Z�[�W ���[�v:
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
//  �֐�: MyRegisterClass()
//
//  �p�r: �E�B���h�E �N���X�̓o�^
//
//  �R�����g:
//
//    ���̊֐�����т��̎g�p�͂��̃R�[�h�� Windows 95 �Ő�ɒǉ����ꂽ
//    'RegisterClassEx' �֐��� Win32 �V�X�e���̌݊�����ێ��������ꍇ��
//    �̂ݕK�v�ƂȂ�܂��B�A�v���P�[�V�������A�A�v���P�[�V�����Ɋ֘A�t��
//    ��ꂽ�X���[�� �A�C�R�����擾�ł���悤�A���̊֐����Ăяo�����Ƃ�
//    �d�v�ł��B
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
//   �֐�: InitInstance(HANDLE, int)
//
//   �p�r: �C���X�^���X �n���h���̕ۑ��ƃ��C�� �E�B���h�E�̍쐬
//
//   �R�����g:
//
//        ���̊֐��ł́A�C���X�^���X �n���h�����O���[�o���ϐ��ɕۑ����A�v���O������
//        ���C�� �E�B���h�E���쐬���\�����܂��B
//
HWND InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	HWND hWnd;

	hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X �n���h����ۑ����܂�

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
//  �֐�: WndProc(HWND, unsigned, WORD, LONG)
//
//  �p�r: ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- �I�����b�Z�[�W�̒ʒm�ƃ��^�[��
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
			// ���j���[�I���̉��:
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

// �o�[�W�������{�b�N�X�p���b�Z�[�W �n���h��
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
