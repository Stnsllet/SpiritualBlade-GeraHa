// TobiGenocide.h: TobiGenocide クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIGENOCIDE_H__E99FCE13_E6E2_461D_8C39_488888C383B7__INCLUDED_)
#define AFX_TOBIGENOCIDE_H__E99FCE13_E6E2_461D_8C39_488888C383B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiObjectBase.h"

class TobiGenocide : public TobiObjectBase  
{
public:
	TobiGenocide();
	virtual ~TobiGenocide();

	virtual bool Job( BattleSystem& System );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual void Damege( BattleSystem& System );
	SkillEffect MyEffect;
};

#endif // !defined(AFX_TOBIGENOCIDE_H__E99FCE13_E6E2_461D_8C39_488888C383B7__INCLUDED_)
