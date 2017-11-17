// TitleWindow.h: TitleWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TITLEWINDOW_H__ADE15B8D_C0B6_4A63_98C6_A583E27397BC__INCLUDED_)
#define AFX_TITLEWINDOW_H__ADE15B8D_C0B6_4A63_98C6_A583E27397BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"

class TitleWindow : public BaseWindow  
{
public:
	TitleWindow();
	virtual ~TitleWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool SetXML( const XMLPaser& MsgXML );
	bool SetMsgMode( bool bMode ){return bMsged=bMode;}
	bool GetMsgMode() const{return bMsged;}

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual bool HitTest( int x, int y ) const;
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	HRESULT DrawDefault( RbSprite& Display, WindowSystem& System );
	HRESULT DrawSideSlash( double Passed, RbSprite& Display, WindowSystem& System );
	HRESULT DrawCurtain( double Passed, RbSprite& Display, WindowSystem& System );
	HRESULT DrawLabel( double Passed, RbSprite& Display, WindowSystem& System );
	HRESULT DrawSlit( double Passed, RbSprite& Display, WindowSystem& System );
	HRESULT DrawFade( double Passed, RbSprite& Display, WindowSystem& System );
	HRESULT DrawCutin( double Passed, RbSprite& Display, WindowSystem& System );
	HRESULT DrawRotate( double Passed, RbSprite& Display, WindowSystem& System );
	HRESULT DrawMist( double Passed, RbSprite& Display, WindowSystem& System );
	HRESULT DrawFall( double Passed, RbSprite& Display, WindowSystem& System );

	enum _ShowType
	{
		eShowNone = -1,
		eShowSideSlash = 0,
		eShowLabel,
		eShowCurtain,
		eShowSlit,
		eShowFade,
		eShowCutin,
		eShowRotate,
		eShowMist,
		eShowFall,

		eShowMax,
	}ShowType;

	double StartTime;
	double Life;
	String TitleMsg;
	String Font;
	String TexString;
	static LPCTSTR TitleFont;
	bool bMsged;
	ARGB Color;
	LockCounter::Locker BattleLocker;
};

#endif // !defined(AFX_TITLEWINDOW_H__ADE15B8D_C0B6_4A63_98C6_A583E27397BC__INCLUDED_)
