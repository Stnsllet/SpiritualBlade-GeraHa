// LoadWindow.cpp: LoadWindow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbWindowSystem.h"
#include "RbSound.h"
#include "RbEventSystem.h"
#include "RbGameData.h"
#include "LoadWindow.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LoadWindow::LoadWindow()
{
}

LoadWindow::~LoadWindow()
{
	TermThread();
}

LPCTSTR LoadWindow::WindowName() const
{
	return _T("Load");
}

bool LoadWindow::CheckParameter( const CComVariant& Value ) const
{
	if( Value.vt == VT_BOOL )return CheckFiles();
	return true;
}

bool LoadWindow::CheckFiles() const
{
	WindowSystem& System = GetWindowSystem();
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return false;
	long FileCount = pPlanes->XML.AttributeLong(_T("count"));

	for( int i = 0; i < FileCount; i++ )
	{
		const String& Path = GetFilePath( i );
		if( (long)GetFileAttributes( Path ) != -1 )return true;
	}
	return false;
}

HRESULT LoadWindow::Draw( RbSprite& Display, WindowSystem& System )
{
	if( UpDateID != OldUpDateID )ScrollTexRelease();
	return ScrollWindow::Draw( Display, System );
}

HRESULT LoadWindow::Restore( DISPDEVICE* pDevice, WindowSystem& System )
{
	if( TexWindow != NULL )return S_FALSE;
	const WindowPlanes* pPlanes = System.Define().GetWindowPlanes(WindowName());
	if( pPlanes == NULL )return E_FAIL;

	Position.Width = pPlanes->Base.Plane.w;
	Position.Height = pPlanes->Base.Plane.h;

	Rects.DispCount = pPlanes->XML.AttributeLong(_T("count"));
	Rects.ListArea = pPlanes->GetRect(_T("list"));
	Rects.File = pPlanes->GetRect(_T("file"));
	Rects.Name = pPlanes->GetRect(_T("name"));

	ExSprite Surface;
	HRESULT hr = RestoreBase( pDevice, Surface, System );
	if( FAILED( hr ) )return hr;

	DrawWindowTag(System,WindowName());

	Files.resize( Rects.DispCount );
	for( int i = 0; i < Rects.DispCount; i++ )
	{
		CriticalSection Locker(Lock);
		LoadFiles& Ref = Files[i];
		Ref.FileNo = i;
		Ref.DisplayName.Format( _T("File %02d"), i );

		Que.Push((void*)&Ref);
	}

	ScrollList.resize(1);
	RestoreScroll( pDevice, System, 0 );
	return S_OK;
}

HRESULT LoadWindow::RestoreScroll( DISPDEVICE* pDevice, WindowSystem& System, long Index )
{
	if( Index != 0 )return ScrollWindow::RestoreScroll( pDevice, System, Index );
	if( Index >= ScrollList.size() )return S_FALSE;

	ScrollInfo& Ref = ScrollList[0];
	ScrollInfo::PosStock PosStock( Ref );

	float ScrollHeight = Rects.File.Height()*Rects.DispCount;
	ScrollHeight += Rects.File.top*2;
	Ref.SetInfo( Rects.ListArea, ScrollHeight );

	ExSprite Surface;
	HRESULT hr = Ref.SetSprite( pDevice, Surface );
	if( FAILED(hr) )return hr;

	OldUpDateID = UpDateID;
	RbRect List = Rects.File;
	for( int i = 0; i < Rects.DispCount; i++ )
	{

		RbRect Name = Rects.Name;
		Name.Move( List.left, List.top );

		CriticalSection Locker(Lock);
		LoadFiles& Ref = Files[i];
		System.Font().Draw( Ref.DisplayName, Name, DT_CENTER );
		List.BoxMove(0,1);
	}

	return S_OK;
}

bool LoadWindow::LoadCache(  void* const & ReadData )
{
	LoadFiles* pFile = (LoadFiles*)ReadData;
	const long FileNo = pFile->FileNo;

	RbGameData Data;
	if( FAILED(Data.LoadGlobalData( GetFilePath(FileNo) )) )return false;
	{
		CriticalSection Locker(Lock);
		pFile->DisplayName = Data.RefStageName();
		::InterlockedIncrement( &UpDateID );
	}
	return true;
}

RbRect LoadWindow::HitCursor( WindowSystem& System, int x, int y )
{
	if( !Rects.ListArea.Pin(x,y) )return RbRect();
	if( ScrollList.size()<=0 )return RbRect();
	const long Hight = Rects.File.Height();
	if( Hight<=0 )return RbRect();

	RbRect File = Rects.File;
	File.Move( Rects.ListArea.left, Rects.ListArea.top );
	long UpPos = (-ScrollList[0].NowPos) % Hight;
	File.Move( 0, UpPos );

	long Count = Files.size();
	for( int i = 0; i < Count; i++ )
	{
		if( File.Pin(x,y) )return File.Clip(Rects.ListArea);
		File.BoxMove(0,1);
	}
	return RbRect();
}

String LoadWindow::GetFilePath(long Index) const
{
	String FilePath;
	FilePath.Format( _T("Save/File%02d.Dat"), Index );
	return FilePath;
}

long LoadWindow::GetFileIndex( int x, int y ) const
{
	if( !Rects.ListArea.Pin(x,y) )return -1;
	if( ScrollList.size() <= 0 )return -1;

	RbRect File = Rects.File;
	File.Move( Rects.ListArea.left, Rects.ListArea.top );
	File.Move( 0, -ScrollList[0].NowPos );

	long Count = Files.size();
	for( int i = 0; i < Count; i++ )
	{
		if( File.Pin(x,y) )return i;
		File.BoxMove(0,1);
	}
	return -1;
}

bool LoadWindow::ClickJob( int x, int y )
{
	long FileIndex = GetFileIndex( x, y );
	if( FileIndex < 0 )return true;

	GetSoundSystem().Play(_T("click"));

	const String& FilePath = GetFilePath( FileIndex );
	if( FileJob( FilePath ) )
		Close( GetWindowSystem() );
	return true;
}

bool LoadWindow::FileJob( const String& FilePath )
{
	if( -1 == (long)::GetFileAttributes(FilePath) )return false;
	if( FAILED(GetGameData().LoadData( FilePath )) )return false;
	GetEventSystem().LoadEvent( _T("Event/LoadGame.xml") );

	return true;
}
