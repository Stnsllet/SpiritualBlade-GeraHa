// TobiWindThrustGene.h: TobiWindThrustGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIWINDTHRUSTGENE_H__4DEB5561_D6CA_4D6B_9E2E_0E4207D374DD__INCLUDED_)
#define AFX_TOBIWINDTHRUSTGENE_H__4DEB5561_D6CA_4D6B_9E2E_0E4207D374DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiGeneratorBase.h"

class TobiWindThrustGene : public TobiGeneratorBase  
{
public:
	TobiWindThrustGene();
	virtual ~TobiWindThrustGene();

protected:
	virtual bool Generate();
};

#endif // !defined(AFX_TOBIWINDTHRUSTGENE_H__4DEB5561_D6CA_4D6B_9E2E_0E4207D374DD__INCLUDED_)
