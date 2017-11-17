// TobiHellFireGene.h: TobiHellFireGene クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIHELLFIREGENE_H__F8B80BF8_4EC2_40C7_AC42_EECC737AF057__INCLUDED_)
#define AFX_TOBIHELLFIREGENE_H__F8B80BF8_4EC2_40C7_AC42_EECC737AF057__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiFrameCircleGene.h"

class TobiHellFireGene : public TobiFrameCircleGene  
{
public:
	TobiHellFireGene();
	virtual ~TobiHellFireGene();

protected:
	virtual TobiFrameCircleBit* CreateFlameObject() const;
};

#endif // !defined(AFX_TOBIHELLFIREGENE_H__F8B80BF8_4EC2_40C7_AC42_EECC737AF057__INCLUDED_)
