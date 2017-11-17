// TobiChainBurst.h: TobiChainBurst クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBICHAINBURST_H__972C12E8_33DB_4FC6_B8A2_51E0CEA3CBD0__INCLUDED_)
#define AFX_TOBICHAINBURST_H__972C12E8_33DB_4FC6_B8A2_51E0CEA3CBD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbBattlePaty.h"
#include "TobiGeneratorBase.h"

class TobiChainBurst : public TobiGeneratorBase  
{
public:
	TobiChainBurst();
	virtual ~TobiChainBurst();

	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual bool Generate();
	long MemberNo;
	RefPtr<RbBattlePaty> pPaty;
};

#endif // !defined(AFX_TOBICHAINBURST_H__972C12E8_33DB_4FC6_B8A2_51E0CEA3CBD0__INCLUDED_)
