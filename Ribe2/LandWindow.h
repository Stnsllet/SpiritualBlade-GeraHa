// LandWindow.h: LandWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDWINDOW_H__0A433A58_CAE8_421F_B1C6_EB747397CAA6__INCLUDED_)
#define AFX_LANDWINDOW_H__0A433A58_CAE8_421F_B1C6_EB747397CAA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseWindow.h"
class RbDisplay;
class LandScape;

class LandWindow : public BaseWindow
{
public:
	LandWindow();
	virtual ~LandWindow();
	virtual LPCTSTR WindowName() const;
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );

	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	HRESULT DrawLand( RbDisplay& Disp, LandScape& Land, Texture& Tex );
	HRESULT DrawLand( RbDisplay& Disp, LandScape& Land, const RbPointF& Center, float Scale, const RbTransform& WorldTrans );
	HRESULT DrawLand( RbDisplay& Disp, LandScape& Land );

	struct TransData;
	HRESULT DrawViewArea( RbSprite& Display, const TransData& Trans );
	HRESULT DrawPoints( RbSprite& Display, const TransData& Trans );

	bool Scroll( int x, int y ) const;
	struct TransData
	{
		TransData( const RbRect& RefDrawArea, float RefViewScale );
		RbPointF ToDisplay( float x, float y ) const;
		RbPointF ToView( float x, float y ) const;

		RbRectF LandArea;
		RbPointF LandCenter;
		RbPointF DrawCenter;
		const RbRect& DrawArea;
		float Scale;
		Display& Disp;
		LandScape& Land;
	};

	float ViewScale;
	float LandScale;
	RbRect LandRect;
	Plane<> White;
	ARGBF FriColor, EneColor;

	double UpdateTime;
};

#endif // !defined(AFX_LANDWINDOW_H__0A433A58_CAE8_421F_B1C6_EB747397CAA6__INCLUDED_)
