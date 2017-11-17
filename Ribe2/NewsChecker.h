// NewsChecker.h: NewsChecker クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWSCHECKER_H__117CE236_39B5_4A4E_920C_9C293BAE1682__INCLUDED_)
#define AFX_NEWSCHECKER_H__117CE236_39B5_4A4E_920C_9C293BAE1682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StringSplitter.h"
#include <list>

class NewsChecker
{
public:
	NewsChecker();
	virtual ~NewsChecker();

	bool SetCheckList( const StringSplitter<>& List, long Index );

	bool CheckALL();
protected:
	bool CheckNext();

	std::list<String> CheckList;
};

#endif // !defined(AFX_NEWSCHECKER_H__117CE236_39B5_4A4E_920C_9C293BAE1682__INCLUDED_)
