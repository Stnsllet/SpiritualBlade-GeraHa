// TobiExBurst.h: TobiExBurst クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXBURST_H__3DF54413_944E_4249_81A8_4EEA57A299EB__INCLUDED_)
#define AFX_TOBIEXBURST_H__3DF54413_944E_4249_81A8_4EEA57A299EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiBurstBase.h"
#include "TobiExObjectBase.h"

class TobiExBurst : public TobiExObjectBase2<TobiBurstBase>  
{
public:
	TobiExBurst();
	virtual ~TobiExBurst();

	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
protected:
};

#endif // !defined(AFX_TOBIEXBURST_H__3DF54413_944E_4249_81A8_4EEA57A299EB__INCLUDED_)
