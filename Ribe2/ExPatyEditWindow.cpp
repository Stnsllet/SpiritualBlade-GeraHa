// ExPatyEditWindow.cpp: ExPatyEditWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbGameData.h"
#include "RbWindowSystem.h"
#include "StrCommander.h"
#include "ExPatyEditWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ExPatyEditWindow::ExPatyEditWindow()
{
	Title = _T("Paty");
}

ExPatyEditWindow::~ExPatyEditWindow()
{
}

LPCTSTR ExPatyEditWindow::WindowName() const
{
	return _T("ExPatyEdit");
}

PatyStatus ExPatyEditWindow::GetStatus()
{
	EditPatyNum = UserData.RefExPatyNum();
	PatyStatus EditStatus;
	if( !UserData.GetPaty( EditPatyNum, EditStatus ) )
		return AllZero(EditStatus);
	return EditStatus;
}

bool ExPatyEditWindow::ClickMember( int x, int y )
{
	String Name = GetMemberName( x, y );
	if( Name.empty() )return false;

	StrCommander().ShowMemberState( Name );
	RefPtr<BaseWindow> pFindWnd = GetWindowSystem().GetNameWindow(_T("EqStatus"));
	if( pFindWnd == NULL )return true;
	pFindWnd->SetParameter(false);
	return true;
}
