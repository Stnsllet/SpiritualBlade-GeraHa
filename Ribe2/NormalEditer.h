
#ifndef __NORMAL_EDITER__
#define __NORMAL_EDITER__

#include <vector>
#include "RbPosition.h"

template<class T, long Tsize=sizeof(T)> class NormalEditer
{
public:
	NormalEditer( T* ptr, long Count, bool bGlowMode ) : pSrc(ptr), SrcCount(Count), bGlow( bGlowMode )
	{
		NormalList.resize( Count );
	}
	void SetNormal( int a, int b, int c, bool bReverse=false )
	{
		if( bGlow )SetGlow(a,b,c,bReverse);
		else SetFlat(a,b,c,bReverse);
	}
	RbPosition GetNormal( int a, int b, int c, bool bReverse=false )
	{
		if( a < 0 || a >= SrcCount )return RbPosition(0,0,0);
		if( b < 0 || b >= SrcCount )return RbPosition(0,0,0);
		if( c < 0 || c >= SrcCount )return RbPosition(0,0,0);
		D3DXPLANE Plane;
		D3DXPlaneFromPoints(&Plane, &pSrc[a], &pSrc[b], &pSrc[c] );
		RbPosition Ret( Plane.a, Plane.b, Plane.c );
		if( bReverse )Ret = -Ret;
		return Ret.Normalize();
	}
	void End()
	{
		DubleCheck();
		for( int i = 0; i < SrcCount; i++ )
		{
			RbPosition& Normal = NormalList[i];
			T& Ref = pSrc[i];
			Normal.Normalize();
			Ref.nx = Normal.x;
			Ref.ny = Normal.y;
			Ref.nz = Normal.z;
		}
	}
protected:
	void DubleCheck()
	{
		if( !bGlow )return;
		for( int i = 0; i < SrcCount; i++ )
		{
			const T& Ref1 = pSrc[i];
			for( int j = i+1; j < SrcCount; j++ )
			{
				const T& Ref2 = pSrc[j];
				if( Ref1.x != Ref2.x )continue;
				if( Ref1.y != Ref2.y )continue;
				if( Ref1.z != Ref2.z )continue;
				NormalList[i] += NormalList[j];
				NormalList[j] = NormalList[i];
			}
		}
	}
	void SetGlow( int a, int b, int c, bool bReverse=false )
	{
		RbPosition Add = GetNormal(a,b,c,bReverse);
		NormalList[a] += Add;
		NormalList[b] += Add;
		NormalList[c] += Add;
	}
	void SetFlat( int a, int b, int c, bool bReverse=false )
	{
		RbPosition Set = GetNormal(a,b,c,bReverse);
		NormalList[a] = Set;
		NormalList[b] = Set;
		NormalList[c] = Set;
	}
	T* pSrc;
	bool bGlow;
	long SrcCount;
	std::vector<RbPosition> NormalList;
};

#endif // __NORMAL_EDITER__
