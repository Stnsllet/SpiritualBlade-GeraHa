// ComBaseWindow.cpp: ComBaseWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLList.h"
#include "ParamXML.h"
#include "RbWindowSystem.h"
#include "RbEventSystem.h"
#include "StrCommander.h"
#include "RbGameData.h"
#include "RbCache.h"
#include "ComBaseWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ComBaseWindow::ComBaseWindow()
{
}

ComBaseWindow::~ComBaseWindow()
{
}

LPCTSTR ComBaseWindow::WindowName() const
{
	if( MyWindowName.empty() )return _T("CommandWindow");
	return MyWindowName;
}

bool ComBaseWindow::SetDefaultPosition( WindowSystem& System )
{
	if( !SourceXML )return false;
	float x = SourceXML.AttributeLong(TAG::X);
	float y = SourceXML.AttributeLong(TAG::Y);
	SetFloatPosition( System, x/100.0f, y/100.0f );
	return true;
}

bool ComBaseWindow::CommandPlane::SetXML( const XMLPaser& PlaneXML )
{
	FlgOn = PlaneXML.AttributeString(TAG::ON);
	FlgOff = PlaneXML.AttributeString(TAG::OFF);

	Plane<> Reader;
	Reader.ReadXML( PlaneXML );
	Rect = RbRect( Reader );
	Msg = PlaneXML.AttributeString(TAG::NAME);
	Com = PlaneXML.AttributeString(_T("com"));
	Event = PlaneXML.AttributeString(_T("event"));
	Tex = PlaneXML.AttributeString(_T("tex"));

	bExec = !Com.empty() || !Event.empty();
	bView = false;
	return true;
}

bool ComBaseWindow::CommandPlane::CheckView( const RbGameData& UserData )
{
	bView = UserData.CheckFlgs( FlgOn, true ) && UserData.CheckFlgs( FlgOff, false );
	return bView;
}

bool ComBaseWindow::SetCommand( WindowSystem& System, LPCTSTR WindowName )
{
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes( WindowName );
	if( pPlanes == NULL )return false;
	return SetXML( System, pPlanes->XML );
	MyWindowName = WindowName;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;
	SetDefaultPosition( System );
	Font = pPlanes->XML.AttributeString(_T("font"));
	FloatPos.bLock = pPlanes->XML.AttributeBool(_T("lock"));
	Title = pPlanes->XML.AttributeString(_T("title"));
	UserOrStr(Title);
	bMoveOK = bDestroyOK = !FloatPos.bLock;

	XMLList ComPlaneXML;
	ComPlaneXML.Query( pPlanes->XML, TAG::PLANE );

	int Count = ComPlaneXML.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		AddPlane( ParamXML(ComPlaneXML.GetItem(i)) );
	}
	return true;
}

bool ComBaseWindow::SetXML( WindowSystem& System, const XMLPaser& ComXML )
{
	if( !ComXML )return false;
	SourceXML = ComXML;
	MyWindowName = SourceXML.AttributeString( TAG::NAME );

	Position.Width = SourceXML.AttributeLong(TAG::W);
	Position.Height = SourceXML.AttributeLong(TAG::H);
	SetDefaultPosition( System );
	Font = SourceXML.AttributeString(_T("font"));
	FloatPos.bLock = SourceXML.AttributeBool(_T("lock"));
	Title = SourceXML.AttributeString(_T("title"));
	bMoveOK = bDestroyOK = !FloatPos.bLock;

	XMLList ComPlaneXML;
	ComPlaneXML.Query( SourceXML, TAG::PLANE );

	int Count = ComPlaneXML.GetCount();
	for( int i = 0; i < Count; i++ )
	{
		AddPlane( ParamXML(ComPlaneXML.GetItem(i)) );
	}
	return true;
}

bool ComBaseWindow::AddPlane( const XMLPaser& PlaneXML )
{
	CommandPlane AddData;
	if( !AddData.SetXML( PlaneXML ) )return false;
	ComPlaneList.push_back( AddData );
	return true;
}

// レストア
HRESULT ComBaseWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_OK;
	if( Font.empty() )Font = _T("default");

	ExSprite Surface;
	HRESULT ret = RestoreBase( pDevice, Surface, System );
	if( FAILED(ret) )return ret;

	DrawWindowTag( System, Title );

	const RbGameData& UserData = GetGameData();
	std::list<CommandPlane>::iterator itr = ComPlaneList.begin();
	while( itr != ComPlaneList.end() )
	{
		CommandPlane& Ref = *(itr++);
		if( !Ref.CheckView(UserData) )continue;
		if( !Ref.Tex.empty() )
		{
			RbCache& Cache = GetCache();
			Plane<> P = Cache.SystemPlane.GetPlane( Ref.Tex );
			if( P.pTexture == NULL )
				P = Cache.PathTex.Get( Ref.Tex );
			Ref.Rect.DefaultSize( P );
			Surface.Draw( P, RbRect(P), Ref.Rect );
		}
		else
		{
			if( Ref.Rect.IsNone() )
			{
				RbRect TexRect = System.Font().GetTextRect( Font, Ref.Msg );
				Ref.Rect.ReSize( TexRect.Width(), TexRect.Height() );
			}
			System.Font().Draw( Font, Ref.Msg, Ref.Rect, DT_CENTER );
		}
	}
	return S_OK;
}

RbRect ComBaseWindow::HitCursor( WindowSystem& System, int x, int y )
{
	const RbGameData& UserData = GetGameData();
	std::list<CommandPlane>::const_iterator itr = ComPlaneList.begin();
	while( itr != ComPlaneList.end() )
	{
		const CommandPlane& Ref = *(itr++);
		if( !Ref.bExec )continue;
		if( !Ref.bView )continue;
		if( !Ref.Rect.Pin( x, y ) )continue;
		return Ref.Rect;
	}
	return RbRect();
}

bool ComBaseWindow::ClickJob( int x, int y )
{
	StrCommander Com;
	std::list<CommandPlane>::const_iterator itr = ComPlaneList.begin();
	while( itr != ComPlaneList.end() )
	{
		const CommandPlane& Ref = *(itr++);
		if( !Ref.bExec )continue;
		if( !Ref.bView )continue;
		if( Ref.Rect.Pin( x, y ) )
		{
			bool bRet = false;
			if(!Ref.Com.empty() )bRet |= Com.Command( Ref.Com );
			if(!Ref.Event.empty() )bRet |= !FAILED(GetEventSystem().LoadEvent( Ref.Event ));
			return bRet;
		}
	}
	return false;
}