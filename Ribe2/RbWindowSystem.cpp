// RbWindowSystem.cpp: RbWindowSystem �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AllWindow.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "RbEventSystem.h"
#include "BattleSystem.h"
#include "RbWindowSystem.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RbWindowSystem::RbWindowSystem()
{
}

RbWindowSystem::~RbWindowSystem()
{
	ClearData();
}

void RbWindowSystem::ClearData()
{
	MsgWndList.clear();
	MsgTrapList.clear();
	WindowList.clear();
	DestroyStock();
	pJobWindow = NULL;
	pTutorialWindow = NULL;
}

void RbWindowSystem::Release( DISPDEVICE* pDisplay )
{
	WindowSystem::Release( pDisplay );
	if( pTutorialWindow != NULL )pTutorialWindow->Command( pDisplay, *this, BaseWindow::eReleaseSurface );
}

// �L�[����
bool RbWindowSystem::KeyJob( int KeyCode, bool bPress, const GameMessage& State )
{
	if( bPress )return false;
	if( State.IsDraging() )return false;
	RefPtr<BaseWindow> pHitWnd = GetHitWindow( State.x, State.y );
	switch( KeyCode )
	{
	case VK_F1:
		if( OnMsg() )return false;
		if( pHitWnd != NULL )if( pHitWnd->IsHide() )return true;
		pTutorialWindow = pHitWnd;
		GetEventSystem().LoadEvent(_T("Event/Tutorial.xml"));
		return true;
	case VK_SPACE:
		AllClose();
		return true;
	}
	return WindowSystem::KeyJob( KeyCode, bPress, State );
}

// �}�E�X����
bool RbWindowSystem::_MouseJob( BaseWindow* pHitWnd, GameMessage& State )
{
	if( MessageJob( pHitWnd, State ) )return true;
	return WindowSystem::_MouseJob( pHitWnd, State );
}

// ���b�Z�[�W�p�W���u
bool RbWindowSystem::MessageJob( BaseWindow* pHitWnd, GameMessage& State )
{
	if( MsgWndList.size() <= 0 )return false;
	long JobCount = 0;
	RbEventSystem& Event = GetEventSystem();
	WndArray::iterator itr = MsgWndList.begin();
	if( !Event.ActLock.IsLock() )
	{
		while( itr != MsgWndList.end() )
		{
			RefPtr<BaseWindow> pJob = (*itr);
			if( pJob->MouseJob( *this, State ) )JobCount++;
			if( MsgWndList.size() <= 0 )break;

			if( !IsRegist(pJob) )itr = MsgWndList.erase( itr );
			else itr++;
		}
	}
	itr = MsgTrapList.begin();
	while( itr != MsgTrapList.end() )
	{
		RefPtr<BaseWindow> pJob = (*itr);
		pJob->MouseJob( *this, State );
		if( MsgTrapList.size() <= 0 )break;

		if( !IsRegist(pJob) )itr = MsgTrapList.erase( itr );
		else itr++;
	}
//	if( JobCount > 0 )GetEventSystem().ListEvent();
	if( MsgWndList.size() <= 0 )Event.ListEvent();
	return (JobCount>0)?(true):(Event.ActLock.IsLock());
}

// �h���b�O�E�C���h�E�ݒ�
void RbWindowSystem::SetJobWindow( BaseWindow* pWnd )
{
	WindowSystem::SetJobWindow( pWnd );
	RefPtr<HelpWindow> pHelp = GetHelpWindow();
	if( pHelp == NULL )return;
	if( pHelp == pWnd )return;
	pHelp->SetHide();
}

// ���b�Z�[�W�E�C���h�E�ǉ�
void RbWindowSystem::AddMsgList( BaseWindow* pMsgWnd )
{
	if( pMsgWnd == NULL )return;
	MsgWndList.push_back( pMsgWnd );
}

// ���b�Z�[�W�g���b�v�ǉ�
bool RbWindowSystem::AddMsgTrap( BaseWindow* pMsgWnd )
{
	if( pMsgWnd == NULL )return false;
	MsgTrapList.push_back( pMsgWnd );
	return true;
}

// �p�[�e�B�ǉ�
bool RbWindowSystem::AddPatyHp( long PatyID )
{
	RefPtr<BaseWindow> pGetWnd = GetNameWindow(_T("PatyHp") );
	if( pGetWnd == NULL )return false;
	return pGetWnd->SetParameter( PatyID );
}

// Powered �\��
bool RbWindowSystem::Powered( const XMLPaser& MsgXML )
{
	RefPtr<PoweredWindow> pWnd( new PoweredWindow );

	if( !pWnd->SetXML( MsgXML ) )return false;
	AddWindow( pWnd );

	return true;
}

// �^�C�g���\��
bool RbWindowSystem::Title( const XMLPaser& MsgXML )
{
	RefPtr<TitleWindow> pWnd( new TitleWindow );

	bool bWait = MsgXML.AttributeBool(_T("wait"));
	pWnd->SetMsgMode( bWait );

	if( !pWnd->SetXML( MsgXML ) )return false;
	AddWindow( pWnd );
	if( bWait )AddMsgList( pWnd );
	return true;
}

// ���ʃ^�C�g���\��
bool RbWindowSystem::Result( const XMLPaser& MsgXML )
{
	RefPtr<ResultWindow> pWnd( new ResultWindow );

	bool bWait = MsgXML.AttributeBool(_T("wait"));
	pWnd->SetMsgMode( bWait );

	if( !pWnd->SetXML( MsgXML ) )return false;
	AddWindow( pWnd );
	if( bWait )AddMsgList( pWnd );
	return true;
}

// �|�b�v�A�b�v�\��
bool RbWindowSystem::PopupMsg( RbBattleStatus* pStatus, LPCTSTR Message )
{
	RefPtr<PopUpMsgWindow> pWnd( new PopUpMsgWindow );
	if( !pWnd->SetMessage( Message ) )return false;
	pWnd->SetCharactor( pStatus );
	AddWindow( pWnd );
	return true;
}

// �|�b�v�A�b�v�\��
bool RbWindowSystem::PopupMsg( RbBattleStatus* pStatus, const XMLPaser& MsgXML )
{
	RefPtr<PopUpMsgWindow> pWnd( new PopUpMsgWindow );
	if( !pWnd->SetXML( MsgXML ) )return false;
	pWnd->SetCharactor( pStatus );
	AddWindow( pWnd );
	return true;
}

// ���b�Z�[�W�\��
bool RbWindowSystem::Message( RbBattleStatus* pStatus, const XMLPaser& MsgXML )
{
	RefPtr<SelectMsgWindow> pWnd( new SelectMsgWindow );
	if( !pWnd->SetXML( MsgXML ) )return false;
	pWnd->SetCharactor( pStatus );
	AddWindow( pWnd );
	AddMsgList( pWnd );
	return true;
}

// ���b�Z�[�W�\��
bool RbWindowSystem::Message( RbBattleStatus* pStatus, LPCTSTR Message, LPCTSTR FontName )
{
	RefPtr<MsgWindow> pWnd( new MsgWindow );
	if( !pWnd->SetMessage( Message ) )return false;
	pWnd->SetCharactor( pStatus );
	pWnd->SetFont( FontName );
	AddWindow( pWnd );
	AddMsgList( pWnd );
	return true;
}

// �w���v
HelpWindow* RbWindowSystem::GetHelpWindow()
{
	RefPtr<BaseWindow> pGetWnd = GetNameWindow(_T("Help"));
	if( pGetWnd != NULL )return (HelpWindow*)(BaseWindow*)pGetWnd;

	RefPtr<HelpWindow> pHelpWindow( new HelpWindow );
	AddWindow( pHelpWindow );
	return pHelpWindow;
}

// �w���v
bool RbWindowSystem::Help( LPCTSTR HelpMsg, LPCTSTR InfoMsg, const RbRect& FlotingRect )
{
	RefPtr<HelpWindow> pHelpWindow = GetHelpWindow();
	if( pHelpWindow == NULL )return false;
	pHelpWindow->SetSourceRect( FlotingRect );
	return pHelpWindow->SetHelp( InfoMsg, HelpMsg );
}

// �w���v
bool RbWindowSystem::HelpSkill( long SkillNo, const RbRect& FlotingRect )
{
	RefPtr<HelpWindow> pHelpWindow = GetHelpWindow();
	if( pHelpWindow == NULL )return false;
	const SkillCache::SkillData* pData = GetCache().Skill.GetSkill( SkillNo );
	if( pData == NULL )return false;
	pHelpWindow->SetSourceRect( FlotingRect );
	return pHelpWindow->SetHelp( pData->Name, pData->Help );
}

// �w���v
bool RbWindowSystem::HelpItem( long ItemNum, const RbRect& FlotingRect )
{
	RefPtr<HelpWindow> pHelpWindow = GetHelpWindow();
	if( pHelpWindow == NULL )return false;
	const ItemStatus& Ref = GetCache().Item.Get( ItemNum );
//	if( Ref.id.Number <= 0 )return false;
	pHelpWindow->SetSourceRect( FlotingRect );
	if( !pHelpWindow->SetHelp( Ref.Name, Ref.Help ) )return false;
	for( int i = 0; i < count_of(Ref.Skill); i++ )
	{
		pHelpWindow->AddSkill( Ref.Skill[i].name, Ref.Skill[i].Level );
	}
	return true;
}

// ���[�h�ύX
bool RbWindowSystem::ChangeGameMode( GameMode NewMode )
{
	RefPtr<DummyWindow> pDumWnd( new DummyWindow );
	if( pDumWnd->SetMode( NewMode ) )AddWindow( pDumWnd );

	return true;
}

// �`���[�g���A��
bool RbWindowSystem::Tutorial( LPCTSTR WndName, LPCTSTR Taget )
{
	RefPtr<BaseWindow> pWnd = GetNameWindow( WndName );
	if( String(WndName) != _T("none") )
	{
		if( pWnd == NULL )return false;
		if( pWnd->IsHide() )return false;
	}

	pTutorialWindow = pWnd;
	GetEventSystem().LoadEvent(_T("Event/Tutorial.xml"));
	return true;
}

// �`���[�g���A��
bool RbWindowSystem::Tutorial( const XMLPaser& EventXML )
{
	String CheckWndName = EventXML.AttributeString(_T("check"));
	if( !CheckWndName.empty() )
	{
		if( CheckWndName == _T("drop") )
		{
			pTutorialWindow = NULL;
			return true;
		}
		String WindowName = _T("none");
		if( pTutorialWindow != NULL )WindowName = pTutorialWindow->WindowName();
		if( WindowName != CheckWndName )return false;
		if( WindowName == _T("none") )pTutorialWindow = NULL;
		GetEventSystem().ChildEvent( EventXML );
		return true;
	}

	MoveTop( pTutorialWindow );
	RefPtr<TutorialWindow> pWnd( new TutorialWindow );
	pWnd->SetWnd( pTutorialWindow );
	pWnd->SetXML( EventXML );

	AddWindow( pWnd );
	AddMsgList( pWnd );
	return true;
}

// �R�}���h�E�C���h�E
bool RbWindowSystem::ComWindow( LPCTSTR WndName )
{
	RefPtr<BaseWindow> pGetWnd = GetNameWindow( WndName );
	if( pGetWnd != NULL )
	{
		MoveTop( pGetWnd );
		return true;
	}

	RefPtr<ComBaseWindow> pComWnd( new ComBaseWindow );
	if( !pComWnd->SetCommand(*this, WndName) )return false;
	AddWindow( pComWnd );
	return true;
}

// �R�}���h�E�C���h�E
bool RbWindowSystem::ComWindow( const XMLPaser& WindowXML )
{
	String WndName = WindowXML.AttributeString(TAG::NAME);
	RefPtr<BaseWindow> pGetWnd = GetNameWindow( WndName );
	if( pGetWnd != NULL )
	{
		MoveTop( pGetWnd );
		return true;
	}

	RefPtr<ComBaseWindow> pComWnd( new ComBaseWindow );
	if( !pComWnd->SetXML(*this, WindowXML) )return false;
	AddWindow( pComWnd );
	return true;
}
