// MultiWave.h: MultiWave クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIWAVE_H__DB9982EA_DC48_4CD2_9359_356E10C980B7__INCLUDED_)
#define AFX_MULTIWAVE_H__DB9982EA_DC48_4CD2_9359_356E10C980B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <list>
#include "WaveCache.h"

class MultiWave  
{
public:
	MultiWave();
	virtual ~MultiWave();

	HRESULT ChangeVolume( long Volume );

public:
	HRESULT InitSound(HWND hwind);
	void LightCache();
	virtual bool ClearCache();

	IDirectSound* GetDirectSound() const{return pDSound;}
	//Waveファイルを鳴らす
	HRESULT SoundPlay(int WaveNo);
	HRESULT SoundPlay(int WaveNo, float x, float y, float z );
	//WaveファイルをLoad
	HRESULT SetData( const String& FileName,int WaveNo,int Volume=0, int Interval = 200);
	HRESULT SoundLoad( const String& FileName,int WaveNo,int Volume=0, int Interval = 200);

	// リスナー設定
	HRESULT SetRollOff( float RollOff = 1.0f );
	HRESULT SetListenerPosition( float x = 0.0f, float y = 0.0f, float z = 0.0f );
	HRESULT SetListenerPosition( const D3DVECTOR& Pos ){return SetListenerPosition(Pos.x,Pos.y,Pos.z);}

protected:
	void DeleteSound();
	HRESULT CreateListener( IDirectSound* pDSound );

	WaveCache BufCache;
	D3DVECTOR LisnerPos;
	CComPtr<IDirectSound> pDSound;
	CComPtr<IDirectSound3DListener> pListener;
	int AllVolum;

	class WaveFile
	{
	public:
		WaveFile();
		~WaveFile();
		void Clear();
		void ClearBuffer();
		bool SetData( WaveCache& RefCache, const String& FileName, int Volume, int Interval = 200);
		bool SoundLoad( WaveCache& RefCache, const String& FileName, int Volume, int Interval = 200);

		HRESULT Play( float x, float y, float z, int Volume = 0 );
		const String& RefPath() const{return Path;}
	private:
		HRESULT DuplicateBuffer( IDirectSound* pDSound, IDirectSoundBuffer** ppBuffer, IDirectSound3DBuffer** pp3DBuffer, int Volume =0 );
		int DefaultVol;
		DWORD MinInterval;
		DWORD LastTime;
		String Path;
		CComPtr<IDirectSoundBuffer> pMasterData;
		struct WavDat
		{
			CComPtr<IDirectSound3DBuffer> p3D;
			CComPtr<IDirectSoundBuffer> pSound;
		};
		std::list<WavDat> WaveList;
		WaveCache* pCache;
		HRESULT PlayWave( WavDat& PlayData, float x, float y, float z, int Volume ) const;
	};

	std::map<int,WaveFile> WaveFileList;
};

#endif // !defined(AFX_MULTIWAVE_H__DB9982EA_DC48_4CD2_9359_356E10C980B7__INCLUDED_)
