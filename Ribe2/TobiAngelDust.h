// TobiAngelDust.h: TobiAngelDust クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIANGELDUST_H__08651342_759B_419B_ABA2_2CD8D2D8BCF3__INCLUDED_)
#define AFX_TOBIANGELDUST_H__08651342_759B_419B_ABA2_2CD8D2D8BCF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiAngelDust : public TobiGeneratorBase  
{
public:
	TobiAngelDust();
	virtual ~TobiAngelDust();

protected:
	bool Generate();
};

#endif // !defined(AFX_TOBIANGELDUST_H__08651342_759B_419B_ABA2_2CD8D2D8BCF3__INCLUDED_)
