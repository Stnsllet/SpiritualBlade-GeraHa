// PopUpMsgWindow.cpp: PopUpMsgWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "RbActStatus.h"
#include "WindowSystem.h"
#include "PopUpMsgWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

PopUpMsgWindow::PopUpMsgWindow()
{
	Life = 3000;
	bDestroyOK = false;
	StartTime = RbTimer();
}

PopUpMsgWindow::~PopUpMsgWindow()
{
}

// 描写
HRESULT PopUpMsgWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	RbTimer NewTime;
	if( NewTime.Passed( StartTime ) > Life )
	{
		Command( NULL, System, eDestroy );
		return S_OK;
	}

	return MsgBaseWindow::Draw( Display ,System );
}

bool PopUpMsgWindow::SetMessage( LPCTSTR NewMessage )
{
	if( !MsgBaseWindow::SetMessage( NewMessage ) )return false;
	StartTime = RbTimer();
	return true;
}
