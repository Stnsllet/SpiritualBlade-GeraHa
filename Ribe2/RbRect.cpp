// RbRect.cpp: RbRect クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbRect.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


template<class T> RbRectT<T>::RbRectT(T x, T y, T w, T h)
{
	left = x;
	top = y;
	right = left + w;
	bottom = top + h;
}

template<class T> RbRectT<T>::RbRectT(T w, T h)
{
	left = 0;
	top = 0;
	right = w;
	bottom = h;
}

template<class T> RbRectT<T>::RbRectT(const TexRect& src)
{
	bool MirrorX = (src.w<0);
	bool MirrorY = (src.h<0);
	int w = (src.w<0)?(-src.w):(src.w);
	int h = (src.h<0)?(-src.h):(src.h);
	left = src.x;
	top = src.y;
	right = left + w;
	bottom = top + h;

//	if( MirrorX )std::swap( left, right );
//	if( MirrorY )std::swap( top, bottom );
}

template<class T> bool RbRectT<T>::Pin( T X, T Y )const
{
	if( X < left )return false;
	if( Y < top )return false;
	if( X >= right )return false;
	if( Y >= bottom )return false;
	return true;
}

template<class T> bool RbRectT<T>::PinSize( T X, T Y, T Size )const
{
	if( X-Size < left )return false;
	if( Y-Size < top )return false;
	if( X+Size >= right )return false;
	if( Y+Size >= bottom )return false;
	return true;
}

template<class T> bool RbRectT<T>::PoutSize( T X, T Y, T Size )const
{
//	if( X+Size < left )return true;
//	if( Y+Size < top )return true;
//	if( X-Size >= right )return true;
//	if( Y-Size >= bottom )return true;
//	return false;
	if( X+Size < left )return false;
	if( Y+Size < top )return false;
	if( X-Size >= right )return false;
	if( Y-Size >= bottom )return false;
	return true;
}

template<class T> bool RbRectT<T>::RectIn( const RbRectT<T>& RefRect ) const
{
	if( !Pin( RefRect.left, RefRect.top ) )return false;
	if( !Pin( RefRect.right, RefRect.bottom ) )return false;

	return true;
}

template<class T> bool RbRectT<T>::RectHit( const RbRectT<T>& RefRect ) const
{
	if( RefRect.right < left )return false;
	if( RefRect.left >= right )return false;
	if( RefRect.bottom < top )return false;
	if( RefRect.top >= bottom )return false;

	return true;
}

template<class T> RbRectT<T>& RbRectT<T>::OR( const RbRectT<T>& Rect )
{
	if( Rect.left < left  )left = Rect.left;
	if( Rect.top < top  )top = Rect.top;
	if( Rect.bottom > bottom  )bottom = Rect.bottom;
	if( Rect.right > right  )right = Rect.right;
	return *this;
}

template<class T> RbRectT<T>& RbRectT<T>::Clip( const RbRectT<T>& Rect )
{
	if( left < Rect.left )left = Rect.left;
	if( top < Rect.top  )top = Rect.top;
	if( bottom > Rect.bottom )bottom = Rect.bottom;
	if( right > Rect.right )right = Rect.right;
	return *this;
}

template<class T> RbRectT<T>& RbRectT<T>::BoxMove( T x, T y )
{
	x = x*Width();
	y = y*Height();
	return Move( x, y );
}

template<class T> RbRectT<T>& RbRectT<T>::Move( T x, T y )
{
	left += x;
	top += y;
	right += x;
	bottom += y;
	return *this;
}

template<class T> RbRectT<T> RbRectT<T>::Shift( T x, T y ) const
{
	return RbRectT<T>(*this).Move( x, y );
}

template<class T> RbRectT<T>& RbRectT<T>::ReSize( T Width, T Height )
{
	right = left + Width;
	bottom = top + Height;
	return *this;
}

template<class T> RbRectT<T>& RbRectT<T>::DefaultSize( const TexRect& src )
{
	if( !IsNone() )return *this;
	return ReSize( src.w, src.h );
}

template<class T> RbRectT<T>& RbRectT<T>::AddPoint( T x, T y )
{
	if( x < left ) left = x;
	if( x > right ) right = x;
	if( y < top ) top = y;
	if( y > bottom ) bottom = y;
	return *this;
}

template<class T> RbRectT<T>& RbRectT<T>::SetCenter( T x, T y )
{
	RbPointM Pos = Center();
	x -= Pos.x;
	y -= Pos.y;
	return Move( x, y );
}

template<class T> RbRectT<T>& RbRectT<T>::SetCenterY( T y )
{
	y -= (bottom+top)/2;
	return Move( 0, y );
}

template<class T> RbRectT<T>& RbRectT<T>::operator *= (float Scale)
{
	left *= Scale;
	top *= Scale;
	right *= Scale;
	bottom *= Scale;
	return *this;
}

template<class T> RbRectT<T>& RbRectT<T>::operator = (const RECT& src)
{
	left = src.left;
	top  = src.top;
	right = src.right;
	bottom = src.bottom;
	return *this;
}

template<class T> RbRectT<T>::operator const RECT&() const
{
	return *((RECT*)this);
}

template struct RbRectT<>;
template struct RbRectT<float>;
