// StatusWindow.h: StatusWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSWINDOW_H__3E3CF82A_B374_4D4B_96B7_C08A05BEE431__INCLUDED_)
#define AFX_STATUSWINDOW_H__3E3CF82A_B374_4D4B_96B7_C08A05BEE431__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbStatus.h"
#include "RbPosition.h"
#include "StatusBaseWinow.h"

class StatusWindow : public StatusBaseWinow  
{
public:
	StatusWindow();
	virtual ~StatusWindow();
	virtual LPCTSTR WindowName() const;
	virtual bool SetParameter( const CComVariant& Value );

	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual bool Floating( int x, int y );
	virtual bool ClickJob( int x, int y );

	void DrawSkills( RbSprite& Surfase );
	void DrawItem( WindowSystem& System, const WindowPlanes* pPlanes, const String& Name, long ItemNum ) const;
	virtual void DrawValue( WindowSystem& System, const WindowPlanes* pPlanes, const String& Name ) const;
	void DrawStr( WindowSystem& System, LPCTSTR Msg, const RbRect& Rect ) const;
	void DrawFloat( WindowSystem& System, LPCTSTR Font, float Value, const RbRect& Rect, DWORD Style ) const;

	RbRect GetSkillRect( long Index ) const;
protected:
	struct _Rects
	{
		RbRect Name;
		RbRect Job;
		RbRect Wepon;
		RbRect Body;
		RbRect Acs;
		RbRect Tag;
		RbRect Value;
		RbRect Now;
		RbRect Slash;
		RbRect Source;
		RbRect Plus;
		RbRect Leader;
		RbRect UpDown;
		RbRect ItemTag;
		RbRect Item;
		RbRect SkillArea;
		RbRect Skill;
		RbRect NewsBtn;
		long SkillCount;
	}Rects;
	bool bEquipMode;
};

#endif // !defined(AFX_STATUSWINDOW_H__3E3CF82A_B374_4D4B_96B7_C08A05BEE431__INCLUDED_)
