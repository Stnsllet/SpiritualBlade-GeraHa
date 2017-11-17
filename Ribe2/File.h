// File.h: File クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILE_H__390FD113_7970_4B60_AA9B_F94EE1EE549F__INCLUDED_)
#define AFX_FILE_H__390FD113_7970_4B60_AA9B_F94EE1EE549F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class File  
{
public:
	File();
	virtual ~File();

	virtual HRESULT Open( LPCTSTR FilaName, bool bWrite=false );
	void Close();
	long GetSize() const;
	FILETIME GetWriteTime() const;

	template<class T> HRESULT Write( const T& Data ){return WriteData(&Data, sizeof(Data));}
	template<class T> HRESULT Read( T& Data ){return ReadData(&Data, sizeof(Data));}
	template<class T> HRESULT WriteStr( const T& Str )
	{
		long Length = Str.length();
		if( Length <= 0 )return Write((long)0);
		return WriteArray((LPCTSTR)Str,Length*sizeof(TCHAR));
	}
	template<class T> HRESULT ReadStr( T& Str )
	{
		long Length = 0;
		HRESULT hr = Read(Length);
		if( FAILED(hr) )return hr;
		if( Length <= 0 )return S_OK;
		long BufSize = (Length+1)*sizeof(TCHAR);
		LPTSTR pBuf = (LPTSTR)::_alloca(BufSize);
		::memset( pBuf, 0, BufSize );
		hr = ReadArray( pBuf, Length*sizeof(TCHAR) );
		Str = pBuf;
		return hr;
	}

	template<class T> HRESULT WriteArray( const T* pData, long Size )
	{
		HRESULT ret = Write( Size );
		if( FAILED(ret) )return ret;
		for( long i = 0; i < Size; i++ )
		{
			ret = Write( pData[i] );
			if( FAILED(ret) )return ret;
		}
		return S_OK;
	}
	template<class T> HRESULT ReadArray( T* pData, long ReadCount )
	{
		for( long i = 0; i < ReadCount; i++ )
		{
			HRESULT ret = Read( pData[i] );
			if( FAILED(ret) )return ret;
		}
		return S_OK;
	}

	virtual HRESULT WriteData( const void* pPointer , int Size );
	virtual HRESULT ReadData( void* pPointer , int Size );
private:
	HANDLE fp;

};

#endif // !defined(AFX_FILE_H__390FD113_7970_4B60_AA9B_F94EE1EE549F__INCLUDED_)
