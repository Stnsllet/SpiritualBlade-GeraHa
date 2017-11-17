// TutorialWindow.h: TutorialWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUTORIALWINDOW_H__DEFEF1DC_B268_41B4_8B10_4869C20F10C8__INCLUDED_)
#define AFX_TUTORIALWINDOW_H__DEFEF1DC_B268_41B4_8B10_4869C20F10C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbTexture.h"
#include "SelectMsgWindow.h"

class TutorialWindow : public SelectMsgWindow  
{
public:
	TutorialWindow();
	virtual ~TutorialWindow();
	virtual LPCTSTR WindowName() const;

	bool SetWnd( BaseWindow* pNewWnd );
	virtual bool SetXML( const XMLPaser& MsgXML );
	bool SetWndRect( WindowSystem& System, LPCTSTR RectName=NULL );

	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );

	static bool SetMsgBaseInfo( const WindowSystem& System );
protected:

	RbRect GetTagetRect( const WindowPlanes* pPlanes, LPCTSTR RectName ) const;
	HRESULT DrawL( RbSprite& Display, WindowSystem& System, const RbRect& TagetRect );
	HRESULT DrawR( RbSprite& Display, WindowSystem& System, const RbRect& TagetRect );
	HRESULT DrawFuki( RbSprite& Display, WindowSystem& System, int x, int y, bool Rev );
	HRESULT DrawNoWindow( RbSprite& Display, WindowSystem& System );
	struct _TagetWnd
	{
		RefPtr<BaseWindow> pWnd;
		RbRect Rect;
	}TagetWnd;
	struct _TutoriaTex
	{
		bool Set( LPCTSTR Name );
		operator RbTexture*() const{return pTexture;}
		RefPtr<RbTexture> pTexture;
		float Size, Height;
		struct Image
		{
			const RbTexture::ActTime* pAct;
		//	RbRect Bound;
		}Img, Left, Right;
		const RbTexture::ActTime *pImg, *pLeft, *pRight;
	}TutoriaTex;
	Plane<> FukiL;
	Plane<> FukiR;
	Plane<> FukiT;
	static RbPointF DefaultMsgPos;
};

#endif // !defined(AFX_TUTORIALWINDOW_H__DEFEF1DC_B268_41B4_8B10_4869C20F10C8__INCLUDED_)
