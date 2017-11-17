// LoadWindow.h: LoadWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADWINDOW_H__C39026B7_029A_4859_9E8A_7C36538F80A2__INCLUDED_)
#define AFX_LOADWINDOW_H__C39026B7_029A_4859_9E8A_7C36538F80A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScrollWindow.h"
#include "BackReader.h"

class LoadWindow : public ScrollWindow, BackReader<void*,THREAD_PRIORITY_NORMAL>
{
public:
	LoadWindow();
	virtual ~LoadWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool CheckParameter( const CComVariant& Value ) const;
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
protected:
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual HRESULT RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual bool LoadCache(  void* const & ReadData );

	long GetFileIndex( int x, int y ) const;
	virtual bool ClickJob( int x, int y );
	virtual String GetFilePath(long Index) const;
	virtual bool FileJob( const String& FilePath );
	bool CheckFiles() const;

	struct _Rects
	{
		RbRect ListArea;
		RbRect File;
		RbRect Name;
		long DispCount;
	}Rects;

	struct LoadFiles
	{
		long FileNo;
		String DisplayName;
	};
	std::vector<LoadFiles> Files;
	long UpDateID;
	long OldUpDateID;
};

#endif // !defined(AFX_LOADWINDOW_H__C39026B7_029A_4859_9E8A_7C36538F80A2__INCLUDED_)
