// PolyStation.h: PolyStation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYSTATION_H__25152FFC_22C1_4617_8AC5_26EEADFA25F4__INCLUDED_)
#define AFX_POLYSTATION_H__25152FFC_22C1_4617_8AC5_26EEADFA25F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RefObject.h"
#include "RbTransform.h"
#include "RbPolyPoly.h"
class XMLPaser;
class RbDisplay;
class LandScape;

class PolyStation : public RefAuto
{
public:
	PolyStation();
	virtual ~PolyStation();
	bool SetPolygon( const String& Name );
	bool SetXML( const XMLPaser& PolyXML, const LandScape& Land );
	bool SetStation( const String& PolyName, const LandScape& Land,
		const RbPosition& NewPos, float NewDirection, float NewScale=1, float NewAngle=0, bool bNewRevDraw=false );

	HRESULT Draw( Display& Disp );
	HRESULT DrawTrans( Display& Disp, const RbTransform& Trans );

	bool Pin( const RbPosition& CheckPos, float Size ) const;
	bool Pout( const RbPosition& CheckPos, float Size ) const;
	float Hight( float x, float y ) const;

	RbPolyPoly* RefPoly() const{return pSource;}
	const float& RefScale() const{return Scale;}
	const RbPosition& RefPos() const{return Pos;}
	const RbRectF& RefRect() const{return CheckArea;}
	const RbTransform& RefTrans() const{return Trans;}
	RbPosition UnBlockVector( const RbPosition& Pos ) const;
	RbPosition UnBlockPos( const RbPosition& Pos, float Size ) const;
protected:
	RefPtr<RbPolyPoly> pSource;
	RbPosition Pos;
	RbRectF CheckArea;
	float Top, CheckRange;
	float Scale;
	bool bRevDraw;
	struct _BoxAngle
	{
		float LT,LB,RT,RB;
	}BoxAngle;
	RbTransform Trans, InvTrans;
	PanelVector HightGurid;
};

#endif // !defined(AFX_POLYSTATION_H__25152FFC_22C1_4617_8AC5_26EEADFA25F4__INCLUDED_)
