// NewsChecker.cpp: NewsChecker クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewsListWindow.h"
#include "NewsChecker.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

NewsChecker::NewsChecker()
{
}

NewsChecker::~NewsChecker()
{
}

bool NewsChecker::SetCheckList( const StringSplitter<>& List, long Index )
{
	CheckList.clear();
	long Count = List.Count();
	for( int i = Index; i < Count; i++ )
	{
		CheckList.push_back( List[i] );
	}
	return true;
}

bool NewsChecker::CheckALL()
{
	while( CheckList.size() > 0 )
	{
		if( !CheckNext() )continue;
		CheckList.clear();
		return true;
	}
	return false;
}

bool NewsChecker::CheckNext()
{
	std::list<String>::iterator itr = CheckList.begin();
	String File = *itr;
	CheckList.erase(itr);

	RefPtr<NewsListWindow> pWnd( new NewsListWindow );
	if( !pWnd->SetParameter( (LPCTSTR)File ) )return false;
	if( !pWnd->CheckParameter( true ) )return false;
	return true;
}
