// TobiRoket.h: TobiRoket クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIROKET_H__0CF662A1_07DC_45DC_B845_D89266C836D6__INCLUDED_)
#define AFX_TOBIROKET_H__0CF662A1_07DC_45DC_B845_D89266C836D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMissleBase.h"

class TobiRoket : public TobiMissleBase  
{
public:
	TobiRoket();
	virtual ~TobiRoket();

	virtual bool SetResource();
protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIROKET_H__0CF662A1_07DC_45DC_B845_D89266C836D6__INCLUDED_)
