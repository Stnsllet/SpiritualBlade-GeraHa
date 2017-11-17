// WaveCache.h: WaveCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVECACHE_H__0B18B221_7382_4C55_90D3_E59653445C22__INCLUDED_)
#define AFX_WAVECACHE_H__0B18B221_7382_4C55_90D3_E59653445C22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include "String.h"
#include "BackReader.h"

class WaveCache : protected BackReader<String>
{
public:
	WaveCache();
	virtual ~WaveCache();
	void ClearAll();
	void Clear();

	IDirectSoundBuffer* Get( const String& Path );

	bool SetDirectSound( IDirectSound* pNewSound );
	void Cache( const String& Add ){Que.Push(Add);}
	IDirectSound* RefDirectSound() const{return pDSound;}
protected:
	IDirectSoundBuffer* Find( const String& Path ) const;
	virtual bool LoadCache( const String& ReadData );
	HRESULT CreateBuffer( IDirectSoundBuffer** ppBuffer, const String& FileName );

	struct Wavedata
	{
		CComPtr<IDirectSoundBuffer> pSound;
	};
	std::map<String,Wavedata> SoundList;
	CComPtr<IDirectSound> pDSound;

};

#endif // !defined(AFX_WAVECACHE_H__0B18B221_7382_4C55_90D3_E59653445C22__INCLUDED_)
