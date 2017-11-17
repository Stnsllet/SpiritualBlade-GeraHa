// BGMMaker.cpp: BGMMaker クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MusicPortSelecter.h"
#include "BGMMaker.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

BGMMaker::BGMMaker()
{
	LastVolume = BaseVolume = 0;
}

BGMMaker::~BGMMaker()
{
	if( pPlayer != NULL )pPlayer->CloseDown();
}

long BGMMaker::SetBaseVolume( long NewVolume )
{
//	if( BaseVolume == NewVolume )return LastVolume+BaseVolume;
	BaseVolume = NewVolume;
	ChangeVolume( LastVolume );
	return LastVolume+BaseVolume;
}

// 初期化します ディレクトリを指定しない場合はカレント下の "Midi" フォルダを探します
HRESULT BGMMaker::Init( LPCTSTR pBGMDirectry )
{
	pPlayer.Release();
	pLoader.Release();
	pBGMPath.Release();

	DWORD PathType = DMUS_APATH_SHARED_STEREOPLUSREVERB;
	DWORD ChannelCount = 64;

	HRESULT ret = CoCreateInstance(CLSID_DirectMusicLoader,NULL,CLSCTX_INPROC,IID_IDirectMusicLoader8,(void **)&pLoader);
//	HRESULT ret = pLoader.CoCreateInstance(CLSID_DirectMusicLoader);
	if( FAILED(ret) )return ret;

	CComBSTR SerachDirectry;
	if( pBGMDirectry != NULL )
	{
		SerachDirectry = pBGMDirectry;
	}
	else
	{
		TCHAR dir[MAX_PATH];
		AllZero( dir );
		GetCurrentDirectory(MAX_PATH,dir);
		SerachDirectry = dir;
		SerachDirectry += L"\\Midi";
	}
//	pLoader->SetSearchDirectory( GUID_DirectMusicAllTypes, SerachDirectry, FALSE );

	DMUS_AUDIOPARAMS Params;
	AllZero(Params).dwSize = sizeof(Params);

	ret = CoCreateInstance(CLSID_DirectMusicPerformance,NULL,CLSCTX_INPROC,IID_IDirectMusicPerformance8,(void **)&pPlayer);
//	ret = pPlayer.CoCreateInstance(CLSID_DirectMusicPerformance);
	if( FAILED(ret) )return ret;

	ret = pPlayer->InitAudio(NULL ,NULL,NULL,
		PathType, ChannelCount, DMUS_AUDIOF_ALL, &Params);
	FAILEDTRACE(ret);
	if( FAILED(ret) )return ret;

	ret = pPlayer->CreateStandardAudioPath( PathType, ChannelCount, TRUE, &pBGMPath );
	if( FAILED(ret) )return ret;
/*
		CComPtr<IDirectMusicPort> pPort;
	pBGMPath->GetObjectInPath( 0, DMUS_PATH_PORT , 0, GUID_All_Objects, 0,
		IID_IDirectMusicPort, (void**)&pPort);

	DMUS_PORTCAPS Caps;
	AllZero(Caps).dwSize = sizeof(DMUS_PORTCAPS);
	pPort->GetCaps( &Caps );
*/
	return S_OK;
}

// ファイルの読み込み
HRESULT BGMMaker::LoadMusic( MUSICOBJECT** ppMusicObject, LPCTSTR pFileName )
{
	if( ppMusicObject == NULL )return E_POINTER;
	if( pLoader == NULL )return E_PENDING;
	if( pFileName == NULL )return E_INVALIDARG;

	CComBSTR FileName( pFileName );

	HRESULT ret = pLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,
		IID_IDirectMusicSegment8, FileName, (void **)ppMusicObject );
	FAILEDTRACE(ret);
	if( FAILED(ret) )return ret;
/*
	DMUS_OBJECTDESC Desc;
	AllZero(Desc).dwSize = sizeof(DMUS_OBJECTDESC);
    Desc.guidObject = GUID_DefaultGMCollection;  
    Desc.guidClass = CLSID_DirectMusicCollection;  
	Desc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_OBJECT;
 
	CComPtr<IDirectMusicCollection8> pCollection;
	ret = pLoader->GetObject(&Desc,
		IID_IDirectMusicCollection8,
		(void **) &pCollection);
*/
	ret = (*ppMusicObject)->SetParam(GUID_StandardMIDIFile, (DWORD)-1, 0, 0, NULL );
//	if( FAILED(ret) )return ret;

//	ret = (*ppMusicObject)->SetParam(GUID_ConnectToDLSCollection, (DWORD)-1, 0, 0, pCollection );
//	if( FAILED(ret) )return ret;

	ret = (*ppMusicObject)->Download( pPlayer );
	if( FAILED(ret) )return ret;

	return S_OK;
}

// BGMとして再生
HRESULT BGMMaker::PlayBGM( LPCTSTR pFileName, MUSIC_TIME LoopStart )
{
	if( pPlayer == NULL )return E_PENDING;
	if( pLoader == NULL )return E_PENDING;

	CComPtr<MUSICOBJECT> pMusic;

	HRESULT ret = LoadMusic( &pMusic, pFileName );
	if( FAILED(ret) )return ret;

	if( LoopStart > 0 )
	{
		MUSIC_TIME LoopEnd = 0;
		ret = pMusic->GetLength( &LoopEnd );
		if( LoopEnd > LoopStart )ret = pMusic->SetLoopPoints( LoopStart, LoopEnd );
	}
	ret = pMusic->SetRepeats( DMUS_SEG_REPEAT_INFINITE );

	StopAll();

	ret = pMusic->Download( pPlayer );
	if( FAILED(ret) )return ret;

	ret = pPlayer->PlaySegmentEx( pMusic, NULL, NULL, DMUS_SEGF_DEFAULT, 0,NULL, NULL, pBGMPath );
	if( FAILED(ret) )return ret;

	pLastMusic = pMusic;
	return S_OK;
}

// とまります
HRESULT BGMMaker::StopAll()
{
	if( pPlayer == NULL )return E_PENDING;
	HRESULT hr = pPlayer->Stop( NULL, NULL, 0, 0 );

	if( pLastMusic != NULL )
	{
		pLastMusic->Unload( pPlayer );
		pLastMusic = NULL;
	}
	return hr;
}

// 音量変更
HRESULT BGMMaker::ChangeVolume( long Volume, long DelayTime )
{
	if( pBGMPath == NULL )return E_PENDING;
	LastVolume = Volume;
	Volume += BaseVolume;
	if( Volume > 0 )Volume = 0;
	if( Volume < -9600 )Volume = -9600;
	return pBGMPath->SetVolume( Volume, DelayTime );
}

// キャッシュのみ
HRESULT BGMMaker::CacheFile( LPCTSTR FilePath )
{
	CComPtr<MUSICOBJECT> pObject;
	return LoadMusic( &pObject, FilePath );
}

// キャッシュ開放
HRESULT BGMMaker::ReleaseCache()
{
	if( pLoader == NULL )return E_PENDING;
	return pLoader->ClearCache( GUID_DirectMusicAllTypes );
}
