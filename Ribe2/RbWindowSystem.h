// RbWindowSystem.h: RbWindowSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBWINDOWSYSTEM_H__2D37DD58_2D87_45CC_948B_76B6C2CA1D53__INCLUDED_)
#define AFX_RBWINDOWSYSTEM_H__2D37DD58_2D87_45CC_948B_76B6C2CA1D53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WindowSystem.h"
class RbActStatus;
class RbBattleStatus;

class HelpWindow;

class RbWindowSystem : public WindowSystem  
{
public:
	RbWindowSystem();
	virtual ~RbWindowSystem();
	void ClearData();
	virtual void Release( DISPDEVICE* pDisplay );
	virtual void SetJobWindow( BaseWindow* pWnd );

	virtual bool KeyJob(  int KeyCode, bool bPress, const GameMessage& State );

	bool Powered( const XMLPaser& EventXML );
	bool Title( const XMLPaser& MsgXML );
	bool Result( const XMLPaser& MsgXML );
	bool Message( RbBattleStatus* pStatus, const XMLPaser& MsgXML );
	bool Message( RbBattleStatus* pStatus, LPCTSTR Message, LPCTSTR FontName=NULL );
	bool PopupMsg( RbBattleStatus* pStatus, LPCTSTR Message );
	bool PopupMsg( RbBattleStatus* pStatus, const XMLPaser& MsgXML );
	bool AddPatyHp( long PatyID );

	bool Help( LPCTSTR HelpMsg, LPCTSTR InfoMsg, const RbRect& FlotingRect );
	bool HelpSkill( long SkillNo, const RbRect& FlotingRect );
	bool HelpItem( long ItemNum, const RbRect& FlotingRect );
	bool Tutorial( const XMLPaser& EventXML );
	bool Tutorial( LPCTSTR WndName, LPCTSTR Taget=NULL );
	bool ComWindow( LPCTSTR WndName );
	bool ComWindow( const XMLPaser& WindowXML );

	bool AddMsgTrap( BaseWindow* pMsgWnd );
	void AddMsgList( BaseWindow* pMsgWnd );
	bool ChangeGameMode( GameMode NewMode );

	bool OnMsg() const{return (MsgWndList.size()>0); }
protected:
	virtual bool _MouseJob( BaseWindow* pHitWnd, GameMessage& State );
	bool MessageJob( BaseWindow* pHitWnd, GameMessage& State );
	HelpWindow* GetHelpWindow();

	WndArray MsgWndList;
	WndArray MsgTrapList;
	RefPtr<BaseWindow> pTutorialWindow;
};

RbWindowSystem& GetWindowSystem();

#endif // !defined(AFX_RBWINDOWSYSTEM_H__2D37DD58_2D87_45CC_948B_76B6C2CA1D53__INCLUDED_)
