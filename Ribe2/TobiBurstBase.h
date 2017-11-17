// TobiBurstBase.h: TobiBurstBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIBURSTBASE_H__C08ED9D1_8B78_4FEB_97A2_2F90DE803AB3__INCLUDED_)
#define AFX_TOBIBURSTBASE_H__C08ED9D1_8B78_4FEB_97A2_2F90DE803AB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMagicBase.h"

class TobiBurstBase : public TobiMagicBase  
{
public:
	TobiBurstBase();
	virtual ~TobiBurstBase();

	virtual bool SetEffect( const SkillEffect& Effect );
//	virtual bool SetTaget( RbActStatus* pSource, RbActStatus* pTaget );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	float SpreadRange;
	virtual void Damege( BattleSystem& System );
	virtual RefPtr<TobiObjectBase> GetBurstObject();

	float AttackRange;
};

#endif // !defined(AFX_TOBIBURSTBASE_H__C08ED9D1_8B78_4FEB_97A2_2F90DE803AB3__INCLUDED_)
