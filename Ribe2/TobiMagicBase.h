// TobiMagicBase.h: TobiMagicBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIMAGICBASE_H__7E190195_26C6_40B4_AF53_F19308B2F9FA__INCLUDED_)
#define AFX_TOBIMAGICBASE_H__7E190195_26C6_40B4_AF53_F19308B2F9FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSingleBase.h"

class TobiMagicBase : public TobiSingleBase  
{
public:
	TobiMagicBase();
	virtual ~TobiMagicBase();

protected:
	virtual void Damege( BattleSystem& System );
	bool bAllHit;
};

#endif // !defined(AFX_TOBIMAGICBASE_H__7E190195_26C6_40B4_AF53_F19308B2F9FA__INCLUDED_)
