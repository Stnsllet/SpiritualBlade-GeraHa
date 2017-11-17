// PopUpMsgWindow.cpp: PopUpMsgWindow �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbTimer.h"
#include "RbActStatus.h"
#include "WindowSystem.h"
#include "PopUpMsgWindow.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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

// �`��
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
