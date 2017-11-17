
#ifndef __RB_BUFFER__
#define __RB_BUFFER__

#include <vector>
#include "File.h"

template<class T> class RbBuffer
{
public:
	RbBuffer(){}

	UINT StructSize() const{return sizeof(T);}

	operator T*() const{return const_cast<T*>(pBuff.begin());}
	void Allocate( int num ) {pBuff.resize( num );}
	void clear(){ pBuff.clear(); }
	int Count() const{return pBuff.size();}

	HRESULT Save( File& fp ) const
	{
		long Size = Count();
		HRESULT hr = fp.WriteArray( (T*)*this, Size );
		if( FAILED(hr) )return hr;
		return S_OK;
	}
	HRESULT Load( File& fp )
	{
		long Size = 0;
		HRESULT hr = fp.Read( Size );
		if( FAILED(hr) )return hr;
		Allocate( Size );
		hr = fp.ReadArray( (T*)*this, Size );
		if( FAILED(hr) )return hr;
		return S_OK;
	}
	bool Add( const T& New )
	{
		pBuff.push_back( New );
		return true;
	}
private:
	std::vector<T> pBuff;
};

#endif // __RB_BUFFER__