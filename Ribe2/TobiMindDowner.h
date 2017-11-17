// TobiMindDowner.h: TobiMindDowner クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIMINDDOWNER_H__901E958A_B6E1_4007_952D_60E2729A8185__INCLUDED_)
#define AFX_TOBIMINDDOWNER_H__901E958A_B6E1_4007_952D_60E2729A8185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiRecoverBase.h"

class TobiMindDowner : public TobiRecoverBase  
{
public:
	TobiMindDowner();
	virtual ~TobiMindDowner();

protected:
	virtual void Damege( BattleSystem& System );
	virtual bool Heal( RbActStatus& Taget);
};

#endif // !defined(AFX_TOBIMINDDOWNER_H__901E958A_B6E1_4007_952D_60E2729A8185__INCLUDED_)
