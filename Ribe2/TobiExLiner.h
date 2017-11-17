// TobiExLiner.h: TobiExLiner クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXLINER_H__EEF24EF8_A815_4BE4_8CBB_B4858187179E__INCLUDED_)
#define AFX_TOBIEXLINER_H__EEF24EF8_A815_4BE4_8CBB_B4858187179E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiLinerBase.h"
#include "TobiExObjectBase.h"

class TobiExLiner : public TobiExObjectBase3<TobiLinerBase>  
{
public:
	TobiExLiner();
	virtual ~TobiExLiner();

	virtual bool SetEffect( const SkillEffect& Effect );
	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
};

#endif // !defined(AFX_TOBIEXLINER_H__EEF24EF8_A815_4BE4_8CBB_B4858187179E__INCLUDED_)
