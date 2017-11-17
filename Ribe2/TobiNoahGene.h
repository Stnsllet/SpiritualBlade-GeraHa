// TobiNoahGene.h: TobiNoahGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBINOAHGENE_H__3441C7B3_F161_49F1_A504_714F46270112__INCLUDED_)
#define AFX_TOBINOAHGENE_H__3441C7B3_F161_49F1_A504_714F46270112__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiNoahGene : public TobiGeneratorBase  
{
public:
	TobiNoahGene();
	virtual ~TobiNoahGene();
protected:
	bool Generate();
};

#endif // !defined(AFX_TOBINOAHGENE_H__3441C7B3_F161_49F1_A504_714F46270112__INCLUDED_)
