// WindowSystem.h: WindowSystem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSYSTEM_H__C5D9E6EA_4D4E_4088_9497_B8DCB1ECD993__INCLUDED_)
#define AFX_WINDOWSYSTEM_H__C5D9E6EA_4D4E_4088_9497_B8DCB1ECD993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <list>
#include "BaseWindow.h"
#include "TextFont.h"
#include "WindowTexture.h"
//#include "DDObject.h"
#include "WindowDefine.h"

class WindowSystem  
{
public:
	WindowSystem();
	virtual ~WindowSystem();

	HRESULT InitSystem( DISPDEVICE* pDisplay );
	virtual bool LoadSystemXML( const XMLPaser& SystemXML );
	HRESULT DrawMain( DISPDEVICE* pDisplay, const GameMessage& State );

	bool MouseJob( GameMessage& State );
	virtual bool KeyJob(  int KeyCode, bool bPress, const GameMessage& State );

	bool ChangeWindowTexture( long No );
	const WindowTexture& Texture() const{return BaseTextue;}
	const TextFont& Font() const{return FontList;}
	const WindowDefine& Define() const{return WinDefine;}
	const DefaultPlanes& MsgPlane() const{return MsgWindowPlane;}

	bool ChangeDrawCursor( BaseWindow* pWnd, const RECT& Rect, bool Sound=true );
	virtual void SetJobWindow( BaseWindow* pWnd ){pJobWindow = pWnd;}
	virtual void Release( DISPDEVICE* pDisplay );
	void AllCommand( DISPDEVICE* pDisplay, BaseWindow::command Com );
	void AllClose();
	bool CloseWindow( LPCTSTR WindowName, const CComVariant& CheckValue = CComVariant() );

	void DestroyWindow( BaseWindow* pWindow );
	void MoveTop( BaseWindow* pWnd );

	RbPoint GetWindowSize() const;
	void AddWindow( BaseWindow* pWindow );
	bool IsRegist( BaseWindow* pWindow ) const;
	BaseWindow* GetHitWindow( int x, int y, BaseWindow* pUnCheck=NULL ) const;
	BaseWindow* GetNameWindow( LPCTSTR WindowName, const CComVariant& CheckValue = CComVariant() ) const;
protected:
	virtual bool _MouseJob( BaseWindow* pHitWnd, GameMessage& State );

	virtual HRESULT DrawMain( RbSprite& Display, const GameMessage& State );
	void DestroyStock();

protected:	
//	DDObject Test;
	struct _LastCursor
	{
		void * pLastWnd;
		RbRect LastRect;
	}LastCursor;

	typedef std::list<RefPtr<BaseWindow> > WndArray;

	WindowTexture BaseTextue;
	TextFont FontList;

	RefPtr<BaseWindow> pJobWindow;
	WndArray WindowList;
	WndArray DestroyList;
	WindowDefine WinDefine;
	DefaultPlanes MsgWindowPlane;
};

#endif // !defined(AFX_WINDOWSYSTEM_H__C5D9E6EA_4D4E_4088_9497_B8DCB1ECD993__INCLUDED_)
