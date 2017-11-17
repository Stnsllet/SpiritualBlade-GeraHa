// BillbordPlane.h: BillbordPlane クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BILLBORDPLANE_H__3AE7D022_C2E7_40E7_B8EE_FC338CC785D8__INCLUDED_)
#define AFX_BILLBORDPLANE_H__3AE7D022_C2E7_40E7_B8EE_FC338CC785D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VertexDef.h"
#include "Plane.h"
#include "RefObject.h"
#include "Texture.h"
#include "RbTransform.h"

class BillbordPlane
{
public:
	BillbordPlane(){AllZero( Ver );}
	BillbordPlane( const HandTexRect& Plane, const Texture& Tex )
		{SetData(Plane, Tex);}
	BillbordPlane( const TexRect& Plane, const Texture& Tex )
		{SetData(Plane, Tex);}
	BillbordPlane( const HandTexRect& Plane, MYTEXTURE* pTexture )
		{SetData(Plane, pTexture);}
	BillbordPlane( const TexRect& Plane, MYTEXTURE* pTexture )
		{SetData(Plane, pTexture);}
	~BillbordPlane(){}

	void SetData( const TexRect& Plane, MYTEXTURE* pTexture )
	{
		Texture Tex;
		Tex.SetTexture( pTexture );
		SetData( Plane, Tex );
	}
	void SetData( const HandTexRect& Plane, MYTEXTURE* pTexture )
	{
		Texture Tex;
		Tex.SetTexture( pTexture );
		SetData( Plane, Tex );
	}

	void SetData( const TexRect& Plane, const Texture& Tex )
	{
		HandTexRect SetPlane;
		(TexRect&)SetPlane = Plane;
		SetPlane.angle = 0;
		SetPlane.px = 0;
		SetPlane.py = Plane.h/2;
		SetData( SetPlane, Tex );
	}

	void SetData( const HandTexRect& Plane, const Texture& Tex )
	{
		int Width = Tex.ImageInfo().Width;
		int Height = Tex.ImageInfo().Height;
		bool bMirrorX = Plane.w<0;
		bool bMirrorY = Plane.h<0;
		int w = (bMirrorX)?(-Plane.w):(Plane.w);
		int h = (bMirrorY)?(-Plane.h):(Plane.h);
		pTexture = Tex;

		AllZero( Ver );
	//	Ver[0].z = Ver[1].z = Ver[2].z = Ver[3].z = 0;
		Ver[3].x = Ver[0].x = -w/ 2;
		Ver[3].tu = Ver[0].DotU( Plane.x, Width );
		Ver[2].x = Ver[1].x = -Ver[0].x;
		Ver[2].tu = Ver[1].DotU( Plane.x+w, Width );

		Ver[0].y = Ver[1].y = -h;
		Ver[0].tv = Ver[1].DotV( Plane.y, Height );
		Ver[2].y = Ver[3].y = 0;
		Ver[2].tv = Ver[3].DotV( Plane.y+h, Height );

		if( bMirrorX )MirrorX();
		if( bMirrorY )MirrorY();

	//	SetColor( D3DCOLOR_RGBA(255,255,255,255) );

		if( Plane.angle != 0 )Rotate( Plane.angle );
		if( Plane.px != 0 || Plane.py != 0 )Shift( Plane.px, Plane.py );
	}

	void MirrorX()
	{
		std::swap( Ver[1].tu, Ver[0].tu );
		std::swap( Ver[2].tu, Ver[3].tu );
	}
	void MirrorY()
	{
		std::swap( Ver[0].tv, Ver[2].tv );
		std::swap( Ver[1].tv, Ver[3].tv );
	}

	void Scaling( float Scale )
	{
		for( int i = 0; i < count_of(Ver); i++ )Ver[i] *= Scale;
	}

	void Shift( const D3DVECTOR& Pos )
	{
		for( int i = 0; i < count_of(Ver); i++ )Ver[i] += Pos;
	}

	void Shift( float px, float py )
	{
		for( int i = 0; i < count_of(Ver); i++ )
		{
			Ver[i].x += px;
			Ver[i].y += py;
		}
	}
	void Rotate( float angle )
	{
		if( angle < 0 )angle += 2.0f;
		if( angle > 2 )angle -= 2.0f;
		angle *= D3DX_PI;

		float Sin = ::sinf( angle );
		float Cos = ::cosf( angle );
		for( int i = 0; i < count_of(Ver); i++ )
		{
			float sx = Ver[i].x;
			Ver[i].x = sx*Cos - Ver[i].y*Sin;
			Ver[i].y = sx*Sin + Ver[i].y*Cos;
		}
	}
//	void SetColor( D3DCOLOR NewColor )
//	{
//		for( int i = 0; i < count_of(Ver); i++ )Ver[i].color = NewColor;
//	}

	void Trans( const RbTransform& Transform )
	{
		for( int i = 0; i < count_of(Ver); i++ )Ver[i].Pos() = Transform.Trans( Ver[i] );
	}

	HRESULT Draw( DISPDEVICE* pDevice ) const
	{
		if( pDevice == NULL )return E_FAIL;
	//	ATLASSERT( Ver[0].color != 0 );

		pDevice->SetVertexShader(Ver[0].Def());
		pDevice->SetTexture( 0, *this );
		return pDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Ver, sizeof(Ver[0]) );
	}

	operator TexVertex* () {return Ver;}
	operator MYTEXTURE* () const {return pTexture;}
	TexVertex Ver[4];
	CComPtr<MYTEXTURE> pTexture;
};

class RefBord : public RefAuto, public BillbordPlane
{
public:
	RefBord(const HandTexRect& Plane, MYTEXTURE* pTexture)
	: BillbordPlane( Plane, pTexture){NEW_CHECK(this);SetSource(Plane);}
	RefBord(const TexRect& Plane, MYTEXTURE* pTexture)
	: BillbordPlane( Plane, pTexture){NEW_CHECK(this);SetSource(Plane);}
	virtual ~RefBord(){}
	void SetSource( const HandTexRect& Plane ){SourcePlane = Plane;}
	void SetSource( const TexRect& Plane )
	{
		(TexRect&)SourcePlane = Plane;
		SourcePlane.angle = 0;
		SourcePlane.px = 0;
		SourcePlane.py = Plane.h/2;
	}

	HandTexRect SourcePlane;
};

#endif // !defined(AFX_BILLBORDPLANE_H__3AE7D022_C2E7_40E7_B8EE_FC338CC785D8__INCLUDED_)
