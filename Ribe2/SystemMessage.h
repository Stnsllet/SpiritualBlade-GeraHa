
#ifndef __SYSTEM_MESSAGE__
#define __SYSTEM_MESSAGE__

#include "RefObject.h"

enum SystemMessage
{
	eNonMessage = 0,
	eLDown,
	eRDown,
	eLUp,
	eRUp,
	eLRDown,
	eLDblClick,
	eRDblClick,
	eDrop,

	eMouseMove,
	eScrollUp,
	eScrollDown,

};

enum GameMode
{
	eNoneMode = -1,
	eWorldMode = 0,
	eBattleMode,
	eEventMode,
};

enum DragingType
{
	eNonDraging = 0,
	eDisplayScroll,
	eDragWindow,
	eDragMember,
	eDragItem,
};

struct GameMessage
{
protected:
	SystemMessage SysMsg;
public:
	HWND hWnd;
	operator =( const SystemMessage& NewMsg )
	{
		SysMsg = NewMsg;
		SetDrop();
	}
	operator const SystemMessage&() const{return SysMsg;}
	const SystemMessage& Msg() const{return *this;}
	bool LDown;
	bool RDown;
	struct _Drag
	{
		_Drag(){bDrag = false;}
		SystemMessage StartKey;
		bool bDrag;
	}Drag;
	int x,y;
	int OldX, OldY;
	D3DVECTOR ViewPos;
	D3DVECTOR OldViewPos;
	bool IsDraging() const{return Drag.bDrag;}
	void DragStart()
	{
		::SetCapture( hWnd );
		Drag.StartKey = SysMsg;
		Drag.bDrag = true;
	}
	bool SetDrop()
	{
		if( !IsDraging() )return false;
		if( Drag.StartKey == eRDown )if( SysMsg == eRUp )SysMsg =eDrop;
		if( Drag.StartKey == eLDown )if( SysMsg == eLUp )SysMsg =eDrop;
		if( Drag.StartKey == eLRDown )if( SysMsg == eLUp || SysMsg == eRUp )SysMsg =eDrop;
		return (SysMsg == eDrop);
	}
	void DragEnd()
	{
		Drag.bDrag = false;
		::ReleaseCapture();
	}
};

#endif // __SYSTEM_MESSAGE__