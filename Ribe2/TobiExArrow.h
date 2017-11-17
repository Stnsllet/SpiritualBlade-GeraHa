// TobiExArrow.h: TobiExArrow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXARROW_H__B1237EA7_75A1_492E_B595_162B65B20AB7__INCLUDED_)
#define AFX_TOBIEXARROW_H__B1237EA7_75A1_492E_B595_162B65B20AB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiArrow.h"
#include "TobiExObjectBase.h"

class TobiExArrow : public TobiExObjectBase2<TobiArrow>  
{
public:
	TobiExArrow();
	virtual ~TobiExArrow();

	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
};

#endif // !defined(AFX_TOBIEXARROW_H__B1237EA7_75A1_492E_B595_162B65B20AB7__INCLUDED_)
