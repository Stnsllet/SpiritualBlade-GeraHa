// TobiMeteorGene.h: TobiMeteorGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIMETEORGENE_H__E730AD17_56CB_4551_B361_A4C217F2986E__INCLUDED_)
#define AFX_TOBIMETEORGENE_H__E730AD17_56CB_4551_B361_A4C217F2986E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiMeteorGene : public TobiGeneratorBase  
{
public:
	TobiMeteorGene();
	virtual ~TobiMeteorGene();

protected:
	bool Generate();
};

#endif // !defined(AFX_TOBIMETEORGENE_H__E730AD17_56CB_4551_B361_A4C217F2986E__INCLUDED_)
