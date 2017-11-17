// SPWindow.h: SPWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPWINDOW_H__94EFE1FD_337B_43E2_946F_9B2724B09FAE__INCLUDED_)
#define AFX_SPWINDOW_H__94EFE1FD_337B_43E2_946F_9B2724B09FAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"
#include "RbActStatus.h"
class RbSprite;

class SPWindow : public BaseWindow  
{
public:
	SPWindow();
	virtual ~SPWindow();
	virtual LPCTSTR WindowName() const;
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );

	bool SetCharactor( RbActStatus* pNewStatus );

protected:
	virtual bool ClickJob( int x, int y );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	long HitSkill( int x, int y ) const;
	RbRect GetRect( long Index ) const;
	virtual bool Floating( int x, int y );
	HRESULT DrawSkill( RbSprite& Surface, WindowSystem& System, long ID, long level, bool bUsed, long Line );

	RefPtr<RbActStatus> pStatus;
	TexRect ImgPlane;
	TexRect NamePlane;

	long CheckLevel, CheckCount;
	long SkillCount;
};

#endif // !defined(AFX_SPWINDOW_H__94EFE1FD_337B_43E2_946F_9B2724B09FAE__INCLUDED_)
