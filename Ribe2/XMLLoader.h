// XMLLoader.h: XMLLoader クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLLOADER_H__B91C7711_C021_48B9_BCEC_494202546E00__INCLUDED_)
#define AFX_XMLLOADER_H__B91C7711_C021_48B9_BCEC_494202546E00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "Path.h"

class XMLLoader
{
public:
	XMLLoader();
	~XMLLoader();

	HRESULT Search( LPCTSTR pPath );

	HRESULT Save() const;
	HRESULT Save( LPCTSTR FileName ) const;

private:
	HRESULT SearchXML( LPCTSTR pPath );
	struct XMLFile
	{
		String Tag;
		String name;
		Path FilePath;
	};
	Path Root;
	std::vector<XMLFile> FilePathList;
};

#endif // !defined(AFX_XMLLOADER_H__B91C7711_C021_48B9_BCEC_494202546E00__INCLUDED_)
