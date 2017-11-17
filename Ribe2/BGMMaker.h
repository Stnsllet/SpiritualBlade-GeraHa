// BGMMaker.h: BGMMaker クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BGMMAKER_H__83676A59_AA20_49D0_B0B8_4838CB85F16F__INCLUDED_)
#define AFX_BGMMAKER_H__83676A59_AA20_49D0_B0B8_4838CB85F16F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MUSICOBJECT IDirectMusicSegment8

class BGMMaker  
{
public:
	BGMMaker();
	virtual ~BGMMaker();

	HRESULT Init( LPCTSTR pBGMDirectry = NULL );
	HRESULT PlayBGM( LPCTSTR pFileName, MUSIC_TIME LoopStart = 0 );
	HRESULT ChangeVolume( long Volume, long DelayTime=0 );
	HRESULT StopAll();

	HRESULT CacheFile( LPCTSTR Name );
	HRESULT ReleaseCache();

	long SetBaseVolume( long NewVolume );
protected:
	HRESULT LoadMusic( MUSICOBJECT** ppMusicObject, LPCTSTR pFileName );

	CComPtr<IDirectMusicPerformance8> pPlayer;
	CComPtr<IDirectMusicLoader8> pLoader;
	CComPtr<IDirectMusicAudioPath8> pBGMPath;
	long BaseVolume;
	long LastVolume;
	CComPtr<MUSICOBJECT> pLastMusic;
};

#endif // !defined(AFX_BGMMAKER_H__83676A59_AA20_49D0_B0B8_4838CB85F16F__INCLUDED_)
