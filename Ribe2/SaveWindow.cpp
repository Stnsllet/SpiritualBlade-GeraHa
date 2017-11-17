// SaveWindow.cpp: SaveWindow �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbGameData.h"
#include "RbEventSystem.h"
#include "SaveWindow.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SaveWindow::SaveWindow()
{

}

SaveWindow::~SaveWindow()
{
}

LPCTSTR SaveWindow::WindowName() const
{
	return _T("Save");
}

bool SaveWindow::FileJob( const String& FilePath )
{
	GetEventSystem().LoadEvent( _T("Event/SaveGame.xml") );
	if( FAILED(GetGameData().SaveData( FilePath )) )return false;
	SystemPopup( _T("save") );
	AllTexRelease();
	return true;
}
