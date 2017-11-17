// PopUpMsgWindow.h: PopUpMsgWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPUPMSGWINDOW_H__85864CF3_1C78_4235_B751_E455AB10E0FB__INCLUDED_)
#define AFX_POPUPMSGWINDOW_H__85864CF3_1C78_4235_B751_E455AB10E0FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MsgBaseWindow.h"
class RbBattleStatus;

class PopUpMsgWindow : public MsgBaseWindow  
{
public:
	PopUpMsgWindow();
	virtual ~PopUpMsgWindow();
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );

	bool SetMessage( LPCTSTR NewMessage );
protected:
	double StartTime;
	double Life;
};

#endif // !defined(AFX_POPUPMSGWINDOW_H__85864CF3_1C78_4235_B751_E455AB10E0FB__INCLUDED_)
