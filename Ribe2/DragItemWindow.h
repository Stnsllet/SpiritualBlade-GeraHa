// DragItemWindow.h: DragItemWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGITEMWINDOW_H__9A28743A_C89C_44E8_9947_C5EF1B006E4A__INCLUDED_)
#define AFX_DRAGITEMWINDOW_H__9A28743A_C89C_44E8_9947_C5EF1B006E4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbTexture.h"
#include "BaseWindow.h"

class DragItemWindow : public BaseWindow  
{
	struct _DragData
	{
		String Name;
		RefPtr<RbTexture> pRbTex;
		MemberStatus Member;
		ItemStatus Item;
		double StartTime;
		bool bFromExPaty;
	};
public:
	DragItemWindow();
	virtual ~DragItemWindow();
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual HRESULT DrawCursor( RbSprite& Display, WindowSystem& System, const GameMessage& State );

	virtual bool HitTest( int x, int y ) const{return false;}
	bool SetItemData( long ItemNum );
	bool SetMemberData( const MemberStatus& State, bool bFromEx=false );
	bool SetDrag( WindowSystem& System, GameMessage& State );
	const RbPoint& Pos() const{return DropPos;}
	const DragingType& DragType() const{return Type;}
	const _DragData& DragData() const{return Data;}
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	HRESULT RestoreString( DISPDEVICE* pDevice, WindowSystem& System );
	HRESULT DrawString( RbSprite& Display, WindowSystem& System );
	HRESULT DrawMember( RbSprite& Display, WindowSystem& System );

	DragingType Type;
	_DragData Data;
	RbPoint DropPos;
};

#endif // !defined(AFX_DRAGITEMWINDOW_H__9A28743A_C89C_44E8_9947_C5EF1B006E4A__INCLUDED_)
