// WaveCache.cpp: WaveCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveFileData.h"
#include "WaveCache.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

WaveCache::WaveCache()
{
}

WaveCache::~WaveCache()
{
	TermThread();
}

void WaveCache::ClearAll()
{
	TermThread();
	SoundList.clear();
	pDSound.Release();
}

void WaveCache::Clear()
{
	CriticalSection Locker(Lock);
	SoundList.clear();
}

bool WaveCache::SetDirectSound( IDirectSound* pNewSound )
{
	if( pNewSound == NULL )return false;
	pDSound = pNewSound;
	return true;
}

bool WaveCache::LoadCache( const String& ReadData )
{
	CComPtr<IDirectSoundBuffer> pFind = Find( ReadData );
	if( pFind != NULL )return false;
	Wavedata NewData;
	HRESULT hr = CreateBuffer( &NewData.pSound, ReadData );
	FAILEDTRACE( hr );
//	ATLASSERT( !FAILED(hr) );
	if( FAILED(hr) )return false;
	{
		CriticalSection Locker(Lock);
		if( SoundList.find( ReadData )!=SoundList.end() )return false;
		Wavedata& Ref = SoundList[ReadData];
		Ref = NewData;
	}
	return true;
}

IDirectSoundBuffer* WaveCache::Get( const String& Path )
{
	CComPtr<IDirectSoundBuffer> pFind = Find( Path );
	if( pFind != NULL )return pFind;

	LoadCache( Path );
	pFind = Find( Path );
	return pFind;
}

IDirectSoundBuffer* WaveCache::Find( const String& Path ) const
{
	if( Path.empty() )return NULL;
	CriticalSection Locker(Lock);
	std::map<String,Wavedata>::const_iterator itr = SoundList.find( Path );
	if( itr == SoundList.end() )return NULL;
	return itr->second.pSound;

}

// DirectSoundBufferを作る
HRESULT WaveCache::CreateBuffer( IDirectSoundBuffer** ppBuffer, const String& FileName )
{
	if( pDSound == NULL )return E_POINTER;
	if( ppBuffer == NULL )return E_POINTER;

	WaveFileData FileData;
	if( !FileData.LoadFile( FileName ) )return E_INVALIDARG;

	DSBUFFERDESC Desc;
	ZeroMemory(&Desc,sizeof(DSBUFFERDESC));
	WAVEFORMATEX FormatWaveEx = FileData.RefFormat();
	Desc.dwSize = sizeof(DSBUFFERDESC);
	Desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
//	Desc.dwFlags = DSBCAPS_CTRLDEFAULT |DSBCAPS_STATIC;
	Desc.dwBufferBytes = FileData.GetDataSize();

	Desc.lpwfxFormat=&FormatWaveEx;

	CComPtr<IDirectSoundBuffer> pBuffer;
	HRESULT ret = pDSound->CreateSoundBuffer(&Desc, &pBuffer, NULL);
	if( pBuffer == NULL )return ret;

	LPVOID pData1 = NULL;
	DWORD dwBytes1 = 0;
	pBuffer->Lock(0,Desc.dwBufferBytes,&pData1,&dwBytes1,NULL,NULL,0);
	CopyMemory(pData1,FileData.RefSourceData(),dwBytes1);
	pBuffer->Unlock(pData1,dwBytes1,NULL,0);

	*ppBuffer = pBuffer.Detach();

	return S_OK;
}
