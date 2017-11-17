// TobiGeneratorBase.h: TobiGeneratorBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIGENERATORBASE_H__CC88768E_2383_4A43_B368_6D422A7782F1__INCLUDED_)
#define AFX_TOBIGENERATORBASE_H__CC88768E_2383_4A43_B368_6D422A7782F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ActLocker.h"
#include "TobiObjectBase.h"

class TobiGeneratorBase : public TobiObjectBase  
{
public:
	TobiGeneratorBase();
	virtual ~TobiGeneratorBase();

	virtual bool Job( BattleSystem& System );
	virtual bool SetEffect( const SkillEffect& Effect );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual bool Generate();
	virtual bool AddRandomTaget( TobiObjectBase* pBit, float Range );
	ActLocker Locker;
	double GenerateTime;
	double GenerateInterval;
	SkillEffect MyEffect;
	bool bActLock;
};

#endif // !defined(AFX_TOBIGENERATORBASE_H__CC88768E_2383_4A43_B368_6D422A7782F1__INCLUDED_)
