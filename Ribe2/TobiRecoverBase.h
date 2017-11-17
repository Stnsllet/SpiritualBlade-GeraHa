// TobiRecoverBase.h: TobiRecoverBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIRECOVERBASE_H__B2FAF55A_6497_4487_B99E_C0665FD1548D__INCLUDED_)
#define AFX_TOBIRECOVERBASE_H__B2FAF55A_6497_4487_B99E_C0665FD1548D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiObjectBase.h"

class TobiRecoverBase : public TobiObjectBase  
{
public:
	TobiRecoverBase();
	virtual ~TobiRecoverBase();
	virtual bool Job( BattleSystem& System );

	virtual bool SetEffect( const SkillEffect& Effect );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual void Damege( BattleSystem& System );
	virtual bool Heal( RbActStatus& Taget);
	SkillEffect MyEffect;
};

#endif // !defined(AFX_TOBIRECOVERBASE_H__B2FAF55A_6497_4487_B99E_C0665FD1548D__INCLUDED_)
