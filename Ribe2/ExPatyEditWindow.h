// ExPatyEditWindow.h: ExPatyEditWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPATYEDITWINDOW_H__A3A5E9E3_82B4_42DB_B58C_DC72C4DB6039__INCLUDED_)
#define AFX_EXPATYEDITWINDOW_H__A3A5E9E3_82B4_42DB_B58C_DC72C4DB6039__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PatyEditWindow.h"

class ExPatyEditWindow : public PatyEditWindow  
{
public:
	ExPatyEditWindow();
	virtual ~ExPatyEditWindow();
	virtual LPCTSTR WindowName() const;

protected:
	virtual PatyStatus GetStatus();
	virtual bool ClickMember( int x, int y );
};

#endif // !defined(AFX_EXPATYEDITWINDOW_H__A3A5E9E3_82B4_42DB_B58C_DC72C4DB6039__INCLUDED_)
