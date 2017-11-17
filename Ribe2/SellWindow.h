// SellWindow.h: SellWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELLWINDOW_H__CA110CAB_8D03_4C7C_8489_C0A78DDD2E05__INCLUDED_)
#define AFX_SELLWINDOW_H__CA110CAB_8D03_4C7C_8489_C0A78DDD2E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BuyWindow.h"

class SellWindow : public BuyWindow  
{
public:
	SellWindow();
	virtual ~SellWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool DropJob( const DragItemWindow& DragItem );
protected:
	virtual bool SetItem( short ItemNum );
	virtual long GetPrice() const;
	virtual bool AcceptJob();
};

#endif // !defined(AFX_SELLWINDOW_H__CA110CAB_8D03_4C7C_8489_C0A78DDD2E05__INCLUDED_)
