// TobiChainFlare.h: TobiChainFlare クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBICHAINFLARE_H__64BB942A_FFB7_43E2_AC4A_F477D5F7A880__INCLUDED_)
#define AFX_TOBICHAINFLARE_H__64BB942A_FFB7_43E2_AC4A_F477D5F7A880__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RbBattlePaty.h"
#include "TobiGeneratorBase.h"

class TobiChainFlare : public TobiGeneratorBase  
{
public:
	TobiChainFlare();
	virtual ~TobiChainFlare();

	virtual bool SetTagetPos( RbActStatus* pSource, const RbPosition& BasePos, const RbPosition& NewTagetPos );
protected:
	virtual bool Generate();
	long MemberNo;
	RefPtr<RbBattlePaty> pPaty;
};

#endif // !defined(AFX_TOBICHAINFLARE_H__64BB942A_FFB7_43E2_AC4A_F477D5F7A880__INCLUDED_)
