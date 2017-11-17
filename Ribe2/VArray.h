// VArray.h: VArray クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARRAY_H__8ACF9F69_8BFF_4A46_ADC8_46C6098547F0__INCLUDED_)
#define AFX_VARRAY_H__8ACF9F69_8BFF_4A46_ADC8_46C6098547F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VertexDef.h"
#include "RbBuffer.h"

template<class T,class T2> class VArrayLocker;

template<class T,class T2> class VArray  
{
public:
	typedef VArrayLocker<T,T2> Locker;
	VArray():BufCount(0),DataCount(0){}
	~VArray(){}

	D3DFORMAT Define() const{return (D3DFORMAT)T::Def();}
	UINT StructSize() const{return sizeof(T);}

	void clear(){ pBuf.Release();BufCount=0;DataCount=0; }
	bool Delete( const T& Data )
	{
		if( Count() <= 0 )return false;
		Locker Lock(*this);
		for( long i = 0; i < Count(); i++ )
		{
			if( !(Lock[i] == Data) )continue;
			long Next = i+1;
			if( Next < Count() )::memmove( &(Lock[i]), &(Lock[Next]), (Count()-Next)*sizeof(T) );
			--DataCount;
			return true;
		}
		return false;
	}
	bool Add( DISPDEVICE* pDevice, const T& Data )
	{
		static long Append = 16;
		if( BufCount <= 0 )Reserve( pDevice, Append );
		else if( DataCount >= BufCount )
		{
			VArray<T,T2> NewBuf;
			if( !NewBuf.Copy( *this, DataCount+Append) )return false;
			pBuf = NewBuf.pBuf;
			BufCount = NewBuf.BufCount;
		}
		Locker Lock(*this);
		Lock[DataCount++] = Data;
		return true;
	}

	bool Allocate( DISPDEVICE* pDevice, long num )
	{
		if( !Reserve( pDevice, num ) )return false;
		DataCount = BufCount;
		return true;
	}
	bool Reserve( DISPDEVICE* pDevice, long num )
	{
		if( pDevice == NULL )return false;
		CComPtr<T2> pNewBuf;
		if( !CreateBuff( pDevice, num, &pNewBuf ) )return false;
		clear();
		pBuf = pNewBuf;
		BufCount = num;
		DataCount = 0;
		return true;
	}
	void ZeroClear()
	{
		Locker Data( *this );
		T* pPtr = Data;
		ZeroMemory(pPtr,sizeof(T)*Count() );
	}
	int Count() const{return DataCount;}

	VArray<T,T2>& operator =( const VArray<T,T2>& Source ){Copy(Source);return *this;}
	operator T2*() const{return pBuf;}

	bool Lock( T** ppPtr )
	{
		if( ppPtr == NULL )return false;
		if( pBuf == NULL )return false;
		return ( !FAILED(pBuf->Lock( 0, 0, (BYTE**)ppPtr, 9 )) );
	}
	bool UnLock()
	{
		if( pBuf == NULL )return false;
		return ( !FAILED(pBuf->Unlock()) );
	}
	T Get( long Index ) const
	{
		Locker LockData( *this );
		return LockData[Index];
	}

	bool Copy( const VArray<T,T2>& Source, long ReserveSize=0 )
	{
		if( Source.Count() <= 0 )
		{
			clear();
			return true;
		}
		if( ReserveSize < Source.Count() )ReserveSize = Source.Count();

		CComPtr<DISPDEVICE> pDevice;
		Source.pBuf->GetDevice( &pDevice );
		if( !Reserve(pDevice,ReserveSize) )return false;
		DataCount = Source.Count();

		Locker Lock1( *this );
		Locker Lock2( Source );
		::memmove( (T*)Lock1, (T*)Lock2, sizeof(T)*Source.Count() );
		return true;
	}

	HRESULT Save( File& fp ) const
	{
		long Size = Count();
		Locker Data( *this );
		HRESULT hr = fp.WriteArray( (T*)Data, Size );
		if( FAILED(hr) )return hr;
		return S_OK;
	}
	HRESULT Load( DISPDEVICE* pDevice, File& fp )
	{
		long Size = 0;
		HRESULT hr = fp.Read( Size );
		if( FAILED(hr) )return hr;
		clear();
		if( Size <= 0 )return S_OK;

		Allocate( pDevice, Size );
		Locker Data( *this );
		hr = fp.ReadArray( (T*)Data, Size );
		if( FAILED(hr) )return hr;
		return S_OK;
	}
private:
	bool CreateBuff( DISPDEVICE* pDevice, long num, IDirect3DVertexBuffer8** ppBuf )
	{
		return !FAILED(pDevice->CreateVertexBuffer( num*sizeof(T), 0, Define(), D3DPOOL_MANAGED, ppBuf ));
	}
	bool CreateBuff( DISPDEVICE* pDevice, long num, IDirect3DIndexBuffer8** ppBuf )
	{
		return !FAILED(pDevice->CreateIndexBuffer( num*sizeof(T), 0, Define(), D3DPOOL_MANAGED, ppBuf ));
	}
	CComPtr<T2> pBuf;
	long BufCount;
	long DataCount;

};

template<class T,class T2> class VArrayLocker
{
public:
	VArrayLocker():pSrc(NULL),pPtr(NULL){}
	VArrayLocker( const VArray<T,T2>& Source ):pSrc(NULL),pPtr(NULL){Lock(Source);}
	~VArrayLocker(){Release();}
	void Release()
	{
		if( pSrc != NULL )pSrc->UnLock();
		pSrc = NULL;
		pPtr = NULL;
	}
	bool Lock( const VArray<T,T2>& Source )
	{
		Release();
		VArray<T,T2>* pNew = const_cast<VArray<T,T2>*>(&Source);
		if( !pNew->Lock( &pPtr ) )return false;
		pSrc = pNew;
		return true;
	}
	operator T*() const{return pPtr;}
	operator void*() const{return pPtr;}
private:
	VArray<T,T2>* pSrc;
	T* pPtr;
};

typedef VArray<VerIndex,IDirect3DIndexBuffer8> IndexBuffer;
typedef VArray<LandVertex,IDirect3DVertexBuffer8> LandBuffer;

#endif // !defined(AFX_VARRAY_H__8ACF9F69_8BFF_4A46_ADC8_46C6098547F0__INCLUDED_)
