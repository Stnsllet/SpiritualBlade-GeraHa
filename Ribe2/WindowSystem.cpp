// WindowSystem.cpp: WindowSystem クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PaformanceChecker.h"
#include "RbSprite.h"
#include "GameSystem.h"
#include "TutorialWindow.h"
#include "WindowSystem.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

WindowSystem::WindowSystem()
{
	AllZero( LastCursor );
}

WindowSystem::~WindowSystem()
{

}

// サーフェースのリリース
void WindowSystem::Release( DISPDEVICE* pDisplay )
{
	FontList.Release();
	AllCommand( pDisplay, BaseWindow::eReleaseSurface );
	if( pJobWindow != NULL )pJobWindow->Command( pDisplay, *this, BaseWindow::eReleaseSurface );
}

// すべてのウインドウを閉じる
void WindowSystem::AllClose()
{
	WndArray::const_iterator itr = WindowList.begin();
	while( itr != WindowList.end() )(*itr++)->Close( *this );
	DestroyStock();
}

// 名前でウインドウを閉じる
bool WindowSystem::CloseWindow( LPCTSTR WindowName, const CComVariant& CheckValue )
{
	RefPtr<BaseWindow> pWnd = GetNameWindow( WindowName, CheckValue );
	if( pWnd == NULL )return false;
	return pWnd->Close(*this);
}

// すべてのウインドウにコマンド
void WindowSystem::AllCommand( DISPDEVICE* pDisplay, BaseWindow::command Com )
{
	WndArray::const_iterator itr = WindowList.begin();
	while( itr != WindowList.end() )
	{
		(*itr)->Command( pDisplay, *this, Com );
		itr++;
	}
}

// 上に持ってくる
void WindowSystem::MoveTop( BaseWindow* pWnd )
{
	if( pWnd == NULL )return;
	RefPtr<BaseWindow> pStock = pWnd;
	WndArray::iterator itr = WindowList.begin();
	while( itr != WindowList.end() )
	{
		if( (*itr) == pWnd )
		{
			WindowList.erase( itr );
			break;
		}
		itr++;
	}

	long Level = pStock->RefTopLevel();

	itr = WindowList.begin();
	while( itr != WindowList.end() )
	{
		if( Level >= (*itr)->RefTopLevel() )break;
		itr++;
	}
	WindowList.insert( itr, pStock );
}

void WindowSystem::DestroyWindow( BaseWindow* pWindow )
{
	WndArray::iterator itr = WindowList.begin();
	while( itr != WindowList.end() )
	{
		if( (*itr) == pWindow )
		{
			DestroyList.push_back( (*itr) );
		//	WindowList.erase( itr );
			break;
		}
		itr++;
	}
}

void WindowSystem::DestroyStock()
{
	WndArray::iterator itrD = DestroyList.begin();
	while( itrD != DestroyList.end() )
	{
		WndArray::iterator itr = WindowList.begin();
		while( itr != WindowList.end() )
		{
			if( (*itr) == (*itrD) )
			{
				WindowList.erase( itr );
				break;
			}
			itr++;
		}
		itrD++;
	}
	DestroyList.clear();
}

// システムファイル設定
bool WindowSystem::LoadSystemXML( const XMLPaser& SystemXML )
{
	return BaseTextue.LoadSystemXML( SystemXML );
}

// ウインドウ種類変更
bool WindowSystem::ChangeWindowTexture( long No )
{
	RbDisplay& Display = GetRbDisplay();
	Release( Display );
	if( FAILED(BaseTextue.SetTexture(Display,No)) )return false;
	return true;
}

// 初期化
HRESULT WindowSystem::InitSystem( DISPDEVICE* pDisplay )
{
	HRESULT ret = BaseTextue.Load( pDisplay, g_pSystem->Config.Window.WindowTexNo );
	if( FAILED(ret) )return ret;

	ret = FontList.InitFonts( pDisplay );
	if( FAILED(ret) )return ret;

	ret = WinDefine.LoadWindowDefine();
	if( FAILED(ret) )return ret;

	ret = MsgWindowPlane.LoadPlane( pDisplay, _T("System/MsgWndTexture.xml") );
	if( FAILED(ret) )return ret;

	MsgBaseWindow::SetMsgBaseInfo( *this );
	TutorialWindow::SetMsgBaseInfo( *this );

	return S_OK;
}

// 描写
HRESULT WindowSystem::DrawMain( DISPDEVICE* pDisplay, const GameMessage& State )
{
	PEFORMANCE("Window Draw");
	RbSprite DrawTaget;
	HRESULT ret = DrawTaget.InitSprite( pDisplay );
	if( FAILED(ret) )return ret;

	ret = DrawMain( DrawTaget, State );
	if( FAILED(ret) )return ret;

/*	{
		PEFORMANCE("VRAM Draw");
		DWORD Free, All;
		Test.GetVideoMemory( Free, All );
		D3DVECTOR Pos = g_pSystem->GetMousePos();
		String Msg;
		DWORD Used = All - Free;
		Msg.Format( _T("VRAM %.3f / %.3f (MB)\nx %4.0f Y %4.0f z %3.0f"),
			Used/1024.0f/1024.0f, All/1024.0f/1024.0f,
			Pos.x, Pos.y, Pos.z);
		Font().Draw( "default", Msg, 0, 0 );
	}*/

	DestroyStock();
	return S_OK;
}

// 描写
HRESULT WindowSystem::DrawMain( RbSprite& Display, const GameMessage& State )
{
	RefPtr<BaseWindow> pHitWnd = GetHitWindow( State.x, State.y );

	WndArray::reverse_iterator itr = WindowList.rbegin();
	while( itr != WindowList.rend() )
	{
		const RefPtr<BaseWindow>& Ref = *itr++;
		Ref->Draw( Display, *this );
		if( Ref==pHitWnd )
			if( !State.IsDraging() || pJobWindow != pHitWnd )
				Ref->DrawCursor( Display, *this, State );
	}
	return S_OK;
}

// ポインタの位置のウインドウ取得
BaseWindow* WindowSystem::GetHitWindow( int x, int y, BaseWindow* pUnCheck ) const
{
	WndArray::const_iterator itr = WindowList.begin();
	while( itr != WindowList.end() )
	{
		if( (*itr)->HitTest( x,y ) )
			if( (*itr) != pUnCheck )return (*itr);
		itr++;
	}
	return NULL;
}

// 現在のウインドウサイズ取得
RbPoint WindowSystem::GetWindowSize() const
{
	const D3DVIEWPORT8& Ref = GetRbDisplay().ViewPort();
	return RbPoint( Ref.Width, Ref.Height );
}

// カーソルが変更されたことを認識
bool WindowSystem::ChangeDrawCursor( BaseWindow* pWnd, const RECT& Rect, bool Sound )
{
	if( LastCursor.pLastWnd == pWnd && LastCursor.LastRect == Rect )return false;
	LastCursor.pLastWnd = pWnd;
	LastCursor.LastRect = Rect;
	if( pWnd == NULL || LastCursor.LastRect.IsNone() )
	{
		AllZero( LastCursor );
		return true;
	}
	if( Sound )GetSoundSystem().Play(_T("cursor"));
	return true;
}

// ウインドウ追加
void WindowSystem::AddWindow( BaseWindow* pWindow )
{
	MoveTop( pWindow );
}

// 登録されているか確認
bool WindowSystem::IsRegist( BaseWindow* pWindow ) const
{
	if( pWindow == NULL )return false;
	WndArray::const_iterator itr = DestroyList.begin();
	for( ;itr != DestroyList.end(); itr++ )
		if( (*itr) == pWindow )return false;

	itr = WindowList.begin();
	for( ;itr != WindowList.end(); itr++ )
		if( (*itr) == pWindow )return true;
	return false;
}

// 名前が一致するウインドウ取得
BaseWindow* WindowSystem::GetNameWindow( LPCTSTR WindowName, const CComVariant& CheckValue ) const
{
	String Name( WindowName );
	WndArray::const_iterator itr = WindowList.begin();
	while( itr != WindowList.end() )
	{
		if( Name == (*itr)->WindowName() )
		{
			if( CheckValue.vt == VT_EMPTY )return (*itr);
			if( (*itr)->CheckParameter( CheckValue ) )return (*itr);
		}
		itr++;
	}
	return NULL;
}

// キー処理
bool WindowSystem::KeyJob(  int KeyCode, bool bPress, const GameMessage& State )
{
	return false;
}

// マウス処理
bool WindowSystem::MouseJob( GameMessage& State )
{
	RefPtr<BaseWindow> pHitWnd = GetHitWindow( State.x, State.y );
	bool bRet = _MouseJob( pHitWnd, State );
	DestroyStock();
	return bRet;
}

// マウス処理
bool WindowSystem::_MouseJob( BaseWindow* pHitWnd, GameMessage& State )
{
	if( State.IsDraging() )
	{
		if( pJobWindow != NULL )return pJobWindow->MouseJob( *this, State );
	}
	if( pHitWnd != NULL )return pHitWnd->MouseJob( *this, State );
	return false;
}
