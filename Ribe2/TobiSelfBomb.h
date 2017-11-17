// TobiSelfBomb.h: TobiSelfBomb クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBISELFBOMB_H__FE367628_B5EC_4B9A_B176_A854A6E05624__INCLUDED_)
#define AFX_TOBISELFBOMB_H__FE367628_B5EC_4B9A_B176_A854A6E05624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiAttackSpreadBase.h"

class TobiSelfBomb : public TobiAttackSpreadBase  
{
public:
	TobiSelfBomb();
	virtual ~TobiSelfBomb();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBISELFBOMB_H__FE367628_B5EC_4B9A_B176_A854A6E05624__INCLUDED_)
