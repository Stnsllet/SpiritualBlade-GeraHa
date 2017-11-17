// MoneyWindow.h: MoneyWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONEYWINDOW_H__9803A11D_19CC_423A_B7C6_F04DF4039A86__INCLUDED_)
#define AFX_MONEYWINDOW_H__9803A11D_19CC_423A_B7C6_F04DF4039A86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"

class MoneyWindow : public BaseWindow  
{
public:
	MoneyWindow();
	virtual ~MoneyWindow();
	virtual LPCTSTR WindowName() const;

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );

	long NowMoney;
};

#endif // !defined(AFX_MONEYWINDOW_H__9803A11D_19CC_423A_B7C6_F04DF4039A86__INCLUDED_)
