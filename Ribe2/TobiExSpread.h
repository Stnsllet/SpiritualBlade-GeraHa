// TobiExSpread.h: TobiExSpread クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXSPREAD_H__EE7C2AD4_87C7_4F15_8063_AE35C732869A__INCLUDED_)
#define AFX_TOBIEXSPREAD_H__EE7C2AD4_87C7_4F15_8063_AE35C732869A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSpreadBase.h"
#include "TobiExObjectBase.h"

class TobiExSpread : public TobiExObjectBase<TobiSpreadBase>  
{
public:
	TobiExSpread();
	virtual ~TobiExSpread();

	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIEXSPREAD_H__EE7C2AD4_87C7_4F15_8063_AE35C732869A__INCLUDED_)
