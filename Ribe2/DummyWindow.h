// DummyWindow.h: DummyWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUMMYWINDOW_H__6E230AEB_A64E_40CF_AA63_741876BE7FCF__INCLUDED_)
#define AFX_DUMMYWINDOW_H__6E230AEB_A64E_40CF_AA63_741876BE7FCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BattleSystem;
class AllWorld;
#include "BaseWindow.h"

class DummyWindow : public BaseWindow  
{
public:
	DummyWindow();
	virtual ~DummyWindow();
	virtual LPCTSTR WindowName() const;

	bool SetMode( GameMode Mode );
	virtual bool HitTest( int x, int y ) const{return true;}
	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual HRESULT DrawCursor( RbSprite& Display, WindowSystem& System, const GameMessage& State );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	BattleSystem* pBattle;
	AllWorld* pWorld;
};

#endif // !defined(AFX_DUMMYWINDOW_H__6E230AEB_A64E_40CF_AA63_741876BE7FCF__INCLUDED_)
