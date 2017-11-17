// ScrollWindow.h: ScrollWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCROLLWINDOW_H__620FECA4_60A2_466D_83F8_39E0B71FEB03__INCLUDED_)
#define AFX_SCROLLWINDOW_H__620FECA4_60A2_466D_83F8_39E0B71FEB03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "BaseWindow.h"

class ScrollWindow : public BaseWindow  
{
public:
	ScrollWindow();
	virtual ~ScrollWindow();

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual HRESULT Command( DISPDEVICE* pDevice, WindowSystem& System, command com );
protected:
	bool AllTexRelease();
	bool ScrollTexRelease();
	long ScrollPos( long Index=0 ) const;
	HRESULT DrawScrollList( RbSprite& Display, WindowSystem& System );
	virtual HRESULT RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index );
	struct ScrollInfo
	{
		ScrollInfo();
		struct PosStock
		{
			PosStock( ScrollInfo& Base );
			~PosStock();
			int OldPos;
			ScrollInfo& Ref;
		};
		bool SetInfo( const RbRect& Area, int SrcHeight );
		bool MouseJob( WindowSystem& System, GameMessage& State, int x, int y );
		bool Scroll( int y );
		bool Pin( int x, int y ) const;
		bool CanScroll() const;
		HRESULT CreateTexture( DISPDEVICE* pDevice );
		HRESULT SetSprite( DISPDEVICE* pDevice, RbSprite& Surface );
		int NowPos;
		int Height;
		int ScrollDepth;
		RbRect ViewArea;
		Texture Tex;
		Plane<> TopPlane, BarPlane, BottomPlane;
		HRESULT Draw( RbSprite& Display, float x, float y );
		HRESULT DrawBar( RbSprite& Display, float x, float y );
	};
	std::vector<ScrollInfo> ScrollList;
};

#endif // !defined(AFX_SCROLLWINDOW_H__620FECA4_60A2_466D_83F8_39E0B71FEB03__INCLUDED_)
