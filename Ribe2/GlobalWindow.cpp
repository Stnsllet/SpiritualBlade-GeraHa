// GlobalWindow.cpp: GlobalWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

GlobalWindow::GlobalWindow()
{
	MsgID = RegisterWindowMessage( _T("Rb_System2_Wnd") );
}

GlobalWindow::~GlobalWindow()
{

}


void GlobalWindow::Send( GlobalMsg Msg, LPARAM Value )
{
	::SendMessage( HWND_BROADCAST, MsgID, (WPARAM)Msg, Value );
}
