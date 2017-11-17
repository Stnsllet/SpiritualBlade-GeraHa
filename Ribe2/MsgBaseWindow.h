// MsgBaseWindow.h: MsgBaseWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGBASEWINDOW_H__C975D93E_B2F0_42E5_A412_71B039E60AC1__INCLUDED_)
#define AFX_MSGBASEWINDOW_H__C975D93E_B2F0_42E5_A412_71B039E60AC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "String.h"
#include "BaseWindow.h"
#include "RbBattleStatus.h"

class MsgBaseWindow : public BaseWindow
{
public:
	MsgBaseWindow();
	virtual ~MsgBaseWindow();

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual LPCTSTR WindowName() const;
	virtual bool SetMessage( LPCTSTR NewMessage );
	virtual bool SetXML( const XMLPaser& MsgXML );
	virtual bool SetFont( LPCTSTR FontName );
	virtual bool SetCharactor( RbBattleStatus* pData );
	virtual bool SetFace( LPCTSTR FaceName );

	static bool SetMsgBaseInfo( const WindowSystem& System );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	HRESULT RestoreMsgBase( DISPDEVICE* pDevice, RbSprite& Surface, WindowSystem& System );

	bool IsScrollLock() const;
	RbRect GetMsgRect( LPCTSTR Message );
	Plane<> GetFace() const;
	HRESULT DrawFace( RbSprite& Surface ) const;

	HRESULT DMsg( WindowSystem& System, LPCTSTR Message, const RECT& MsgRect ) const;
	HRESULT DrawMsgBase( RbSprite& Surface, const RbRect& MsgArea, WindowSystem& System );
	HRESULT DrawStatusPos( RbSprite& Display, WindowSystem& System );
	HRESULT DrawCenterPos( RbSprite& Display, WindowSystem& System );
	RbPoint ChipSize() const{return RbPoint(8,8);}
	RbRect GetFaceRect( const TexRect& Face ) const;

	static LPCTSTR MsgFont;

	RefPtr<RbBattleStatus> pStatus;
	Plane<> Fukidashi;
	RbRect BaseMsgRect;
	String FaceStr;
	String Message;
	String Font;
	bool bTextRead;
	static RbPointF DefaultMsgPos;
};

#endif // !defined(AFX_MSGBASEWINDOW_H__C975D93E_B2F0_42E5_A412_71B039E60AC1__INCLUDED_)
