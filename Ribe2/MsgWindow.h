// MsgWindow.h: MsgWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGWINDOW_H__CE73D7B8_1AF3_41F6_9684_35480B1B81FF__INCLUDED_)
#define AFX_MSGWINDOW_H__CE73D7B8_1AF3_41F6_9684_35480B1B81FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MsgBaseWindow.h"

class MsgWindow : public MsgBaseWindow
{
public:
	MsgWindow();
	virtual ~MsgWindow();
	virtual LPCTSTR WindowName() const;
	virtual bool SetXML( const XMLPaser& MsgXML );

	virtual bool SetMessage( LPCTSTR NewMessage );
	virtual bool SetCharactor( RbBattleStatus* pData );

	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual HRESULT Command( DISPDEVICE* pDevice, WindowSystem& System, command com );

protected:
	bool LockDisplay( const D3DVECTOR& Pos );
	LockCounter::Locker BattleLocker;
	bool bDisplayLock;
};

#endif // !defined(AFX_MSGWINDOW_H__CE73D7B8_1AF3_41F6_9684_35480B1B81FF__INCLUDED_)
