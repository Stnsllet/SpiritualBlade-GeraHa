// RbGlobal.cpp: RbGlobal クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameSystem.h"
#include "RbGlobal.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbGlobal::RbGlobal()
{
	hGlobal = ::CreateMutex( NULL, TRUE, _T("Rb_System2") );
	bMaster = (GetLastError() == 0);
}

RbGlobal::~RbGlobal()
{
	ReleaseMutex( hGlobal );
}

bool RbGlobal::CheckMaster()
{
	if( bMaster )return true;
	Send( eGlobalActive, true );
	return false;
}

bool RbGlobal::WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	if( Message != MsgID )return false;
	if( g_pSystem == NULL )return true;

	RbConfig& Config = g_pSystem->Config;
	GameSystem& System = *g_pSystem;

	GlobalMsg Msg = (GlobalMsg)wParam;
	switch( Msg )
	{
	case eGlobalActive:
		::SetForegroundWindow( hWnd );
		break;
	case eDispScale:
		Config.Display.Scale = *((float*)(long*)&lParam);
		System.DisplaySystem.ChangeDisplayBaseScale( Config.Display.Scale, System.NowMode()==eBattleMode );
		System.RedrawWindow();
		break;
	case eBGMVol:
		Config.Volume.BGMVolume = (long)lParam;
		System.BGMSystem.SetBaseVolume( Config.Volume.BGMVolume );
		break;
	case eSoundVol:
		Config.Volume.SoundVolume = (long)lParam;
		System.SoundSystem.ChangeVolume( Config.Volume.SoundVolume );
		System.SoundSystem.Play(_T("test"));
		break;
	case eWndChange:
		Config.Window.WindowTexNo = (long)lParam;
		System.WindowSys.ChangeWindowTexture( Config.Window.WindowTexNo );
		System.RedrawWindow();
		break;
	case eMojiAdjust:
		Config.Display.Adjuster = *((float*)(long*)&lParam);
		System.ChangeAduster( Config.Display.Adjuster );
		System.RedrawWindow();
		break;
	}


	return true;
}
