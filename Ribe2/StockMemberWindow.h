// StockMemberWindow.h: StockMemberWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCKMEMBERWINDOW_H__F47CFBB4_57FE_444B_BE6E_4ECCE48361A5__INCLUDED_)
#define AFX_STOCKMEMBERWINDOW_H__F47CFBB4_57FE_444B_BE6E_4ECCE48361A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "ScrollWindow.h"
class RbGameData;

class StockMemberWindow : public ScrollWindow  
{
public:
	StockMemberWindow();
	virtual ~StockMemberWindow();
	virtual LPCTSTR WindowName() const;

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool DropJob( const DragItemWindow& DragItem );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual HRESULT RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual bool DragStartJob( WindowSystem& System, GameMessage& State );
	virtual bool ClickJob( int x, int y );

	long GetHitIndex( int x, int y ) const;
	long GetHitPasonID( int x, int y ) const;
	struct StockData
	{
		long PasonID;
		long Level;
		String Name;
		String TexName;
		bool SetData( long SrcPasonID, const MemberStatus& State, const RbGameData& UserData );
		bool IsFront( const StockData& Ref ) const;
	};

	bool DrawMember( RbSprite& Surface, WindowSystem& System, const StockData& Data, const RbRect& Rect ) const;
	bool ReLoadStockMember();
	bool StockJob( const MemberStatus& State, bool bFromEx=false ) const;

	struct _Rects
	{
		RbRect ListArea;
		RbRect Member;
		RbRect Image;
		RbRect Name;
		RbRect Level;
	}Rects;

	std::list<StockData> StockMemberList;
	long OldUpdateID;
	long MaxPaty;
	RbGameData& UserData;
};

#endif // !defined(AFX_STOCKMEMBERWINDOW_H__F47CFBB4_57FE_444B_BE6E_4ECCE48361A5__INCLUDED_)
