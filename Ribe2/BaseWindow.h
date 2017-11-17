// BaseWindow.h: BaseWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEWINDOW_H__BCBB1761_A31B_45AC_909D_FF9622B6BAD4__INCLUDED_)
#define AFX_BASEWINDOW_H__BCBB1761_A31B_45AC_909D_FF9622B6BAD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Device;
class WindowSystem;
class DragItemWindow;
#include "RbRect.h"
#include "RefObject.h"
#include "ExSprite.h"
#include "Texture.h"
#include "WindowDefine.h"

class BaseWindow  : public RefAuto
{
public:
	typedef WindowDefine::WindowPlane WindowPlane;
	typedef WindowDefine::WindowPlanes WindowPlanes;
	typedef WindowDefine::WindowPlaneMap WindowPlaneMap;
	enum command
	{
		eNoneCommand = 0,
		eCreate,
		eRestore,
		eReleaseSurface,
		eDestroy,
	};
public:
	BaseWindow();
	virtual ~BaseWindow();
	virtual LPCTSTR WindowName() const;
	bool IsHide() const{return bHide;}
	const long& RefTopLevel() const{return TopLevel;}

	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
	virtual HRESULT DrawCursor( RbSprite& Display, WindowSystem& System, const GameMessage& State );
	virtual HRESULT Command( DISPDEVICE* pDevice, WindowSystem& System, command com );
	virtual bool SetParameter( const CComVariant& Value ){return false;}
	virtual bool DropJob( const DragItemWindow& DragItem ){return false;}
	virtual bool CheckParameter( const CComVariant& Value ) const{return false;}
	virtual bool HitTest( int x, int y ) const;
	virtual bool MouseJob( WindowSystem& System, GameMessage& State );
	virtual bool SetDefaultPosition( WindowSystem& System );
	virtual void SetPosition( int x, int y ){Position.x=x;Position.y=y;}
	void SetFloatPosition( WindowSystem& System, float x, float y );
	virtual void SetFloatPosition( int Width, int Height, float x, float y );
	RbPointF GetFloatPosition( WindowSystem& System ) const;
	RbRect GetRect() const{return RbRect( Position.x,Position.y,Position.Width,Position.Height);}
	RbRect ToGlobalRect( const RbRect& Ref ) const{return RbRect(Ref).Move(Position.x,Position.y);}
	bool Close( WindowSystem& System );

protected:
	bool RestoreCheck( RbSprite& Display, WindowSystem& System );
	HRESULT SpriteRestore( RbSprite& Display, WindowSystem& System );
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	HRESULT RestoreBase( DISPDEVICE* pDevice, RbSprite& Surface, WindowSystem& System );
	HRESULT CreateBase( DISPDEVICE* pDevice, WindowSystem& System, int Width, int Height );
	HRESULT DrawSurfaceBase( RbSprite& Surface, WindowSystem& System, const RbRect& Rect );
	HRESULT DrawWindowTag( WindowSystem& System, LPCTSTR Name, int x=8, int y=1 ) const;
	bool ClipPosition( WindowSystem& System );
	bool ClipPosition( RbSprite& Display );
	bool ClipPosition( int Width, int Height );

	virtual bool ClickJob( int x, int y ){return true;}
	virtual bool Floating( int x, int y ){return false;}
	virtual bool DragStartJob( WindowSystem& System, GameMessage& State ){return false;}
	virtual HRESULT NormalCursor( RbSprite& Display, WindowSystem& System, int x, int y );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
protected:
	struct WindowPosition
	{
		int x, y, Width, Height;
	}Position;
	struct FloatPosition
	{
		bool bLock;
		float x, y;
		float sx,sy;
	}FloatPos;
	RbPoint DragPoint;
	static LPCTSTR WindowTagFont;
	RbRect CenterRect;
	Texture TexWindow;
	long TopLevel;
	bool bDestroyOK,  bMoveOK, bHide, bCursorSound, bOutView;
};

#endif // !defined(AFX_BASEWINDOW_H__BCBB1761_A31B_45AC_909D_FF9622B6BAD4__INCLUDED_)
