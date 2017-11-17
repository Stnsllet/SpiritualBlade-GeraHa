// ShopListWindow.h: ShopListWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOPLISTWINDOW_H__3884D1C1_DD4E_49C6_AABD_D9EB560DD4B7__INCLUDED_)
#define AFX_SHOPLISTWINDOW_H__3884D1C1_DD4E_49C6_AABD_D9EB560DD4B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemListWindow.h"

class ShopListWindow : public ItemListWindow  
{
public:
	ShopListWindow();
	virtual ~ShopListWindow();
	virtual LPCTSTR WindowName() const;

protected:
	virtual bool ReLoadList();
	virtual bool DragStartJob( WindowSystem& System, GameMessage& State ){return false;}
	virtual bool ClickJob( int x, int y );
	virtual bool DropJob( const DragItemWindow& DragItem );
};

#endif // !defined(AFX_SHOPLISTWINDOW_H__3884D1C1_DD4E_49C6_AABD_D9EB560DD4B7__INCLUDED_)
