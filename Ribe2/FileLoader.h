// FileLoader.h: FileLoader クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILELOADER_H__74DE15BE_D6F7_40C4_9BC5_BB4FB500536D__INCLUDED_)
#define AFX_FILELOADER_H__74DE15BE_D6F7_40C4_9BC5_BB4FB500536D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "Path.h"

class FileLoader  
{
public:
	FileLoader();
	~FileLoader();

	HRESULT SearchDirectory( LPCTSTR pSerchRoot, LPCTSTR pMatch=NULL );
	HRESULT SearchFile( LPCTSTR pSerchRoot, LPCTSTR pMatch=NULL );

	int GetCount() const;
	String& operator[](int Index){return FindFiles[Index];}

private:
	HRESULT Search( LPCTSTR pSerchRoot, LPCTSTR pMatch );
	bool Check( const WIN32_FIND_DATA& File ) const;

	struct CheckState
	{
		bool Directory;
		bool FileOnly;
	}State;

	std::vector<Path> FindFiles;
};

#endif // !defined(AFX_FILELOADER_H__74DE15BE_D6F7_40C4_9BC5_BB4FB500536D__INCLUDED_)
