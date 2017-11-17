// NewsListWindow.h: NewsListWindow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWSLISTWINDOW_H__FB482A7B_5A08_4F3E_8B25_D5EE38701C38__INCLUDED_)
#define AFX_NEWSLISTWINDOW_H__FB482A7B_5A08_4F3E_8B25_D5EE38701C38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "ScrollWindow.h"
#include "BackReader.h"

class NewsListWindow : public ScrollWindow, BackReader<RefPtr<RefAuto>,THREAD_PRIORITY_NORMAL>
{
public:
	NewsListWindow();
	virtual ~NewsListWindow();
	virtual LPCTSTR WindowName() const;

	virtual bool CheckParameter( const CComVariant& Value ) const;
	virtual bool SetParameter( const CComVariant& Value );
	virtual HRESULT Draw( RbSprite& Display, WindowSystem& System );
protected:
	virtual bool LoadCache( const RefPtr<RefAuto>& ReadData );
	virtual HRESULT Restore( DISPDEVICE* pDevice, WindowSystem& System );
	virtual HRESULT RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index );
	virtual RbRect HitCursor( WindowSystem& System, int x, int y );
	virtual bool ClickJob( int x, int y );
	virtual String GetXMLPath( BSTR Param ) const;
	long GetNewsIndex( int x, int y ) const;
	bool CheckNew( const String& FilePath ) const;

	bool LoadList( LPCTSTR FilePath );
	bool AddNews( const XMLPaser& NewsXML );
	struct _Rects
	{
		RbRect ListArea;
		RbRect NewsRect;
		RbRect NewTag;
		RbRect Title;
	}Rects;
	struct NewsItem : public RefAuto
	{
		bool SetXML( const XMLPaser& NewsXML, const String& FlgPrefix );
		String Name;
		String Flg;
		String Title;
		bool bNew;
	};
	std::list<RefPtr<NewsItem> > NewsList;
	String Prefix;
	String Reading;
	String XMLFilePath;
	long UpDateID, OldUpDateID;
};

#endif // !defined(AFX_NEWSLISTWINDOW_H__FB482A7B_5A08_4F3E_8B25_D5EE38701C38__INCLUDED_)
