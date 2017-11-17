// TobiHalfMoon.h: TobiHalfMoon クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIHALFMOON_H__2348D8A1_F92F_4E91_BD19_F85A8AD746BB__INCLUDED_)
#define AFX_TOBIHALFMOON_H__2348D8A1_F92F_4E91_BD19_F85A8AD746BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiAttackSpreadBase.h"
#include "AngleChecker.h"

class TobiHalfMoon : public TobiAttackSpreadBase  
{
public:
	TobiHalfMoon();
	virtual ~TobiHalfMoon();
	virtual void Draw( Display& Disp );

	virtual bool SetResource();
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual void Damege( BattleSystem& System );
	float MyAngle;
	AngleChecker::Checker CheckAngle;
};

#endif // !defined(AFX_TOBIHALFMOON_H__2348D8A1_F92F_4E91_BD19_F85A8AD746BB__INCLUDED_)
