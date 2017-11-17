// ItemListWindow.h: ItemListWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMLISTWINDOW_H__EE3FEF92_C13B_439C_8D84_B9EE94E69319__INCLUDED_)
#define AFX_ITEMLISTWINDOW_H__EE3FEF92_C13B_439C_8D84_B9EE94E69319__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShopSystem.h"
#include "ScrollWindow.h"

class ItemListWindow : public ScrollWindow  
{
public:
	ItemListWindow();
	virtual ~ItemListWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual bool SetParameter( const CComVariant& Value );
	virtual HRESULT Command( DISPDEVICE* pDevice, WindowSystem& System, command com );
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual HRESULT RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index );
	HRESULT DrawItem( WindowSystem& System, const RbRect& Rect, const ShopSystem::ShopData& Data ) const;
	void DrawType( WindowSystem& System, LPCTSTR Name, const RbRect& Rect, long Type ) const;
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual bool ClickJob( int x, int y );
	virtual bool Floating( int x, int y );
	virtual bool DragStartJob( WindowSystem& System, GameMessage& State );
	virtual bool ReLoadList();
	long GetItemIndex( int x, int y ) const;
	long GetItemNum( int x, int y ) const;
	bool ChangeItemType( long NewType );
	ARGB GetCol( long Type ) const;
	bool EquipJob( long ItemNum, bool bChange ) const;
	bool EquipCancel() const;

	bool bEquipJobOK;
	long OldUpdateID;
	long NowItemType;
	String EquipType;
	ShopSystem::ItemList List;
	struct _Rects
	{
		RbRect ListArea;
		RbRect Item;
		RbRect Name;
		RbRect Count;
		RbRect Price;
		RbRect WepTag;
		RbRect BodyTag;
		RbRect AcsTag;
		RbRect PowTag;
		RbRect EventTag;
		String HighStr;
	}Rects;
	bool ShowType[eTypeItemMax];
	String UnitStr;
};

#endif // !defined(AFX_ITEMLISTWINDOW_H__EE3FEF92_C13B_439C_8D84_B9EE94E69319__INCLUDED_)
