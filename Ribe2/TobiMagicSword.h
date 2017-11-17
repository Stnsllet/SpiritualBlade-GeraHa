// TobiMagicSword.h: TobiMagicSword クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIMAGICSWORD_H__43A9DF5A_2404_4FA3_AC29_CFE935B15A86__INCLUDED_)
#define AFX_TOBIMAGICSWORD_H__43A9DF5A_2404_4FA3_AC29_CFE935B15A86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMissleBase.h"

class TobiMagicSword : public TobiMissleBase  
{
public:
	TobiMagicSword();
	virtual ~TobiMagicSword();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBIMAGICSWORD_H__43A9DF5A_2404_4FA3_AC29_CFE935B15A86__INCLUDED_)
