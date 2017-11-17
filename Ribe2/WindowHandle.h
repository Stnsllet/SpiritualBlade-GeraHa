// WindowHandle.h: WindowHandle クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWHANDLE_H__8DDE2EBF_A5BE_47DE_A61F_7A74098E2CA0__INCLUDED_)
#define AFX_WINDOWHANDLE_H__8DDE2EBF_A5BE_47DE_A61F_7A74098E2CA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class WindowHandle  
{
public:
	WindowHandle();
	WindowHandle( HWND hWnd );
	virtual ~WindowHandle();

	bool SetHandle( HWND hWnd );
	operator HWND() const{return m_hWnd;}

	RECT ClientToWindow( UINT Width, UINT Height );
	bool SetClientSize( UINT Width, UINT Height );

	RECT ScalingSize( const RECT& Rect, float ScaleWidth, float ScaleHeight, bool bChangeHeight=true );
	RECT ScaleSizing( const RECT& TagetRect, float ScaleWidth, float ScaleHeight, UINT MinWidth, UINT MinHeight );
	RECT MiniClip( const RECT& TagetRect, UINT MinWidth, UINT MinHeight );
protected:
	HWND m_hWnd;
};

#endif // !defined(AFX_WINDOWHANDLE_H__8DDE2EBF_A5BE_47DE_A61F_7A74098E2CA0__INCLUDED_)
