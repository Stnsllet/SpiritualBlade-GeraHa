// AutoBuyWindow.h: AutoBuyWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOBUYWINDOW_H__F59D6A66_4E7B_4309_B5F9_535AC1890D22__INCLUDED_)
#define AFX_AUTOBUYWINDOW_H__F59D6A66_4E7B_4309_B5F9_535AC1890D22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"
class RbGameData;
struct PatyStatus;

class AutoBuyWindow : public BaseWindow  
{
public:
	AutoBuyWindow();
	virtual ~AutoBuyWindow();

	virtual LPCTSTR WindowName() const;

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool SetParameter( const CComVariant& Value );
protected:
	virtual bool ClickJob( int x, int y );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	long AutoBuyAll( long& Money, bool bUpdate=false );
	long AutoBuyPaty( const PatyStatus& PatyData, long& Money, bool bUpdate=false );
	long AutoBuyMember( LPCTSTR MemberName, long& Money, bool bUpdate=false );

	RbRect AcceptRect;
	long OldUpdateID;
	RbGameData& UserData;
	String MemberName;
};

#endif // !defined(AFX_AUTOBUYWINDOW_H__F59D6A66_4E7B_4309_B5F9_535AC1890D22__INCLUDED_)
