// RbPolyPoly.cpp: RbPolyPoly クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "String.h"
#include "Path.h"
#include "File.h"
#include "XMLList.h"
#include "RbCache.h"
#include "RbDisplay.h"
#include "DispStocker.h"
#include "RbPolyPoly.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Version RbPolyPoly::MyVersion = Version( 1,0,0 );

RbPolyPoly::RbPolyPoly()
{
	MaxRange = MaxZ = MinZ = 0;
	bUseMaterial = 0;
	NEW_CHECK(this);
}

RbPolyPoly::~RbPolyPoly()
{
}

HRESULT RbPolyPoly::LoadByName( Display& Disp, LPCTSTR Name )
{
	if( Name == NULL )return E_FAIL;
	String XMLPath(_T("Poly/"));
	XMLPath += Name;
	String LoadPath = XMLPath;
	LoadPath += _T(".poly");
	File fp;
	HRESULT hr = fp.Open( LoadPath );
	if( FAILED(hr) )
	{
		XMLPath  += _T(".xml");
		return LoadPoly( Disp, XMLPath );
	}
	return Load(Disp,fp);
}

HRESULT RbPolyPoly::Save( File& fp ) const
{
	HRESULT hr = fp.Write(MyVersion);
	if( FAILED(hr) )return hr;

	hr = fp.WriteStr(TexPath);
	if( FAILED(hr) )return hr;

	hr = fp.Write(BoundRect);
	if( FAILED(hr) )return hr;
	hr = fp.Write(MaxRange);
	if( FAILED(hr) )return hr;
	hr = fp.Write(MaxZ);
	if( FAILED(hr) )return hr;
	hr = fp.Write(MinZ);
	if( FAILED(hr) )return hr;

	hr = fp.Write(bUseMaterial);
	if( FAILED(hr) )return hr;
	hr = fp.Write(MyMaterial);
	if( FAILED(hr) )return hr;

	hr = Vertex.Save( fp );
	if( FAILED(hr) )return hr;

	long Count = PolyList.size();
	hr = fp.Write(Count);
	if( FAILED(hr) )return hr;
	for( int i = 0; i < Count; i++ )
	{
		hr = PolyList[i].Save(fp);
		if( FAILED(hr) )return hr;	
	}
	return S_OK;
}

HRESULT RbPolyPoly::Load( Display& Disp, File& fp )
{
	Version ReadVersion(1,0,0);
	HRESULT hr = fp.Read(ReadVersion);
	if( FAILED(hr) )return hr;
	if( ReadVersion != MyVersion )return E_INVALIDARG;

	hr = fp.ReadStr(TexPath);
	if( FAILED(hr) )return hr;

	hr = fp.Read(BoundRect);
	if( FAILED(hr) )return hr;
	hr = fp.Read(MaxRange);
	if( FAILED(hr) )return hr;
	hr = fp.Read(MaxZ);
	if( FAILED(hr) )return hr;
	hr = fp.Read(MinZ);
	if( FAILED(hr) )return hr;

	hr = fp.Read(bUseMaterial);
	if( FAILED(hr) )return hr;
	hr = fp.Read(MyMaterial);
	if( FAILED(hr) )return hr;

	hr = Vertex.Load( Disp, fp );
	if( FAILED(hr) )return hr;

	long Count = 0;
	hr = fp.Read(Count);
	if( FAILED(hr) )return hr;
	PolyList.resize(Count);
	for( int i = 0; i < Count; i++ )
	{
		hr = PolyList[i].Load(Disp,fp);
		if( FAILED(hr) )return hr;	
	}
	if( !TexPath.empty() )
	{
		pTexture = GetCache().PathTex.Get(TexPath);
		if( pTexture == NULL )return E_FAIL;
	}
	return S_OK;
}

HRESULT RbPolyPoly::DrawPos( Display& Disp, const RbPosition& Pos, float Angle, float Scale )
{
	RbTransform Trans; 
	Trans.Scale( Scale );
	Trans.RotateZ( Angle );
	Trans.AddScroll( Pos );
	return DrawTrans( Disp, Trans );
}

HRESULT RbPolyPoly::DrawTrans( Display& Disp, const RbTransform& Trans )
{

	DispStocker::Transform Stock( Disp, D3DTS_WORLD );
//	RbTransform AllTrans = Trans;
//	AllTrans *= Stock.OldTrans;
	Stock.Set( Trans );

	return Draw( Disp );
}

HRESULT RbPolyPoly::Draw( Display& Disp )
{
	DispStocker::Render Stock( Disp, D3DRS_SHADEMODE );
	Disp->SetTexture( 0, pTexture );
	if( bUseMaterial )
	{
		DispStocker::Material Mate( Disp );
		Mate.Set( MyMaterial );
		for( int i = 0; i < PolyList.size(); i++ )
			PolyList[i].Draw( Disp, Vertex );
	}
	else
	{
		for( int i = 0; i < PolyList.size(); i++ )
			PolyList[i].Draw( Disp, Vertex );
	}
	return S_OK;
}

HRESULT RbPolyPoly::LoadPoly( Display& Disp, LPCTSTR FileName )
{
	XMLPaser PolyXML;
	HRESULT hr = PolyXML.Load( FileName );
	if( FAILED(hr) )return hr;
	hr = LoadPoly( Disp, PolyXML );
	if( FAILED(hr) )return hr;
	Path CangePath = FileName;
	CangePath.ChangeExt(_T("poly"));
	{
		File fp;
		fp.Open( CangePath, true );
		hr = Save(fp);
	}
	if( FAILED(hr) )::DeleteFile(CangePath);
	return S_OK;
}

HRESULT RbPolyPoly::LoadPoly( Display& Disp, const XMLPaser& PolyXML )
{
	TexPath = PolyXML.AttributeString(_T("tex"));
	pTexture = GetCache().PathTex.Get( TexPath );

	bUseMaterial = MyMaterial.SetXML( PolyXML );

	XMLList VerListXML;
	VerListXML.Query( PolyXML, _T("V") );
	long Count = VerListXML.GetCount();
	Vertex.Allocate(Disp,Count);
	PolyBuffer::Locker VData(Vertex);
	for( int i = 0; i < Count; i++ )
	{
		VData[i] = RbPoly::ReadVertex(VerListXML.GetItem(i));
	}

	XMLList ListXML;
	ListXML.Query( PolyXML, _T("Poly") );
	Count = ListXML.GetCount();
	if( Count <= 0 )return E_FAIL;
	PolyList.resize( Count );

	for( i = 0; i < Count; i++ )
	{
		PolyList[i].LoadPoly( Disp, ListXML.GetItem(i) );
		PolyList[i].BoundRect( BoundRect, MaxRange, MaxZ, MinZ );
	}
	return S_OK;
}
