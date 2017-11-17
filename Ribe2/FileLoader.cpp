// FileLoader.cpp: FileLoader クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileLoader.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

FileLoader::FileLoader()
{
	AllZero( State );
}

FileLoader::~FileLoader()
{

}

int FileLoader::GetCount() const
{
	return FindFiles.size();
}

bool FileLoader::Check( const WIN32_FIND_DATA& File ) const
{
	bool Directry = !!(File.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	bool Hide = !!(File.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);

	if( File.cFileName[0] == _T('.') )return false;	// is System

	if( Hide )return false;

	if( State.Directory && !Directry )return false;
	if( State.FileOnly && Directry )return false;

	return true;
}

HRESULT FileLoader::SearchDirectory( LPCTSTR pSerchRoot, LPCTSTR pMatch )
{
	AllZero( State );
	State.Directory = true;
	return Search( pSerchRoot, pMatch );
}

HRESULT FileLoader::SearchFile( LPCTSTR pSerchRoot, LPCTSTR pMatch )
{
	AllZero( State );
	State.FileOnly = true;
	return Search( pSerchRoot, pMatch );
}

HRESULT FileLoader::Search( LPCTSTR pSerchRoot, LPCTSTR pMatch )
{
	FindFiles.clear();

	Path Condition( pSerchRoot );
	if( pMatch == NULL )Condition += _T("*");
	else Condition += pMatch;

	WIN32_FIND_DATA FindFile;
	HANDLE hHand = FindFirstFile( Condition, &FindFile );
	if( hHand == INVALID_HANDLE_VALUE )return E_FAIL;
 
	do{
		if( Check(FindFile) )FindFiles.push_back( FindFile.cFileName );

	}while( FindNextFile( hHand, &FindFile ) );

	FindClose( hHand );
	return S_OK;
}
