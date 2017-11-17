// PolyMaster.h: PolyMaster クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYMASTER_H__7DDDA1AF_1D53_410E_85F0_2E673868D388__INCLUDED_)
#define AFX_POLYMASTER_H__7DDDA1AF_1D53_410E_85F0_2E673868D388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "PolyStation.h"
class XMLPaser;
class Display;

class PolyMaster  
{
public:
	PolyMaster();
	virtual ~PolyMaster();
	void Clear();

	HRESULT DrawPoly( Display& Disp );
	PolyStation* AddXML( const XMLPaser& PolyXML, const LandScape& Land );
	PolyStation* AddStation( const String& PolyName, const LandScape& Land,
		const RbPosition& Pos, float Direction, float Scale=1, float Angle=0, bool bRevDraw=false );

	float CheckHight( float x, float y, float z ) const;
	long Delete( const RbPosition& Pos );
	long Delete( const PolyStation* pStation );
	bool Pin( const RbPosition& Pos, float Size ) const;
	bool Pout( const RbPosition& Pos, float Size ) const;
	const PolyStation* BlockCheck( const RbPosition& Pos, float Size ) const;
protected:
	typedef std::vector<RefPtr<PolyStation> > PolyData;
	PolyData PolyList;
};

#endif // !defined(AFX_POLYMASTER_H__7DDDA1AF_1D53_410E_85F0_2E673868D388__INCLUDED_)
