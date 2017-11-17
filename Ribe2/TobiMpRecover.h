// TobiMpRecover.h: TobiMpRecover クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIMPRECOVER_H__C66D562A_DA87_4478_A6A3_FFCE3196543E__INCLUDED_)
#define AFX_TOBIMPRECOVER_H__C66D562A_DA87_4478_A6A3_FFCE3196543E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiRecoverBase.h"

class TobiMpRecover : public TobiRecoverBase  
{
public:
	TobiMpRecover();
	virtual ~TobiMpRecover();
protected:
	virtual bool Heal( RbActStatus& Taget);

};

#endif // !defined(AFX_TOBIMPRECOVER_H__C66D562A_DA87_4478_A6A3_FFCE3196543E__INCLUDED_)
