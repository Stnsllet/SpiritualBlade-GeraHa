// GlobalWindow.cpp: GlobalWindow �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalWindow.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
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
