// PoweredWindow.h: PoweredWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWEREDWINDOW_H__621133A9_7CB2_4DE2_B91E_E0CC08F97844__INCLUDED_)
#define AFX_POWEREDWINDOW_H__621133A9_7CB2_4DE2_B91E_E0CC08F97844__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "BaseWindow.h"

class PoweredWindow : public BaseWindow  
{
public:
	PoweredWindow();
	virtual ~PoweredWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool SetXML( const XMLPaser& MsgXML );
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool HitTest( int x, int y ) const{return false;}
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	bool AddPow( const XMLPaser& PowXML );
	struct PowMsg
	{
		RbRect Rect;
		String TexPath;
		String Msg;
		String Font;
		ARGB Color;
	};
	std::list<PowMsg> PowList;
	ARGB BackColor;
	double Life;
	double StartTime;
};

#endif // !defined(AFX_POWEREDWINDOW_H__621133A9_7CB2_4DE2_B91E_E0CC08F97844__INCLUDED_)
