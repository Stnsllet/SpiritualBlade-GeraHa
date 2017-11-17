// EquipStatusWindow.h: EquipStatusWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EQUIPSTATUSWINDOW_H__6906DA44_E4D0_4105_80F5_62D5D877CCD2__INCLUDED_)
#define AFX_EQUIPSTATUSWINDOW_H__6906DA44_E4D0_4105_80F5_62D5D877CCD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusWindow.h"
class RbGameData;

class EquipStatusWindow : public StatusWindow  
{
public:
	EquipStatusWindow();
	virtual ~EquipStatusWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool SetParameter( const CComVariant& Value );

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual bool DropJob( const DragItemWindow& DragItem );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual void DrawValue( WindowSystem& System, const WindowPlanes* pPlanes, const String& Name ) const;
	virtual bool ClickJob( int x, int y );
	bool ShowItemList( short Type ) const;
	virtual bool RedrawCheck() const;

	bool SetMember( LPCTSTR PasonName );
	bool SetItem( long ItemLong );
	bool RestoreStatus();
	bool EquipJob();
	bool UnEquipJob( int x, int y );
	bool UnEquipJob( short Type );

	String MemberName;
	ItemID EquipItem;
	DefaultStatus EquipDiff;

	RbGameData& UserData;
	long OldUpdateID;
	bool bExPatyMode;
};

#endif // !defined(AFX_EQUIPSTATUSWINDOW_H__6906DA44_E4D0_4105_80F5_62D5D877CCD2__INCLUDED_)
