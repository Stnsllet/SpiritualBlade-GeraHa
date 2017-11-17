// TobiMissleBase.h: TobiMissleBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIMISSLEBASE_H__0546606B_476B_4D32_AAD8_5F559E27A629__INCLUDED_)
#define AFX_TOBIMISSLEBASE_H__0546606B_476B_4D32_AAD8_5F559E27A629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMagicBase.h"

class TobiMissleBase : public TobiMagicBase  
{
public:
	TobiMissleBase();
	virtual ~TobiMissleBase();

	virtual bool Job( BattleSystem& System );
	virtual bool SetTaget( RbActStatus* pSource, RbActStatus* pNewTaget );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual bool TagetHitJob( BattleSystem& System );
	RefPtr<RbActStatus> pTaget;
	float MaxSpeed;
	float DownSpeed;
	float TurnSpeed;
};

#endif // !defined(AFX_TOBIMISSLEBASE_H__0546606B_476B_4D32_AAD8_5F559E27A629__INCLUDED_)
