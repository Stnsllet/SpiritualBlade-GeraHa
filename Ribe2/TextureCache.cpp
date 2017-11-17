// TextureCache.cpp: TextureCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbDisplay.h"
#include "TextureCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

TextureCache::TextureCache()
{
}

TextureCache::~TextureCache()
{
	TermThread();
}

void TextureCache::Clear()
{
	CriticalSection Locker(Lock);
	TextureList.clear();
}

HRESULT TextureCache::Init()
{
	CriticalSection Locker(Lock);
	ActionNameList[eActStop].SetString( _T("stop/image/move") );
	ActionNameList[eActWait].SetString( _T("wait/stop/move") );
	ActionNameList[eActAttack].SetString( _T("attack/move") );
	ActionNameList[eActMove].SetString( _T("move") );
	ActionNameList[eActDamege].SetString( _T("damege/image/move") );
	ActionNameList[eActSpecial].SetString( _T("waza/attack/move") );
	ActionNameList[eActRecover].SetString( _T("recover/move") );
	ActionNameList[eActDead].SetString( _T("dead/damege/image/move") );

	return S_OK;
}

BattleAction TextureCache::GetBattleID( const String& Name ) const
{
	for( int i = 0; i < eActMax; i++ )
	{
		if( (ActionNameList[i])[0] == Name )return (BattleAction)i;
	}
	return eActNone;
}

RbTexture* TextureCache::Get( const String& NameID )
{
	RefPtr<RbTexture> pTex = FindTex( NameID );
	if( pTex == NULL )LoadCache( String(NameID) );
	return FindTex( NameID );
}

RbTexture* TextureCache::FindTex( const String& NameID )
{
	CriticalSection Locker(Lock);
	std::map<String,RefPtr<RbTexture> >::const_iterator itr = TextureList.find(NameID);
	if( itr == TextureList.end() )return NULL;
	return itr->second;
}

bool TextureCache::LoadCache( const String& ReadData )
{
	if( ReadData.empty() )return false;
	RefPtr<RbTexture> pTex = FindTex( ReadData );
	if( pTex != NULL )return false;

	if( g_pSystem == NULL )return false;

	RefPtr<RbTexture> pNewTex( new RbTexture );
	HRESULT ret = pNewTex->LoadRbTexture( GetRbDisplay(), ReadData );
	if( FAILED(ret) )return false;

	for( int i = 0; i < eActMax; i++ )
	{
		const StringSplitter<>& Ref = ActionNameList[i];
		long Count = Ref.Count();
		for( int j = 0; j < Count; j++ )
		{
			if( pNewTex->SetBattleID( (BattleAction)i, Ref[j]) )break;
		}
	}

	ATLTRACE(_T("Cache Texture(%04x) %s\n"), GetCurrentThreadId(),(LPCTSTR)ReadData );

	{
		CriticalSection Locker(Lock);
		if(TextureList.find(ReadData)==TextureList.end())TextureList[ReadData] = pNewTex;
	}
	return true;
}
