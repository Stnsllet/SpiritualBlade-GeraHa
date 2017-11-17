// PatyEditWindow.h: PatyEditWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATYEDITWINDOW_H__611D5A36_B326_4CB1_A238_735A9E29F8F0__INCLUDED_)
#define AFX_PATYEDITWINDOW_H__611D5A36_B326_4CB1_A238_735A9E29F8F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"
class RbGameData;
class DragItemWindow;

class PatyEditWindow : public BaseWindow  
{
public:
	PatyEditWindow();
	virtual ~PatyEditWindow();
	virtual LPCTSTR WindowName() const;

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool SetParameter( const CComVariant& Value );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	RbRect FormBoxRect( const RbPointF& Pos ) const;
	bool DrawMember( RbSprite& Surface, float formX, float formY, LPCTSTR Name ) const;
	bool DrawBox( RbSprite& Surface, const RbPointF& BoxPos ) const;
	virtual PatyStatus GetStatus();

	virtual bool ClickJob( int x, int y );
	virtual bool DragStartJob( WindowSystem& System, GameMessage& State );
	virtual bool DropJob( const DragItemWindow& DragItem );
	long GetFormMember( const PatyStatus& State, int x, int y ) const;
	String GetMemberName( int x, int y );
	bool ChangeEscapeHp( float Add );
	bool ChangeTactics();
	virtual bool ClickMember( int x, int y );

	struct _Rects
	{
		RbRect FormArea;
		RbRect EscapeHp;
		RbRect Tactics;
		RbRect EscapeUp;
		RbRect EscapeDown;
	}Rects;

	std::vector<RbPointF> BoxList;
	Plane<> FormBox;
	float FormationSize;
	float FormationScale;
	long EditPatyNum;
	long OldUpDateID;
	RbGameData& UserData;
	String Title;
};

#endif // !defined(AFX_PATYEDITWINDOW_H__611D5A36_B326_4CB1_A238_735A9E29F8F0__INCLUDED_)
