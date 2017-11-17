// TobiBurst.h: TobiBurst クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIBURST_H__6B0C2C47_32D8_4BCD_A8DC_85609CE337B6__INCLUDED_)
#define AFX_TOBIBURST_H__6B0C2C47_32D8_4BCD_A8DC_85609CE337B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiAttackSpreadBase.h"

class TobiBurst : public TobiAttackSpreadBase  
{
public:
	TobiBurst();
	virtual ~TobiBurst();

	virtual bool SetResource();
};

#endif // !defined(AFX_TOBIBURST_H__6B0C2C47_32D8_4BCD_A8DC_85609CE337B6__INCLUDED_)
