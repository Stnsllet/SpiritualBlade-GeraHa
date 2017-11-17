// SelectMsgWindow.h: SelectMsgWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTMSGWINDOW_H__0ED6C904_F884_48CF_9B92_E24B2603578F__INCLUDED_)
#define AFX_SELECTMSGWINDOW_H__0ED6C904_F884_48CF_9B92_E24B2603578F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLPaser.h"
#include "MsgWindow.h"

class SelectMsgWindow : public MsgWindow  
{
public:
	SelectMsgWindow();
	virtual ~SelectMsgWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool SetXML( const XMLPaser& MsgXML );

	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
protected:
	bool ClickJob( const GameMessage& State ) const;
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	struct SelectData
	{
		XMLPaser XML;
		RbRect MsgRect;
		String Msg;
		String Flag;
	};
	RbRect BoundRect;

	RbRect BoundCheck( const TexRect& RefFace );
	bool AddSelectItem( const XMLPaser& AddXML );
	std::vector<SelectData> SelectList;
};

#endif // !defined(AFX_SELECTMSGWINDOW_H__0ED6C904_F884_48CF_9B92_E24B2603578F__INCLUDED_)
