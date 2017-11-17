// RbRect.h: RbRect クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBRECT_H__82E28F3B_2DF1_426B_9677_EF1C9379F0E0__INCLUDED_)
#define AFX_RBRECT_H__82E28F3B_2DF1_426B_9677_EF1C9379F0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusStruct.h"
#include "RbPoint.h"

template<class T=int> struct RbRectT
{
public:
	typedef RbPointT<T> RbPointM;
	T left, top, right, bottom;

	RbRectT<T>(){AllZero(*this);}
	RbRectT<T>(const TexRect& src);
	RbRectT<T>( const RECT& src ){(*this) = src;}
	RbRectT<T>(T x, T y, T w, T h);
	RbRectT<T>(T w, T h);
	RbRectT<T>& operator = (const RECT& src);
	bool operator == (const RbRectT<T>& src) const{return 0==::memcmp(this, &src, sizeof(*this));}
	RbRectT<T>& operator *= (float Scale);
	RbRectT<T>& operator += (const RbPointM& Pos){return Move(Pos.x,Pos.y);}
	RbRectT<T> operator + (const RbPointM& Pos) const{return (RbRectT<T>(*this)+=Pos);}
	bool Pin( T X, T Y )const;
	bool PinSize( T X, T Y, T Size )const;
	bool PoutSize( T X, T Y, T Size )const;
	bool RectHit( const RbRectT<T>& RefRect ) const;
	bool RectIn( const RbRectT<T>& RefRect ) const;

	operator const RECT&() const;

	bool IsNone() const{return (Width()==0 && Height()==0);}

	T Width() const{return right-left;}
	T Height() const{return bottom-top;}
	RbPointM LT() const{return RbPointM(left,top);}
	RbPointM LB() const{return RbPointM(left,bottom);}
	RbPointM RT() const{return RbPointM(right,top);}
	RbPointM RB() const{return RbPointM(right,bottom);}
	RbPointM Center() const{return RbPointM((left+right)/2,(top+bottom)/2);}

	RbRectT<T> Shift( T x, T y ) const;
	RbRectT<T>& Move( T x, T y );
	RbRectT<T>& BoxMove( T x, T y );
	RbRectT<T>& ReSize( T Width, T Height );
	RbRectT<T>& DefaultSize( const TexRect& src );
	RbRectT<T>& Clip( const RbRectT<T>& Area );
	RbRectT<T>& OR( const RbRectT<T>& Rect );
	RbRectT<T>& AddPoint( T x, T y );

	RbRectT<T>& SetCenter( const RbPointM& Point ){return SetCenter(Point.x,Point.y);}
	RbRectT<T>& SetCenter( T x, T y );
	RbRectT<T>& SetCenterY( T y );
};

typedef RbRectT<> RbRect;
typedef RbRectT<float> RbRectF;

#endif // !defined(AFX_RBRECT_H__82E28F3B_2DF1_426B_9677_EF1C9379F0E0__INCLUDED_)
