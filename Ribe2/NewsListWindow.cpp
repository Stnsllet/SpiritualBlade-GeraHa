// NewsListWindow.cpp: NewsListWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbCache.h"
#include "RbGameData.h"
#include "RbWindowSystem.h"
#include "XMLList.h"
#include "NewsWindow.h"
#include "StrCommander.h"
#include "NewsListWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

NewsListWindow::NewsListWindow()
{
	Prefix = _T("n_");
	Reading = _T("Reading...");
	UpDateID = OldUpDateID = 0;
}

NewsListWindow::~NewsListWindow()
{
	TermThread();
}

bool NewsListWindow::SetParameter( const CComVariant& Value )
{
	if( Value.vt != VT_BSTR )return false;
	const String& NewPath = GetXMLPath(Value.bstrVal);
	if( NewPath.empty() )return false;
	if( XMLFilePath == NewPath )return true;
	XMLFilePath = NewPath;
	NewsList.clear();
	ScrollTexRelease();
	return true;
}

bool NewsListWindow::CheckParameter( const CComVariant& Value ) const
{
	if( Value.vt == VT_BSTR )return (XMLFilePath==GetXMLPath(Value.bstrVal));
	if( Value.vt == VT_BOOL )return CheckNew(XMLFilePath);
	return false;
}

String NewsListWindow::GetXMLPath( BSTR Param ) const
{
	if( Param == NULL )return String();
	String Dir = _T("Data/");
	String File(Param);
	Dir += File;
	return Dir;
}

bool NewsListWindow::CheckNew( const String& FilePath ) const
{
	const RbGameData& UserData = GetGameData();

//	XMLPaser AllXML;
//	HRESULT hr = AllXML.Load( FilePath );
//	if( FAILED(hr) )return false;
	XMLPaser AllXML = GetCache().XML.Get( FilePath );
	if( !AllXML )return false;

	XMLList NewsListXML;
	NewsListXML.Query( AllXML, _T("News") );
	long Count = NewsListXML.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		NewsItem NewData;
		if( !NewData.SetXML( NewsListXML.GetItem(i), Prefix ) )continue;
		if( !UserData.GetFlg(NewData.Flg) )return true;
	}
	return false;
}

LPCTSTR NewsListWindow::WindowName() const
{
	return _T("NewsList");
}

bool NewsListWindow::LoadList( LPCTSTR FilePath )
{
	XMLPaser AllXML;
	HRESULT hr = AllXML.Load( FilePath );
	if( FAILED(hr) )return false;

	XMLList NewsListXML;
	NewsListXML.Query( AllXML, _T("News") );
	long Count = NewsListXML.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		AddNews( NewsListXML.GetItem(i) );
	}
	return true;
}

bool NewsListWindow::AddNews( const XMLPaser& NewsXML )
{
	RefPtr<NewsItem> pAddItem(new NewsItem);
	if( !pAddItem->SetXML( NewsXML, Prefix ) )return false;
	pAddItem->Title = Reading;

	std::list<RefPtr<NewsItem> >::iterator pos = NewsList.end();
	if( pAddItem->bNew )
		for( pos = NewsList.begin(); pos != NewsList.end(); pos++ )
			if( !(*pos)->bNew )break;

	NewsList.insert( pos, pAddItem );
	Que.Push((RefAuto*)pAddItem);
	return true;
}

bool NewsListWindow::NewsItem::SetXML( const XMLPaser& NewsXML, const String& FlgPrefix )
{
	const RbGameData& UserData = GetGameData();

	String FlgOn = NewsXML.AttributeString(TAG::ON);
	if( !FlgOn.empty() )if( !UserData.CheckFlgs( FlgOn, true ) )return false;
	String FlgOff = NewsXML.AttributeString(TAG::OFF);
	if( !FlgOff.empty() )if( !UserData.CheckFlgs( FlgOff, false ) )return false;

	Name = NewsXML.AttributeString(TAG::NAME);
	if( Name.empty() )return false;
	Flg = NewsXML.AttributeString(TAG::FLG);
	if( Flg.empty() ){(Flg = FlgPrefix) += Name;}
	bNew = !UserData.GetFlg( Flg );

	return true;
}

HRESULT NewsListWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( UpDateID != OldUpDateID )
	{
		ScrollTexRelease();
		OldUpDateID = UpDateID;
	}
	return ScrollWindow::Draw( Display, System );
}

HRESULT NewsListWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_FALSE;
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	if( XMLFilePath.empty() )XMLFilePath = pPlanes->XML.AttributeString(TAG::PATH);

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	Rects.ListArea = pPlanes->GetRect(_T("list"));
	Rects.NewsRect = pPlanes->GetRect(_T("news"));
	Rects.NewTag = pPlanes->GetRect(_T("new"));
	Rects.Title = pPlanes->GetRect(_T("title"));

	ExSprite Surface;
	HRESULT hr = RestoreBase( pDevice, Surface, System );
	if( FAILED(hr) )return hr;
	DrawWindowTag(System, WindowName() );

	ScrollList.resize(1);
	RestoreScroll( pDevice, System, 0 );
	return S_OK;
}

HRESULT NewsListWindow::RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index )
{
	if( Index >= ScrollList.size() )return S_FALSE;
	if( Index != 0 )return ScrollWindow::RestoreScroll( pDevice, System, Index );

	if( NewsList.size() <= 0 )LoadList( XMLFilePath );

	const RbGameData& UserData = GetGameData();
	ScrollInfo& Ref = ScrollList[Index];
	ScrollInfo::PosStock PosStock( Ref );

	Plane<> NewPlane = GetCache().SystemPlane.GetPlane(_T("new"));

	long Count = NewsList.size();
	long ScrollHeight = Rects.NewsRect.Height() * Count;
	ScrollHeight += Rects.NewsRect.top * 2;
	Ref.SetInfo( Rects.ListArea, ScrollHeight );

	HRESULT hr = Ref.CreateTexture( pDevice );
	if( FAILED(hr) )return hr;

	ExSprite Surface;
	Surface.InitSprite( pDevice, Ref.Tex );
	Surface.Clear();

	RbRect NewsRect = Rects.NewsRect;
	std::list<RefPtr<NewsItem> >::const_iterator itr = NewsList.begin();
	while( itr != NewsList.end() )
	{

		RbRect NewTagRect = Rects.NewTag;
		NewTagRect.Move( NewsRect.left, NewsRect.top );
		RbRect TitleRect = Rects.Title;
		TitleRect.Move( NewsRect.left, NewsRect.top );

		const NewsItem& RefItem = *(*itr++);
		if( !UserData.GetFlg(RefItem.Flg) )
			Surface.DrawPlane(NewPlane,NewTagRect.left,NewTagRect.top);
		{
			CriticalSection Locker(Lock);
			System.Font().Draw(RefItem.Title,TitleRect,DT_CENTER);
		}
		NewsRect.BoxMove(0,1);
	}
	return S_OK;
}

bool NewsListWindow::LoadCache( const RefPtr<RefAuto>& ReadData )
{
	RefPtr<NewsWindow> pNews( new NewsWindow );
	NewsItem& Ref = *(NewsItem*)(RefAuto*)ReadData;
//	bool bOK = pNews->SetParameter( CComVariant(Ref.Name) );
//	if( !bOK )return false;
	String Title = pNews->ReadTitle(Ref.Name);
	if( Title.empty() )Title = _T("No File");
	{
		CriticalSection Locker(Lock);
		Ref.Title = Title;
		::InterlockedIncrement( &UpDateID );
	}
	return true;
}

RbRect NewsListWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( ScrollList.size() <= 0 )return RbRect(); 
	if( !Rects.ListArea.Pin( x, y ) )return RbRect();

	RbRect NewsRect = Rects.NewsRect;
	NewsRect.Move( Rects.ListArea.left, Rects.ListArea.top - ScrollList[0].NowPos );
	long Count = NewsList.size();
	for( int i = 0; i < Count; i++ )
	{
		if( NewsRect.Pin( x, y ) )return NewsRect.Clip(Rects.ListArea);
		NewsRect.BoxMove(0,1);
	}
	return RbRect();
}

long NewsListWindow::GetNewsIndex( int x, int y ) const
{
	if( ScrollList.size() <= 0 )return -1; 
	if( !Rects.ListArea.Pin( x, y ) )return -1;

	RbRect NewsRect = Rects.NewsRect;
	NewsRect.Move( Rects.ListArea.left, Rects.ListArea.top - ScrollList[0].NowPos );
	long Count = NewsList.size();
	for( int i = 0; i < Count; i++ )
	{
		if( NewsRect.Pin( x, y ) )return i;
		NewsRect.BoxMove(0,1);
	}
	return -1;
}

bool NewsListWindow::ClickJob( int x, int y )
{
	long Index = GetNewsIndex( x, y );
	if( Index < 0 || Index >= NewsList.size() )return true;

	std::list<RefPtr<NewsItem> >::const_iterator itr = NewsList.begin();
	while( (Index--) > 0 )itr++;
	const NewsItem& Ref = *(*itr);

	String Command = _T("window/show/News/");
	Command += Ref.Name;

	StrCommander com;
	com.Command( Command );

	RbGameData& UserData = GetGameData();
	UserData.SetFlg( Ref.Flg );
	::InterlockedIncrement( &UpDateID );
	return true;
}
