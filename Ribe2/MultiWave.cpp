// MultiWave.cpp: MultiWave クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Timer.h"
#include "WaveFileData.h"
#include "MultiWave.h"

//DirectX補足ヘッダー 定数
#ifndef DSBCAPS_CTRLDEFAULT
#define DSBCAPS_CTRLDEFAULT         (DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME)
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

MultiWave::MultiWave()
{
	AllVolum = 0;
	LisnerPos.x = LisnerPos.y = LisnerPos.z = 0;
}

MultiWave::~MultiWave()
{
	DeleteSound();
}

// キャッシュクリア
bool MultiWave::ClearCache()
{
	std::map<int,WaveFile>::iterator itr = WaveFileList.begin();
	while( itr != WaveFileList.end() )
		(itr++)->second.Clear();
	BufCache.Clear();
	return true;
}

// 無駄バッファ排除
void MultiWave::LightCache()
{
	std::map<int,WaveFile>::iterator itr = WaveFileList.begin();
	while( itr != WaveFileList.end() )
	{
		WaveFile& Ref = (itr++)->second;
		Ref.ClearBuffer();
	}
}

// 音量変更
HRESULT MultiWave::ChangeVolume( long Volume )
{
	AllVolum = Volume;
	return S_OK;
}

// Waveファイルからデータ読み込みます
HRESULT MultiWave::SoundLoad( const String& FileName,int WaveNo,int Volume, int Interval )
{
	ATLASSERT( WaveFileList.find(WaveNo) == WaveFileList.end() );
	HRESULT ret = WaveFileList[WaveNo].SoundLoad( BufCache, FileName, Volume, Interval );
	if( FAILED(ret) )return ret;
	return S_OK;
}

// Waveファイル情報を設定
HRESULT MultiWave::SetData( const String& FileName,int WaveNo,int Volume, int Interval )
{
	ATLASSERT( WaveFileList.find(WaveNo) == WaveFileList.end() );
	HRESULT ret = WaveFileList[WaveNo].SetData( BufCache, FileName, Volume, Interval );
	if( FAILED(ret) )return ret;
	return S_OK;
}

// ロールオフ係数の設定 数値が小さいほど遠くの音が聞こえます( 0は無限 )
HRESULT MultiWave::SetRollOff( float RollOff )
{
	if( pListener == NULL )return E_PENDING;
	return pListener->SetRolloffFactor( RollOff, DS3D_IMMEDIATE );
}

// 聞き手の位置を設定
HRESULT MultiWave::SetListenerPosition( float x, float y, float z )
{
	if( pListener == NULL )return E_PENDING;
	LisnerPos.x = x;
	LisnerPos.y = y;
	LisnerPos.z = z;
	return pListener->SetPosition( x, y, z, DS3D_IMMEDIATE );
}

HRESULT MultiWave::InitSound(HWND hwind)
{
	pDSound.Release();

	HRESULT ret = DirectSoundCreate(NULL,&pDSound,NULL);
	if (pDSound==NULL)return ret;

	ret = pDSound->SetCooperativeLevel(hwind,DSSCL_NORMAL);
	if ( FAILED(ret) )return ret;

	ret = CreateListener( pDSound );
	if ( FAILED(ret) )return ret;

	BufCache.SetDirectSound( pDSound );

	return S_OK;
}

HRESULT MultiWave::CreateListener(  IDirectSound* pDSound  )
{
	if( pDSound == NULL )return E_POINTER;

	// プライマリ サウンドバッファ作成
	DSBUFFERDESC desc;
	ZeroMemory(&desc,sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;

	CComPtr<IDirectSoundBuffer> pSoundBuffer;
	HRESULT ret = pDSound->CreateSoundBuffer( &desc, &pSoundBuffer, NULL);
	if( FAILED(ret) )return ret;

	pListener.Release();
	ret = pSoundBuffer->QueryInterface( IID_IDirectSound3DListener, (void **)&pListener );
	if (FAILED(ret) )return ret;

	// リスナーのパラメータの設定
	pListener->SetPosition(LisnerPos.x,LisnerPos.y,LisnerPos.z,DS3D_IMMEDIATE);
	pListener->SetVelocity(0.0f,0.0f,0.0f,DS3D_IMMEDIATE);
	pListener->SetOrientation(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,DS3D_IMMEDIATE);

	return S_OK;
}

void MultiWave::DeleteSound()
{
	WaveFileList.clear();
	pListener.Release();
	pDSound.Release();
	BufCache.ClearAll();
}

HRESULT MultiWave::SoundPlay(int WaveNo)
{
	return SoundPlay( WaveNo, LisnerPos.x, LisnerPos.y, LisnerPos.z );
}

HRESULT MultiWave::SoundPlay(int WaveNo, float x, float y, float z )
{
	return WaveFileList[WaveNo].Play( x, y, z, AllVolum );
}

bool MultiWave::WaveFile::SoundLoad( WaveCache& RefCache, const String& FileName, int Volume, int Interval)
{
	if( !SetData( RefCache, FileName, Volume, Interval ) )return false;
	pMasterData = RefCache.Get(Path);
	return true;
}

bool MultiWave::WaveFile::SetData( WaveCache& RefCache, const String& FileName, int Volume, int Interval)
{
	//Waveファイルからデータ読み込みます
	pCache = &RefCache;
	Path = FileName;

	ClearBuffer();

	DefaultVol = Volume;
	MinInterval = Interval;

	Timer NowTime;
	LastTime = (DWORD)NowTime - MinInterval;

	pMasterData.Release();
	return true;
}

MultiWave::WaveFile::WaveFile()
{
	MinInterval = 200;
	DefaultVol = 0;
	LastTime = Timer();
	pCache = NULL;
}

MultiWave::WaveFile::~WaveFile()
{
	pMasterData.Release();
}

void MultiWave::WaveFile::Clear()
{
	WaveList.clear();
	pMasterData.Release();
}

void MultiWave::WaveFile::ClearBuffer()
{
	WaveList.clear();
}

// 音を出す
HRESULT MultiWave::WaveFile::Play( float x, float y, float z, int Volume )
{
	if( pMasterData == NULL )
	{
		if( pCache == NULL )return E_PENDING;
		pMasterData = pCache->Get(Path);
		if( pMasterData == NULL )return E_PENDING;
	}

	Volume += DefaultVol;
	if( Volume > DSBVOLUME_MAX )Volume = DSBVOLUME_MAX;
	if( Volume < DSBVOLUME_MIN )Volume = DSBVOLUME_MIN;

	Timer NowTime;
	if( NowTime.Passed( LastTime ) < MinInterval )return S_FALSE;
	LastTime = NowTime;

	std::list<WavDat>::iterator itr = WaveList.begin();
	while( itr != WaveList.end() )
	{
		DWORD dwWord = 0;
		itr->pSound->GetStatus(&dwWord);
		if(dwWord&DSBSTATUS_BUFFERLOST)
		{
			itr = WaveList.erase( itr );
			continue;
		}
		if(!(dwWord&DSBSTATUS_PLAYING))
			return PlayWave( *itr, x, y, z, Volume );

		itr++;
	}
	WavDat NewDat;
	HRESULT ret = DuplicateBuffer( pCache->RefDirectSound(),
		&(NewDat.pSound), &(NewDat.p3D), Volume );
	if( FAILED(ret) )return ret;
	WaveList.push_back( NewDat );
	return PlayWave( NewDat, x, y, z, Volume );
}

// 実際に演奏
HRESULT MultiWave::WaveFile::PlayWave( WavDat& PlayData, float x, float y, float z, int Volume ) const
{
	if( PlayData.pSound == NULL )return E_PENDING;
	PlayData.pSound->SetVolume(Volume);
	PlayData.p3D->SetPosition( x, y, z, DS3D_IMMEDIATE );
	return PlayData.pSound->Play(0,0,0);
}

// 同じバッファを作る
HRESULT MultiWave::WaveFile::DuplicateBuffer( IDirectSound* pDSound, IDirectSoundBuffer** ppBuffer, IDirectSound3DBuffer** pp3DBuffer, int Volume )
{
	if( pDSound == NULL )return E_POINTER;
	if( ppBuffer == NULL )return E_POINTER;
	if( pMasterData == NULL )return E_PENDING;

	CComPtr<IDirectSoundBuffer> pBuffer;
	HRESULT ret = pDSound->DuplicateSoundBuffer( pMasterData, &pBuffer );
	if( pBuffer == NULL )return ret;

	ret = pBuffer->QueryInterface(IID_IDirectSound3DBuffer,(void **)pp3DBuffer);
	if( *pp3DBuffer == NULL )return ret;
	pBuffer->SetVolume(Volume);
	*ppBuffer = pBuffer.Detach();
	return S_OK;
}
