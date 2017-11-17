// RbPoint.h: RbPoint クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBPOINT_H__A913BAC6_71BF_415F_94FA_9BBFF9B51DE1__INCLUDED_)
#define AFX_RBPOINT_H__A913BAC6_71BF_415F_94FA_9BBFF9B51DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class T=int> class RbPointT
{
public:
	RbPointT(){AllZero(*this);}
	~RbPointT(){}
	RbPointT( const RbPointT<T>& Src ){x=Src.x;y=Src.y;}
	RbPointT( const POINT& Src ){x=Src.x;y=Src.y;}
	RbPointT( T X, T Y ){x=X;y=Y;}
	RbPointT& operator+=( const RbPointT<T>& Add ){x+=Add.x;y+=Add.y;return *this;}
	RbPointT& operator-=( const RbPointT<T>& Add ){x-=Add.x;y-=Add.y;return *this;}
	RbPointT& operator*=( const float& Scale ){x*=Scale;y*=Scale;return *this;}
	RbPointT& operator/=( const float& Scale ){x/=Scale;y/=Scale;return *this;}
	RbPointT operator+( const RbPointT<T>& Add ) const{return RbPointT<T>(*this)+=Add;}
	RbPointT operator-( const RbPointT<T>& Add ) const{return RbPointT<T>(*this)-=Add;}
	RbPointT operator*( const float& Scale ) const{return RbPointT<T>(*this)*=Scale;}
	RbPointT operator/( const float& Scale ) const{return RbPointT<T>(*this)/=Scale;}
	template <class P> bool operator==( const P& Src ) const{return (x==Src.x && y==Src.y);}
	template <class P> RbPointT<T>& operator=( const P& Src ){x=Src.x;y=Src.y;return *this;}

	float Length() const{return sqrt(Length2());}
	float Length2() const{return (x*x)+(y*y);}
	float Length(const RbPointT<T>& Pos) const{return sqrt(Length2(Pos));}
	float Length2(const RbPointT<T>& Pos) const{return (RbPointT<T>(x,y)-=Pos).Length2();}

	RbPointT<T>& Normalize()
	{
		float Len = Length();
		if( Len > 0 ){x /= Len; y /= Len;}
		return *this;
	}
	RbPointT<T>& Normalize( float Length ){ return(Normalize()*=Length); }

	float GetDirection () const
	{
		RbPointT<float> Vector( x, y );
		Vector.Normalize();
		float Dir = ::asinf( Vector.x );
		Dir /= D3DX_PI;
		if( Vector.y > 0 )Dir = 1-Dir;
		if( Dir < 0 )Dir += 2;
		return Dir;
	}
	float GetDirection ( const RbPointT<T>& Pos ) const{return (Pos-*this).GetDirection();}
	RbPointT<T>& Rotate( float Angle )
	{
		if( Angle == 0 )return *this;
		Angle *= D3DX_PI;
		float Sin = ::sinf( Angle );
		float Cos = ::cosf( Angle );
		float sx = x;
		x = sx*Cos - y*Sin;
		y = sx*Sin + y*Cos;
		return *this;
	}
	T x, y;
};

typedef RbPointT<> RbPoint;
typedef RbPointT<float> RbPointF;

#endif // !defined(AFX_RBPOINT_H__A913BAC6_71BF_415F_94FA_9BBFF9B51DE1__INCLUDED_)
