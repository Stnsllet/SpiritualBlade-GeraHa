// TobiExAllHit.h: TobiExAllHit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIEXALLHIT_H__8B5897B2_3866_43DF_A648_48C5ECD2071F__INCLUDED_)
#define AFX_TOBIEXALLHIT_H__8B5897B2_3866_43DF_A648_48C5ECD2071F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiWhiteBlowGene.h"
#include "TobiExObjectBase.h"

class TobiExAllHit : public TobiExObjectBase<TobiWhiteBlowGene> 
{
public:
	TobiExAllHit();
	virtual ~TobiExAllHit();

	virtual bool Job( BattleSystem& System );
	virtual bool SetExDefine( ExCreator::BaseDef* pNewDef );
protected:
	virtual bool AddBlow( RbActStatus& Taget );
	TobiCreator* pBitCreator;
};

#endif // !defined(AFX_TOBIEXALLHIT_H__8B5897B2_3866_43DF_A648_48C5ECD2071F__INCLUDED_)
