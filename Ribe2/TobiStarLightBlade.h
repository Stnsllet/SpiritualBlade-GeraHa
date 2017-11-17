// TobiStarLightBlade.h: TobiStarLightBlade クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISTARLIGHTBLADE_H__710A8247_1004_4CB0_A41E_A342C34F8D26__INCLUDED_)
#define AFX_TOBISTARLIGHTBLADE_H__710A8247_1004_4CB0_A41E_A342C34F8D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiStarLightBlade : public TobiGeneratorBase  
{
public:
	TobiStarLightBlade();
	virtual ~TobiStarLightBlade();

protected:
	bool Generate();
};

#endif // !defined(AFX_TOBISTARLIGHTBLADE_H__710A8247_1004_4CB0_A41E_A342C34F8D26__INCLUDED_)
