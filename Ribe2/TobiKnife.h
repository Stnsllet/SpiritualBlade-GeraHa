// TobiKnife.h: TobiKnife クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOBIKNIFE_H__F7D33368_70F7_4AF7_950B_1B9F85EF2B42__INCLUDED_)
#define AFX_TOBIKNIFE_H__F7D33368_70F7_4AF7_950B_1B9F85EF2B42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TobiSingleBase.h"

class TobiKnife : public TobiSingleBase  
{
public:
	TobiKnife();
	virtual ~TobiKnife();
	virtual bool SetResource();
};

#endif // !defined(AFX_TOBIKNIFE_H__F7D33368_70F7_4AF7_950B_1B9F85EF2B42__INCLUDED_)
