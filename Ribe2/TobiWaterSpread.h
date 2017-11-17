// TobiWaterSpread.h: TobiWaterSpread クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIWATERSPREAD_H__01EF52F9_45A2_4FBD_A286_95E5B235A81B__INCLUDED_)
#define AFX_TOBIWATERSPREAD_H__01EF52F9_45A2_4FBD_A286_95E5B235A81B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSpreadBase.h"

class TobiWaterSpread : public TobiSpreadBase
{
public:
	TobiWaterSpread();
	virtual ~TobiWaterSpread();
	virtual bool Job( BattleSystem& System );

	virtual bool SetResource();
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );

	void SetInfo( const RbPosition& AddKnockBack, float NewSpreadRange, float NewSpreadKnockBack=2 );
protected:
	virtual RbPosition GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const;

	RbPosition KnockBack;
	RbPosition Vector;
};

#endif // !defined(AFX_TOBIWATERSPREAD_H__01EF52F9_45A2_4FBD_A286_95E5B235A81B__INCLUDED_)
