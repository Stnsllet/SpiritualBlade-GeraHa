// RbPoli.h: RbPoli クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBPOLI_H__E5B9B481_407D_40D7_960B_002AA8BCA970__INCLUDED_)
#define AFX_RBPOLI_H__E5B9B481_407D_40D7_960B_002AA8BCA970__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "String.h"
#include "RbRect.h"
#include "VArray.h"
class XMLPaser;
class Display;
class File;

class RbPoly  
{
public:
struct PolyVer :public Vector, TexNormal, TexUV
{
	static DWORD Def(){return (Vector::Def() | TexNormal::Def() | TexUV::Def());}
};
typedef VArray<PolyVer,IDirect3DVertexBuffer8> PolyBuffer;

public:
	enum eDrawType
	{
		eDrawUnknown=0,
		eDrawFan,
		eDrawStrip,
		eDrawIndex,
		eDrawWire,
	};
	RbPoly();
	virtual ~RbPoly();
	HRESULT Draw( Display& Disp, const PolyBuffer& BaseVertex ) const;

	HRESULT LoadPoly( Display& Disp, LPCTSTR FileName );
	HRESULT LoadPoly( Display& Disp, const XMLPaser& PolyXML );
	HRESULT Save( File& fp ) const;
	HRESULT Load( Display& Disp, File& fp );
	void BoundRect( RbRectF& Inout, float& Range, float& MaxZ, float& MinZ ) const;

	static PolyVer ReadVertex( const XMLPaser& PolyXML );
protected:
	VerIndex ReadIndex( const XMLPaser& PolyXML ) const;
	bool SetNormal();

	eDrawType Type;
	bool bGlow;
	String TexPath;
	PolyBuffer Vertex;
	IndexBuffer Index;
	CComPtr<MYTEXTURE> pTexture;
};

#endif // !defined(AFX_RBPOLI_H__E5B9B481_407D_40D7_960B_002AA8BCA970__INCLUDED_)
