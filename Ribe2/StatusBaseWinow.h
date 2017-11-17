// StatusBaseWinow.h: StatusBaseWinow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSBASEWINOW_H__797575AF_2EA5_4AB5_9212_08D0642396DC__INCLUDED_)
#define AFX_STATUSBASEWINOW_H__797575AF_2EA5_4AB5_9212_08D0642396DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbStatus.h"
#include "BaseWindow.h"

class StatusBaseWinow : public BaseWindow  
{
public:
	StatusBaseWinow();
	virtual ~StatusBaseWinow();

	virtual bool SetParameter( const CComVariant& Value );
	virtual bool CheckParameter( const CComVariant& Value ) const;

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
protected:
	HRESULT DrawImage( RbSprite& Display, float x, float y );
	HRESULT DrawImage( RbSprite& Display, const RbRect& Rect );
	virtual bool RedrawCheck() const;
	long GetBattleID() const;
	bool SetViewOn() const;
	float GetValue( const DefaultStatus& Ref, const String& Name ) const;
	bool CopyStatusWindow( WindowSystem& System, BaseWindow* pWnd )const;

	RefPtr<RbStatus> pStatus;
	double LastRedraw;
	long UpdateID;

	RbRect ImageArea;

	static LPCTSTR StatusFont;
	static LPCTSTR MinNumFont;
	static LPCTSTR NumFont;
	static LPCTSTR TagFont;
};

#endif // !defined(AFX_STATUSBASEWINOW_H__797575AF_2EA5_4AB5_9212_08D0642396DC__INCLUDED_)
