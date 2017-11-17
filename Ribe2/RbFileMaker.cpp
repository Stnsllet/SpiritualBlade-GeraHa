// RbFileMaker.cpp: RbFileMaker クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbFileMaker.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbFileMaker::RbFileMaker()
{

}

RbFileMaker::~RbFileMaker()
{

}

#define BITMASK(x)	((1<<x)-1)

const long RbFileMaker::FlagFile::Filter = 0xdcdcdcdc;
const BYTE RbFileMaker::FlagFile::MaskList[] = 
{
	BITMASK(1), BITMASK(3), BITMASK(4), BITMASK(5), BITMASK(6), BITMASK(7), BITMASK(8)
};

RbFileMaker::FlagFile::FlagFile()
{
	Compress = 0;
	AllZero( DataCount );
}

RbFileMaker::FlagFile::~FlagFile()
{
}

HRESULT RbFileMaker::FlagFile::ReadFile( LPCTSTR pFileName, BYTE** ppOutput )
{
	if( ppOutput == NULL )return E_POINTER;
	*ppOutput = NULL;

	FILE* fp = fopen( pFileName, "rb" );
	fseek( fp, 0, SEEK_END );
	long FileSize = ftell( fp );
	rewind( fp );

	if( FileSize <= sizeof(FlagData) )
	{
		fclose(fp);
		return E_FAIL;
	}

	long BufSize = (FileSize+3)&~3;
	AutoPtr<BYTE> pPtr( new BYTE[BufSize] );
	if( FileSize > (long)fread( (BYTE*)pPtr, FileSize, 1, fp ) )
	{
		fclose(fp);
		return E_FAIL;
	}
	fclose(fp);

	for( int i = 0; i < BufSize/4; i++ )
	{
		((long*)(BYTE*)pPtr)[i] ^= Filter;
	}

	HRESULT ret = ReadData( (FlagData*)(BYTE*)pPtr, ppOutput );

	return ret;
}

HRESULT RbFileMaker::FlagFile::ReadData( const FlagData* pData, BYTE** ppOutput )
{
	long BufSize = (pData->SourceSize+3)&~3;

	AutoPtr<BYTE> pPtr( new BYTE[BufSize] );

	long ReadCount = 0;
	long FlagPos = 0;
	while( ReadCount < pData->SourceSize )
	{
		BYTE ZeroBitCount = ReadFlag( pData->pData, FlagPos, MaskList[3] );
		FlagPos += 3;
		BYTE DataBit = 8-ZeroBitCount;
		pPtr[ReadCount] = ReadFlag( pData->pData, FlagPos, MaskList[DataBit] );
		FlagPos += DataBit;
		ReadCount++;
	}

	if( pData->NestCompress )return ReadData( (FlagData*)(BYTE*)pPtr, ppOutput );

	*ppOutput = pPtr.Detach();
	return S_OK;
}

BYTE RbFileMaker::FlagFile::ReadFlag( const BYTE* pData, long FlagPos, BYTE ReadMask )
{
	return ( (*((WORD*)&pData[FlagPos>>3]))>>(FlagPos&3) ) & ReadMask;
}