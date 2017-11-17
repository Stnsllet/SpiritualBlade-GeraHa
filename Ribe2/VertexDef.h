
#ifndef __VERTEX_DEF__
#define __VERTEX_DEF__

struct Vector : public D3DXVECTOR3
{
	D3DXVECTOR3& Pos(){ return (*this);}
	static DWORD Def(){return (D3DFVF_XYZ);}
};

struct TexNormal
{
	float nx,ny,nz;
	TexNormal& Normal(){ return (*this);}
	static DWORD Def(){return (D3DFVF_NORMAL);}
};

struct TexUV
{
	float tu,tv;
	float DotU( float x, float TextureWidth ){return (tu = x/TextureWidth);}
	float DotV( float y, float TextureHeight ){return (tv = y/TextureHeight);}
	static DWORD Def(){return D3DFVF_TEX1;}
};

struct ARGB
{
	ARGB( BYTE a, BYTE r, BYTE g, BYTE b):color(D3DCOLOR_ARGB(a,r,g,b)){}
	ARGB():color(D3DCOLOR_RGBA(255,255,255,255)){}
	ARGB( D3DCOLOR newcol ):color(newcol){}
	ARGB( float A, float R, float G, float B )
	{
		a() = (BYTE)(A*255.0f);
		r() = (BYTE)(R*255.0f);
		g() = (BYTE)(G*255.0f);
		b() = (BYTE)(B*255.0f);
	}
	ARGB( const D3DCOLORVALUE& NewColor )
	{
		a() = (BYTE)(NewColor.a*255.0f);
		r() = (BYTE)(NewColor.r*255.0f);
		g() = (BYTE)(NewColor.g*255.0f);
		b() = (BYTE)(NewColor.b*255.0f);
	}
	D3DCOLOR color;
	BYTE& a(){ return ((BYTE*)&color)[3]; }
	BYTE& r(){ return ((BYTE*)&color)[2]; }
	BYTE& g(){ return ((BYTE*)&color)[1]; }
	BYTE& b(){ return ((BYTE*)&color)[0]; }

	BYTE& Add( BYTE& Taget, BYTE AddCol ) const
	{
		int AddVal = (int)Taget + AddCol;
		return (Taget=(BYTE)Trim( AddVal, 255, 0 ) );
	}
	operator DWORD() const{return color;}
};

struct ARGBF : public D3DCOLORVALUE
{
	ARGBF(){a=r=g=b=1;}
	ARGBF( D3DCOLOR Color )
	{
		ARGB Col( Color );
		a=Col.a()/255.0f;
		r=Col.r()/255.0f;
		g=Col.g()/255.0f;
		b=Col.b()/255.0f;
	}
	ARGBF( float A, float R, float G, float B){a=A;r=R;g=G;b=B;}
	ARGBF( const D3DCOLORVALUE& Ref ){(D3DCOLORVALUE&)(*this)=Ref;}
	ARGBF& operator=( const D3DCOLORVALUE& Ref )
	{
		((D3DCOLORVALUE&)*this)=Ref;
		return *this;
	}
	ARGBF& operator*=( const D3DCOLORVALUE& Ref )
	{
		a *= Ref.a;r *= Ref.r;g *= Ref.g;b *= Ref.b;
		return *this;
	}
};

struct TexColor : public ARGB
{
	D3DCOLOR& Alpha( int Alpha=255 ){ color = D3DCOLOR_RGBA(255,255,255,Alpha); }
	static DWORD Def(){return D3DFVF_DIFFUSE;}
};
/*
struct TexVertex : public Vector, TexColor, TexUV
{
	static DWORD Def(){return (Vector::Def() | TexColor::Def() | TexUV::Def());}
};
*/
struct TexVertex : public Vector, TexUV
{
	static DWORD Def(){return (Vector::Def() | TexUV::Def());}
};

struct LandVertex : public Vector, TexNormal, TexUV
{
	static DWORD Def(){return (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);}
	operator = ( const D3DVECTOR& Src )
		{x=Src.x;y=Src.y;z=Src.z;}
	operator += ( const D3DVECTOR& Src )
		{x+=Src.x;y+=Src.y;z+=Src.z;}
};

struct VerIndex
{
	WORD a,b,c;
	bool operator==( const VerIndex& Ref ) const{return (a==Ref.a&&b==Ref.b&&c==Ref.c);}
	static DWORD Def(){return (D3DFMT_INDEX16);}
};

struct PanelVector : public D3DXPLANE
{
	operator = ( const D3DXPLANE& Src ) const {(*(D3DXPLANE*)this) = Src;}
	static DWORD Def(){return (D3DFVF_NORMAL);}
};

#endif // __VERTEX_DEF__
