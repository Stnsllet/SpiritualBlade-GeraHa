// TobiDarkBlazeGene.h: TobiDarkBlazeGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIDARKBLAZEGENE_H__69A56B2A_F44E_465D_955C_6225582617DC__INCLUDED_)
#define AFX_TOBIDARKBLAZEGENE_H__69A56B2A_F44E_465D_955C_6225582617DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiFrameCircleGene.h"

class TobiDarkBlazeGene : public TobiFrameCircleGene  
{
public:
	TobiDarkBlazeGene();
	virtual ~TobiDarkBlazeGene();

protected:
	virtual TobiFrameCircleBit* CreateFlameObject() const;
};

#endif // !defined(AFX_TOBIDARKBLAZEGENE_H__69A56B2A_F44E_465D_955C_6225582617DC__INCLUDED_)
