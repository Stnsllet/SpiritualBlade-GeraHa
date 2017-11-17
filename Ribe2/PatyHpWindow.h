// PatyHpWindow.h: PatyHpWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATYHPWINDOW_H__EFA6B6B6_D42E_42EE_9B7E_A8EAB1EFB490__INCLUDED_)
#define AFX_PATYHPWINDOW_H__EFA6B6B6_D42E_42EE_9B7E_A8EAB1EFB490__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <list>
#include "RbActStatus.h"
#include "BaseWindow.h"
class RbBattlePaty;

class PatyHpWindow : public BaseWindow  
{
public:
	PatyHpWindow();
	virtual ~PatyHpWindow();
	virtual LPCTSTR WindowName() const;
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool SetParameter( const CComVariant& Value );
	virtual bool CheckParameter( const CComVariant& Value ) const;

	bool AddPaty( RbBattlePaty* pPaty );

protected:
	virtual bool ClickJob( int x, int y );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	struct _Rects
	{
		RbRect Patys;
		RbRect Image;
		RbRect Hmp;
		RbRect Hp;
		RbRect Mp;
		RbRect SP;
		Plane<> HpPlane;
		Plane<> EHpPlane;
		Plane<> MpPlane;
		Plane<> HMpPlane;
		Plane<> SPPlane;
		Plane<> DeadPlane;
	}Rects;
	struct PatyHp
	{
		struct MemberHp
		{
			RefPtr<RbActStatus> pStatus;
			bool SetMember( RbActStatus* pMem );
			HRESULT DrawHp( RbSprite& Surface, const _Rects& Rects, const RbRect& Rect );
			HRESULT DrawHp( RbSprite& Surface, const _Rects& Rects, const RbRect& Rect, long HpHight, long MpHight );
		};
		bool SetPaty( RbBattlePaty* pPaty );
		HRESULT DrawImage( RbSprite& Surface, const RbRect& Rect ) const;
		HRESULT DrawBox( RbSprite& Surface, const _Rects& Rects, const RbRect& Rect ) const;
		HRESULT DrawHp( RbSprite& Surface, const _Rects& Rects, const RbRect& Rect );
		long ID;
		std::vector<MemberHp> Members;
	};
	std::list<PatyHp> Patys;

protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	bool ClickMember( const PatyHp::MemberHp& RefMember );
};

#endif // !defined(AFX_PATYHPWINDOW_H__EFA6B6B6_D42E_42EE_9B7E_A8EAB1EFB490__INCLUDED_)
