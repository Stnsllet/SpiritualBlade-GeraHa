// TobiSoulBurst.h: TobiSoulBurst クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISOULBURST_H__EC3F444E_8FCF_4058_ABFA_4D92C38CED34__INCLUDED_)
#define AFX_TOBISOULBURST_H__EC3F444E_8FCF_4058_ABFA_4D92C38CED34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiSoulBurst : public TobiGeneratorBase  
{
public:
	TobiSoulBurst();
	virtual ~TobiSoulBurst();

protected:
	bool Generate();
};

#endif // !defined(AFX_TOBISOULBURST_H__EC3F444E_8FCF_4058_ABFA_4D92C38CED34__INCLUDED_)
