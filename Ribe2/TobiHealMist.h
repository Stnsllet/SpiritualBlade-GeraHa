// TobiHealMist.h: TobiHealMist クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIHEALMIST_H__3B25D022_2924_424B_BCF2_1A5EAB7523F0__INCLUDED_)
#define AFX_TOBIHEALMIST_H__3B25D022_2924_424B_BCF2_1A5EAB7523F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ActLocker.h"
#include "TobiSpreadBase.h"

class TobiHealMist : public TobiSpreadBase  
{
public:
	TobiHealMist();
	virtual ~TobiHealMist();

	virtual bool SetResource();
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
	virtual bool Job( BattleSystem& System );
	virtual bool SetEffect( const SkillEffect& Effect );
protected:
	virtual void Damege( BattleSystem& System );

	ActLocker Locker;
	long EffectCount;
	double EffectInterval;
	double NowEffectTime;
};

#endif // !defined(AFX_TOBIHEALMIST_H__3B25D022_2924_424B_BCF2_1A5EAB7523F0__INCLUDED_)
