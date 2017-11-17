// RbPolyPoly.h: RbPolyPoly クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBPOLYPOLY_H__2008D87E_97F1_4B4B_95CC_54A05C48721C__INCLUDED_)
#define AFX_RBPOLYPOLY_H__2008D87E_97F1_4B4B_95CC_54A05C48721C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "Version.h"
#include "String.h"
#include "RefObject.h"
#include "RbPoly.h"
#include "Material.h"
class RbTransform;
class RbPosition;
class Display;
class File;

class RbPolyPoly : public RefAuto
{
public:
	typedef RbPoly::PolyBuffer PolyBuffer;
	RbPolyPoly();
	virtual ~RbPolyPoly();

	HRESULT LoadByName( Display& Disp, LPCTSTR Name );

	HRESULT Draw( Display& Disp );
	HRESULT DrawPos( Display& Disp, const RbPosition& Pos, float Angle=0, float Scale=1 );
	HRESULT DrawTrans( Display& Disp, const RbTransform& Trans );
	const RbRectF& Bound() const{return BoundRect;}
	const float& Range() const{return MaxRange;}
	const float& Top() const{return MaxZ;}
	const float& Bottom() const{return MinZ;}
protected:
	HRESULT LoadPoly( Display& Disp, LPCTSTR FileName );
	HRESULT LoadPoly( Display& Disp, const XMLPaser& PolyXML );
	HRESULT Save( File& fp ) const;
	HRESULT Load( Display& Disp, File& fp );

	static Version MyVersion;

	String TexPath;
	RbRectF BoundRect;
	float MaxZ, MinZ, MaxRange;
	std::vector<RbPoly> PolyList;
	CComPtr<MYTEXTURE> pTexture;

	bool bUseMaterial;
	Material MyMaterial;

	PolyBuffer Vertex;
};

#endif // !defined(AFX_RBPOLYPOLY_H__2008D87E_97F1_4B4B_95CC_54A05C48721C__INCLUDED_)
