// TobiArrow.h: TobiArrow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIARROW_H__0D4D58D3_E1EA_48BA_B1FA_D22239D181CF__INCLUDED_)
#define AFX_TOBIARROW_H__0D4D58D3_E1EA_48BA_B1FA_D22239D181CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSingleBase.h"

class TobiArrow : public TobiSingleBase  
{
public:
	TobiArrow();
	virtual ~TobiArrow();
	virtual bool SetResource();
	virtual bool Job( BattleSystem& System );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );

protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIARROW_H__0D4D58D3_E1EA_48BA_B1FA_D22239D181CF__INCLUDED_)
