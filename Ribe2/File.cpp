// File.cpp: File クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

File::File()
{
	fp = INVALID_HANDLE_VALUE;
}

File::~File()
{
	Close();
}

FILETIME File::GetWriteTime() const
{
	FILETIME Ret;
	AllZero(Ret);
	if(fp == INVALID_HANDLE_VALUE)return Ret;
	DWORD Size = 0;
	GetFileTime( fp, NULL, NULL, &Ret );
	return Ret;
}

long File::GetSize() const
{
	if(fp == INVALID_HANDLE_VALUE)return 0;
	DWORD Size = 0;
	GetFileSize( fp, &Size );
	return Size;
}

void File::Close()
{
	if(fp!=INVALID_HANDLE_VALUE)::CloseHandle( fp );
	fp = INVALID_HANDLE_VALUE;
}

HRESULT File::Open( LPCTSTR FilaName, bool bWrite )
{
	Close();
	DWORD Access = GENERIC_READ;
	DWORD Share = FILE_SHARE_READ;
	DWORD Dispotion = OPEN_EXISTING;
	if( bWrite )
	{
		Access |= GENERIC_WRITE;
		Share = 0;
		Dispotion = CREATE_ALWAYS;
	}
	fp = ::CreateFile( FilaName, Access,
		Share, NULL, Dispotion, FILE_ATTRIBUTE_NORMAL, NULL );

	if(fp == INVALID_HANDLE_VALUE)return E_FAIL;

	return S_OK;
}

HRESULT File::WriteData( const void* pPointer , int Size )
{
	if(fp == INVALID_HANDLE_VALUE)return E_PENDING;

	DWORD Written = 0;
	WriteFile( fp, pPointer, Size, &Written,  NULL );
	if( (int)Written != Size )return E_FAIL;
	return S_OK;
}

HRESULT File::ReadData( void* pPointer , int Size )
{
	if(fp == INVALID_HANDLE_VALUE)return E_PENDING;

	DWORD Readed = 0;
	ReadFile( fp, pPointer, Size, &Readed,  NULL );
	if( (int)Readed != Size )return E_FAIL;
	return S_OK;
}
