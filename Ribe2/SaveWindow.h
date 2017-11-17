// SaveWindow.h: SaveWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEWINDOW_H__32D3E6EF_2440_4182_B065_37086D152CF6__INCLUDED_)
#define AFX_SAVEWINDOW_H__32D3E6EF_2440_4182_B065_37086D152CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadWindow.h"

class SaveWindow : public LoadWindow  
{
public:
	SaveWindow();
	virtual ~SaveWindow();
	virtual LPCTSTR WindowName() const;

protected:
	virtual bool FileJob( const String& FilePath );
};

#endif // !defined(AFX_SAVEWINDOW_H__32D3E6EF_2440_4182_B065_37086D152CF6__INCLUDED_)
