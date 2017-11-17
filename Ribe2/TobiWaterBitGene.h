// TobiWaterBitGene.h: TobiWaterBitGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIWATERBITGENE_H__3B6FE353_E7D3_4912_B2ED_41958932A092__INCLUDED_)
#define AFX_TOBIWATERBITGENE_H__3B6FE353_E7D3_4912_B2ED_41958932A092__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiWaterBitGene : public TobiGeneratorBase
{
public:
	TobiWaterBitGene();
	virtual ~TobiWaterBitGene();
	virtual bool SetEffect( const SkillEffect& Effect );
protected:
	virtual bool Generate();
	long GeneCount;
};

#endif // !defined(AFX_TOBIWATERBITGENE_H__3B6FE353_E7D3_4912_B2ED_41958932A092__INCLUDED_)
