// TobiFullRecover.h: TobiFullRecover クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIFULLRECOVER_H__D5052E17_41F6_484B_9502_B698383A8944__INCLUDED_)
#define AFX_TOBIFULLRECOVER_H__D5052E17_41F6_484B_9502_B698383A8944__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiRecoverBase.h"

class TobiFullRecover : public TobiRecoverBase  
{
public:
	TobiFullRecover();
	virtual ~TobiFullRecover();
protected:
	virtual bool Heal( RbActStatus& Taget);

};

#endif // !defined(AFX_TOBIFULLRECOVER_H__D5052E17_41F6_484B_9502_B698383A8944__INCLUDED_)
