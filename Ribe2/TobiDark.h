// TobiDark.h: TobiDark クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIDARK_H__2C2EF2B1_2E5E_4339_8CA8_ED97AE1F33D7__INCLUDED_)
#define AFX_TOBIDARK_H__2C2EF2B1_2E5E_4339_8CA8_ED97AE1F33D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiBurstBase.h"

class TobiDark : public TobiBurstBase  
{
public:
	TobiDark();
	virtual ~TobiDark();

	virtual bool SetResource();
protected:
	virtual void Damege( BattleSystem& System );
};

#endif // !defined(AFX_TOBIDARK_H__2C2EF2B1_2E5E_4339_8CA8_ED97AE1F33D7__INCLUDED_)
