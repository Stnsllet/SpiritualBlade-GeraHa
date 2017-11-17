// TobiFire.h: TobiFire クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIFIRE_H__8B48469F_54E2_4D6F_A224_BEAD68E0055E__INCLUDED_)
#define AFX_TOBIFIRE_H__8B48469F_54E2_4D6F_A224_BEAD68E0055E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMagicBase.h"

class TobiFire : public TobiMagicBase  
{
public:
	TobiFire();
	virtual ~TobiFire();
	virtual bool SetResource();
};

#endif // !defined(AFX_TOBIFIRE_H__8B48469F_54E2_4D6F_A224_BEAD68E0055E__INCLUDED_)
