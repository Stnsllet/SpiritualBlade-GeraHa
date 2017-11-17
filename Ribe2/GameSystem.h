// GameSystem.h: GameSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESYSTEM_H__38C9A1CA_1262_4C8E_BC63_E294E471F78C__INCLUDED_)
#define AFX_GAMESYSTEM_H__38C9A1CA_1262_4C8E_BC63_E294E471F78C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbConfig.h"
#include "RbGlobal.h"
#include "RbTimer.h"
#include "D3DObject.h"
#include "RbDisplay.h"
#include "ExtraFliper.h"
#include "RbBGMMaker.h"
#include "RbSound.h"
#include "RbWindowSystem.h"
#include "BattleSystem.h"
#include "AllWorld.h"
#include "RbEventSystem.h"
#include "ShopSystem.h"
#include "RbGameData.h"
#include "RbCache.h"
#include "KeyEvents.h"

class GameSystem  
{
protected:
#define WM_USER_DUMMY WM_USER
	GameMode NowGameMode;
	GameMessage MouseState;
	bool bInitFinish;
	bool bWindowActive;
	bool bFullScreen;

public:
	GameSystem();
	virtual ~GameSystem();
	HRESULT Init( HWND hWnd );

	HRESULT Job();
	HRESULT DrawMain();

	bool DummyMsg() const;
	bool NextMsg() const;
	bool ChangeGameMode( GameMode NewMode );

	bool IsFullScreen() const;
	bool SetMenu( bool bOn ) const;
	bool WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool MouseJob(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool KeyJob( int KeyCode, bool bPress );
	bool SystemMessage( const GameMessage& State );
	void Release();
	void WindowSizeChange( DWORD Msg, int Width, int Height );
	void JobFile( HDROP hDrop );
	const GameMessage& RefMessage() const{return MouseState;}
	const GameMode& NowMode() const{return NowGameMode;}
	void ResetConfig( bool bRedraw=false );
	bool RedrawWindow() const;
	bool SetActive( bool bActive );
	bool ReleaseCache( bool bWindowMemory=true );
	HRESULT RestoreDevice();
	bool ChangeAduster( float New );

	bool GameOverJob();
public:
	RbGameData GameData;

	RbMasterTimer TimeSystem;
	RbGlobal GlobalSystem;
	RbCache Cache;
	RbConfig Config;
	D3DObject DirectXSystem;
	RbDisplay DisplaySystem;
	RbBGMMaker BGMSystem;
	RbSound SoundSystem;
	RbEventSystem EventSystem;
	ShopSystem Shop;

	RbWindowSystem WindowSys;
	BattleSystem BattleSys;
	AllWorld WorldSystem;

	LandScape& ActiveLand();
	bool LoadSystemXML( LPCTSTR FileName );
	LPCTSTR WindowName() const{return SystemData.GameName;}
	const DWORD& WindowStyle() const{return SystemData.WindowStyle;}
protected:
	struct _SystemData
	{
		String GameName;
		bool bNoEx;
		bool bWindowSizeLock;
		bool bForceMirror;
		DWORD WindowStyle;
	}SystemData;
protected:

	KeyEvents KeyEventList;
	double OldJobTime;
	ExtraFliper Fliper;
	HWND m_hWnd;
	HMENU m_hMenu;
	RECT WindowRect;
	int MiniWindowWidth, MiniWindowHeight;
	static LPCTSTR GameStartXMLPath;
};

#endif // !defined(AFX_GAMESYSTEM_H__38C9A1CA_1262_4C8E_BC63_E294E471F78C__INCLUDED_)
