// SplashWindow.h: SplashWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLASHWINDOW_H__4180FC05_2B46_43BD_BF1B_F1C53BDF34D9__INCLUDED_)
#define AFX_SPLASHWINDOW_H__4180FC05_2B46_43BD_BF1B_F1C53BDF34D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "String.h"
#include "CriticalSection.h"

class SplashWindow
{
public:
	class SplashMSG
	{
	public:
		SplashMSG( SplashWindow& Wnd, LPCTSTR Msg );
		~SplashMSG();
	private:
		SplashWindow& MyWnd;
		bool bLocked;
	};
	SplashWindow();
	~SplashWindow();

	void LockSplash( LPCTSTR ViewString );
	void ReleaseSplash();

protected:
	HWND CreateWnd( HINSTANCE hInstance );
	void BeginWindow();

	static void MainRoop( void* pParam );
	bool WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	CriticalSection::CriticalSectionObject CriticalObject;
	long RefCount;
	String ViewMsg;
	HBITMAP hBitMap;
	int Width, Height;
	HANDLE hWakeup;
	HWND m_hWnd;
private:
	static LPCTSTR WndClassName;
};

extern SplashWindow* g_pSplashWnd;

#define SplashMsgX(x,msg)	SplashWindow::SplashMSG SplashM_##x( *g_pSplashWnd, msg )
#define SplashMsg(msg)		SplashMsgX(0,msg)

#endif // !defined(AFX_SPLASHWINDOW_H__4180FC05_2B46_43BD_BF1B_F1C53BDF34D9__INCLUDED_)
