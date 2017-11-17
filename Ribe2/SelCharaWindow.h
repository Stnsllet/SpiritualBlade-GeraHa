// SelCharaWindow.h: SelCharaWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELCHARAWINDOW_H__E12144D9_70E6_4418_9C55_C35FDE04A3AC__INCLUDED_)
#define AFX_SELCHARAWINDOW_H__E12144D9_70E6_4418_9C55_C35FDE04A3AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusBaseWinow.h"

class SelCharaWindow : public StatusBaseWinow  
{
public:
	SelCharaWindow();
	virtual ~SelCharaWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual bool RedrawCheck() const;
};

#endif // !defined(AFX_SELCHARAWINDOW_H__E12144D9_70E6_4418_9C55_C35FDE04A3AC__INCLUDED_)
