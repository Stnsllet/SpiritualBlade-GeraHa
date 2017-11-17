// WaveFileData.h: WaveFileData クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEFILEDATA_H__11965DB7_06FF_437F_9770_A3042C6834D6__INCLUDED_)
#define AFX_WAVEFILEDATA_H__11965DB7_06FF_437F_9770_A3042C6834D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <mmsystem.h>

class WaveFileData  
{
public:
	WaveFileData();
	virtual ~WaveFileData();

	virtual bool LoadFile( LPCTSTR FileName );
	virtual void Close();
	const WAVEFORMATEX& RefFormat() const{return WaveFormat;}
	const BYTE* RefSourceData() const{return pData.begin();}
	long GetDataSize() const{return pData.size();}
protected:
	bool ReadChunk(MMCKINFO& Output) const;
	bool ReadFormat(WAVEFORMATEX& Output) const;
	typedef std::vector<BYTE> DataBuf;
	bool ReadBuffer( DataBuf& Output ) const;
	HMMIO Handle;

	MMCKINFO Chunk;
	WAVEFORMATEX WaveFormat;
	DataBuf pData;
};

#endif // !defined(AFX_WAVEFILEDATA_H__11965DB7_06FF_437F_9770_A3042C6834D6__INCLUDED_)
