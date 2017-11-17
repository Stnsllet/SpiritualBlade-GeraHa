// GlobalWindow.h: GlobalWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALWINDOW_H__D424197C_0987_44C4_92D1_AFAC2E351B02__INCLUDED_)
#define AFX_GLOBALWINDOW_H__D424197C_0987_44C4_92D1_AFAC2E351B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GlobalWindow  
{
public:
	enum GlobalMsg
	{
		eGlobalNone=0,
		eGlobalActive,
		eFullScreen,
		eDispScale,
		eWindowChange,
		eBGMVol,
		eSoundVol,
		eWndChange,
		eMojiAdjust,
	};
	GlobalWindow();
	virtual ~GlobalWindow();
	template<class T> void SendCast( GlobalMsg Msg, T& Value )
	{
		ATLASSERT( sizeof(T) == sizeof(LPARAM) );
		Send( Msg, *((LPARAM*)(T*)&Value) );
	}
	void Send( GlobalMsg Msg, LPARAM Value );
protected:
	UINT MsgID;
};

#endif // !defined(AFX_GLOBALWINDOW_H__D424197C_0987_44C4_92D1_AFAC2E351B02__INCLUDED_)
