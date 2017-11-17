// MiniStatusWindow.h: MiniStatusWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINISTATUSWINDOW_H__41D904F0_23EF_4439_B8BE_D2E8A5A7A9C0__INCLUDED_)
#define AFX_MINISTATUSWINDOW_H__41D904F0_23EF_4439_B8BE_D2E8A5A7A9C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusBaseWinow.h"
class TextFont;

class MiniStatusWindow : public StatusBaseWinow  
{
public:
	MiniStatusWindow();
	virtual ~MiniStatusWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );

	void DrawValue( const TextFont& Font, const WindowPlanes* pPlanes, const String& Name, LPCTSTR ShowName ) const;
	struct _Rects
	{
		RbRect Name;
		RbRect Tag;
		RbRect Value;
		RbRect Now;
		RbRect Slash;
		RbRect Source;
	}Rects;
};

#endif // !defined(AFX_MINISTATUSWINDOW_H__41D904F0_23EF_4439_B8BE_D2E8A5A7A9C0__INCLUDED_)
