// DivisionWindow.h: DivisionWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIVISIONWINDOW_H__73BAE4DA_4C9B_4C88_989F_CEFE5CABAD14__INCLUDED_)
#define AFX_DIVISIONWINDOW_H__73BAE4DA_4C9B_4C88_989F_CEFE5CABAD14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScrollWindow.h"
class RbGameData;

class DivisionWindow : public ScrollWindow  
{
public:
	DivisionWindow();
	virtual ~DivisionWindow();
	virtual LPCTSTR WindowName() const;

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool DropJob( const DragItemWindow& DragItem );
protected:
	struct CheckData
	{
		CheckData(){PatyNum=MemberNum=-1;}
		CheckData(long Paty,long Member){PatyNum=Paty; MemberNum=Member;}
		operator bool() const{return PatyNum>=0;}
		long PatyNum;
		long MemberNum;
	};
	CheckData HitCheck( int x, int y ) const;
	CheckData HitCheckMain( int x, int y ) const;
	RbRect GetCheckRect( const CheckData& Pos ) const;
	virtual RbRect HitComCursor( WindowSystem& System, int x, int y );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	HRESULT RestoreCommand( RbSprite& Surface, WindowSystem& System, const RbRect& ComArea );
	virtual HRESULT RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index );

	void DrawMyPlane( RbSprite& Surface, LPCTSTR Name, const RbRect& Rect, const RbPoint& Pos ) const;
	virtual HRESULT DrawPaty( RbSprite& Display, WindowSystem& System, const PatyStatus& State, float x, float y );
	virtual HRESULT DrawMember( RbSprite& Display, WindowSystem& System, const MemberStatus& State, float x, float y );
	virtual bool DragStartJob( WindowSystem& System, GameMessage& State );
	virtual bool ClickJob( int x, int y );
	virtual bool Floating( int x, int y );
	bool ShowFormation( LPCTSTR Name=NULL, bool bFromEx=false ) const;
	bool ClickCommand( int x, int y );
	bool MemberJob( int x, int y, const MemberStatus& State, bool bFromEx=false ) const;
	bool EquipJob( int x, int y, const ItemID& Item ) const;
	struct Defines
	{
		RbRect Patys;
		RbRect Paty;
		RbRect Member;
		RbRect LevTag;
		RbRect Commnand;
		RbRect Training;
		RbRect AutoBuy;
		RbRect Formation;
		RbRect StockMember;
		RbRect Employ;
		bool bNewPatyOk;
	}Rects;

	long OldUpdateID;
	long MaxPaty;
	long PatyAreaCount;
	Plane<> GlayDown;
	RbGameData& UserData;
};

#endif // !defined(AFX_DIVISIONWINDOW_H__73BAE4DA_4C9B_4C88_989F_CEFE5CABAD14__INCLUDED_)
