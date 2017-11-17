// RbGlobal.h: RbGlobal クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBGLOBAL_H__DDC659D9_7A78_41FB_8D8D_6792C77EEBC3__INCLUDED_)
#define AFX_RBGLOBAL_H__DDC659D9_7A78_41FB_8D8D_6792C77EEBC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlobalWindow.h"

class RbGlobal : public GlobalWindow  
{
public:
	RbGlobal();
	virtual ~RbGlobal();

	bool CheckMaster();
	bool WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );

private:
	HANDLE hGlobal;
	bool bMaster;
};

#endif // !defined(AFX_RBGLOBAL_H__DDC659D9_7A78_41FB_8D8D_6792C77EEBC3__INCLUDED_)
