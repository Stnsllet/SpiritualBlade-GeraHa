// TobiWhiteBlowGene.h: TobiWhiteBlowGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIWHITEBLOWGENE_H__B89AF1A9_5EB8_4648_BE42_AA7D3DC5FB36__INCLUDED_)
#define AFX_TOBIWHITEBLOWGENE_H__B89AF1A9_5EB8_4648_BE42_AA7D3DC5FB36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiWhiteBlowGene : public TobiGeneratorBase  
{
public:
	TobiWhiteBlowGene();
	virtual ~TobiWhiteBlowGene();

	virtual bool Job( BattleSystem& System );
	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );

protected:
	virtual bool AddBlow( RbActStatus& Taget );
	float StartAngle, NowAngle;
};

#endif // !defined(AFX_TOBIWHITEBLOWGENE_H__B89AF1A9_5EB8_4648_BE42_AA7D3DC5FB36__INCLUDED_)
