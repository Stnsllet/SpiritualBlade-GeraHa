// WaveFileData.cpp: WaveFileData クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveFileData.h"

#pragma comment(lib, "winmm.lib")

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

WaveFileData::WaveFileData()
{
	Handle = NULL;
}

WaveFileData::~WaveFileData()
{
}

void WaveFileData::Close()
{
	if( Handle == NULL )return;
	::mmioClose( Handle, 0 );
	Handle = NULL;
}

bool WaveFileData::LoadFile( LPCTSTR FileName )
{
	Close();
	Handle = ::mmioOpen(const_cast<LPSTR>(FileName), NULL, MMIO_ALLOCBUF | MMIO_READ);

	if( Handle == NULL )return false;
	if( !ReadChunk( Chunk ) )return false;
	if( !ReadFormat( WaveFormat ) )return false;
	if( !ReadBuffer( pData ) )return false;

	return true;
}

bool WaveFileData::ReadChunk(MMCKINFO& Output) const
{
	AllZero( Output );
	return (::mmioDescend(Handle, &Output, NULL, 0) == MMSYSERR_NOERROR );
}

bool WaveFileData::ReadFormat(WAVEFORMATEX& Output) const
{
	MMCKINFO ckRiff = Chunk;
	AllZero(Output);
// フォーマットチャンクの読み込み
	ckRiff.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if(::mmioDescend(Handle, &ckRiff, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR )
		return false;

	long BufSize = sizeof(PCMWAVEFORMAT);
//	if( ckRiff.cksize != BufSize )return false;

	if((long)::mmioRead(Handle, (HPSTR)&Output, BufSize) != BufSize)
		return false;
	if(::mmioAscend(Handle, &ckRiff, 0) != MMSYSERR_NOERROR)return false;
	return true;
}

bool WaveFileData::ReadBuffer( DataBuf& Output ) const
{
	MMCKINFO ckRiff = Chunk;
	ckRiff.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(::mmioDescend(Handle, &ckRiff, NULL, MMIO_FINDCHUNK) != MMSYSERR_NOERROR )
		return false;

	long BufSize = ckRiff.cksize;
	Output.resize( BufSize );
	char* pBuf = (char*)Output.begin();
	if( ::mmioRead(Handle, pBuf, BufSize) != BufSize )return false;
	if(::mmioAscend(Handle, &ckRiff, 0) != MMSYSERR_NOERROR)return false;
	return true;
}
