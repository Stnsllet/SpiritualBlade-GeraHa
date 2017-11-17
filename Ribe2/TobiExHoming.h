// TobiExHoming.h: TobiExHoming クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXHOMING_H__2AE77A24_B907_41E7_A891_1EDE8F14BEED__INCLUDED_)
#define AFX_TOBIEXHOMING_H__2AE77A24_B907_41E7_A891_1EDE8F14BEED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMissleBase.h"
#include "TobiExObjectBase.h"

class TobiExHoming : public TobiExObjectBase2<TobiMissleBase>  
{
public:
	TobiExHoming();
	virtual ~TobiExHoming();

	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
};

#endif // !defined(AFX_TOBIEXHOMING_H__2AE77A24_B907_41E7_A891_1EDE8F14BEED__INCLUDED_)
