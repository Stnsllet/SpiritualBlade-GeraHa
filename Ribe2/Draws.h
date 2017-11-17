// Draws.h: Draws クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWS_H__761B9F11_6246_4214_8311_945992E7B4BE__INCLUDED_)
#define AFX_DRAWS_H__761B9F11_6246_4214_8311_945992E7B4BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "VArray.h"
#include "BillbordPlane.h"
class Display;
class RbPosition;

struct Draws
{
	class Liner
	{
	public:
		Liner();
		bool InitLine( LPCTSTR Name );
		bool InitLine( const HandTexRect& Rect, MYTEXTURE* pTexture );
		HRESULT Draw( Display& Disp, const RbPosition& Pos1, const RbPosition& Pos2 ) const;
	private:
		BillbordPlane LinePlane;
	};
	class Bord
	{
	public:
		Bord( Display& Ref );
		HRESULT Draw( const BillbordPlane& BordPlane, const RbPosition& Pos ) const;
	private:
		Display& Disp;
	};
	class Poly
	{
	public:
		Poly( Display& SetDisp );
		template<class T> HRESULT Draw( D3DPRIMITIVETYPE Type, const VArray<T,IDirect3DVertexBuffer8>& Vertex )
		{
			return Draw( Type, Vertex, Vertex.Count() + GetCount(Type) );
		}
		template<class T> HRESULT Draw( D3DPRIMITIVETYPE Type, const VArray<T,IDirect3DVertexBuffer8>& Vertex, long Count )
		{
			Disp->SetVertexShader(T::Def());
			Disp->SetStreamSource( 0, Vertex, Vertex.StructSize() );
			return Disp->DrawPrimitive( Type, 0, Count );
		}
		template<class T> HRESULT Draw( const VArray<T,IDirect3DVertexBuffer8>& Vertex, const IndexBuffer& Index )
		{
			Disp->SetVertexShader(T::Def());
			Disp->SetStreamSource( 0, Vertex, Vertex.StructSize() );
			Disp->SetIndices( Index, 0 );
			return Disp->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
				0, Vertex.Count(), 0, Index.Count() );
		}
		long GetCount( D3DPRIMITIVETYPE Type ) const;
	private:
		Display& Disp;
	};

	template<class T> class Echo
	{
	public:
		struct citr : public std::list<T>::const_iterator
		{
			citr(const Echo<T>& RefClass):Ref(RefClass)
				,std::list<T>::const_iterator( RefClass.List.begin() )
				,no(0){}
			citr operator++(int)
			{
				citr tmp = *this;
				++((std::list<T>::const_iterator&)(*this));
				++no;
				return tmp;
			}
			operator const T&() const{return *(*this);}
			bool operator!() const{return Ref.List.end()==(*this);}
			bool ok() const{return Ref.List.end()!=(*this);}
			float val() const{return Ref.GetVal(no);}
			long no;
			const Echo<T>& Ref;
		};
		Echo( double Inter, long Max )
			:MaxCount(Max),Interval(Inter),LastTime(-Inter){}
		bool IsEcho() const{return (MaxCount>0);}
		bool Add( const T& AddData, double Time )
		{
			if( Time < LastTime+Interval )return false;
			LastTime = Time;
			List.push_back( AddData );
			if( List.size() > MaxCount )List.pop_front();
			return true;
		}
		void clear()
		{
			List.clear();
		}
		void SetCount( double Inter, long Max )
		{
			Interval = Inter;
			MaxCount = Max;
			while( List.size() > MaxCount )List.pop_front();
		}
		citr rbegin() const
		{
			return citr(*this);
		}
		float GetVal( long no ) const
		{
			return ((float)(no+1))/(List.size()+1);
		}
	protected:
		friend struct citr;
		double LastTime;
		double Interval;
		long MaxCount;
		std::list<T> List;
	};
};

#endif // !defined(AFX_DRAWS_H__761B9F11_6246_4214_8311_945992E7B4BE__INCLUDED_)
