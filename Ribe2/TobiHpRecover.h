// TobiHpRecover.h: TobiHpRecover クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIHPRECOVER_H__B9F33AFE_0B71_4F6E_A16F_D064939855CF__INCLUDED_)
#define AFX_TOBIHPRECOVER_H__B9F33AFE_0B71_4F6E_A16F_D064939855CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiRecoverBase.h"

class TobiHpRecover : public TobiRecoverBase  
{
public:
	TobiHpRecover();
	virtual ~TobiHpRecover();
protected:
	virtual bool Heal( RbActStatus& Taget);
};

#endif // !defined(AFX_TOBIHPRECOVER_H__B9F33AFE_0B71_4F6E_A16F_D064939855CF__INCLUDED_)
