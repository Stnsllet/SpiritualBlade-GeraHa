// TobiStar.h: TobiStar クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISTAR_H__5F2B4509_47EA_4A9A_A14D_EE61DB6E5DC1__INCLUDED_)
#define AFX_TOBISTAR_H__5F2B4509_47EA_4A9A_A14D_EE61DB6E5DC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiMissleBase.h"

class TobiStar : public TobiMissleBase  
{
public:
	TobiStar();
	virtual ~TobiStar();
	virtual bool SetResource();
};

#endif // !defined(AFX_TOBISTAR_H__5F2B4509_47EA_4A9A_A14D_EE61DB6E5DC1__INCLUDED_)
