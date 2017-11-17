// RbPoli.cpp: RbPoli クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "String.h"
#include "NormalEditer.h"
#include "XMLList.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "Draws.h"
#include "RbPoly.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RbPoly::RbPoly()
{
	Type = eDrawUnknown;
	bGlow = false;
}

RbPoly::~RbPoly()
{
}

HRESULT RbPoly::Save( File& fp ) const
{
	HRESULT hr = fp.Write(Type);
	if( FAILED(hr) )return hr;
	hr = fp.Write(bGlow);
	if( FAILED(hr) )return hr;

	hr = fp.WriteStr(TexPath);
	if( FAILED(hr) )return hr;

	hr = Vertex.Save(fp);
	if( FAILED(hr) )return hr;
	hr = Index.Save(fp);
	if( FAILED(hr) )return hr;
	return S_OK;
}

HRESULT RbPoly::Load( Display& Disp, File& fp )
{
	HRESULT hr = fp.Read(Type);
	if( FAILED(hr) )return hr;
	hr = fp.Read(bGlow);
	if( FAILED(hr) )return hr;

	hr = fp.ReadStr(TexPath);
	if( FAILED(hr) )return hr;
	if( !TexPath.empty() )
	{
		pTexture = GetCache().PathTex.Get( TexPath );
		if( pTexture == NULL )return E_FAIL;
	}

	hr = Vertex.Load(Disp,fp);
	if( FAILED(hr) )return hr;
	hr = Index.Load(Disp,fp);
	if( FAILED(hr) )return hr;
	return S_OK;
}

HRESULT RbPoly::LoadPoly( Display& Disp, LPCTSTR FileName )
{
	XMLPaser PolyXML;
	HRESULT hr = PolyXML.Load( FileName );
	if( FAILED(hr) )return hr;
	return LoadPoly( Disp, PolyXML );
}

HRESULT RbPoly::LoadPoly( Display& Disp, const XMLPaser& PolyXML )
{
	String DrawType = PolyXML.AttributeString(_T("type"));
	if( DrawType==_T("strip") )Type = eDrawStrip;
	else if( DrawType==_T("fan") )Type = eDrawFan;
	else if( DrawType==_T("index") )Type = eDrawIndex;
	else if( DrawType==_T("wire") )Type = eDrawWire;

	bGlow = PolyXML.AttributeBool(_T("glow"));

	TexPath = PolyXML.AttributeString(_T("tex"));
	pTexture = GetCache().PathTex.Get( TexPath );

	XMLList ListXML;
	ListXML.Query( PolyXML, _T("V") );
	long Count = ListXML.GetCount();
	Vertex.Allocate(Disp,Count);
	PolyBuffer::Locker VData(Vertex);
	for( int i = 0; i < Count; i++ )
	{
		VData[i] = ReadVertex(ListXML.GetItem(i));
	}
	ListXML.Query( PolyXML, _T("I") );
	Count = ListXML.GetCount();
	Index.Allocate(Disp,Count);
	IndexBuffer::Locker IData(Index);
	for( i = 0; i < Count; i++ )
	{
		IData[i] = ReadIndex(ListXML.GetItem(i));
	}
	if( Type == eDrawUnknown )
	{
		if( Count > 0 )Type = eDrawIndex;
	}

	SetNormal();

	if( Index.Count()>0 )Type = eDrawIndex;
	return (Vertex.Count()>0)?(S_OK):(E_FAIL);
}

RbPoly::PolyVer RbPoly::ReadVertex( const XMLPaser& PolyXML )
{
	PolyVer Ret;
	Ret.nx = Ret.ny = Ret.nz = 0;

	Ret.x = PolyXML.AttributeFloat(_T("x"));
	Ret.y = PolyXML.AttributeFloat(_T("y"));
	Ret.z = PolyXML.AttributeFloat(_T("z"));

	Ret.tu = PolyXML.AttributeFloat(_T("u"));
	Ret.tv = PolyXML.AttributeFloat(_T("v"));

	return Ret;
}

VerIndex RbPoly::ReadIndex( const XMLPaser& PolyXML ) const
{
	short Max = Vertex.Count()-1;
	VerIndex Ret;
	Ret.a= min(PolyXML.AttributeLong(_T("a")),Max);
	Ret.b = min(PolyXML.AttributeLong(_T("b")),Max);
	Ret.c = min(PolyXML.AttributeLong(_T("c")),Max);

	return Ret;
}

HRESULT RbPoly::Draw( Display& Disp, const PolyBuffer& BaseVertex ) const
{
	if( pTexture != NULL )Disp->SetTexture( 0, pTexture );

	// シェーディングに設定
	if( bGlow )Disp->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	else Disp->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

	Draws::Poly Draw( Disp );
	switch( Type )
	{
	default:
		return E_FAIL;
	case eDrawStrip:
		return Draw.Draw( D3DPT_TRIANGLESTRIP, Vertex );
	case eDrawFan:
		return Draw.Draw( D3DPT_TRIANGLEFAN, Vertex );
	case eDrawWire:
		return Draw.Draw( D3DPT_LINESTRIP, Vertex );
	case eDrawIndex:
		if( Vertex.Count() <= 0 )Draw.Draw( BaseVertex, Index );
		return Draw.Draw( Vertex, Index );
	}
	return S_OK;
}

void RbPoly::BoundRect( RbRectF& Inout, float& Range, float& MaxZ, float& MinZ ) const
{
	PolyBuffer::Locker VData(Vertex);
	for( int i = 0; i < Vertex.Count(); i++ )
	{
		const PolyVer& Ref = VData[i];
		Inout.AddPoint( Ref.x, Ref.y );
		Range = max(Range, RbPosition(Ref.x,Ref.y,Ref.z).Length() );
		MaxZ = max(MaxZ, Ref.z);
		MinZ = min(MinZ, Ref.z);
	}
}

bool RbPoly::SetNormal()
{
	int Count = Vertex.Count();
	int IndexCount = Index.Count();
	PolyBuffer::Locker VData(Vertex);
	IndexBuffer::Locker IData(Index);
	NormalEditer<PolyVer> Editer( VData, Count, bGlow );
	int i=0;
	switch( Type )
	{
	default:
		return false;
	case eDrawStrip:
		for( i = 2; i < Count; i++ )
			Editer.SetNormal( i-2, i-1, i, (i&1) );
		break;
	case eDrawFan:
		for( i = 2; i < Count; i++ )
			Editer.SetNormal( 0, i-1, i );
		break;
	case eDrawIndex:
		for( i = 0; i < IndexCount; i++ )
		{
			const VerIndex& Ref = IData[i];
			Editer.SetNormal( Ref.a, Ref.b, Ref.c );
		}
		break;
	}
	Editer.End();

	return true;
}
