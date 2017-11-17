// BuyWindow.h: BuyWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUYWINDOW_H__CCFFF6F3_8EBB_4CC9_92BD_7755DF1E991E__INCLUDED_)
#define AFX_BUYWINDOW_H__CCFFF6F3_8EBB_4CC9_92BD_7755DF1E991E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusStruct.h"
#include "BaseWindow.h"
class RbGameData;

class BuyWindow : public BaseWindow  
{
public:
	BuyWindow();
	virtual ~BuyWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool SetParameter( const CComVariant& Value );
	virtual bool CheckParameter( const CComVariant& Value ) const;

protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual bool ClickJob( int x, int y );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual bool SetItem( short ItemNum );
	bool AddCount();
	virtual long GetPrice() const;
	virtual bool AcceptJob();
	RbRect ClickArea;
	String AcceptMsg;

	ItemStatus ItemData;
	long ItemCount;
	RbGameData& UserData;
};

#endif // !defined(AFX_BUYWINDOW_H__CCFFF6F3_8EBB_4CC9_92BD_7755DF1E991E__INCLUDED_)
