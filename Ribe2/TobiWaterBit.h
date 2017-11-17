// TobiWaterBit.h: TobiWaterBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIWATERBIT_H__3916FF57_CED4_4F9A_A994_C9B04EE916BA__INCLUDED_)
#define AFX_TOBIWATERBIT_H__3916FF57_CED4_4F9A_A994_C9B04EE916BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMagicBase.h"

class TobiWaterBit : public TobiMagicBase
{
public:
	TobiWaterBit();
	virtual ~TobiWaterBit();

	virtual bool SetResource();
	virtual bool Job( BattleSystem& System );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
	virtual bool SetEffect( const SkillEffect& Effect );
protected:
	virtual RbPosition GetKnockBack( const RbPosition& Base, const RbActStatus& Taget ) const;
	virtual bool ResetTrans();

	SkillEffect MyEffect;
	float NowAngle;
	double Time;
};

#endif // !defined(AFX_TOBIWATERBIT_H__3916FF57_CED4_4F9A_A994_C9B04EE916BA__INCLUDED_)
