// TobiFlameBirdGene.h: TobiFlameBirdGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIFLAMEBIRDGENE_H__79A67587_11FC_49CD_A4AE_76D4C8580FAE__INCLUDED_)
#define AFX_TOBIFLAMEBIRDGENE_H__79A67587_11FC_49CD_A4AE_76D4C8580FAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiFlameBirdGene : public TobiGeneratorBase  
{
public:
	TobiFlameBirdGene();
	virtual ~TobiFlameBirdGene();
protected:
	virtual bool Generate();
};

#endif // !defined(AFX_TOBIFLAMEBIRDGENE_H__79A67587_11FC_49CD_A4AE_76D4C8580FAE__INCLUDED_)
