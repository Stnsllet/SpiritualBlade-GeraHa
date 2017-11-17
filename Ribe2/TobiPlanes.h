// TobiPlanes.h: TobiPlanes クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIPLANES_H__41DAA304_DE1D_4298_B14A_ECA2FACAEAD3__INCLUDED_)
#define AFX_TOBIPLANES_H__41DAA304_DE1D_4298_B14A_ECA2FACAEAD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "Plane.h"
#include "RbPosition.h"
class RbSprite;

class TobiPlanes  
{
public:
	enum PlaneType
	{
		ePlaneDefault = 0,
		ePlaneBord,
	};
	TobiPlanes();
	~TobiPlanes();

	bool AddPlane( LPCTSTR PlaneName, const D3DVECTOR& BasePos, int Life=2000/20 );
	bool AddBord( const String& PlaneName, const D3DVECTOR& BasePos );
	void Job();
	void Draw( Display& Disp ) const;
	void Draw( Display& Disp, RbSprite& Surface ) const;
	void ClearData();

protected:
	struct TobiPDef
	{
		bool Job();
		bool Draw( Display& Disp, RbSprite& Surface ) const;
		bool operator==( const TobiPDef& Ref )const;
		PlaneType Type;
		Plane<> ViewPlane;
		RbPosition BasePos;
		int HightPos;
		int Life;
	};
	bool AddPlaneData( const TobiPDef& AddData );
	std::list<TobiPDef> TobiPlaneList;
};

#endif // !defined(AFX_TOBIPLANES_H__41DAA304_DE1D_4298_B14A_ECA2FACAEAD3__INCLUDED_)
