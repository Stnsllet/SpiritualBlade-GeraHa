// TobiAttackSpreadBase.h: TobiAttackSpreadBase クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIATTACKSPREADBASE_H__B9C73769_0394_4D62_9CD7_DA63B72F9B98__INCLUDED_)
#define AFX_TOBIATTACKSPREADBASE_H__B9C73769_0394_4D62_9CD7_DA63B72F9B98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSpreadBase.h"

class TobiAttackSpreadBase : public TobiSpreadBase  
{
public:
	TobiAttackSpreadBase();
	virtual ~TobiAttackSpreadBase();

protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIATTACKSPREADBASE_H__B9C73769_0394_4D62_9CD7_DA63B72F9B98__INCLUDED_)
