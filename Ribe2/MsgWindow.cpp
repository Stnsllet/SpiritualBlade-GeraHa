// MsgWindow.cpp: MsgWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSystem.h"
#include "WindowSystem.h"
#include "RbDisplay.h"
#include "RbSound.h"
#include "GameSystem.h"
#include "MsgWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MsgWindow::MsgWindow()
{
	bDisplayLock = false;
}

MsgWindow::~MsgWindow()
{
}

LPCTSTR MsgWindow::WindowName() const
{
	return _T("MsgWindow");
}

bool MsgWindow::SetXML( const XMLPaser& MsgXML )
{
	String ViewPos = MsgXML.AttributeString(_T("view"));
	UserOrStr(ViewPos);
	if( !ViewPos.empty() )
	{
		RbPosition Pos = g_pSystem->ActiveLand().GetStrPos( ViewPos );
		LockDisplay( Pos );
	}

	if( !MsgBaseWindow::SetXML( MsgXML ) )return false;

	return true;
}

bool MsgWindow::LockDisplay( const D3DVECTOR& Pos )
{
	if( bDisplayLock )return false;	//ロック済み

	GetRbDisplay().LockView( Pos );
	bDisplayLock = true;
	return true;
}

bool MsgWindow::SetCharactor( RbBattleStatus* pData )
{
	MsgBaseWindow::SetCharactor( pData );
	if( pStatus != NULL && IsScrollLock() )
	{
		RbPosition Pos = pStatus->GetPosition();
		Pos.y -= pStatus->Hight();
		LockDisplay( Pos );
	}
	return true;
}

bool MsgWindow::SetMessage( LPCTSTR NewMessage )
{
	if( !MsgBaseWindow::SetMessage(NewMessage) )return false;
	BattleLocker.Lock( GetBattleSystem().BattleLock );
	return true;
}

HRESULT MsgWindow::Command( DISPDEVICE* pDevice, WindowSystem& System, command com )
{
	if( com == eDestroy )
	{
		if( bDisplayLock )GetRbDisplay().UnLockView();
		GetSoundSystem().Play(_T("msg"));
	}
	return MsgBaseWindow::Command( pDevice, System, com );
}

bool MsgWindow::MouseJob( WindowSystem& System, GameMessage& State )
{
	switch( State.Msg() )
	{
	case eLDown: case eLDblClick: case eRDblClick:
		Command( NULL, System, eDestroy );
		return true;
	}

	return MsgBaseWindow::MouseJob( System, State );
}
