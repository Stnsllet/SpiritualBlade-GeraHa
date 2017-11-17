// TextureCache.h: TextureCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURECACHE_H__4641101C_D7C8_4298_BB32_2D2AE7DC405F__INCLUDED_)
#define AFX_TEXTURECACHE_H__4641101C_D7C8_4298_BB32_2D2AE7DC405F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "StatusStruct.h"
#include "StringSplitter.h"
#include "RbTexture.h"
#include "BackReader.h"

class TextureCache : protected BackReader<String>
{
public:
	TextureCache();
	virtual ~TextureCache();
	HRESULT Init();
	void Clear();

	RbTexture* Get( const String& NameID );
	BattleAction GetBattleID( const String& Name ) const;
	void Cache( const String& Add ){Que.Push(Add);}

private:
	RbTexture* FindTex( const String& NameID );
	virtual bool LoadCache( const String& ReadData );
	std::map<String,RefPtr<RbTexture> > TextureList;
	StringSplitter<> ActionNameList[eActMax];
};

#endif // !defined(AFX_TEXTURECACHE_H__4641101C_D7C8_4298_BB32_2D2AE7DC405F__INCLUDED_)
