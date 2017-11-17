// TobiHellFireBit.h: TobiHellFireBit クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIHELLFIREBIT_H__BA85A17F_15D3_4700_90A4_AED22174E84F__INCLUDED_)
#define AFX_TOBIHELLFIREBIT_H__BA85A17F_15D3_4700_90A4_AED22174E84F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiFrameCircleBit.h"

class TobiHellFireBit : public TobiFrameCircleBit  
{
public:
	TobiHellFireBit();
	virtual ~TobiHellFireBit();

	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIHELLFIREBIT_H__BA85A17F_15D3_4700_90A4_AED22174E84F__INCLUDED_)
