// RbDisplay.h: RbDisplay クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBDISPLAY_H__B510EB82_B8EA_4E18_BA42_C344C5AE060F__INCLUDED_)
#define AFX_RBDISPLAY_H__B510EB82_B8EA_4E18_BA42_C344C5AE060F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbPosition.h"
#include "RbRect.h"
#include "Display.h"
class XMLPaser;

class RbConfig;

class RbDisplay : public Display  
{
public:
	RbDisplay();
	virtual ~RbDisplay();
	bool LoadSystemXML( const XMLPaser& ReadXML );

	bool SetDefaultScale( float Scale );
	bool SetDefaultAngle( float angle );
	HRESULT SetConfig( RbConfig& Config ) const;
//	void SetZoom( float NewZoom, float NewAngle, double Life=500 );
	void SetZoom( float NewZoom, double Life=500 );
	void SetAngle( float NewAngle, double Life=500 );
	void SetViewOn( const D3DVECTOR& Pos, double Life=500 );
	void SetViewOn( float x, float y, float z=0, double Life=500 ){SetViewOn(RbPosition(x,y,z),Life);}
	void SetViewArea( const RbRectF& Area );
	void SetShake( float Scale, double Life=500 );
	HRESULT Job();
	void ClearJob();

	virtual HRESULT SetViewPos( float x, float y, float z=0.0f);
	virtual HRESULT ScrollView( float x, float y, float z=0.0f);
	virtual HRESULT SetRenderState( const D3DCAPS8& HalCaps );
	virtual HRESULT WindowSizeChange( UINT Width, UINT Height );
	HRESULT FullScreenWindow( const D3DDISPLAYMODE& DisplayMode );
	virtual HRESULT FullScreenWindow( const D3DDISPLAYMODE& DisplayMode, UINT Width, UINT Height );
	HRESULT ChangeDisplayBaseScale( float NewScale=1.0f, bool bNow=false );

	void LockView( const D3DVECTOR& Pos, double Life=500 );
	void LockView( float x, float y, float z=0, double Life=500 ){LockView(RbPosition(x,y,z),Life);}
	void UnLockView( double Life=500 );

	float GetShake() const;
	float DestZoom() const{return (Zoom.Dest<=0)?(1):(1/Zoom.Dest);}
	float NowZoom() const{return (Zoom.Now<=0)?(1):(1/Zoom.Now);}
protected:
	bool IsViewLock() const{return false;}
	bool ClipView();
	void ChangeDispSize( float Width, float Height );
	bool ResetViewData();

	struct _ZoomInfo
	{
		float Now;
		float Base;
		float Dest;
		double Life;
		double StartTime;
	}Zoom;
	struct _AngleInfo
	{
		float Now;
		float Base;
		float Dest;
		double Life;
		double StartTime;
	}AngleInfo;
	struct _ScrollInfo
	{
		RbPosition Dest;
		double Life;
		double StartTime;
	}Scroll;
	double JobInterval;
	double JobOldTime;
	float DefaltScale;

	struct _ViewLock
	{
		RbPosition OldPos;
		long LockCount;
	}ViewLock;

	struct _Shake
	{
		float Scale;
		double Life;
		double StartTime;
	}Shake;

	struct _ViewArea
	{
		RbRectF Area;
	}ViewArea;
};

RbDisplay& GetRbDisplay();

#endif // !defined(AFX_RBDISPLAY_H__B510EB82_B8EA_4E18_BA42_C344C5AE060F__INCLUDED_)
