// TobiExSimple.h: TobiExSimple クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXSIMPLE_H__200661F6_F37B_4DC9_A4D6_5A18B1FA7D99__INCLUDED_)
#define AFX_TOBIEXSIMPLE_H__200661F6_F37B_4DC9_A4D6_5A18B1FA7D99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSingleBase.h"
#include "TobiExObjectBase.h"

class TobiExSimple : public TobiExObjectBase2<TobiSingleBase>  
{
public:
	TobiExSimple();
	virtual ~TobiExSimple();

	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
};

#endif // !defined(AFX_TOBIEXSIMPLE_H__200661F6_F37B_4DC9_A4D6_5A18B1FA7D99__INCLUDED_)
